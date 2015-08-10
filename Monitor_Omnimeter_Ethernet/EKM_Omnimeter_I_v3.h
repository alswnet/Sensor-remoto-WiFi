#ifndef EKM_Omnimeter_I_v3_h_Incluida
#define EKM_Omnimeter_I_v3_h_Incluida

#include <Arduino.h>
#include <SoftwareSerial.h>

//Enumeracion de los parametros manejados por el medidor
typedef enum {
  PO_KWH_TOT, PO_KWH_REV,
  PO_V1, PO_V2, PO_V3,
  PO_A1, PO_A2, PO_A3,
  PO_P1, PO_P2, PO_P3, PO_P_TOT,
  PO_PF1, PO_PF2, PO_PF3,
  PO_MAX_DMD,
  PO_ANO, PO_MES, PO_FECHA, PO_DIA,
  PO_HORA, PO_MINUTO, PO_SEGUNDO,
} PARAMETRO_OMNIMETER;

class OMNIMETER {
  public:
    //El constructor requiere un puntero a un puerto serie por software
    //(el cual debe ser creado e inicializado por el usuario) y el
    //numero de pin que controla la direccion de transferencia
    //(transmision o recepcion) del convertidor de nivel MAX3483
    OMNIMETER(SoftwareSerial *puertoSerie, int pinDireccion);
    //Funcion de comunicacion y actualizacion de parametros (lecturas)
    //del omnimeter
    bool actualizar();
    //Funcion usada para retirar los parametros del omnimeter. Se
    //devuelven las cadenas de texto literales tal como las transmite el
    //medidor
    void leerParametro(char *cadena, PARAMETRO_OMNIMETER parametro);
  private:
    //Funcion de transmision de bytes al medidor
    void txByte(byte dato);
    //Funcion de calculo de CRC
    word CRC16(byte Dato, word ValorCRC);
    //Funcion de extraccion de cadenas del buffer
    void copiarNumeroBuffer(char *cadena, byte offset, byte nDig,
                            byte nDec);

    //Puntero a la instancia de puerto serie por software para
    //comunicarse con el Omnimeter (via convertidor RS-485)
    SoftwareSerial *swSerial;
    //Pin de control de direccion de envio de datos mediante RS-485
    int pinDir;
    //Buffer de recepcion de datos (contiene 255 bytes para un reporte
    //completo del EKM-Omnimeter I v.3)
    byte buffer[255];
};

#endif //EKM_Omnimeter_I_v3_h_Incluida
