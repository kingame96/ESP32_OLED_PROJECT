#ifndef DISPLAY_CONTROL_H
#define DISPLAY_CONTROL_H

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RTClib.h>
#include "Wifi_Control.h"
#include "ntp_control.h"
#include "Temp_Sensor.h"
#include "Global.h"

typedef enum {
    DISPLAY_INIT,
    DISPLAY_WIFI_CONNECTING,
    DISPLAY_SHOW_DATE_TIME,
    DISPLAY_POWER_DOWN,
    DISPLAY_IDLE
} Display_Ctrl_State;

bool display_init();
void display_wifi_connecting();
void display_show_date_time();
void display_ctrl();
void display_set_ctrl_state(Display_Ctrl_State state);


#endif /* DISPLAY_CONTROL_H */
