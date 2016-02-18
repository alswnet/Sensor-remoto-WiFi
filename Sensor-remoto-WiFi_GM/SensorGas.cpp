#include <Arduino.h>
#include "SensorGas.h"

const unsigned long tiempoPrec = 180000; //Tiempo de precalentamiento en milisengundos

//+--------------------------------+
//| Funciones publicas de la clase |
//+--------------------------------+
SensorGas::SensorGas(TIPO_SENSOR tipoSensor, int pinADC) {
  //El constructor simplemente copia los argumentos e inicializa el estado del calefactor
  //como inicialmente frio
  tSensor = tipoSensor;
  pin = pinADC;
  estadoCalefactor = EC_FRIO;
}

void SensorGas::atender() {
  switch (estadoCalefactor) {
    case EC_FRIO:
      //Si el sensor se encuentra frio (estado inicial) entonces se toma el tiempo
      //de inicio de precalentamiento y se pasa al estado correspondiente
      tInicioPrec = millis();
      estadoCalefactor = EC_PRECALENTAMIENTO;
      break;
    case EC_PRECALENTAMIENTO:
      //Una vez elapsa el tiempo de precalentado, se pasa al estado listo y se calibra
      //el sensor exactamente una vez
      if (millis() - tInicioPrec >= tiempoPrec) {
        estadoCalefactor = EC_LISTO;
        calibrar();
      }
      break;
    case EC_LISTO:
      //Una vez el sensor esta calibrado, no quedan mas tareas de mantenimiento
      break;
  }
}

bool SensorGas::estaListo() {
  if (estadoCalefactor == EC_LISTO) return true;
  else return false;
}

unsigned long SensorGas::leerPPM(TIPO_GAS tipoGas) {
  int i;
  float lecturaADC = 0.0;
  float Rs_RL;
  float Rs_Ro;
  float ppm;

  //Si se intenta leer el sensor mientras no esta listo, simplemente se retorna 0
  if (estadoCalefactor != EC_LISTO) return 0;

  //Toma 10 lecturas del ADC y las promedia
  for (i = 0; i < 10; i++) {
    lecturaADC += analogRead(pin);
    delay(10);
  }
  lecturaADC /= 10.0;

  //Determina la relacion de valor entre la resistencia del sensor y la carga
  //a partir de la tension
  //Rs / RL = Vs / VL
  //        = (5V - VL) / VL
  //Sufijos:
  // L - Resistencia de carga
  // s - sensor de gas
  Rs_RL = (1023.0 - lecturaADC) / lecturaADC;

  //Determina la relacion Rs/Ro para la lectura (Ro_RL contiene el valor
  //de calibracion)
  Rs_Ro = Rs_RL / Ro_RL;

  //Obtiene las partes por millon haciendo una aproximacion lineal basado en
  //las graficas de las especificaciones
  switch (tSensor) {
    case MQ2:
      switch (tipoGas) {
        case HUMO: ppm = calcularPPM(Rs_Ro, &curvaHumoMQ2); break;
      }
      break;
  }

/*
  //Descomentar para depuracion solamente
  Serial.println("");
  Serial.print("ADC: ");
  Serial.print(lecturaADC);
  Serial.print("   Rs/RL = ");
  Serial.print(Rs_RL);
  Serial.print("   Rs/Ro = ");
  Serial.print(Rs_Ro);
  Serial.print("   ppm = ");
  Serial.println(ppm);
*/

  return ppm;
}

//+--------------------------------+
//| Funciones privadas de la clase |
//+--------------------------------+
void SensorGas::calibrar() {
  int i;
  float lecturaADC = 0.0;
  float Rs_RL;
  float Rs_Ro_Aire;

  //Toma 100 lecturas del ADC y las promedia
  for (i = 0; i < 100; i++) {
    lecturaADC += analogRead(pin);
    delay(10);
  }
  lecturaADC /= 100.0;

  //Determina la relacion de valor entre la resistencia del sensor y la carga
  Rs_RL = (1023.0 - lecturaADC) / lecturaADC;

  //Selecciona el factor Rs/Ro del aire segun el tipo de sensor
  //(ver grafica segun el tipo de sensor)
  switch (tSensor) {
    case MQ2: Rs_Ro_Aire = 9.4; break;
  }

  //Determina el factor Ro/RL para este sensor (Comunmente llamado "Ro", aqui no se
  //omite RL)
  Ro_RL = Rs_RL / Rs_Ro_Aire;
}

float SensorGas::calcularPPM(float Rs_Ro, const CURVA_SENSOR *curva) {
  //Para determinar la cantidad correcta de partes por millon, se hace una aproximacion
  //lineal en la grafica log/log del sensor.
  //Dada la naturaleza logaritmica del grafico, se obtiene primero el logaritmo base 10
  //de la relacion Rs/Ro para ubicarla en el el eje vertical, luego se aplica la ecuacion
  //de linea recta de 2 puntos para obtener la posicion en el eje horizontal de partes por
  //millon, pero como se encuentra en escala logaritmica, se eleva 10 a la potencia de esa
  //posicion para obtener las partes por millon en escala lineal nuevamente.
  return pow(10.0, (log10(Rs_Ro) - curva->y1)
                   * (curva->x2 - curva->x1) / (curva->y2 - curva->y1)
                   + curva->x1);
}
