#include <OneWire.h>

//Instancia de clase One Wire, con el DS18B20 (sensor de temp.)
//conectado al pin 10
OneWire ds(10);

//Direccion del sensor de temperatura (determinada durante
//la ejecucion del sketch)
byte dirSensorTemp[8];

void inicializarDS18B20() {
  //Busca posibles sensores de temperatura conectados, si
  //existe uno, guarda su direccion
  if (!ds.search(dirSensorTemp)) {
    Serial.println("Sensor no encontrado");
    for (;;);
  }

  //Establece la sensibilidad a 0.5 grados (tiempo maximo
  //de conversion de 93.75ms
  ds.reset();
  ds.select(dirSensorTemp);
  ds.write(0x4E);
  ds.write(0x00);
  ds.write(0x00);
  ds.write(0x1F);
}

float leerDS18B20() {
  int i;
  byte data[9];
  float temp;

  //Inicia una conversion del sensor de temperatura
  ds.reset();
  ds.select(dirSensorTemp);
  ds.write(0x44);

  //Espera a que la conversion termine (depende de la
  //resolucion usada)
  delay(100);

  //Envia el comando de lecura de scratchpad
  ds.reset();
  ds.select(dirSensorTemp);
  ds.write(0xBE);

  //Se procede a leer 9 bytes
  for ( i = 0; i < 9; i++)
    data[i] = ds.read();

  //Se calcula la temperatura obtenida
  temp = (data[0] + data[1]*256) / 16.0;

  //Retorna el valor obtenido
  return temp;
}
