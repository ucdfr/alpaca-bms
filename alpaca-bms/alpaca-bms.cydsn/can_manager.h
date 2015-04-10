#ifndef CAN_MANAGER_H
#define CAN_MANAGER_H

#include <project.h>
#include "data.h"


typedef struct
{
	uint16_t status_code;
} Status;


void can_send_temp();
void can_send_volt();
void can_send_current();
void can_send_status();


#endif // CAN_MANAGER_H
