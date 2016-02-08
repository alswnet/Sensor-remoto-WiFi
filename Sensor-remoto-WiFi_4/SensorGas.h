#ifndef SensorGas_h_Incluida
#define SensorGas_h_Incluida

//Tipos de sensor de gas soportados
enum TIPO_SENSOR {
  MQ2,
};

//Tipos de gases para los que hay curvas registradas
enum TIPO_GAS {
  HUMO,
};

//Estado del calefactor
enum ESTADO_CALEFACTOR {
  EC_FRIO, EC_PRECALENTAMIENTO, EC_LISTO,
};

//Estructura con los puntos de la recta que aproximan una curva del sensor de gas
typedef struct {
  float x1, y1, x2, y2;
} CURVA_SENSOR;

//Curvas de gases registrados
const CURVA_SENSOR curvaHumoMQ2 = { log10(800.0), log10(2.0), log10(5000.0), log10(0.92) };

//Clase que engloba a los sensores de gas
class SensorGas {
  public:
    //Metodos publicos
    SensorGas(TIPO_SENSOR tipoSensor, int pinADC);
    void atender();
    bool estaListo();
    unsigned long leerPPM(TIPO_GAS tipoGas);
  private:
    //Metodos privados
    void calibrar();
    float calcularPPM(float Rs_Ro, const CURVA_SENSOR *curva);

    //Miembros privados
    TIPO_SENSOR tSensor;
    int pin;
    ESTADO_CALEFACTOR estadoCalefactor;
    unsigned long tInicioPrec;
    float Ro_RL;
};

#endif //SensorGas_h_Incluida
