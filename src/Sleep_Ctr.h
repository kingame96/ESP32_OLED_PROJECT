
#ifndef SLEEP_CTR_H
#define SLEEP_CTR_H

#include <Arduino.h>
#include <RTClib.h>
#include "Global.h"
#include "ntp_control.h"
#include "Database_ctrl.h"

void sleep_setup();
void sleep_ctrl();
void on_receive_from_database(bool status);

#endif /* SLEEP_CTR_H */

