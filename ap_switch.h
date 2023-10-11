#ifndef ap_switch 
#define ap_switch_h

struct ap_switch_gpio {
   uint8_t ap_button;
};

class ApSwitch {
   public:
      static struct ap_switch_gpio gpio;

      bool ap_mode = false;
      int ap_mode_retries = 10;

      ApSwitch();

      void init();
      void ap_button_press();
      void ap_button_isr();
};

#endif
