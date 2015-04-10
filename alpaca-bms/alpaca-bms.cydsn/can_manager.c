#include "can_manager.h"

volatile uint8_t can_buffer[8];


/* Data Frame format for Voltage and Temperature
The datatype consists of three bytes:
1. Identifying Number (eg cell #1)
2. upper byte of data
3. lower byte of data
*/



inline void load_buffer(uint8_t num, uint16_t data[])
{
	uint8_t i, j;
	for(i=0; i<num; i++)
	{
		j = i*3; // actual index
		can_buffer[j] = i;
		can_buffer[j+1] = 0xFF & (data[i]>>8); // upper byte
		can_buffer[j+2] = 0xFF & data[i]; // lower byte
	} // for all temperature probes
} // load_buffer()



void can_send_temp(uint16_t temp[])
{
	load_buffer(NUM_TEMP, temp);
	CAN_1_SendMsgtemp();
} // can_send_temp()



void can_send_volt(uint16_t cell_volt[])
{
	load_buffer(NUM_CELL, cell_volt);
	CAN_1_SendMsgvolt();
} // can_send_volt()



void can_send_current(uint16_t battery_current)
{
	can_buffer[0] = 0xFF & (battery_current>>8); // upper byte
	can_buffer[1] = 0xFF & battery_current; // lower byte
	CAN_1_SendMsgcurrent();
} // can_send_current()



void can_send_status(uint8_t battery_status)
{
	can_buffer[0] = battery_status;
	CAN_1_SendMsgstatus();
} // can_send_status()
