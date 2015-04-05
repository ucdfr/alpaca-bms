#include <project.h>
#include <stdint.h>
#include "cell_interface.h"

#define C1_CELLS 28
#define C2_CELLS 28
#define C3_CELLS 28
#define NUM_CELLS C1_CELLS + C2_CELLS + C3_CELLS

uint16_t cell_volt[NUM_CELLS];



int main(void)
{
	CyGlobalIntEnable;
	// TODO Watchdog Timer

	for(;;)
	{
		// TODO Check if chip exists
		// TODO Check if cell exists
		get_cell_volt(); // TODO Get voltage
		get_cell_temp(); // TODO Get temperature


		// TODO Determine if need to deassert OK pin. (emergency stop)


			// poll current
		ADC_current_StartConvert();
		ADC_current_IsEndConversion(ADC_current_WAIT_FOR_RESULT);
		ADC_current_StopConvert();
		//sample_full = ADC_DelSig_GetResult16();

		// get current
		// calculate SOC()
		// send to CAN()

	} // main loop
	return 0;
} // main()
