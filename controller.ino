#include "controller.h"

#include <Arduino.h>

#include "sensors.h"
#include "valve.h"
#include "controller_mqtt.h"
#include "local.h"

Controller::Controller() {}

void Controller::init(const struct controller_conf *conf)
{
   this->conf = conf;
}

/*
   Duration is determined using an emulated logistic curve:
    - If amb_temp is between min_temp and max_temp, then it lies within the
      linear region computed using the intercepts (min_temp, min_duration)
      and (max_temp, max_duration).
    - If amb_temp < min_temp, then duration = min_duration.
    - If amb_temp > max_temp, then duration = max_duration.
*/
int Controller::comp_duration(int duration_override)
{
   int duration;
   float amb_temp = sensors.get_temp();

   if (duration_override > 0 && duration_override <= conf->max_duration) {
      duration = duration_override;
   }
   else if (amb_temp < conf->min_temp) {
      duration = conf->min_duration;
   }
   else if (amb_temp > conf->max_temp) {
      duration = conf->max_duration;
   }
   else {
      // rate = amount of watering duration per degree temperature.
      const float rate = (float)(conf->max_duration - conf->min_duration)/(conf->max_temp - conf->min_temp);
      /*
         Compute watering duration as a linear function of temperature.
            f(x) = m*x + c
                 = m*(x-x0) + y0
         That is:
            Shift function along x-axis by min_temp (x0).
            Scale by scale factor m = (y1-y0)/(x1-x0).
            Shift function along y-axis by min_duration (y0).
      */
      duration = rate * (amb_temp - conf->min_temp) + conf->min_duration;
   }

   return duration;
}

void Controller::irrigate(int duration_override)
{
   if (valve.is_open()) {
      log_info("Irrigation already started.");
      return;
   }

   int new_duration = comp_duration(duration_override);

   char result_topic[80];
   sprintf(result_topic, "%s%s", TOPIC_RESULT, TOPIC_TEMPERATURE);
   mqtt_utils.publish(result_topic, String(sensors.get_temp()).c_str());
   sprintf(result_topic, "%s%s", TOPIC_RESULT, TOPIC_DURATION);
   mqtt_utils.publish(result_topic, String(new_duration).c_str());

   valve.open();
   /*
      signaller() serves dual purpose of indicating irrigation is taking
      place via LED, as well as blocking for the given watering duration.
   */
   signaller.notify(new_duration);
   valve.close();
}
