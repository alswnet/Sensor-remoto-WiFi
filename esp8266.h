#ifndef esp8266_h_Incluida
#define esp8266_h_Incluida

#include <Arduino.h>
#include <SoftwareSerial.h>

class ESP8266 {
  public:
    ESP8266(SoftwareSerial *puertoSerie);
    bool reset();
    bool conectarAP(const __FlashStringHelper *ap,
                    const __FlashStringHelper* clave);
    bool conectar(const __FlashStringHelper *url, unsigned int puerto);
    bool desconectar();
    bool enviar(const __FlashStringHelper *cadena);
    bool enviar(char *cadena);
    bool enviar(byte *datos, unsigned int longitud);

    void leerIP(byte *direccionIP);
    void leerMAC(byte *direccionMAC);

  private:
    bool esperarRespuesta(const __FlashStringHelper *resp);
    bool capturarIP();
    bool capturarMAC();
    SoftwareSerial *swSerial;
    byte dirIP[4];
    byte dirMAC[6];
};

#endif //esp8266_h_Incluida
