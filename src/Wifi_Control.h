#ifndef WIFI_CONTROL_H
#define WIFI_CONTROL_H

#include <Arduino.h>
#include <WiFi.h>
#include "Global.h"

typedef enum {
    WIFI_NOT_CONNECTED,
    WIFI_WAIT_TO_CONNECT,
    WIFI_CONNECTED
} Wifi_Ctrl_State;

void wifi_init();
Wifi_Ctrl_State wifi_get_connect_state();
void wifi_ctrl();


#endif