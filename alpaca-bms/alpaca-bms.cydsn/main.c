#include <project.h>
#include <stdint.h>

#include "cell_interface.h"
#include "current_sense.h"
#include "WDT.h"
#include "data.h"
#include "can_manager.h"

volatile uint8_t CAN_UPDATE_FLAG=1;
volatile uint16_t CAN_count=0;
volatile BMS_STATUS warning_err;
volatile BMS_STATUS fatal_err;
volatile uint8_t error_IC;
volatile uint8_t error_CHIP;
extern volatile BATTERYPACK mypack;



CY_ISR(CAN_UPDATE_Handler){
    CAN_UPDATE_FLAG = 1;
    CAN_count++;
}



int main(void)
{
	CyGlobalIntEnable;

	Can_Update_ISR_StartEx(CAN_UPDATE_Handler);
	Can_Update_Timer_Start();
	can_init();
	CyGlobalIntEnable;

	// TODO Watchdog Timer


	// Initialize
	bms_init();
	mypack_init();

	current_init();

	int pin_value=1;

	uint16_t cell_volt[NUM_CELLS];
	uint16_t temp[NUM_TEMP];
	uint16_t battery_current;
	uint8_t battery_status;

	// Initialize err event
	fatal_err = NO_ERROR;
	warning_err = NO_ERROR;

	OK_SIG_Write(0);
    
	for(;;) // main loop
	{   
		uint8_t warning_index=0;

		if (WDT_should_clear())
			WDT_clear();
        
		//check_cfg();
		//check_cells();// TODO Check if cell exists
        get_cell_volt();// TODO Get voltage
		//check_stack_fuse(); // TODO: check if stacks are disconnected
		
		//get_cell_temp();// TODO Get temperature


		//battery_current = get_current(); // TODO get current reading from sensor
		//get_soc(); // TODO calculate SOC()


		if (fatal_err)
			break; // break from main loop and enter fault loop

		if (warning_err){
			for (warning_index =0; warning_index<16;warning_index++){
				if (fatal_err & (0x1<<warning_index)){
					can_send_status(0x00,
							0x00,
							(0x1<<warning_index),
							0x0000,
							0x0000);
				} // fatal_err 
			} // for each warning
		} // if warning err
		else{
			can_send_status(0x00,
					0x00,
					NO_ERROR,
					0x0000,
					0x0000);
		} // else send no error

		//  OK_SIG_Write(1);
		CyDelay(100); // wait for next cycle
	} // main loop


    
	for(;;){
		uint8_t event_index=0;
		//LCD_Position(0u, 0u);
		//LCD_PrintString("FATAL ERR FATAL ERR FA");
		for (;;){
			//fatal error
			CyDelay(500);

			if (WDT_should_clear())
				WDT_clear(); //even in fatal error, the bms should keep alive

			//OK_SIG_Write(0);

			for (event_index =0; event_index<16;event_index++){
				if (fatal_err & (0x1<<event_index)){
					can_send_status(0x00,
							0x00,
							(0x1<<event_index),
							(error_IC<<4) | (error_CHIP),
							0x0000);
				} // if fatal
			} // for every event
		} // fault loop
	} // fault loop: testing?
    
    
	return 0;
} // main()
