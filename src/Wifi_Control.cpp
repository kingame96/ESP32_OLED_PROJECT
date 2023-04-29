#include <Wifi_Control.h>

static Wifi_Ctrl_State wifi_state = WIFI_NOT_CONNECTED;
static uint16_t wifiWaitConnectTime = 0;

// IPAddress ip(192, 168, 1, 33);
// IPAddress dns1(8, 8, 8, 8);
// IPAddress dns2(8, 8, 4, 4);
// IPAddress gateway(192, 168, 1, 1);
// IPAddress subnet(255, 255, 255, 0);

void wifi_init()
{
 WiFi.mode(WIFI_STA);   
}

void wifi_ctrl()
{
    switch (wifi_state)
    {
    case WIFI_NOT_CONNECTED:
        // WiFi.config(ip, gateway, subnet, dns1, dns2);
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
        wifi_state = WIFI_WAIT_TO_CONNECT;
        wifiWaitConnectTime = millis();
        break;
    
    case WIFI_WAIT_TO_CONNECT:
        if (WiFi.status() == WL_CONNECTED)
        {
            wifi_state = WIFI_CONNECTED;
        }
        else
        {
            if (millis() - wifiWaitConnectTime >= 20000)
            {
                Serial.println("ESP restart");
                ESP.restart();
            }
        }
        break;

    case WIFI_CONNECTED:
        if (WiFi.status() != WL_CONNECTED)
        {
            WiFi.disconnect(false, false);
            WiFi.reconnect();
            wifi_state = WIFI_WAIT_TO_CONNECT;
            wifiWaitConnectTime = millis();
        }
        break;

    default:
        break;
    }
}

Wifi_Ctrl_State wifi_get_connect_state()
{
    return wifi_state;
}