#ifndef TEMP_SENSOR_H
#define TEMP_SENSOR_H

#include <Arduino.h>
#include <DHT.h>
#include "Global.h"

typedef enum {
    TEMP_SENSOR_INIT,
    TEMP_SENSOR_READ_TEMPERATURE,
    TEMP_SENSOR_READ_HUMIDITY,
    TEMP_SENSOR_POWER_STOP,
    TEMP_SENSOR_IDLE
} Temp_Sensor_Ctrl_State;

void Temp_Sensor_Init();
void Temp_Sensor_Set_Power(bool state);
void Temp_Sensor_Read_Humidity();
void Temp_Sensor_Read_Temperature();

void Temp_Sensor_Ctrl();
Temp_Sensor_Ctrl_State Temp_Sensor_Set_Ctrl_State();
void Temp_Sensor_Get_Data(float *t, float *h);

#endif