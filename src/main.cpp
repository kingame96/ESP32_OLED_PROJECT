#include <Arduino.h>
#include "Temp_Sensor.h"
#include "Wifi_Control.h"
#include "ntp_control.h"
#include "Display_Control.h"
#include "Database_ctrl.h"
#include "Sleep_Ctr.h"
#include "Global.h"

uint32_t test = 0;

DateTime *dt = new DateTime();

void setup()
{
// #ifdef DEBUG
//     Serial.begin(9600);
// #endif
    // Serial.begin(9600);
    pinMode(DHT_POWER, OUTPUT);
    pinMode(POWER_OUTLET_1_PIN, OUTPUT);
    pinMode(POWER_OUTLET_2_PIN, OUTPUT);
    pinMode(OLED_POWER_PIN, OUTPUT);

    digitalWrite(POWER_OUTLET_1_PIN, HIGH);
    digitalWrite(POWER_OUTLET_2_PIN, HIGH);

    sleep_setup();

    test = millis();
}

void loop()
{
    wifi_ctrl();

    ntp_ctrl();

    Temp_Sensor_Ctrl();

    display_ctrl();

    database_ctrl();

    sleep_ctrl();

#ifdef DEBUG
    if (millis() - test >= 2000)
    {
        test = millis();
        ntp_get_date_time(dt);
        Serial.println(String(dt->hour()) + ":" + String(dt->minute()) + ":" + String(dt->second()));
        Serial.println(WiFi.status());
    }
#endif
}