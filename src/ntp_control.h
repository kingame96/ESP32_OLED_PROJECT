#ifndef NTP_CONTROL_H
#define NTP_CONTROL_H

#include <Arduino.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <RTClib.h>
#include "Global.h"
#include "Wifi_Control.h"

typedef enum {
    NTP_INIT,
    NTP_UPDATE_TIME
} Ntp_Ctrl_State;

void ntp_init();
void ntp_get_date_time(DateTime *dT);
void ntp_ctrl();

#endif /* NTP_CONTROL_H */