#ifndef valve_h
#define valve_h

struct valve_gpio {
   uint8_t valve;
};

class Valve {
   public:
      static struct valve_gpio gpio;

      Valve();

      void init();
      void open();
      void close();
      bool is_open();

   private:
      bool valve_opened = false;
};

#endif
