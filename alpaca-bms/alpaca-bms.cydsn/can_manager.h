#ifndef CAN_MANAGER_H
#define CAN_MANAGER_H

#include <project.h>
#include "data.h"


void can_send_temp(uint16_t temp[]);
void can_send_volt(uint16_t cell_volt[]);
void can_send_current(uint16_t battery_current);
void can_send_status(uint8_t battery_status);

#endif // CAN_MANAGER_H
