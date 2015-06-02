#include "can_manager.h"

volatile uint8_t can_buffer[8];


/* Data Frame format for Voltage and Temperature
The datatype consists of three bytes:
1. Identifying Number (eg cell #1)
2. upper byte of data
3. lower byte of data
*/



void can_send_temp(uint8_t IC_index, uint16_t temp[6])
{
	uint8_t i;
    uint32_t temp_total=0;
    uint16_t temp_max=0;
    for(i=0;i<NUM_TEMP;i++){
        temp_total += temp[i];
        if (temp[i]>temp_max){
            temp_max = temp[i];
        }
    }
    temp_total = temp_total/NUM_TEMP;
    
	for(i=0; i<NUM_TEMP; i++)
	{
		can_buffer[0] = IC_index;
        can_buffer[1] = i;
        
		can_buffer[2] = 0xFF & (temp[i]>>8); // upper byte
		can_buffer[3] = 0xFF & temp[i]; // lower byte
        
        can_buffer[4] = 0xFF & (temp_total>>8); // upper byte
		can_buffer[5] = 0xFF & temp_total; // lower byte
        
        
        can_buffer[6] = 0xFF & (temp_max>>8); // upper byte
		can_buffer[7] = 0xFF & temp_max; // lower byte
        
		CAN_1_SendMsgtemp();
	} // for all temperature probes
} // can_send_temp()



void can_send_volt(uint8_t IC_index,
                    uint8_t cell_index,
                    uint16_t cell_codes[TOTAL_IC][12])
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
    can_buffer[0] = (IC_index);
    can_buffer[1] = cell_index;
	can_buffer[2] = 0xFF & (cell_codes[IC_index][cell_index]>>8); // upper byte
	can_buffer[3] = 0xFF & (cell_codes[IC_index][cell_index]); // lower byte
    
    can_buffer[4] = 0xFF & (total >> 24);
    can_buffer[5] = 0xFF & (total >> 16);
    can_buffer[6] = 0xFF & (total >> 8);
    can_buffer[7] = 0xFF & (total);

	CAN_1_SendMsgvolt();
} // can_send_volt()



void can_send_current(uint8_t IC_index, uint16_t battery_current)
{
    can_buffer[0] = IC_index;
	can_buffer[1] = 0xFF & (battery_current>>8); // upper byte
	can_buffer[2] = 0xFF & battery_current; // lower byte
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

                    

                    
uint8_t can_test_send()
{
    uint16_t ID=0x205;
    uint8_t msg[8];
    //msg[0~3] is left
    msg[0]=0x0f;
    msg[1]=0xf0;
    msg[2]=0x0f;
    msg[3]=0xf0;
    //msg[4~7] is right
    msg[4]=0x9a;
    msg[5]=0xbc;
    msg[6]=0xde;
    msg[7]=0xff;
    
    
    
	uint8_t i, state;
	CAN_1_TX_MSG message;
	CAN_1_DATA_BYTES_MSG payload;

	message.id = ID; // edit for testing
	message.rtr = 0;
	message.ide = 0;
	message.dlc = 0x08;
	message.irq = 1;
	message.msg = &payload;

	for(i=0;i<8;i++)
		payload.byte[i] = msg[i];

	state = CAN_1_SendMsg(&message);

/*
	if(state != CYRET_SUCCESS)
	{
		LED_Write(1);
		return CAN_1_FAIL;
	}

	LED_Write(0);
	return CYRET_SUCCESS;
*/
	
	return state;
} // can_test_send()


void can_init()
{
	CAN_1_GlobalIntEnable(); // CAN Initialization
	CAN_1_Init();
	CAN_1_Start();
} // can_init()