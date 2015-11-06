#ifndef esp8266_nb_h_Incluida
#define esp8266_nb_h_Incluida

#include <Arduino.h>
#include <SoftwareSerial.h>

typedef enum {
  EE_NO_INICIALIZADO, EE_WIFI_ABAJO, EE_WIFI_ARRIBA, EE_CONECTADO_TCP,
} ESTADO_ESP8266;

void esp8266_inicializar(SoftwareSerial *puertoSerie, int pinEnable);
void esp8266_modoCliente(const __FlashStringHelper *nuevo_ssid, const __FlashStringHelper * nuevo_pass);
bool esp8266_conectar(const __FlashStringHelper *url, unsigned int puerto);
bool esp8266_desconectar();
bool esp8266_enviar(const __FlashStringHelper *cadena);
bool esp8266_enviar(char *cadena);
bool esp8266_enviar(byte *datos, unsigned int longitud);

bool esp8266_hacerTareas();

void esp8266_leerIP(byte *direccionIP);
void esp8266_leerMAC(byte *direccionMAC);
ESTADO_ESP8266 esp8266_leerEstado();
bool esp8266_estadoNuevo();

#endif //esp8266_nb_h_Incluida

