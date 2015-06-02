#include "esp8266.h"

//Tiempo maximo a esperar el modulo para que termine los comandos
const unsigned long timeout = 16000;  //Maximo de 16s

ESP8266::ESP8266(SoftwareSerial *puertoSerie)
{
  swSerial = puertoSerie;
}

bool ESP8266::reset() {
  //Envia el comando de inicializacion del modulo
  swSerial->print(F("AT+RST\r\n"));

  //Espera la respuesta "ready"
  if (!esperarRespuesta(F("\nready\r\n"))) return false;

  //Apaga el eco del modulo wifi
  swSerial->print(F("ATE0\r\n"));

  //Espera la respuesta "OK"
  if (!esperarRespuesta(F("\nOK\r\n"))) return false;

  //Si todo salio bien, retorna verdadero
  return true;
}

bool ESP8266::conectarAP(const __FlashStringHelper *ap,
                         const __FlashStringHelper* clave)
{
  //Envia el comando de conexion a access point, seguido del nombre del AP y
  //de la clave
  swSerial->print(F("AT+CWJAP=\""));
  swSerial->print(ap);
  swSerial->print(F("\",\""));
  swSerial->print(clave);
  swSerial->print(F("\"\r\n"));

  //Espera la respuesta "OK"
  if (!esperarRespuesta(F("\nOK\r\n"))) return false;

  //Envia el comando para obtener la direccion IP
  swSerial->print(F("AT+CIFSR\r\n"));

  //Espera la respuesta de direccion IP:
  if (!esperarRespuesta(F("+CIFSR:STAIP,\""))) return false;

  //Captura la direccion IP
  if (!capturarIP()) return false;

  //Espera la respuesta de direccion MAC:
  if (!esperarRespuesta(F("+CIFSR:STAMAC,\""))) return false;

  //Captura la direccion MAC
  if (!capturarMAC()) return false;

  //Espera la respuesta "OK"
  if (!esperarRespuesta(F("\nOK\r\n"))) return false;

  //Si todo salio bien, retorna verdadero
  return true;
}

bool ESP8266::conectar(const __FlashStringHelper *url, unsigned int puerto) {
  //Envia el comando de envio de datos, seguido del protocolo
  //(siempre se usa TCP), la url y el numero de puerto
  swSerial->print(F("AT+CIPSTART=\"TCP\",\""));
  swSerial->print(url);
  swSerial->print(F("\","));
  swSerial->print(puerto);
  swSerial->print(F("\r\n"));

  //Espera la respuesta "CONNECT" y luego "OK", devolviendo verdadero si ocurrio
  return esperarRespuesta(F("CONNECT\r\n\r\nOK\r\n"));

  //Nota: Solo para firmware antiguo del ESP8266
  //Espera la respuesta "Linked" y retorna el resultado
  //return esperarRespuesta(F("\nLinked\r\n"));
}

bool ESP8266::desconectar() {
  //Envia el comando de cierre de conexion
  swSerial->print(F("AT+CIPCLOSE\r\n"));

  //Espera la respuesta "CLOSED" y luego "OK", devolviendo verdadero si ocurrio
  //return esperarRespuesta(F("CLOSED\r\n\r\nOK\r\n"));
  delay(200);
  return true;

  //Nota: Solo para firmware antiguo del ESP8266
  //Espera la respuesta "UnLink" y retorna el resultado
  //return esperarRespuesta(F("\r\nUnlink\r\n"));
}

bool ESP8266::enviar(const __FlashStringHelper *cadena) {
  //Calcula la longtud de la cadena a enviar
  unsigned int longitud = strlen_P((const char *)cadena);

  //Envia el comando AT+CIPSEND seguido de la cantidad de bytes
  swSerial->print(F("AT+CIPSEND="));
  swSerial->print(longitud);
  swSerial->print(F("\r\n"));

  //Espera la respuesta "> ", si no la recibe retorna falso
  if (!esperarRespuesta(F("> "))) return false;

  //Envia la cadena de texto
  swSerial->print(cadena);

  //Espera la respuesta "SEND OK" y retorna el resultado
  return esperarRespuesta(F("\nSEND OK\r\n"));
}

bool ESP8266::enviar(char *cadena) {
  //Calcula la longtud de la cadena a enviar
  unsigned int longitud = strlen(cadena);

  //Envia el comando AT+CIPSEND seguido de la cantidad de bytes
  swSerial->print(F("AT+CIPSEND="));
  swSerial->print(longitud);
  swSerial->print(F("\r\n"));

  //Espera la respuesta "> ", si no la recibe retorna falso
  if (!esperarRespuesta(F("> "))) return false;

  //Envia la cadena de texto
  swSerial->print(cadena);

  //Espera la respuesta "SEND OK" y retorna el resultado
  return esperarRespuesta(F("\nSEND OK\r\n"));
}

bool ESP8266::enviar(byte *datos, unsigned int longitud) {
  unsigned int i;

  //Envia el comando AT+CIPSEND seguido de la cantidad de bytes
  swSerial->print(F("AT+CIPSEND="));
  swSerial->print(longitud);
  swSerial->print(F("\r\n"));

  //Espera la respuesta "> ", si no la recibe retorna falso
  if (!esperarRespuesta(F("> "))) return false;

  //Envia el paquete de datos
  for (i = 0; i < longitud; i++)
    swSerial->write(datos[i]);

  //Espera la respuesta "SEND OK" y retorna el resultado
  return esperarRespuesta(F("\nSEND OK\r\n"));
}

void ESP8266::leerIP(byte *direccionIP) {
  byte i;

  for (i=0; i<4; i++)
    direccionIP[i] = dirIP[i];
}

void ESP8266::leerMAC(byte *direccionMAC) {
  byte i;

  for (i=0; i<6; i++)
    direccionMAC[i] = dirMAC[i];
}

bool ESP8266::esperarRespuesta(const __FlashStringHelper *resp) {
  const char *cadResp = (const char *) resp;
  unsigned int longitud;
  unsigned int pResp = 0;
  unsigned long tInicial = millis();
  char c;

  //Determina la longitud de la respuesta
  longitud = strlen_P(cadResp);

  //Espera hasta que se reciba toda la respuesta
  while (pResp < longitud) {
    //Verifica si hay datos disponibles
    if (swSerial->available()) {
      //Si los hay, toma el siguiente
      c = swSerial->read();

      Serial.write(c);  //Depuracion solamente

      //Verifica si coincide con el siguiente en la cadena
      if (c == pgm_read_byte(cadResp + pResp))
        //De ser asi, apunta al siguiente caracter
        pResp++;
      else
        //Caso contrario, apunta del regreso al inicio
        //(la cadena no coincide)
        pResp = 0;
    }
    //Si elapso el tiempo maximo de envio, se abandona el lazo
    else if (millis() - tInicial > timeout) break;
  }

  //Si al terminar se recibieron todos los caracteres,
  //retornara verdadero (exito) caso contrario retornara falso
  if (pResp == longitud) return true;
  else return false;
}

bool ESP8266::capturarIP() {
  byte pDirIP = 0;
  char caracter;
  char strValor[4];  //Valor numerico de un octeto (3 caracteres + 0 terminador)
  byte pValor = 0;
  unsigned long tInicial = millis();

  //Lee los octetos de la direccion IP de uno en uno, caracter por caracter
  while (pDirIP < 4) {
    if (swSerial->available()) {
      //Toma el caracter del buffer de recepcion
      caracter = swSerial->read();
  
      //Si el caracter es un digito numerico, lo almacena en la cadena de valor
      //(siempre que haya espacio)
      if (caracter >= '0' && caracter <= '9') {
        if (pValor < sizeof(strValor) - 1)
          strValor[pValor++] = caracter;
      }
      //Si el caracter es un punto o bien una comilla doble, procesa el valor
      //numerico almacenado
      else if (caracter == '.' || caracter == '\"') {
        //Agrega un cero terminador a la cadena con el valor del octeto
        strValor[pValor] = 0;

        //Convierte el valor almacenado a un dato tipo entero y lo guarda
        //en el siguiente octeto de la IP
        dirIP[pDirIP++] = strtol(strValor, NULL, 10);

        //Apunta la cadena de valor al principio, para capturar un siguiente
        //octeto
        pValor = 0;
      }
      //Cualquier caracter inesperado provocara que se abandone el lazo
      else break;
    }
    //Si elapso el tiempo maximo de envio, se abandona el lazo
    else if (millis() - tInicial > timeout) break;
  }

  //Si al terminar se recibieron todos los 4 octetos, se retorna verdadero
  if (pDirIP == 4) return true;
  else return false;
}

bool ESP8266::capturarMAC() {
  byte pDirMAC = 0;
  char caracter;
  char strValor[3];  //Valor numerico de un octeto (2 caracteres + 0 terminador)
  byte pValor = 0;
  unsigned long tInicial = millis();

  //Lee los octetos de la direccion IP de uno en uno, caracter por caracter
  while (pDirMAC < 6) {
    if (swSerial->available()) {
      //Toma el caracter del buffer de recepcion
      caracter = swSerial->read();

      //Si el caracter es un digito numerico hexadecimal, lo almacena en la cadena
      //de valor (siempre que haya espacio)
      if ((caracter >= '0' && caracter <= '9') ||
          (caracter >= 'a' && caracter <= 'f'))
      {
        if (pValor < sizeof(strValor) - 1)
          strValor[pValor++] = caracter;
      }
      //Si el caracter es un punto o bien una comilla doble, procesa el valor
      //numerico almacenado
      else if (caracter == ':' || caracter == '\"') {
        //Agrega un cero terminador a la cadena con el valor del octeto
        strValor[pValor] = 0;

        //Convierte el valor almacenado a un dato tipo entero y lo guarda
        //en el siguiente octeto de la IP
        dirMAC[pDirMAC++] =  strtol(strValor, NULL, 16);

        //Apunta la cadena de valor al principio, para capturar un siguiente
        //octeto
        pValor = 0;
      }
      //Cualquier caracter inesperado provocara que se abandone el lazo
      else break;
    }
    //Si elapso el tiempo maximo de envio, se abandona el lazo
    else if (millis() - tInicial > timeout) break;
  }

  //Si al terminar se recibieron todos los 4 octetos, se retorna verdadero
  if (pDirMAC == 6) return true;
  else return false;
}
