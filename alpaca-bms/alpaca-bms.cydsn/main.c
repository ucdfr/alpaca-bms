#include <project.h>
#include <stdint.h>

#include "cell_interface.h"
#include "current_sense.h"
#include "WDT.h"
#include "data.h"
#include "can_manager.h"
#include "uart-terminal.h"
#include "BMS_monitor.h"

volatile uint8_t CAN_UPDATE_FLAG=0;
volatile BMS_STATUS warning_event;
volatile BMS_STATUS fatal_err;
volatile uint8_t error_IC;
volatile uint8_t error_CHIP;
extern volatile BATTERYPACK mypack;
volatile uint8_t CAN_DEBUG=0;



CY_ISR(CAN_UPDATE_Handler){
    Can_Update_Timer_STATUS;
    CAN_UPDATE_FLAG = 1;
}


void process_event(){
    if (warning_event | CHARGEMODE){
    	can_send_status(0x00,
					0x00,
					CHARGEMODE,
					0x00,0x00,0x000);
    }
}


int main(void)
{

	Can_Update_ISR_StartEx(CAN_UPDATE_Handler);
	Can_Update_Timer_Start();
	can_init();
	

	// TODO Watchdog Timer
    CyWdtStart(CYWDT_1024_TICKS,CYWDT_LPMODE_NOCHANGE);

    OK_SIG_Write(1);
	// Initialize
	bms_init();
	mypack_init();
    //monitor_init();

	current_init();
    
    //enable global interrupt
    CyGlobalIntEnable;

	//int pin_value=1;


	// Initialize err event
	fatal_err = NO_ERROR;
	warning_event = NO_ERROR;

    
    //some variables
    uint8_t stack, cell;
    
	for(;;) // main loop
	{   
        //terminal_run();
        OK_SIG_Write(1);
		CyWdtClear();
        
        
		//check_cfg();  //CANNOT be finished, because 
		//check_cells();// TODO This function will be finished in get_cell_volt/check stack fuse
        get_cell_volt();// TODO Get voltage
		check_stack_fuse(); // TODO: check if stacks are disconnected
		get_cell_temp();// TODO Get temperature
		get_current(); // TODO get current reading from sensor
		//get_soc(); // TODO calculate SOC()
        
        //voltage compensate
        

		if (fatal_err)
    		break; // break from main loop and enter fault loop

		if (warning_event){
            process_event();
        }
		else{
			can_send_status(0x00,
					0x00,
					NO_ERROR,
					0x00,
					0x00,
                    0x0000);
		} // else send no error

		
        if(CAN_UPDATE_FLAG){
            can_send_volt();
            can_send_temp();
            can_send_current();
            CAN_UPDATE_FLAG=0;
        }
		CyDelay(100); // wait for next cycle
	} // main loop


    
	for(;;){
		CyWdtClear();
        OK_SIG_Write(0);
		uint8_t index=0;
		if (fatal_err & PACK_TEMP_OVER){        //0x0002
			
			for (stack=0;stack<3;index++){
                for (cell=0;cell<20;cell++){
				    if (mypack.bad_temp[stack][cell]==1){
					    can_send_status(stack,
					    cell,
					    PACK_TEMP_OVER,
					    stack,
					    cell,
					    mypack.temp[stack][cell].value16);
				    }
                CyDelay(1);
                }
            }
		}

		if (fatal_err & PACK_TEMP_UNDER){       //0x0008
			
			for (stack=0;stack<3;index++){
                for (cell=0;cell<20;cell++){
				    if (mypack.bad_temp[stack][cell]==1){
					    can_send_status(0xff & index,
					    0x00,
					    PACK_TEMP_UNDER,
					    stack,
					    cell,
					    mypack.temp[stack][cell].value16);
				    }
                CyDelay(1);
                }
			}
			

		}

		if (fatal_err & STACK_FUSE_BROKEN){         //0x0004
			can_send_status(0x00,
			0x00,
			STACK_FUSE_BROKEN,
			mypack.fuse_fault,
			0x00,
			0x0000);            
		}


		if(fatal_err & CELL_VOLT_OVER){         //0x0800
			for (index=0;index<mypack.bad_cell_index;index++){
				if (mypack.bad_cell[index].error==1){
					can_send_status(0xff & index,
					0x00,
					CELL_VOLT_OVER,
					mypack.bad_cell[index].stack,
					mypack.bad_cell[index].ic*4+mypack.bad_cell[index].cell,
					mypack.bad_cell[index].value16);
				}
                CyDelay(1);
			}
            

		}
		if(fatal_err & CELL_VOLT_UNDER){                //0x1000
			for (index=0;index<mypack.bad_cell_index;index++){
				if (mypack.bad_cell[index].error==0){
					can_send_status(0xff & index,
					0x00,
					CELL_VOLT_UNDER,
					mypack.bad_cell[index].stack,
					mypack.bad_cell[index].ic*4+mypack.bad_cell[index].cell,
					mypack.bad_cell[index].value16);
				}
                CyDelay(1);
			}
		}


		CyDelay(500);
	}
    
    
	return 0;
} // main()
