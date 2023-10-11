#ifndef controller_h
#define controller_h

struct controller_conf {
   /*
    * A bit list of watering hours.
    *    1 = Water.
    *    0 = Don't water.
    * 
    * Store list of watering hours in a 24-bit binary.
    * (a 4-byte long int, instead of an array of 24 bytes). 
    *
    * First bit is 0hr (midnight) and last bit is 23hr (11pm).
    *
    * Example: to water on the following hours: "6, 8, 10, 12, 13, 14, 15,
    * 16, 18, 20":
    *    watering_hours = 0b000000101010111110101000;
    */
   char watering_hours_str[25] = "000000101010111110101000";
   int watering_hours = 0b000000101010111110101000;

   /* 
    * Watering duration is based on ambient temperature.
    * If weather is hot, water more, else water less.
    *
    * Set minimum and maximum limits (in seconds) for watering duration. 
    *
    * Minimum limit required to prevent drip holes from getting clogged due 
    * to salt buildup when not watering for extended periods. 
    *
    * Maximum limit required to prevent controller valves from overheating.
    *
    */
   int min_duration = 45;
   int max_duration = 300;

   /*
    * Min and max temp (°C).
    *
    * Set min_temp to 1°C higher than expected ambient low temperature to 
    * account for sensor being in enclosed space.
    *
    */
   int min_temp = 28;
   int max_temp = 44;
};

class Controller {
   public:
      const struct controller_conf *conf;

      Controller();

      void init(const struct controller_conf *conf);
      int comp_duration(int duration_override=0);
      void irrigate(int duration_override=0);
};

#endif
