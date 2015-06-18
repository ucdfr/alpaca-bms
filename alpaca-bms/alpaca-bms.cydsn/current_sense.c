#include "current_sense.h"
#include "cell_interface.h"
extern volatile BATTERYPACK mypack;
extern volatile BMS_STATUS warning_event;
extern volatile BMS_STATUS fatal_err;

void current_init(void)
{
	ADC_current_Start();
} // current_init()




uint16_t get_current(void)
{
	int16_t current = 0;

	// Delta Sigma measurment of Hall effect sensor
	ADC_current_StartConvert();
	ADC_current_IsEndConversion(ADC_current_WAIT_FOR_RESULT);
	ADC_current_StopConvert();
	current = ADC_current_GetResult16();
    //todo: convert current to float
	// current = (Vi - 2.5)/4
	// Vi referenced to 5V
	current = (10*(current-25)/4)/10;

    mypack.current = current;
    
    //charging?
    if (current<0){
    	warning_event |= CHARGEMODE;
    }
	return current;
}// get_current()



uint16_t get_soc()
{
	return 0;
} // get_soc()
