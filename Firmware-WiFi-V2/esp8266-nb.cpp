#include "esp8266-nb.h"

//Descomentar para hacer que los datos seriales provenientes del esp8266 sean
//enviados a la terminal serie para depuracion
#define DEPURACION_SERIAL_ESP8266

//Macros usadas para generar los diversos mensajes de depuracion, en caso que se habiliten
#ifdef DEPURACION_SERIAL_ESP8266
#define depuracionMensaje(mensaje)   Serial.print(F("=== ")); Serial.print(F(mensaje)); Serial.println(F(" ==="));
#define depuracionComando(comando)   Serial.print(F("--> ")); Serial.print(comando);
static bool bLineaNueva = true;
#define depuracionCaracter(caracter) \
  if (bLineaNueva) { Serial.print(F("<-- ")); bLineaNueva = false; } \
  Serial.write(caracter); \
  if (caracter == '\n') bLineaNueva = true;
#else
#define depuracionMensaje(mensaje)
#define depuracionComando(comando)
#define depuracionCaracter(caracter)
#endif

//Macro usada para hacer el casting necesario para que las funciones que emplean el
//tipo __FlashStringHelper acepten correctamente las cadenas almacenadas en la
//memoria flash declaradas con PROGMEM
#define FSH(cadena_pgm) reinterpret_cast<const __FlashStringHelper *>(cadena_pgm)

//Cadenas constantes en memoria de programa que contienen los comandos AT para el ESP8266
static PROGMEM const char CMD_ATE0[]   = "ATE0\r\n";
static PROGMEM const char CMD_CWMODE[] = "AT+CWMODE=1\r\n";
static PROGMEM const char CMD_CWJAP[]  = "AT+CWJAP=\"";
static PROGMEM const char CMD_CIFSR[]  = "AT+CIFSR\r\n";

//Enumeracion de las respuestas del ESP8266 que se manejan
typedef enum {
  TRE_OK, TRE_READY, TRE_STAIP, TRE_STAMAC,
} TIPO_RESPUESTA_ESP8266;

//Estructura con las variables de estado del automata finito de control del modulo
typedef struct {
  //Estado del automata finito global que controla al ESP8266
  ESTADO_ESP8266 estado = EE_NO_INICIALIZADO;
  //Bandera que indica si el estado global del ESP8266 es reciente
  bool bEstadoNuevo = true;
  //Contador que indica el numero de tarea dentro del estado actual
  int pasoTarea = 0;
  //Bandera que indica si la accion de la tarea actual ya se realizo
  bool bAccionRelizada = false;
} ESTADO_AUTOMATA_CONTROL;
static ESTADO_AUTOMATA_CONTROL estadoControl;

//Enumeracion de los estados posibles del automata finito de recepcion
typedef enum {
  ECS_RESP_FIJA, ECS_DATO_IP, ECS_DATO_MAC, ECS_DATO_IPD,
} ESTADO_COM_SERIE;

//Cadenas constantes en memoria de programa que contienen las respuestas del ESP8266
static PROGMEM const char RSP_OK[]     = "\nOK\r\n";
static PROGMEM const char RSP_READY[]  = "\nready\r\n";
static PROGMEM const char RSP_STAIP[]  = "+CIFSR:STAIP,\"";
static PROGMEM const char RSP_STAMAC[] = "+CIFSR:STAMAC,\"";

//Estructura con la informacion general de las cadenas de respuesta
typedef struct  {
  TIPO_RESPUESTA_ESP8266 tipo;  //Codigo numerico que identifica la respuesta
  PROGMEM const char *cadena;   //Cadena de texto con el contenido identificable de la respuesta
  int longitud;                 //Longitud de la cadena anterior
  int posRecon;                 //Posicion (avance) de reconocimiento de la cadena
} INFO_RESPUESTA;

//Arreglo de estructuras con la coleccion de cadenas de respuesta
static const int totalResp = 4;
INFO_RESPUESTA infoResp[totalResp] = {
  { TRE_OK,     RSP_OK,     0, 0 },   //Nota: Las longitudes se calculan en el constructor de clase
  { TRE_READY,  RSP_READY,  0, 0 },
  { TRE_STAIP,  RSP_STAIP,  0, 0 },
  { TRE_STAMAC, RSP_STAMAC, 0, 0 },
};

//Tiempo maximo a esperar al ESP8266 para que conteste los comandos
//const unsigned long timeoutResp = 16000;  //Maximo de 16s
//Tiempo maximo a esperar para que el modulo termine de transmitir una respuesta
//const unsigned long timeoutCom = 500;     //Maximo de 0.5s (480 caracteres @ 9600,N,0)
//unsigned long tiempoEspera;

//Puntero al objeto de clase SoftwareSerial usado para enviar y recibir datos del ESP8266
static SoftwareSerial *swSerial;
//Numero de pin usado para habilitar/reiniciar el ESP8266
static int pinEn;
//Punteros a las cadenas con los datos de la red WiFi
static const __FlashStringHelper *ssid;
static const __FlashStringHelper *pass;
//Arreglos con las direcciones IP y MAC del ESP8266
static byte dirIP[4];
static byte dirMAC[6];

//Estado del automata finito que maneja la recepcion de datos desde el ESP8266
static ESTADO_COM_SERIE estadoComSerie = ECS_RESP_FIJA;
//Bandera que indica si se ha recibido una respuesta recientemente
static bool bHayRespuesta = false;
//Tipo de la ultima respuesta recibida del ESP8266
static TIPO_RESPUESTA_ESP8266 ultimaRespuesta;
//Contador que apunta al octeto que se esta actualizando de las direcciones MAC e IP
static byte numOcteto;

//Arreglo usado para diferentes operaciones que involucran volumenes de datos extensos con el ESP8266
//(Principalmente usado para almacenar las respuestas +IPD, que contienen datos entrantes)
//Es de notar que si entran mas datos de los que puede contener el arreglo, los mismos se perderan
static byte bufferDatos[512];

static void reiniciar();
static void cambiarEstado(ESTADO_ESP8266 nuevoEstado);
static void reiniciarBusqueda();
static void enviarComando(const __FlashStringHelper *comando);
static void enviarComando(const char *comando);

void esp8266_inicializar(SoftwareSerial *puertoSerie, int pinEnable) {
  //Se copian los argumentos a los miembros de clase
  swSerial = puertoSerie;
  pinEn = pinEnable;

  //Calcula las longitudes de todas las cadenas y las guarda en el mismo arreglo de estructuras
  for (int i = 0; i < totalResp; i++)
    infoResp[i].longitud = strlen_P(infoResp[i].cadena);

  //Inicializa el pin de enable del ESP8266
  pinMode(pinEn, OUTPUT);
  digitalWrite(pinEn, HIGH);

  //Inicializa el puerto serie por software conectado al ESP8266
  swSerial->begin(9600);
}

void esp8266_modoCliente(const __FlashStringHelper *nuevo_ssid, const __FlashStringHelper * nuevo_pass) {
  //Como unicamente el modo cliente es soportado de momento, simplemente se copian los datos del AP
  //a las variables miembro
  ssid = nuevo_ssid;
  pass = nuevo_pass;
}

bool esp8266_conectar(const __FlashStringHelper *url, unsigned int puerto) {

}

bool esp8266_desconectar() {

}

bool esp8266_enviar(const __FlashStringHelper *cadena) {

}

bool esp8266_enviar(char *cadena) {

}

bool esp8266_enviar(byte *datos, unsigned int longitud) {

}

bool esp8266_hacerTareas() {
  int i;
  char caracter;

  //La siguiente accion se determina en base al estado actual del modulo
  switch (estado) {
    case EE_NO_INICIALIZADO:
      if (bEstadoNuevo)
        bEstadoNuevo = false;
      //En caso que el modulo no este inicializado, se inicializa
      switch (pasoTarea) {
        case 0:                                     //Paso 0: Pulsar el bajo el pin de Enable
          if (!bAccionRelizada) {
            depuracionMensaje("Reiniciando ESP8266")
            digitalWrite(pinEn, LOW);
            delay(1);
            digitalWrite(pinEn, HIGH);
            bAccionRelizada = true;
          }

          if (bHayRespuesta) {
            if (ultimaRespuesta == TRE_READY)       //Se espera la respuesta "ready"
              pasoTarea++;
            else
              reiniciar();                          //Si no se obtuvo la respuesta se reinicia
            bAccionRelizada = false;
            bHayRespuesta = false;
          }
          break;
        case 1:                                     //Paso 1: Apagar el eco del modulo
          if (!bAccionRelizada) {
            depuracionMensaje("Apagando eco")
            enviarComando(FSH(CMD_ATE0));
            bAccionRelizada = true;
          }

          if (bHayRespuesta) {
            if (ultimaRespuesta == TRE_OK) {        //Se espera la respuesta "OK"
              cambiarEstado(EE_WIFI_ABAJO);
              pasoTarea = 0;
            }
            else
              reiniciar();                          //Si no se obtuvo la respuesta se reinicia
            bAccionRelizada = false;
            bHayRespuesta = false;
          }
          break;
      }
      break;
    case EE_WIFI_ABAJO:
      if (bEstadoNuevo)
        bEstadoNuevo = false;
      //En caso que el modulo este inicializado pero la conexion WiFi esta abajo, se levanta
      switch (pasoTarea) {
        case 0:                                       //Paso 0: Cambiar a modo cliente (STA)
          if (!bAccionRelizada) {
            depuracionMensaje("Cambiando a modo cliente")
            enviarComando(FSH(CMD_CWMODE));
            bAccionRelizada = true;
          }

          if (bHayRespuesta) {
            if (ultimaRespuesta == TRE_OK)            //Se espera la respuesta "OK"
              pasoTarea++;
            else
              reiniciar();                            //Si no se obtuvo la respuesta se reinicia
            bAccionRelizada = false;
            bHayRespuesta = false;
          }
          break;
        case 1:                                           //Paso 1: Unirse a un AP
          if (!bAccionRelizada) {
            strcpy_P((char *) bufferDatos, CMD_CWJAP);    //Prepara la cadena con el comando en el buffer de datos
            strcat_P((char *) bufferDatos, (PGM_P) ssid);
            strcat_P((char *) bufferDatos, PSTR("\",\""));
            strcat_P((char *) bufferDatos, (PGM_P) pass);
            strcat_P((char *) bufferDatos, PSTR("\"\r\n"));

            depuracionMensaje("Conectando con AP")
            enviarComando((const char *) bufferDatos);
            bAccionRelizada = true;
          }

          if (bHayRespuesta) {
            if (ultimaRespuesta == TRE_OK)                //Se espera la respuesta "OK"
              pasoTarea++;
            else
              pasoTarea = 1;                              //Si no se obtuvo la respuesta intenta reconectarse
            bAccionRelizada = false;
            bHayRespuesta = false;
          }
          break;
        case 2:                                           //Paso 2: Solicitar las direcciones IP y MAC
          if (!bAccionRelizada) {
            depuracionMensaje("Obteniendo parametros de red")
            enviarComando(FSH(CMD_CIFSR));
            bAccionRelizada = true;
          }

          if (bHayRespuesta) {
            if (ultimaRespuesta == TRE_STAIP)             //Se espera la respuesta "+CIFSR:STAIP"
              pasoTarea++;
            else
              pasoTarea = 1;                              //Si no se obtuvo la respuesta intenta reconectarse
            bAccionRelizada = false;
            bHayRespuesta = false;
          }
          break;
        case 3:                                           //Paso 3: Corroborar que se recibio la MAC
          if (bHayRespuesta) {
            if (ultimaRespuesta == TRE_STAMAC)            //Se espera la respuesta "+CIFSR:STAMAC"
              pasoTarea++;
            else
              reiniciar();                                //Si no se obtuvo la respuesta se reinicia
            bAccionRelizada = false;
            bHayRespuesta = false;
          }
          break;
        case 4:                                           //Paso 4: Dar de alta y pasar al siguiente estado
          if (bHayRespuesta) {
            if (ultimaRespuesta == TRE_OK) {              //Se espera la respuesta "OK"
              cambiarEstado(EE_WIFI_ARRIBA);
              pasoTarea = 0;
            }
            else
              reiniciar();                                //Si no se obtuvo la respuesta se reinicia
            bAccionRelizada = false;
            bHayRespuesta = false;
          }
          break;
      }
      break;
    case EE_WIFI_ARRIBA:
      if (bEstadoNuevo)
        bEstadoNuevo = false;
      break;
    case EE_CONECTADO_TCP:
      if (bEstadoNuevo)
        bEstadoNuevo = false;
      break;
  }

  //El siguiente codigo solo actua en respuesta a datos entrando por el puerto serie
  while (swSerial->available()) {
    caracter = swSerial->read();  //Toma el siguiente caracter
    depuracionCaracter(caracter)  //Lo copia a la terminal de depuracion

    switch (estadoComSerie) {
      case ECS_RESP_FIJA:
        //Para respuestas fijas (o paquetes de datos, los cuales siempre comienzan con una
        //parte fija), se compara el caracter entrante con todas las secuencias registradas
        //en el arreglo de estructuras con las respuestas
        for (i = 0; i < totalResp; i++) {
          if (caracter == pgm_read_byte_near(infoResp[i].cadena + infoResp[i].posRecon)) {
            //Si el caracter coincide con el caracter de la respuesta ubicado en la posicion
            //de reconocimiento, se procede a registrarlo
            infoResp[i].posRecon++;   //Avanza al siguiente caracter reconocido
            if (infoResp[i].posRecon == infoResp[i].longitud) {
              //Si se alcanza el final de la cadena, la respuesta se da por reconocida completamente
              ultimaRespuesta = infoResp[i].tipo;
              //La siguiente accion depende del tipo de respuesta
              switch (ultimaRespuesta) {
                case TRE_STAIP:
                  //Si se recibio el inicio de la respuesta "+CIFSR:STAIP" se procede
                  //a capturar la direccion IP
                  estadoComSerie = ECS_DATO_IP;
                  dirIP[0] = 0;   //Limpia el primer octeto
                  numOcteto = 0;  //Apunta al primer octeto
                  break;
                case TRE_STAMAC:
                  //Si se recibio el inicio de la respuesta "+CIFSR:STAMAC" se procede
                  //a capturar la direccion MAC
                  estadoComSerie = ECS_DATO_MAC;
                  dirMAC[0] = 0;
                  numOcteto = 0;
                  break;
                default:
                  reiniciarBusqueda();
                  bHayRespuesta = true;
                  break;
              }
              break;    //Abandona la busqueda pues ya se encontro una coincidencia
            }
          }
          else
            //Si el caracter no coincide, se reinicia la posicion de reconocimiento
            infoResp[i].posRecon = 0;
        }
        break;
      case ECS_DATO_IP:
        //Para respuestas de dato de IP, se actua de acuerdo al tipo de caracter recibido
        if (caracter >= '0' && caracter <= '9') {
          //Si se recibio un digito del 0 al 9, se anexa al octeto actual
          //Notese que al multiplicar el octeto actual por 10, se consigue efectivamente
          //desplazar los digitos decimales a la izquierda
          dirIP[numOcteto] = dirIP[numOcteto] * 10 + (caracter - '0');
        }
        else if (caracter == '.') {
          //Si se recibio un punto se avanza al siguiente octeto, al mismo tiempo que se limpia
          if (numOcteto < 3) numOcteto++;
          dirIP[numOcteto] = 0;
        }
        else if (caracter == '"') {
          //Al recibir la comilla doble de cierre, se reinicia toda busqueda y se marca la respuesta
          //por recibida. Notese que la comilla doble de apertura se encuentra registrada dentro de la
          //parte fija de la respuesta, por lo que no se considera aqui
          reiniciarBusqueda();
          bHayRespuesta = true;
          estadoComSerie = ECS_RESP_FIJA;
        }
        break;
      case ECS_DATO_MAC:
        if (caracter >= '0' && caracter <= '9') {
          //Si se recibio un digito del 0 al 9, se anexa al octeto actual
          //Notese que esta vez desplazar 4 bits a la izquierda equivale a desplazar un digito
          //hexadecimal a la izquierda
          dirMAC[numOcteto] = (dirMAC[numOcteto] << 4) + (caracter - '0');
        }
        else if (caracter >= 'A' && caracter <= 'F') {
          //Se actua de forma similar a los digitos decimales
          dirMAC[numOcteto] = (dirMAC[numOcteto] << 4) + (caracter - 'A' + 10);
        }
        else if (caracter >= 'a' && caracter <= 'f') {
          //Se actua de forma identica a las letras mayusculas
          dirMAC[numOcteto] = (dirMAC[numOcteto] << 4) + (caracter - 'a' + 10);
        }
        else if (caracter == ':') {
          //Si se recibio dos puntos se avanza al siguiente octeto, al mismo tiempo que se limpia
          if (numOcteto < 5) numOcteto++;
          dirMAC[numOcteto] = 0;
        }
        else if (caracter == '"') {
          //La direcion MAC tambien es terminada por una comilla doble de cierre
          reiniciarBusqueda();
          bHayRespuesta = true;
          estadoComSerie = ECS_RESP_FIJA;
        }
        break;
      case ECS_DATO_IPD:
        break;
    }
  }
}

void esp8266_leerIP(byte *direccionIP) {
  int i;
  for (i = 0; i < 4; i++) direccionIP[i] = dirIP[i];
}

void esp8266_leerMAC(byte *direccionMAC) {
  int i;
  for (i = 0; i < 6; i++) direccionMAC[i] = dirMAC[i];
}

ESTADO_ESP8266 esp8266_leerEstado() {
  return estado;
}

bool esp8266_estadoNuevo() {
  return bEstadoNuevo;
}

static void reiniciar() {
  cambiarEstado(EE_NO_INICIALIZADO);
  pasoTarea = 0;
  bAccionRelizada = false;
  estadoComSerie = ECS_RESP_FIJA;
  bHayRespuesta = false;
  reiniciarBusqueda();
  //TODO: Asegurarse de reiniciar todas las variables del automata
}

static void cambiarEstado(ESTADO_ESP8266 nuevoEstado) {
  estado = nuevoEstado;
  bEstadoNuevo = true;
}

static void reiniciarBusqueda() {
  for (int i = 0; i < totalResp; i++) {
    infoResp[i].posRecon = 0;
  }
}

static void enviarComando(const __FlashStringHelper * comando) {
  depuracionComando(comando)
  swSerial->print(comando);
}

static void enviarComando(const char *comando) {
  depuracionComando(comando)
  swSerial->print(comando);
}

