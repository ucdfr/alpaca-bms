#include "current_sense.h"
#include "cell_interface.h"
#include "math.h"
extern volatile BATTERYPACK mypack;
extern volatile BMS_STATUS warning_event;
extern volatile BMS_STATUS fatal_err;

void current_init(void)
{
	ADC_current_Start();
} // current_init()




uint16_t get_current(void)
{
	//uint16_t raw_vol = 0;
	//float current=0;
	int32_t current;

	// Delta Sigma measurment of Hall effect sensor
	/*
	ADC_current_StartConvert();
	ADC_current_IsEndConversion(ADC_current_WAIT_FOR_RESULT);
	ADC_current_StopConvert();
	raw_vol = ADC_current_GetResult16();
	raw_vol = (5*(unsigned)raw_vol/0xffff);
	*/
	current = (int32_t)ADC_current_CountsTo_mVolts(ADC_current_Read32());

	//todo: convert current to float
	// current = (Vi - 2.5)/4
	// Vi referenced to 5V
	//current = ((10*current-25)/4)/10;
	current = (current - 2500)/4;

	//mypack.current = (int16_t)floor(current);
	mypack.current = (int16_t)current; 

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
