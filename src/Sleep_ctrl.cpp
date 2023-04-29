#include "Sleep_Ctr.h"

// RTC_DATA_ATTR bool isWakeUpFromSleep = false;

static uint32_t sleepCtrlTimeStamp = 0;
static uint64_t timeToSleep = uint64_t(TIME_TO_SLEEP_HOUR) * 3600 * US_TO_S_FACTOR;
DateTime *dateTime = new DateTime();
static bool sleep_from_database = false;
static bool is_request_sleep_trigger = false;

void sleep_setup()
{
    esp_sleep_enable_timer_wakeup(timeToSleep);
    sleepCtrlTimeStamp = millis();
}

void sleep_ctrl()
{
    if (millis() - sleepCtrlTimeStamp >= SLEEP_CTRL_INTERVAL)
    {
        sleepCtrlTimeStamp = millis();
        ntp_get_date_time(dateTime);
        if ((dateTime->hour() == TIME_TO_GO_SLEEP && dateTime->minute() == 0)
            || (is_request_sleep_trigger && !sleep_from_database))
        {
            esp_deep_sleep_start();
        }
    }
}

void on_receive_from_database(bool status)
{
    if (status)
    {
        database_send_data(0, !status);
        is_request_sleep_trigger = true; 
        sleep_from_database = true;
    }
    else if (is_request_sleep_trigger == true)
    {
        sleep_from_database = status;
    }
}