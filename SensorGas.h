enum TIPO_SENSOR {
  MQ2,
};

enum ESTADO_CALEFACTOR {
  EC_FRIO, EC_PRE_CALENTANDO, EC_CALIENTE,
};

class SensorGas {
  public:
    SensorGas(TIPO_SENSOR tipo, int pinADC);
    void precalentar();
    bool estaCalentado();
    void calibrar();
    bool estaCalibrado();
    unsigned long leerPPM();
  private:
    TIPO_SENSOR miTipo;
    int miPinADC;
    ESTADO_CALEFACTOR estadoCalefactor;
    float Ro_RL;
    bool calibradoListo;
    unsigned long tInicioPrec;
};
