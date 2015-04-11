#include "can_manager.h"

volatile uint8_t can_buffer[8];


/* Data Frame format for Voltage and Temperature
The datatype consists of three bytes:
1. Identifying Number (eg cell #1)
2. upper byte of data
3. lower byte of data
*/



void can_send_temp(uint16_t temp[])
{
	uint8_t i;
	for(i=0; i<NUM_TEMP; i++)
	{
		can_buffer[0] = i;
		can_buffer[1] = 0xFF & (temp[i]>>8); // upper byte
		can_buffer[3] = 0xFF & temp[i]; // lower byte
		CAN_1_SendMsgtemp();
	} // for all temperature probes
} // can_send_temp()



void can_send_volt(uint16_t cell_volt[])
{
	uint8_t i;
	for(i=0; i<NUM_CELLS; i++)
	{
		can_buffer[0] = i;
		can_buffer[1] = 0xFF & (cell_volt[i]>>8); // upper byte
		can_buffer[3] = 0xFF & cell_volt[i]; // lower byte
		CAN_1_SendMsgtemp();
	} // for all temperature probes

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
