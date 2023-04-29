#include "Display_Control.h"
#include "Database_ctrl.h"

static Display_Ctrl_State display_state = DISPLAY_INIT;

static Adafruit_SSD1306 *pOledDisplay = new Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET_PIN);
static DateTime *pDateTime = new DateTime(0, 0, 0, 0, 0, 0);

static uint8_t wifiSignalStrength = 1;
static uint8_t subLineOptions = 1;
static uint32_t wifiSignalStrengthUpdateTimeStamp = 0;
static uint32_t subLineOptionsUpdateTimeStamp = 0;
static uint32_t displayUpdateTimeStamp = 0;

static String numberToString(uint16_t num)
{
    String convertedString = "";
    if (num < 10)
    {
        convertedString += "0";
    }
    convertedString += String(num);
    return convertedString;
}

bool display_init()
{
    digitalWrite(OLED_POWER_PIN, HIGH);
    pOledDisplay->begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
    return true;
}

void display_wifi_connecting()
{
    pOledDisplay->clearDisplay();
    pOledDisplay->dim(true);

    pOledDisplay->setTextSize(2);
    pOledDisplay->setTextColor(WHITE);
    pOledDisplay->setCursor(5, 5);
    pOledDisplay->print(WIFI_CONNECTING_STRING);

    switch (wifiSignalStrength)
    {
    case 1:
        pOledDisplay->drawBitmap(45, 25, wifi_1, 40, 40, BLACK, WHITE);
        break;
    case 2:
        pOledDisplay->drawBitmap(45, 25, wifi_2, 40, 40, BLACK, WHITE);
        break;
    case 3:
        pOledDisplay->drawBitmap(45, 25, wifi_3, 40, 40, BLACK, WHITE);
        break;
    case 4:
        pOledDisplay->drawBitmap(45, 25, wifi_4, 40, 40, BLACK, WHITE);
        break;
    default:
        break;
    }

    pOledDisplay->display();
}

void display_show_date_time()
{
    String dateString = "";
    String timeString = "";
    float temp = 0;
    float humidity = 0;

    ntp_get_date_time(pDateTime);

    pOledDisplay->clearDisplay();
    pOledDisplay->dim(true);
    pOledDisplay->setTextColor(WHITE);

    pOledDisplay->setTextSize(2);
    pOledDisplay->setCursor(5, 7);
    switch (subLineOptions)
    {
    case 1:
        /* Display Date Info*/
        pOledDisplay->setTextSize(2);
        dateString = numberToString(pDateTime->day()) + "." + numberToString(pDateTime->month()) + "." + numberToString(pDateTime->year());
        pOledDisplay->print(dateString);
        break;

    case 2:
        /* Display Wifi status */
        if(wifi_get_connect_state() == WIFI_CONNECTED)
        {
            pOledDisplay->drawBitmap(25, 5, wifiConnectedStatus, 20, 20, BLACK, WHITE);
        }
        else
        {
            pOledDisplay->drawBitmap(25, 5, wifiDisconnectedStatus, 20, 20, BLACK, WHITE);
        }
        pOledDisplay->setCursor(60, 7);
        dateString = weekDay[pDateTime->dayOfTheWeek()];
        pOledDisplay->print(dateString);
        break;

    case 3:
        /* Display temperature info */
        Temp_Sensor_Get_Data(&temp, &humidity);
        pOledDisplay->drawBitmap(0, 5, tempIcon, 12, 20, BLACK, WHITE);
        pOledDisplay->setCursor(17, 7);
        pOledDisplay->print(temp, 1);

        pOledDisplay->drawBitmap(75, 4, humidityIcon, 16, 20, BLACK, WHITE);
        pOledDisplay->setCursor(97, 7);
        pOledDisplay->print(uint8_t(humidity));
        break;

    default:
        break;
    }

    pOledDisplay->setTextSize(3);
    pOledDisplay->setCursor(0, 35);
    timeString = numberToString(pDateTime->hour()) + ":" + numberToString(pDateTime->minute());
    pOledDisplay->print(timeString);

    pOledDisplay->setTextSize(2);
    pOledDisplay->print(":" + numberToString(pDateTime->second()));

    pOledDisplay->display();
}

void display_ctrl()
{
    switch (display_state)
    {
    case DISPLAY_INIT:
        if (display_init())
        {
            display_state = DISPLAY_WIFI_CONNECTING;
            wifiSignalStrengthUpdateTimeStamp = millis();
        }
        else
        {
            delay(20000);
        }
        break;
    
    case DISPLAY_WIFI_CONNECTING:
        if (millis() - wifiSignalStrengthUpdateTimeStamp >= DISPLAY_UPDATE_SIGNAL_STR_INTERVAL)
        {
            wifiSignalStrengthUpdateTimeStamp = millis();
            ++wifiSignalStrength;

            if (wifiSignalStrength > 4)
            {
                wifiSignalStrength = 1;
            }
        }
        display_wifi_connecting();
        if (wifi_get_connect_state() == WIFI_CONNECTED && database_init_status())
        {
            subLineOptionsUpdateTimeStamp = millis();
            displayUpdateTimeStamp = millis();
            display_state = DISPLAY_SHOW_DATE_TIME;
        }
        break;

    case DISPLAY_SHOW_DATE_TIME:
        if (millis() - subLineOptionsUpdateTimeStamp >= DISPLAY_UPDATE_SUBLINE_INTERVAL)
        {
            subLineOptionsUpdateTimeStamp = millis();
            ++subLineOptions;
            if (subLineOptions > 3)
            {
                subLineOptions = 1;
            }
        }

        if (millis() - displayUpdateTimeStamp >= DISPLAY_UPDATE_SCREEN)
        {
            display_show_date_time();
        }   
        break;

    case DISPLAY_POWER_DOWN:
        digitalWrite(OLED_POWER_PIN, LOW);
        display_state = DISPLAY_IDLE;
        break;

    case DISPLAY_IDLE:
        break;

    default:
        break;
    }
}

void display_set_ctrl_state(Display_Ctrl_State state)
{
    display_state = state;
}