#include <project.h>
#include <stdint.h>

#include "cell_interface.h"
#include "current_sense.h"
#include "WDT.h"
#include "data.h"
#include "can_manager.h"

volatile uint8_t CAN_UPDATE_FLAG=0;
volatile BMS_STATUS warning_event;
volatile BMS_STATUS fatal_err;
volatile uint8_t error_IC;
volatile uint8_t error_CHIP;
extern volatile BATTERYPACK mypack;
volatile uint8_t CAN_DEBUG=1;



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
    //WDT_init();

    OK_SIG_Write(1);
	// Initialize
	bms_init();
	mypack_init();

	current_init();
    
    //enable global interrupt
    CyGlobalIntEnable;

	//int pin_value=1;


	// Initialize err event
	fatal_err = NO_ERROR;
	warning_event = NO_ERROR;

    
	for(;;) // main loop
	{   
        OK_SIG_Write(1);
		if (WDT_should_clear())
			WDT_clear();
        
        
		//check_cfg();  //CANNOT be finished, because 
		//check_cells();// TODO This function will be finished in get_cell_volt/check stack fuse
        get_cell_volt();// TODO Get voltage
		check_stack_fuse(); // TODO: check if stacks are disconnected
		//get_cell_temp();// TODO Get temperature
		//get_current(); // TODO get current reading from sensor
		//get_soc(); // TODO calculate SOC()
        
        //voltage compensate
        //voltage_compensation();

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
           // can_send_temp();
            //can_send_current();
            CAN_UPDATE_FLAG=0;
        }
		CyDelay(1000); // wait for next cycle
	} // main loop


    
	for(;;){
		if (WDT_should_clear()){
				WDT_clear(); //even in fatal error, the bms should keep alive
		}
        OK_SIG_Write(0);
		uint8_t index=0;
		if (fatal_err & PACK_TEMP_OVER){        //0x0002
			for (index=0;index<mypack.bad_temp_index;index++){
				if (mypack.bad_temp[index].error==1){
					can_send_status(0xff & index,
					0x00,
					PACK_TEMP_OVER,
					mypack.bad_temp[index].stack,
					mypack.bad_temp[index].cell,
					mypack.bad_temp[index].value16);
				}
			}

		}

		if (fatal_err & PACK_TEMP_UNDER){       //0x0008
			for (index=0;index<mypack.bad_temp_index;index++){
				if (mypack.bad_temp[index].error==0){
					can_send_status(0xff & index,
					0x00,
					PACK_TEMP_UNDER,
					mypack.bad_temp[index].stack,
					mypack.bad_temp[index].cell,
					mypack.bad_temp[index].value16);
				}
                CyDelay(1);
			}

		}
		if (fatal_err & STACK_FUSE_BROKEN){         //0x0004
			can_send_status(0x00,
			0x00,
			STACK_FUSE_BROKEN,
			mypack.bad_temp[index].stack,
			0x00,
			0x0000);
            
		}


		if(fatal_err & CELL_VOLT_OVER){         //0x0800
			for (index=0;index<mypack.bad_cell_index;index++){
				if (mypack.bad_cell[index].error==1){
					can_send_status(0xff & index,
					0x00,
					CELL_VOLT_OVER,
					mypack.bad_temp[index].stack,
					mypack.bad_temp[index].ic*4+mypack.bad_temp[index].cell,
					mypack.bad_temp[index].value16);
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
					mypack.bad_temp[index].stack,
					mypack.bad_temp[index].ic*4+mypack.bad_temp[index].cell,
					mypack.bad_temp[index].value16);
				}
                CyDelay(1);
			}
		}


		CyDelay(500);
	}
    
    
	return 0;
} // main()
