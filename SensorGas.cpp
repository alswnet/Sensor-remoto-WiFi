#include <Arduino.h>
#include "SensorGas.h"

const float Rs_Ro_Aire[] = { 9.4 };
const unsigned long tiempoPrec = 3 * 60 * 1000; //Tiempo de precalentamiento en milisengundos

SensorGas::SensorGas(TIPO_SENSOR tipo, int pinADC) {
  miTipo = tipo;
  miPinADC = pinADC;
  estadoCalefactor = EC_FRIO;
  calibradoListo = false;
}

void SensorGas::precalentar() {
  if (estadoCalefactor == EC_FRIO) {
    tInicioPrec = millis();
    estadoCalefactor = EC_PRE_CALENTANDO;
  }
}

bool SensorGas::estaCalentado() {
  switch (estadoCalefactor) {
    case EC_FRIO:
      return false;
      break;
    case EC_PRE_CALENTANDO:
      if (millis() - tInicioPrec >= tiempoPrec) {
        estadoCalefactor = EC_CALIENTE;
        return true;
      }
      else {
        return false;
      }
      break;
    case EC_CALIENTE:
      return true;
      break;
  }
}

void SensorGas::calibrar() {
  int i;
  float lecturaADC = 0.0;
  float Rs_RL;

  //Toma 100 lecturas del ADC y las promedia
  for (i = 0; i < 100; i++) {
    lecturaADC += analogRead(miPinADC);
    delay(10);
  }
  lecturaADC /= 100.0;

  Rs_RL = (1023.0 - lecturaADC) / lecturaADC;
  Ro_RL = Rs_RL / Rs_Ro_Aire[miTipo];

  calibradoListo = true;
}

bool SensorGas::estaCalibrado() {
  return calibradoListo;
}

unsigned long SensorGas::leerPPM() {
  int i;
  float lecturaADC = 0.0;
  float Rs_RL;
  float Rs_Ro;

  //Toma 10 lecturas del ADC y las promedia
  for (i = 0; i < 10; i++) {
    lecturaADC += analogRead(miPinADC);
    delay(10);
  }
  lecturaADC /= 10.0;

  Rs_RL = (1023.0 - lecturaADC) / lecturaADC;
  Rs_Ro = Rs_RL / Ro_RL;

  return Rs_Ro*100;
}
