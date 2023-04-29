#include "Power_Outlet_Ctrl.h"

void Power_Outlet_1_Control(bool status)
{
    digitalWrite(POWER_OUTLET_1_PIN, status);
}

void Power_Outlet_2_Control(bool status)
{
    digitalWrite(POWER_OUTLET_2_PIN, status);
}