#include "can_manager.h"

volatile uint8_t can_buffer_status[8];


void can_send_status(void);
