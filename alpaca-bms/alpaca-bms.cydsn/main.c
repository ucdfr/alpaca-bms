#include <project.h>
#include <stdint.h>

#include "cell_interface.h"
#include "current_sense.h"
#include "WDT.h"

#define C1_CELLS 28
#define C2_CELLS 28
#define C3_CELLS 28
#define NUM_CELLS C1_CELLS + C2_CELLS + C3_CELLS

uint16_t cell_volt[NUM_CELLS];



int main(void)
{
    CyGlobalIntEnable;
    WDT_init();
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
    
    LCD_ClearDisplay();
    LCD_Position(0u, 0u);
    LCD_PrintString("BMS DEMO");
    DEBUG_UART_PutString("BMS DEMO START\n");
    CyDelay(2000);
	// TODO Watchdog Timer
    
    
    //initialize
    bms_init();
    
    int pin_value=1;
    
	for(;;)
	{   
        WDT_clear();
        check_cfg();
		check_chips(); // TODO Check if chip exists
		check_cells(); // TODO Check if cell exists
		get_cell_volt(); // TODO Get voltage
		get_cell_temp(); // TODO Get temperature


		// TODO Determine if need to deassert OK pin. (emergency stop)

		
		//get_current(); // TODO get current reading from sensor
		//get_soc(); // TODO calculate SOC()
		// send to CAN()
        CyDelay(10000);

	} // main loop
    
	return 0;
} // main()
