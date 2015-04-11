#include "current_sense.h"



uint16_t get_current()
{
	uint16_t current = 0;

	// poll current
		ADC_current_StartConvert();
		ADC_current_IsEndConversion(ADC_current_WAIT_FOR_RESULT);
		ADC_current_StopConvert();
		//sample_full = ADC_DelSig_GetResult16();
        current = ADC_current_GetResult16();
        LCD_Position(0u,0u);
        LCD_PrintString("Current current:");
        LCD_Position(1u,0u);
        LCD_PrintInt16(current);
	return current;
}// get_current()



uint16_t get_soc()
{
	return 0;
} // get_soc()