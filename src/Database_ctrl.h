#ifndef DATABASE_CTRL_H
#define DATABASE_CTRL_H

#include <Arduino.h>
#include <FirebaseESP32.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"
#include "Power_Outlet_Ctrl.h"
#include "Wifi_Control.h"
#include "Sleep_Ctr.h"
#include "Global.h"

typedef enum
{
    DATABASE_INIT,
    DATABASE_SEND_DATA,
    DATABASE_READ_DATA,
    DATABASE_IDLE
} Database_Ctrl_State;

typedef struct
{
    bool is_device_off;
    bool display_status;
    bool temp_sensor_status;
    bool power_outlet_status[4];
} Control_Data;

typedef struct
{
    String database_path;
    void *status;
    void (*operation)(bool status);
} Database_Data;

bool database_init();
bool database_send_data(int data_position, bool setData);
void database_read_data();
void database_ctrl();
bool database_init_status();

#endif /* DATABASE_CTRL_H */

