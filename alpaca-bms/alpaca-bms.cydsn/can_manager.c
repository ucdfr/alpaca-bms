#include "can_manager.h"

#include "cell_interface.h"

volatile uint8_t can_buffer[8];
extern volatile BATTERYPACK mypack;
extern volatile uint8_t CAN_DEBUG;

/* Data Frame format for Voltage and Temperature
The datatype consists of three bytes:
1. Identifying Number (eg cell #1)
2. upper byte of data
3. lower byte of data
*/



void can_send_temp()
{
    uint16_t avg_cell_temp=0;
    uint16_t avg_board_temp=0;
    uint8_t max_cell_temp=0;
    uint8_t max_board_temp=0;
    uint8_t max_board=0;
    uint8_t max_cell=0;
    uint8_t stack=0;
    uint8_t cell=0;
    
    //process temperature
    for(stack=0;stack<3;stack++){
        avg_cell_temp=0;
        avg_board_temp=0;
        max_cell_temp=0;
        max_board_temp=0;
        max_board=0;
        max_cell=0;

        /*
        for(cell=0;cell<20;cell++){
            
            
            //board
            if (cell/5 == 1 || cell/5 == 3 ){
                if (mypack.temp[stack][cell].value16>max_board_temp){
                    max_board_temp = mypack.temp[stack][cell].value16;
                    max_board = cell;
                }
                avg_board_temp = avg_board_temp + mypack.temp[stack][cell].value16;
            }
            //cell
            if (cell/5 == 0 || cell/5 == 2 ){
                if (mypack.temp[stack][cell].value16>max_cell_temp){
                    max_cell_temp = mypack.temp[stack][cell].value16;
                    max_cell = cell;
                }
                avg_cell_temp = avg_cell_temp + mypack.temp[stack][cell].value16;
            }
            
        }

        avg_cell_temp = avg_cell_temp/10;
        avg_board_temp = avg_board_temp/10;

        can_buffer[0] = stack;
        can_buffer[1] = 0xff & avg_cell_temp;
        can_buffer[2] = max_cell ; // upper byte of C
        can_buffer[3] = max_cell_temp ; // lower byte of C
        
        can_buffer[4] = 0xff & avg_board_temp;
        can_buffer[5] = max_board ; // upper byte of C
        can_buffer[6] = max_board_temp ; // lower byte of C
        can_buffer[7] = 0x00;
          
        */
        
           
        
        
        can_buffer[0] = stack;
        can_buffer[1] = 0xff & cell;
        can_buffer[2] = temp_transfer(mypack.temp[stack][cell].value16,mypack.stack[stack].vcc);
        can_buffer[3] = 0x00 ; // lower byte of C
        
        can_buffer[4] = 0x00;
        can_buffer[5] = 0x00; // upper byte of C
        can_buffer[6] = 0x00; // lower byte of C
        can_buffer[7] = 0x00;
            
            
        
		CAN_1_SendMsgtemp();
        CyDelay(5);
        }
        
        
       // CAN_1_SendMsgtemp();
        //CyDelay(5);
    //}

} // can_send_temp()



void can_send_volt()
{
    uint8_t stack=0;
    uint8_t ic=0;
    uint8_t cell=0;
    
    uint32_t stack_voltage=mypack.stack[0].value32 + mypack.stack[1].value32 + mypack.stack[2].value32;
    stack_voltage = stack_voltage/3;
    //uint32_t stack_voltage = mypack.stack[2].value32;

    if (CAN_DEBUG){
        for (stack=0;stack<3;stack++){
        for(ic=0;ic<4;ic++){
            for(cell=0;cell<7;cell++){
                can_buffer[0] = (stack*4+ic);
                can_buffer[1] = cell;
                can_buffer[2] = 0xFF & (mypack.cell[stack][ic][cell].value16>>8); // upper byte
                can_buffer[3] = 0xFF & mypack.cell[stack][ic][cell].value16; // lower byte
                
                can_buffer[4] = 0xFF & (stack_voltage >> 24);
                can_buffer[5] = 0xFF & (stack_voltage >> 16);
                can_buffer[6] = 0xFF & (stack_voltage >> 8);
                can_buffer[7] = 0xFF & (stack_voltage);
                
                CAN_1_SendMsgvolt();
                CyDelay(5);
            }
        }
    }
    }else{
        for (stack =0;stack<3;stack++){
        stack_voltage = mypack.stack[stack].value32;
        can_buffer[0] = 0xff & stack;
        can_buffer[1] = 0x00;
        can_buffer[2] = 0x00;
        can_buffer[3] = 0x00;
        
        can_buffer[4] = 0xFF & (stack_voltage >> 24);
        can_buffer[5] = 0xFF & (stack_voltage >> 16);
        can_buffer[6] = 0xFF & (stack_voltage >> 8);
        can_buffer[7] = 0xFF & (stack_voltage);
        CAN_1_SendMsgvolt();
        CyDelay(5);
    }
    }
    
    


 

	
} // can_send_volt()



void can_send_current()
{
    int16_t battery_current = mypack.current;
	can_buffer[0] = 0xFF & (battery_current>>8); // upper byte
	can_buffer[1] = 0xFF & battery_current; // lower byte
	CAN_1_SendMsgcurrent();
} // can_send_current()



void can_send_status(uint8_t SOC_P,
                    uint8_t AH,
                    BMS_STATUS status,
                    uint8_t stack,
                    uint8_t cell,
                    uint16_t value16){
//8 SOC Percent
//8 AH used since full charge
//16 BMS Status bits (error flags)
//16 Number of charge cycles
//16 Pack balance (delta) mV
    can_buffer[0] = SOC_P;
    can_buffer[1] = AH;
    can_buffer[2] = (status>>8) & 0xFF;
    can_buffer[3] = (status) & 0xFF;
    can_buffer[4] = stack & 0xFF;
    can_buffer[5] = (cell) & 0xFF;
    can_buffer[6] = (value16>>8) & 0xFF;
    can_buffer[7] = (value16) & 0xFF;

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
