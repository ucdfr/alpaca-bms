#include "current_sense.h"



uint16_t get_current()
{
	uint16_t current = 0;

	// Delta Sigma measurment of Hall effect sensor
	ADC_current_StartConvert();
	ADC_current_IsEndConversion(ADC_current_WAIT_FOR_RESULT);
	ADC_current_StopConvert();
	current = ADC_current_GetResult16();

	// current = (Vi - 2.5)/4
	// Vi referenced to 5V
	current = (current - 0x7FFF) >> 2;

	return current;
}// get_current()



uint16_t get_soc()
{
	return 0;
} // get_soc()
