#ifndef sensors_h
#define sensors_h

struct sensors_gpio {
   uint8_t temp_sensor;
};

class Sensors {
   public:
      static struct sensors_gpio gpio;

      Sensors();

      void init();
      float get_temp();
};

#endif
