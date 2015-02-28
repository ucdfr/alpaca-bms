/* ========================================
 *
 * Copyright Formula Racing at UC Davis, 2015
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF FORMULA RACING 
 * AT UC DAVIS.
 *
 * ========================================
*/
#include <project.h>

int main(void)
{
	CyGlobalIntEnable;

	for(;;)
	{
		// poll voltages

		// poll current
		ADC_current_StartConvert();
		ADC_current_IsEndConversion(ADC_current_WAIT_FOR_RESULT);
		ADC_current_StopConvert();
		//sample_full = ADC_DelSig_GetResult16();
		// calculate SOC

		// send can
	} // main loop
	return 0;
} // main()
