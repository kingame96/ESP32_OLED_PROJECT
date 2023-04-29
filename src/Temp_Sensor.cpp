#include <Temp_Sensor.h>

static Temp_Sensor_Ctrl_State ctrl_state = TEMP_SENSOR_INIT;

static DHT dht(DHT_PIN, DHT_TYPE);

static float humidity = 0;
static float temperature = 0;

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
    humidity = dht.readHumidity();
    if ( isnan(humidity) )
    {
        humidity = 0;
    }
}

void Temp_Sensor_Read_Temperature()
{
    temperature = dht.readTemperature();
    if ( isnan(temperature) )
    {
        temperature = 0;
    }
}

void Temp_Sensor_Ctrl()
{
    switch (ctrl_state)
    {
    case TEMP_SENSOR_INIT:
        Temp_Sensor_Init();
        ctrl_state = TEMP_SENSOR_READ_TEMPERATURE;
        break;
    
    case TEMP_SENSOR_READ_TEMPERATURE:
        Temp_Sensor_Read_Temperature();
        ctrl_state = TEMP_SENSOR_READ_HUMIDITY;
        break;

    case TEMP_SENSOR_READ_HUMIDITY:
        Temp_Sensor_Read_Humidity();
        ctrl_state = TEMP_SENSOR_READ_TEMPERATURE;
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

