#include <project.h>
#include <stdint.h>

#define C1_CELLS 28
#define C2_CELLS 28
#define C3_CELLS 28
#define CELLS C1_CELLS + C2_CELLS + C3_CELLS

uint16_t cell_volt[CELLS];



int main(void)
{
	CyGlobalIntEnable;

	for(;;)
	{
		// poll voltages()
		// adjust cells()

		// poll temperature()
		// poll for the other LTC6801 things()

		// poll current
		ADC_current_StartConvert();
		ADC_current_IsEndConversion(ADC_current_WAIT_FOR_RESULT);
		ADC_current_StopConvert();
		//sample_full = ADC_DelSig_GetResult16();

		// calculate SOC()

		// send to CAN()

	} // main loop
	return 0;
} // main()
