#include "Database_ctrl.h"

static Database_Ctrl_State database_state = DATABASE_INIT;
static bool is_database_init = false;

static FirebaseData firebaseData;
static FirebaseAuth auth;
static FirebaseConfig config;

static uint32_t sendDataTimeStamp = 0;
static uint32_t readDataTimeStamp = 0;
static uint32_t signUpTimeStamp = millis();

static Control_Data controlData = {
    false,
    true,
    true,
    {true, true, true, true}
};

static Database_Data data[DATABASE_DATA_SIZE] = 
{
    {"device/", &controlData.is_device_off, on_receive_from_database},
    {"display/", &controlData.display_status, NULL},
    {"temp_sensor/", &controlData.temp_sensor_status, NULL},
    {"power_outlet_1/", &controlData.power_outlet_status[0], Power_Outlet_1_Control},
    {"power_outlet_2/", &controlData.power_outlet_status[1], Power_Outlet_2_Control},
    {"power_outlet_3/", &controlData.power_outlet_status[2], NULL},
    {"power_outlet_4/", &controlData.power_outlet_status[3], NULL},
};

static uint8_t data_index = 0;

bool database_init()
{
    config.api_key = API_KEY;

    config.database_url =  DATABASE_URL;

    if (Firebase.signUp(&config, &auth, "", ""))
    {
        config.token_status_callback = tokenStatusCallback;

        Firebase.begin(&config, &auth);
        Firebase.reconnectWiFi(true);

        return true;
    }
    else
    {
        Serial.printf("%s\n", config.signer.signupError.message.c_str());
        return false;
    }
}

/**
 * @brief 
 * 
 * @param data_position position of data in data table
 */
bool database_send_data(int data_position, bool setData)
{
    if (Firebase.ready())
    {
        if (Firebase.RTDB.setBool(&firebaseData, "ESP32_OLED_PROJECT/" + String(data[data_position].database_path + "App"), setData))
        {
            return true;
        }
    }

    return true;
}

void database_read_data()
{
    bool status = false;

    if ((millis() - readDataTimeStamp >= DATABASE_READ_DATA_INTERVAL))
    {
        readDataTimeStamp = millis();
        if (Firebase.ready())
        {
            if (Firebase.RTDB.getJSON(&firebaseData, "ESP32_OLED_PROJECT"))
            {
                FirebaseJson &jsondata = firebaseData.jsonObject();
                
                for(uint8_t i = 0; i < DATABASE_DATA_SIZE; ++i)
                {
                    FirebaseJsonData json;
                    jsondata.get(json, data[i].database_path + "App");
                    bool status = json.boolValue;

                    if (status != *(bool *)(data[i].status))
                    {
                        *(bool *)(data[i].status) = status;
                        if (data[i].operation != NULL)
                        {
                            data[i].operation(status);
                        }
                    }
                    is_database_init = true;
                }
            }
        }
        
    }
}

void database_ctrl()
{
    switch (database_state)
    {
    case DATABASE_INIT:
        if (millis() - signUpTimeStamp >= DATABASE_SIGN_UP_INTERVAL)
        {
            signUpTimeStamp = millis();
            if (wifi_get_connect_state() == WIFI_CONNECTED)
            {
                if (database_init())
                {
                    database_state = DATABASE_READ_DATA;
                    readDataTimeStamp = millis();
                }
            }
        }
        break;

    case DATABASE_READ_DATA:
        if (wifi_get_connect_state() == WIFI_CONNECTED)
        {
            database_read_data();
        }
        else
        {
            database_state = DATABASE_IDLE;
        }
        break;

    case DATABASE_IDLE:
        if (wifi_get_connect_state() == WIFI_CONNECTED)
        {
            database_state = DATABASE_READ_DATA;
        }
        break;

    default:
        break;
    }
}

bool database_init_status()
{
    return is_database_init;
}