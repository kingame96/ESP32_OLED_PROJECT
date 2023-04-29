#include <Temp_Sensor.h>

static Temp_Sensor_Ctrl_State ctrl_state = TEMP_SENSOR_INIT;

static DHT dht(DHT_PIN, DHT_TYPE);

static float humidity = 0;
static float temperature = 0;

static uint32_t dht_read_timestamp = 0;

void Temp_Sensor_Set_Power(bool state)
{
    if (state)
    {
        digitalWrite(DHT_POWER, HIGH);
    }
    else
    {
        digitalWrite(DHT_POWER, LOW);
    }
}

void Temp_Sensor_Init()
{
    Temp_Sensor_Set_Power(true);
    dht.begin();
}

void Temp_Sensor_Read_Humidity()
{
    float hmd = dht.readHumidity();
    if ( !isnan(hmd) )
    {
        humidity = hmd;
    }
}

void Temp_Sensor_Read_Temperature()
{
    float temp = dht.readTemperature();
    if ( !isnan(temp) )
    {
        temperature = temp;
    }
}

void Temp_Sensor_Ctrl()
{
    switch (ctrl_state)
    {
    case TEMP_SENSOR_INIT:
        Temp_Sensor_Init();
        ctrl_state = TEMP_SENSOR_READ_TEMPERATURE;
        dht_read_timestamp = millis();
        break;
    
    case TEMP_SENSOR_READ_TEMPERATURE:
        if (millis() - dht_read_timestamp >= DHT_READ_INTERVAL)
        {
            dht_read_timestamp = millis();
            Temp_Sensor_Read_Temperature();
            ctrl_state = TEMP_SENSOR_READ_HUMIDITY;
        }
        break;

    case TEMP_SENSOR_READ_HUMIDITY:
        if (millis() - dht_read_timestamp >= DHT_READ_INTERVAL)
        {
            dht_read_timestamp = millis();
            Temp_Sensor_Read_Humidity();
            ctrl_state = TEMP_SENSOR_READ_TEMPERATURE;
        }
        break;

    case TEMP_SENSOR_POWER_STOP:
        Temp_Sensor_Set_Power(false);
        ctrl_state = TEMP_SENSOR_IDLE;
        break;

    case TEMP_SENSOR_IDLE:
        break;

    default:
        break;
    }
}

Temp_Sensor_Ctrl_State Temp_Sensor_Set_Ctrl_State()
{
    return ctrl_state;
}

void Temp_Sensor_Get_Data(float *t, float *h)
{
    *t = temperature;
    *h = humidity;
}

