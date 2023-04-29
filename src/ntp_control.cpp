#include "ntp_control.h"

WiFiUDP ntpUDP;
NTPClient *timeClient = new NTPClient(ntpUDP);
uint32_t echoTime = 0;

static Ntp_Ctrl_State ntp_state = NTP_INIT;
static Wifi_Ctrl_State wifi_state = WIFI_NOT_CONNECTED;

uint32_t ntp_update_timeStamp = 0;


void ntp_init()
{
    timeClient->begin();
    timeClient->setUpdateInterval(NTP_UPDATE_INTERVAL);
    timeClient->setTimeOffset(NTP_TIME_OFFSET);

    timeClient->update();
    echoTime = timeClient->getEpochTime();

    ntp_update_timeStamp = millis();
}

void ntp_ctrl()
{
    switch (ntp_state)
    {
    case NTP_INIT:
        if (wifi_get_connect_state() == WIFI_CONNECTED)
        {
            ntp_init();
            ntp_state = NTP_UPDATE_TIME;
        }
        break;
    
    case NTP_UPDATE_TIME:
        if (millis() - ntp_update_timeStamp >= NTP_UPDATE_TIME_INTERVAL)
        {
            ntp_update_timeStamp = millis();

            if (wifi_get_connect_state() == WIFI_CONNECTED)
            {
                if(timeClient->update())
                {
                    echoTime = timeClient->getEpochTime();
                }
                else
                {
                    ++echoTime;
                    // ntp_state = NTP_INIT;
                }
                
            }
            else
            {
                ++echoTime;
                // if (wifi_get_connect_state() == WIFI_CONNECTED)
                // {
                //     ntp_state = NTP_INIT;
                // }
            }
        }
        break;

    default:
        break;
    }
}

void ntp_get_date_time(DateTime *dT)
{
    dT->setDateTime(echoTime);
}