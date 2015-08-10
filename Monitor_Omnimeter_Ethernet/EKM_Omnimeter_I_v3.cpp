#include <Arduino.h>
#include <SoftwareSerial.h>
#include "EKM_Omnimeter_I_v3.h"

//+--------------------------------+
//| Funciones publicas de la clase |
//+--------------------------------+-----------------------------------
OMNIMETER::OMNIMETER(SoftwareSerial *puertoSerie, int pinDireccion) {
  //El constructor simplemente copia los parametros a los miembros de
  //clase e inicializa el pin de direccion
  swSerial = puertoSerie;
  pinDir = pinDireccion;
  pinMode(pinDireccion, OUTPUT);
  digitalWrite(pinDir, LOW);
}

bool OMNIMETER::actualizar() {
  byte pBuffer;         //Puntero al buffer de datos
  unsigned long t_ini;  //Tiempo inicial de las transacciones
  word CRC;             //Checksum CRC calculado localmente

  //Coloca el MAX3483 en modo de transmision
  digitalWrite(pinDir, HIGH);
  //Envia los caracteres "/?" (inicio de la trama de solicitud)
  txByte('/');
  txByte('?');
  //Envia 12 veces el caracter '9' (direccion general para toda unidad)
  for (byte i=0; i<12; i++)
    txByte('9');
  //Envia un signo de admiracion para completar la solocitud
  txByte('!');
  //Completa la trama con un fin y retorno de linea
  txByte(0x0D);
  txByte(0x0A);
  //Finalmente cambia el MAX3483 a modo recepcion
  digitalWrite(pinDir, LOW);

  //Limpia el buffer de recepcion
  for (byte i=0; i<255; i++) buffer[i] = 0;

  //Se prepara la recepcion de la respuesta
  pBuffer = 0;           //Apunta al inicio del buffer
  t_ini = millis();      //Toma el tiempo inicial

  //Se reciben 255 bytes
  while (pBuffer < 255) {
    //Verifica si hay datos en el buffer del puerto
    if (swSerial->available())
      //Si los hay, los traslada al buffer de datos
      buffer[pBuffer++] = swSerial->read() & 0x7F;
      //Nota: la mascara permite eliminar el bit de paridad, esta
      //no se usa pues mas adelante se hace una comprobacion de
      //checksum.
    else if (millis() - t_ini >= 1000)
      //Si no hay, abandona la espera cuando elapse 1 segundo
      return false;
  }

  //Inicia el calculo de CRC con valor inicial de todos los bit en 1
  CRC = 0xFFFF;
  //Recorre todo el buffer calculando el CRC para cada dato,
  //exceptuando el primer byte y los ultimos 2 bytes
  for (byte i=1; i<253; i++)
    CRC = CRC16(buffer[i], CRC);
  //Enmascara el CRC para adaptarlo a 2 palabras de 7 bits
  CRC &= 0x7F7F;

  //Si el CRC calculado coincide con los ultimos 2 bytes del buffer
  //(datos recibidos) retorna verdadero. Caso contrario retorna falso.
  if (CRC == (buffer[253] | (buffer[254] << 8)))
    return true;
  else
    return false;
}

void OMNIMETER::leerParametro(char *cadena,
                              PARAMETRO_OMNIMETER parametro)
{
  //Copia el parametro requerido desde el buffer hacia la cadena
  //provista por el usuario. Diferentes parametros tienen diferentes
  //resoluciones, por lo que se especifican la cantidad de digitos
  //antes y despues del punto decimal (el medidor transmite solamente
  //los digitos, no los puntos).
  switch (parametro) {
    case PO_KWH_TOT: copiarNumeroBuffer(cadena, 0x10, 7, 1); break;
    case PO_KWH_REV: copiarNumeroBuffer(cadena, 0x38, 7, 1); break;
    case PO_V1:      copiarNumeroBuffer(cadena, 0x60, 3, 1); break;
    case PO_V2:      copiarNumeroBuffer(cadena, 0x64, 3, 1); break;
    case PO_V3:      copiarNumeroBuffer(cadena, 0x68, 3, 1); break;
    case PO_A1:      copiarNumeroBuffer(cadena, 0x6C, 4, 1); break;
    case PO_A2:      copiarNumeroBuffer(cadena, 0x71, 4, 1); break;
    case PO_A3:      copiarNumeroBuffer(cadena, 0x76, 4, 1); break;
    case PO_P1:      copiarNumeroBuffer(cadena, 0x7B, 7, 0); break;
    case PO_P2:      copiarNumeroBuffer(cadena, 0x82, 7, 0); break;
    case PO_P3:      copiarNumeroBuffer(cadena, 0x89, 7, 0); break;
    case PO_P_TOT:   copiarNumeroBuffer(cadena, 0x90, 7, 0); break;
    case PO_PF1:     copiarNumeroBuffer(cadena, 0x97, 2, 2); break;
    case PO_PF2:     copiarNumeroBuffer(cadena, 0x9B, 2, 2); break;
    case PO_PF3:     copiarNumeroBuffer(cadena, 0x9F, 2, 2); break;
    case PO_MAX_DMD: copiarNumeroBuffer(cadena, 0xA3, 7, 1); break;
    case PO_ANO:     copiarNumeroBuffer(cadena, 0xAC, 2, 0); break;
    case PO_MES:     copiarNumeroBuffer(cadena, 0xAE, 2, 0); break;
    case PO_FECHA:   copiarNumeroBuffer(cadena, 0xB0, 2, 0); break;
    case PO_DIA:     copiarNumeroBuffer(cadena, 0xB2, 2, 0); break;
    case PO_HORA:    copiarNumeroBuffer(cadena, 0xB4, 2, 0); break;
    case PO_MINUTO:  copiarNumeroBuffer(cadena, 0xB6, 2, 0); break;
    case PO_SEGUNDO: copiarNumeroBuffer(cadena, 0xB8, 2, 0); break;
  }
}

void OMNIMETER::copiarNumeroBuffer(char *cadena, byte offset,
                                   byte nDig, byte nDec)
{
  //Esta funcion copia los datos numericos provenientes del medidor
  //a la cadena provista. El offset indica la posicion del buffer de
  //donde se tomaran los datos, mientras que nDig indica los digitos
  //antes del punto (parte entera) y nDec indica los digitos despues
  //del punto (parte fraccionaria). La funcion intercala un punto
  //decimal si se especifican digitos despues del mismo (de nuevo, el
  //medidor no coloca puntos en su repore).

  //Posicion de escritura en la cadena de destino
  byte posCadena = 0;

  //Copia la parte entera del numero, segun la cantidad de digitos
  //solicitada
  for (byte i=0; i<nDig; i++)
    cadena[posCadena++] = buffer[offset++];

  if (nDec > 0) {
    //En caso que haya digitos decimales, agrega un punto
    cadena[posCadena++] = '.';

    //A continuacion copia la cantidad de decimales solicitada
    for (byte i=0; i<nDec; i++)
      cadena[posCadena++] = buffer[offset++];
  }

  //Agrega un cero terminador al final de la cadena
  cadena[posCadena] = 0;
}

//+--------------------------------+
//| Funciones privadas de la clase |
//+--------------------------------+-----------------------------------
void OMNIMETER::txByte(byte dato) {
  byte i;             //Conteo de bits
  byte paridad = 0;   //Bit de paridad inicialmente en 0 (paridad par)
  byte mascara = 0x01;//Mascara para aislar bits

  //Calcula el bit de paridad para los 7 bits del dato
  for (i=0; i<7; i++) {
    //Invierte el resultado por cada bit del dato que sea 1
    if (dato & mascara) paridad ^= 1;
    //Desplaza la mascara para aislar el siguiente bit
    mascara <<= 1;
  }

  //Sobrepone el bit de paridad al dato
  if (paridad) dato |= 0x80;
  else dato &= 0x7F;

  //Envia el dato con la paridad calculada
  swSerial->write(dato);
}

word OMNIMETER::CRC16(byte dato, word valorCRC) {
   const word poly = 0xA001;
   byte i;
   bool hacerXOR;

  //Realiza un calculo de CRC de 16 bits anexando un byte, por medio
  //del metodo de dato inicial directo y mediante el algoritmo de CRC
  //reflejado
  for (i=0; i<8; i++) {
    //Realiza un XOR del bit saliente del registro de CRC con el
    //dato entrante para determinar si se debe hacer una operacion XOR
    //mas adelante.
    hacerXOR = (valorCRC & 0x0001) ^ (dato & 0x01);
    //Notese que se usa el LSB (y no el MSB) del Valor de CRC puesto
    //que el algoritmo es el reflejado. Por otra parte, la direccion
    //de ingreso de los datos esta revertida (entran por el LSB)

    //Desplaza a la derecha el registro de CRC (algoritmo reflejado)
    valorCRC >>= 1;

    //Se realiza un XOR del registro CRC con el polinomio segun la
    //bandera (En terminos de CRC significa que el dividendo es
    //divisible entre el polinomio y se puede restar)
    if (hacerXOR) valorCRC ^= poly;

    //Se desplaza el dato a la derecha para poder obtener el siguiente
    //bit mediante el LSB
    dato >>= 1;
  }

  //Nota: Este algoritmo es directamente equivalente al normal si se
  //refleja el polinomio (se usa 0x8005 en vez de 0xA001), los
  //desplazamientos de CRC se realizan a la izquierda y al final se
  //refleja el resultado. La direccion de ingreso de los bits de datos
  //no debe cambiar (debe ser siempre revertida).

  return valorCRC;
}

