//Sensor remoto basado en Arduino Pro Mini (Sparkfun), Arduino Mini y cactus
//micro, usando sensores de gas (MQ-2), temperatura+humedad (DHT22) y sensor de
//temperatura (DS18B20)

//Advertencia: Antes de compilar y subir, se aconseja ampliar el buffer de datos
//de la libreria SoftwareSerial a 256 bytes. Para ello, es necesario abrir y
//editar el siguiente archivo en arduino ide:
//  hardware/arduino/avr/libraries/SoftwareSerial/SoftwareSerial.h
//La linea a modificar es la siguiente:
//  #define _SS_MAX_RX_BUFF 64 // RX buffer size
//Cambiar el valor de 64 a 256
//--------------------------------------------------------------------------------
//INICIO DE LOS PARAMETROS CONFIGURABLES

//Configuraciones de la red WiFi
//PROGMEM const char AP[] = "DISZASA-PI";
PROGMEM const char AP[] = "icon";
//PROGMEM const char CLAVE[] = "mistic0$";
PROGMEM const char CLAVE[] = "68b369b8ed";

//Configuraciones del servidor remoto
//PROGMEM const char servidor[] = "10.0.10.12";
PROGMEM const char servidor[] = "50.116.76.54";
const int puertoServidor = 80;
//PROGMEM const char pagina[] = "/emoncms";
PROGMEM const char pagina[] = "/sees";
//PROGMEM const char apikey[] = "e1df8e9be3f7cb0e2bd569e1245f1bc5";
PROGMEM const char apikey[] = "d5c746f11b3d2b34ba192ab8f22efe87";

//Descomentar las siguientes definiciones para habilitar los sensores
//correspondientes. Comentar la definicion implica que no se usara el sensor ni
//se enviaran sus datos.
//#define SENSOR_DS18B20
#define SENSOR_DHT22
#define SENSOR_MQ2

//Definiciones de los pines del modulo ESP8266
//const int pinEspRx = 11;   //Cactus micro
//const int pinEspTx = 12;
const int pinEspRx = 5;  //Pro mini y micro
const int pinEspTx = 4;
const int pinEspEn = 13;

//Definiciones de los pines de los diferentes perifericos (no comentar aunque no
//se usen)
const int pinDS18B20 = 10;  //Numero de pin digital
const int pinDHT22 = 10;    //Numero de pin digital
const int pinMQ2 = 0;       //Numero de pin analogico

//Descomentar para habilitar rutinas (para uso normal deben estar
//comentadas todas excepto la de envio de datos por wifi)
#define ENVIAR_DATOS_WIFI
//#define PUENTE_SERIAL_ESP8266
#define MOSTRAR_DATOS_SENSORES

//FIN DE LOS PARAMETROS CONFIGURABLES
//--------------------------------------------------------------------------------
//Se hacen algunos chequeos de las definiciones anteriores para buscar conflictos
#if defined(SENSOR_DS18B20) && defined(SENSOR_DHT22)
  #error "Se han habilitado 2 sensores de temperatura distintos"
#endif

#if defined(PUENTE_SERIAL_ESP8266) && \
  (defined(MOSTRAR_DATOS_SENSORES) || defined(ENVIAR_DATOS_WIFI))
  #error "No se pueden manejar comandos AT a la vez que se muestran o envian datos"
#endif

//Macro usada para hacer el casting necesario para que las funciones que emplean el
//tipo __FlashStringHelper acepten correctamente las cadenas almacenadas en la
//memoria flash declaradas con PROGMEM
#define FSH(cadena_pgm) reinterpret_cast<const __FlashStringHelper *>(cadena_pgm)

//--------------------------------------------------------------------------------
//Librerias usadas para el modulo wifi ESP8266
#include <SoftwareSerial.h>
#include "esp8266.h"
//Instancia de clase de software serial para el modulo wifi
SoftwareSerial serieWifi(pinEspRx, pinEspTx);
//Instancia de clase del modulo ESP8266
ESP8266 esp8266(&serieWifi, pinEspEn);
byte dirIP[4];
byte dirMAC[6];

//Cadena que contendra el ID del nodo
//  4+  Mascara de identificacion del dispositivo (2 bytes)
// 12+  MAC address del adaptador WiFi (6 bytes)
//  1=  Cero terminador
// ---
// 17   Total de bytes
char idNodo[17];

//Bandera que indica si es el primer dato que se envia en la trama GET
bool primerDato = true;

//--------------------------------------------------------------------------------
#ifdef SENSOR_DS18B20
  //Librerias usadas para el ds18b20
  #include <OneWire.h>
  #include <DallasTemperature.h>
  //Variable con la lectura del dato de temperatura
  float temp_DS18B20 = 0;
#endif

//--------------------------------------------------------------------------------
#ifdef SENSOR_DHT22
  //Libreria del sensor DHT22
  #include <DHT.h>
  //Instancia de clase DHT (sensor de temperatura y humedad)
  DHT dht(pinDHT22, DHT22);
  //Variables con las lecturas del DHT22
  float temp_DHT22 = 0;
  float hum_DHT22 = 0;
#endif

//--------------------------------------------------------------------------------
#ifdef SENSOR_MQ2
  #include "SensorGas.h"
  unsigned long ppm_MQ2 = 0;
  SensorGas sensorMQ2(MQ2, pinMQ2);
#endif

//--------------------------------------------------------------------------------
void setup(void) {
  //Inicializa la terminal serie
  Serial.begin(9600);
  delay(2000);

  //Inicializa el modulo WiFi
  serieWifi.begin(9600);
  for (;;) {
    Serial.print(F("Inicializando modulo Wifi... "));
    if (esp8266.reset()) { Serial.println(F("OK")); break; }
    else Serial.println(F("Error al inicializar el modulo"));
  }

  //Se conecta con la red WiFi
  #ifdef ENVIAR_DATOS_WIFI
    for (;;) {
      Serial.print(F("Conectandose a la red \""));
      Serial.print(FSH(AP));
      Serial.print(F("\"... "));
      if (esp8266.conectarAP(FSH(AP), FSH(CLAVE)))
        { Serial.println(F("OK")); break; }
      else
        Serial.println(F("Error al conectarse a la red"));
    }

    //Obtiene las direcciones IP y MAC del modulo WiFi
    esp8266.leerIP(dirIP);
    esp8266.leerMAC(dirMAC);

    //Imprime las direcciones en la terminal serie
    Serial.print(F("Direccion IP: "));
    Serial.print(dirIP[0]); Serial.print(F("."));
    Serial.print(dirIP[1]); Serial.print(F("."));
    Serial.print(dirIP[2]); Serial.print(F("."));
    Serial.print(dirIP[3]); Serial.println(F(""));

    Serial.print(F("Direccion MAC: "));
    Serial.print(dirMAC[0], HEX); Serial.print(F(":"));
    Serial.print(dirMAC[1], HEX); Serial.print(F(":"));
    Serial.print(dirMAC[2], HEX); Serial.print(F(":"));
    Serial.print(dirMAC[3], HEX); Serial.print(F(":"));
    Serial.print(dirMAC[4], HEX); Serial.print(F(":"));
    Serial.print(dirMAC[5], HEX); Serial.println(F(""));

    //Calcula el ID de esta unidad
    calcularID();

    //Imprime el resultado
    Serial.print(F("ID del nodo: "));
    Serial.println(idNodo);
  #endif

  //Inicializa el sensor de temperatura DS18B20
  #ifdef SENSOR_DS18B20
    inicializarDS18B20();
  #endif

  //Inicializa el sensor de temperatura/humedad DHT22
  #ifdef SENSOR_DHT22
    dht.begin();
  #endif
}

void loop(void) {
  //Se leen los datos de todos aquellos sensores que esten habilitados
  #ifdef SENSOR_DHT22
    temp_DHT22 = dht.readTemperature();
    hum_DHT22 = dht.readHumidity();
  #endif

  #ifdef SENSOR_DS18B20
    temp_DS18B20 = leerDS18B20();
  #endif

  #ifdef SENSOR_MQ2
    sensorMQ2.atender();
    if (sensorMQ2.estaListo()) ppm_MQ2 = sensorMQ2.leerPPM(HUMO);
  #endif

  //Se muestran por terminal serie los valores de los sensores
  #ifdef MOSTRAR_DATOS_SENSORES
    #ifdef SENSOR_DS18B20
      Serial.print(F("DS18B20: "));
      Serial.print(temp_DS18B20);
      Serial.print(F("C   "));
    #endif

    #ifdef SENSOR_DHT22
      Serial.print(F("DHT 22: "));
      Serial.print(temp_DHT22);
      Serial.print(F("C/"));
      Serial.print(hum_DHT22);
      Serial.print(F("%   "));
    #endif

    #ifdef SENSOR_MQ2
      Serial.print(F("MQ-2: "));
      Serial.print(ppm_MQ2);
      Serial.print(F("ppm   "));
    #endif

    Serial.print(F("\n"));
  #endif

  //Se entra en modalidad de puente serial si la funcion esta habilitada
  #ifdef PUENTE_SERIAL_ESP8266
    for (;;) {
      while (Serial.available()) serieWifi.write(Serial.read());
      while (serieWifi.available()) Serial.write(serieWifi.read());
    }
  #endif

  //Se envian los datos de los sensores que esten habilitados
  #ifdef ENVIAR_DATOS_WIFI
    int valor;          //Variables usadas para formatear las lecturas
    char strValor[16];

    //Inicia la conexion con el servidor y envia la cabecera HTTP
    if (!iniciarConexion()) return;

    //Envia los datos de aquellos sensores que esten habilitados
    #ifdef SENSOR_DS18B20
      //Multiplica la temperatura por 100 para extraer los digitos antes y despues
      //del punto
      valor = temp_DS18B20 * 100;
      sprintf(strValor, "%i.%02i", valor / 100, valor % 100);
      enviarDato(F("Temperatura"), strValor);

      while (serieWifi.available()) Serial.write(serieWifi.read());
    #endif

    #ifdef SENSOR_DHT22
      valor = temp_DHT22 * 100;
      sprintf(strValor, "%i.%02i", valor / 100, valor % 100);
      enviarDato(F("Temperatura"), strValor);

      while (serieWifi.available()) Serial.write(serieWifi.read());

      valor = hum_DHT22 * 100;
      sprintf(strValor, "%i.%02i", valor / 100, valor % 100);
      enviarDato(F("Humedad"), strValor);

      while (serieWifi.available()) Serial.write(serieWifi.read());
    #endif

    #ifdef SENSOR_MQ2
      valor = ppm_MQ2;
      sprintf(strValor, "%i", valor);
      enviarDato(F("ppm"), strValor);

      while (serieWifi.available()) Serial.write(serieWifi.read());
    #endif

    //Termina la cabecera HTTP, capta la respuesta y cierra la conexion
    terminarConexion();

    delay(10000);
  #endif
}

void calcularID() {
  byte offset = 0;
  byte i;

  strcpy(idNodo, "0000");
  offset = strlen(idNodo);

  for (i=0; i<6; i++) {
    sprintf(&idNodo[offset], "%02X", dirMAC[i]);
    offset = strlen(idNodo);
  }
}

bool iniciarConexion() {
  //Fuerza la desconexion en caso este previamente conectado
  esp8266.desconectar();

  //Inicia la conexion al servidor
  if (!esp8266.conectar(FSH(servidor), puertoServidor)) {
    Serial.println(F("Error al conectar"));
    return false;
  }

  //Envia la cabecera HTTP/GET
  if (!esp8266.enviar(F("GET "))) {
    Serial.println(F("Error al enviar datos"));
    esp8266.desconectar();
    return false;
  }

  if (!esp8266.enviar(FSH(pagina))) {
    Serial.println(F("Error al enviar datos"));
    esp8266.desconectar();
    return false;
  }

  if (!esp8266.enviar(F("/input/post.json?apikey="))) {
    Serial.println(F("Error al enviar datos"));
    esp8266.desconectar();
    return false;
  }

  if (!esp8266.enviar(FSH(apikey))) {
    Serial.println(F("Error al enviar datos"));
    esp8266.desconectar();
    return false;
  }

  if (!esp8266.enviar(F("&node="))) {
    Serial.println(F("Error al enviar datos"));
    esp8266.desconectar();
    return false;
  }

  if (!esp8266.enviar(idNodo)) {
    Serial.println(F("Error al enviar datos"));
    esp8266.desconectar();
    return false;
  }

  if (!esp8266.enviar(F("&json={"))) {
    Serial.println(F("Error al enviar datos"));
    esp8266.desconectar();
    return false;
  }

  //Marca la bandera para que el proximo dato a enviar no se separe del anterior
  //(que no existe) con una coma
  primerDato = true;

  return true;
}

bool enviarDato(const __FlashStringHelper *nombre, char *valor) {
  //Si la bandera de primer dato esta activa omite la coma, sino la anexa
  if (primerDato)
    primerDato = false;
  else {
    if (!esp8266.enviar(F(","))) {
      Serial.println(F("Error al enviar datos"));
      esp8266.desconectar();
      return false;
    }
  }

  //Envia el nombre del dato seguido de 2 puntos y luego el valor
  if (!esp8266.enviar(nombre)) {
    Serial.println(F("Error al enviar datos"));
    esp8266.desconectar();
    return false;
  }

  if (!esp8266.enviar(F(":"))) {
    Serial.println(F("Error al enviar datos"));
    esp8266.desconectar();
    return false;
  }

  if (!esp8266.enviar(valor)) {
    Serial.println(F("Error al enviar datos"));
    esp8266.desconectar();
    return false;
  }
}

bool terminarConexion() {
  unsigned long tInicial;

  //Envia el resto de la cabecera HTTP/GET
  if (!esp8266.enviar(F("} HTTP/1.1\r\nHost: "))) {
    Serial.println(F("Error al enviar datos"));
    esp8266.desconectar();
    return false;
  }

  if (!esp8266.enviar(FSH(servidor))) {
    Serial.println(F("Error al enviar datos"));
    esp8266.desconectar();
    return false;
  }

  if (!esp8266.enviar(F("\r\nConnection: close\r\n\r\n"))) {
    Serial.println(F("Error al enviar datos"));
    esp8266.desconectar();
    return false;
  }

  //Espera durante un segundo la respuesta del servidor y la imprime en terminal
  //serie
  tInicial = millis();
  while(millis() - tInicial < 1000) {
    if (serieWifi.available())
      Serial.write(serieWifi.read());
  }

  //Cierra la conexion (por si el servidor no la cerro)
  esp8266.desconectar();
  return true;
}

