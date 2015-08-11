//Monitor remoto basado en Arduino Uno, usando el medidor de multi parametros
//electricos EKM OmniMeter I v.3, conectado a traves de un convertidor de nivel
//RS-485 (MAX485, MAX3483, etc.)

#include <SPI.h>
#include <Ethernet.h>
#include <SoftwareSerial.h>
#include "EKM_Omnimeter_I_v3.h"

//--------------------------------------------------------------------------------
//INICIO DE LOS PARAMETROS CONFIGURABLES

//Configuraciones del adaptador de red
byte mac[] = { 0x18, 0xFE, 0x34, 0xFE, 0x3B, 0xD1 };
IPAddress ip(192, 168, 1, 100);

//Configuraciones del servidor remoto
//const char servidor[] = "10.42.0.84";
const char servidor[] = "45.40.135.188";
const int puertoServidor = 80;
//PROGMEM const char apikey[] = "f0eab95e4073c12a90efe523b242cbe2";
PROGMEM const char apikey[] = "aa9e208a81b9ae180044d3da9c08dfae";
PROGMEM const char nodo[] = "Alimentos_MOR";

//Definicion de los pines conectados al omnimeter via convertidor de
//nivel RS-485
const int pinRX = 2;
const int pinTX = 3;
const int pinDERE = 4;

//Definiciones de otros pines
const int pinLedComunicacion = 5;

//FIN DE LOS PARAMETROS CONFIGURABLES
//--------------------------------------------------------------------------------

//Macro usada para hacer el casting necesario para que las funciones que emplean el
//tipo __FlashStringHelper acepten correctamente las cadenas almacenadas en la
//memoria flash declaradas con PROGMEM
#define FSH(cadena_pgm) reinterpret_cast<const __FlashStringHelper *>(cadena_pgm)

//Instancias de clase del puerto serie por software y medidor omnimeter
SoftwareSerial serieOmnimeter(pinRX, pinTX);
OMNIMETER medidor(&serieOmnimeter, pinDERE);

//Instancia de clase de cliente de ethernet
EthernetClient cliente;

void setup() {
  pinMode(pinLedComunicacion, OUTPUT);

  Serial.begin(9600);
  serieOmnimeter.begin(9600);

  for (;;) {
    Serial.println(F("Configurando red y obteniendo IP..."));
    if (Ethernet.begin(mac)) break;
    Serial.println(F("No se pudo obtener direccion de red, reintentando"));
  }

  Serial.print(F("Direccion de red: "));
  Serial.println(Ethernet.localIP());
}

void loop() {
  if (!medidor.actualizar()) {
    Serial.println(F("Error al leer el medidor"));
    delay(1000);
    return;
  }

  mostrarMediciones();
  digitalWrite(pinLedComunicacion, HIGH);
  enviarMediciones();
  digitalWrite(pinLedComunicacion, LOW);

  delay(5000);
}

void mostrarMediciones() {
  char cadena[16];

  medidor.leerParametro(cadena, PO_KWH_TOT);
  Serial.print(F("KWH_TOT: "));
  Serial.print(cadena);

  medidor.leerParametro(cadena, PO_P_TOT);
  Serial.print(F(" - P_TOT: "));
  Serial.println(cadena);

  medidor.leerParametro(cadena, PO_V1);
  Serial.print(F("V1: "));
  Serial.print(cadena);

  medidor.leerParametro(cadena, PO_A1);
  Serial.print(F(" - A1: "));
  Serial.print(cadena);

  medidor.leerParametro(cadena, PO_V2);
  Serial.print(F("V2: "));
  Serial.print(cadena);

  medidor.leerParametro(cadena, PO_A2);
  Serial.print(F(" - A2: "));
  Serial.print(cadena);
  
  medidor.leerParametro(cadena, PO_V3);
  Serial.print(F("V3: "));
  Serial.print(cadena);

  medidor.leerParametro(cadena, PO_A3);
  Serial.print(F(" - A3: "));
  Serial.print(cadena);
  
  medidor.leerParametro(cadena, PO_PF1);
  Serial.print(F(" - PF1: "));
  Serial.println(&cadena[1]);
}

void enviarMediciones() {
  char cadena[16];

  //Muestra un mensaje informativo de conexion
  Serial.print(F("Conectando con "));
  Serial.print(servidor);
  Serial.print(F(" ... "));

  //Se intenta conectar al servidor
  if (cliente.connect(servidor, puertoServidor)) {
    //Si la conexion fue exitosa, se notifica por terminal serie
    Serial.println(F("OK"));

    //Se envia la cadena de solicitud de pagina con los datos a enviar
    cliente.print(F("GET /emoncms/input/post.json?apikey="));
    cliente.print(FSH(apikey));             //Anexa la apikey del usuario
    cliente.print(F("&node="));
    cliente.print(FSH(nodo));               //Anexa el identificador de nodo
    cliente.print(F("&json={"));

    //Toma las mediciones del omnimeter y las anexa una a una a la solicitud HTTP
    medidor.leerParametro(cadena, PO_KWH_TOT);
    enviarParametro(F("KWH_TOT"), cadena, true);
    medidor.leerParametro(cadena, PO_P_TOT);
    enviarParametro(F("P_TOT"), cadena, false);
    medidor.leerParametro(cadena, PO_V1);
    enviarParametro(F("V1"), cadena, false);
    medidor.leerParametro(cadena, PO_A1);
    enviarParametro(F("A1"), cadena, false);
    medidor.leerParametro(cadena, PO_V2);
    enviarParametro(F("V2"), cadena, false);
    medidor.leerParametro(cadena, PO_A2);
    enviarParametro(F("A2"), cadena, false);
    medidor.leerParametro(cadena, PO_V3);
    enviarParametro(F("V3"), cadena, false);
    medidor.leerParametro(cadena, PO_A3);
    enviarParametro(F("A3"), cadena, false);
    medidor.leerParametro(cadena, PO_PF1);
    enviarParametro(F("PF1"), &cadena[1], false);

    //Finaliza la solicitud HTTP
    cliente.println(F("} HTTP/1.1"));
    cliente.print(F("Host: "));
    cliente.println(servidor);
    cliente.println(F("Connection: close"));
    cliente.println();
  }
  else
    //Si no hubo exito al conectar, se notifica por terminal serie
    Serial.println(F("Error"));

  //Se escucha la respuesta del servidor mientras la conexion esta abierta
  while (cliente.connected()) {
    //Se toman los datos del cliente mientras esten disponibles
    while (cliente.available()) {
      //Se imprimen por terminal serie los datos recibidos
      Serial.write(cliente.read());
    }
  }

  //Separa la respuesta con 2 fines de linea
  Serial.println();
  Serial.println();

  //Cierra el cliente (la conexion ya fue cerrada)
  cliente.stop();
}

void enviarParametro(const __FlashStringHelper *nombre, char *valor, bool inicial)
{
  //Funcion helper usada para formatear la lista JSON con los parametros de
  //medicion
  if (!inicial) cliente.print(',');
  cliente.print(nombre);
  cliente.print(':');
  cliente.print(valor);
}
