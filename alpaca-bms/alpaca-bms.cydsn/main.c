#include <project.h>
#include <stdint.h>

#include "cell_interface.h"
#include "current_sense.h"

#define C1_CELLS 28
#define C2_CELLS 28
#define C3_CELLS 28
#define NUM_CELLS C1_CELLS + C2_CELLS + C3_CELLS

uint16_t cell_volt[NUM_CELLS];



int main(void)
{
	CyGlobalIntEnable;
	// TODO Watchdog Timer
    
    
    //initialize
    bms_init();
    
	for(;;)
	{
		check_chips(); // TODO Check if chip exists
		check_cells(); // TODO Check if cell exists
		get_cell_volt(); // TODO Get voltage
		get_cell_temp(); // TODO Get temperature


		// TODO Determine if need to deassert OK pin. (emergency stop)

		
		get_current(); // TODO get current reading from sensor
		get_soc(); // TODO calculate SOC()
		// send to CAN()

	} // main loop
	return 0;
} // main()
