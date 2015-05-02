#include <project.h>
#include <stdint.h>

#include "cell_interface.h"
#include "current_sense.h"
#include "WDT.h"
#include "data.h"
#include "can_manager.h"

volatile uint8_t CAN_UPDATE_FLAG=0;

CY_ISR(CAN_UPDATE_Handler){
    CAN_UPDATE_FLAG = 1;
}



int main(void)
{
	CyGlobalIntEnable;
	//WDT_init();
	red_led_1_Write(1);
	CyDelay(100);
	red_led_1_Write(0);
	CyDelay(100);
	red_led_1_Write(1);
	CyDelay(100);
	red_led_1_Write(0);
	LCD_Start();
	DEBUG_UART_Start();
	CyGlobalIntEnable;

    
    Can_Update_ISR_StartEx(CAN_UPDATE_Handler);
	LCD_ClearDisplay();
	LCD_Position(0u, 0u);
	LCD_PrintString("BMS DEMO");
	DEBUG_UART_PutString("BMS DEMO START\n");
	CyDelay(2000);
	// TODO Watchdog Timer


	//initialize
	bms_init();

	int pin_value=1;

	uint16_t cell_volt[NUM_CELLS];
	uint16_t temp[NUM_TEMP];
	uint16_t battery_current;
	uint8_t battery_status;
    red_led_1_Write(0);
    Can_Update_Timer_Start();
    
	for(;;)
	{   
		if (WDT_should_clear()) {
			WDT_clear();
		}
	//	check_cfg();
	//	if(check_cells()){
     //       break;
     //   }// TODO Check if cell exists
		if (get_cell_volt()){
            break;
        }// TODO Get voltage
		if (get_cell_temp()){
            break;
        }// TODO Get temperature


		// TODO Determine if need to deassert OK pin. (emergency stop)


		//get_current(); // TODO get current reading from sensor
		//get_soc(); // TODO calculate SOC()
		// send to CAN()
        can_send_status(0x12,
                    0x34,
                    NO_ERROR,
                    0x5678,
                    0x9abc);
        
        
        OK_SIG_Write(1);
		CyDelay(500);

	} // main loop
    
    for(;;){
        //fatal error
        red_led_1_Write(1);
        OK_SIG_Write(0);
        LCD_Position(0u, 0u);
        LCD_PrintString("FATAL ERR FATAL ERR FA");
        can_send_status(0x12,
                    0x34,
                    COM_FAILURE,
                    0x5678,
                    0x9abc);
        
        
    }
    
    
	return 0;
} // main()
