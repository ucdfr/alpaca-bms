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



void can_send_volt(uint8_t IC_index,
                    uint8_t cell_index,
                    uint16_t **cell_codes)
{
	uint8_t i;
    uint8_t ic;
    uint32_t total=0;
    for (ic=0;ic<TOTAL_IC;ic++){
	    for(i=0; i<12; i++)
	    {
            total += cell_codes[ic][i];
        }
    }
    can_buffer[0] = IC_index*12 + cell_index;
	can_buffer[1] = 0xFF & (cell_codes[IC_index][cell_index]>>8); // upper byte
	can_buffer[2] = 0xFF & (cell_codes[IC_index][cell_index]); // lower byte
    
    can_buffer[3] = 0xFF & (total >> 24);
    can_buffer[4] = 0xFF & (total >> 16);
    can_buffer[5] = 0xFF & (total >> 8);
    can_buffer[6] = 0xFF & (total);
    can_buffer[7] = 0x00;

	CAN_1_SendMsgvolt();
} // can_send_volt()



void can_send_current(uint16_t battery_current)
{
	can_buffer[0] = 0xFF & (battery_current>>8); // upper byte
	can_buffer[1] = 0xFF & battery_current; // lower byte
	CAN_1_SendMsgcurrent();
} // can_send_current()



void can_send_status(uint8_t SOC_P,
                    uint8_t AH,
                    BMS_STATUS status,
                    uint16_t charge_cy,
                    uint16_t delta){
//8 SOC Percent
//8 AH used since full charge
//16 BMS Status bits (error flags)
//16 Number of charge cycles
//16 Pack balance (delta) mV
    can_buffer[0] = SOC_P;
    can_buffer[1] = AH;
    can_buffer[2] = (status>>8) & 0xFF;
    can_buffer[3] = (status) & 0xFF;
    can_buffer[4] = (charge_cy>>8) & 0xFF;
    can_buffer[5] = (charge_cy) & 0xFF;
    can_buffer[6] = (delta>>8) & 0xFF;
    can_buffer[7] = (delta) & 0xFF;
    CAN_1_SendMsgstatus();
}
