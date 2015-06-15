/* LICENSE
	cell_interface.h and cell_interface.c are derivatives of source from Linear
	Technology Corp.(LTC)
*/

/************************************
REVISION HISTORY
$Revision: 1000 $
$Date: 2013-07-15 

Copyright (c) 2013, Linear Technology Corp.(LTC)
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are those
of the authors and should not be interpreted as representing official policies,
either expressed or implied, of Linear Technology Corp.

The Linear Technology Linduino is not affiliated with the official Arduino team.
However, the Linduino is only possible because of the Arduino team's commitment
to the open-source community.  Please, visit http://www.arduino.cc and
http://store.arduino.cc , and consider a purchase that will help fund their
ongoing work.

Copyright 2013 Linear Technology Corp. (LTC)
***********************************************************/



#include "cell_interface.h"
#include "LTC68041.h"

#define OVER_VOLTAGE (0x4000)
#define UNDER_VOLTAGE (0x1000)

//#define DEBUG_LCD 0

#define OVER_TEMP (20000)
#define UNDER_TEMP (10000)

extern volatile uint8_t CAN_UPDATE_FLAG;
extern volatile BMS_STATUS fatal_err;
extern volatile BMS_STATUS warning_err;
extern volatile uint8_t error_IC;
extern volatile uint8_t error_CHIP;
volatile uint8_t error_voltage_count=0;
volatile uint8_t error_temperature_count=0;


/**
 * @initialize. In case need to setup anything, write them in it.
 *
 * @param no input parameters.
 * @return 1 if everything is OK. 0 for hard failure.
 */
void  bms_init(){
    //setup SS pin
    SS_SetDriveMode(SS_DM_RES_UP);
    LTC68_Start();
    LTC6804_initialize();
    LTC6804_wrcfg(TOTAL_IC,tx_cfg);
}


/**
 * @wake all BMSs up, waiting for commands
 *
 * @param no input parameters.
 * @return 1 if everything is OK. 0 for hard failure.
 */
void  wake_up(){
    wakeup_sleep();
}

void check_cfg(){
    //DEBUG_UART_PutString("Enter Check_CFG\n");
    int i=0;
    wakeup_sleep();
    LTC6804_rdcfg(TOTAL_IC,rx_cfg);
    //LCD_Position(1u,0u);
    for (i=0;i<8;i++){
        if (rx_cfg[i] != tx_cfg[i]){
              fatal_err = COM_FAILURE;
            return;
        }
    }
}



void check_chips(){
    
}// check_chips()

uint8_t check_cells(){ 
    //using ADOW
  uint16_t cell_pu[TOTAL_IC][12];
  uint16_t cell_pd[TOTAL_IC][12];
  int error;
  uint8_t i_IC=0;
  uint8_t i_cell=0;

  wakeup_sleep();

  LTC6804_adow(ADOW_PUP_UP);
  error = LTC6804_rdcv(0, TOTAL_IC,cell_pu); // Set to read back all cell voltage registers

  wakeup_sleep();

  LTC6804_adow(ADOW_PUP_DOWN);
  error = LTC6804_rdcv(0, TOTAL_IC,cell_pd); // Set to read back all cell voltage registers

  if (error==-1){
    return 1;
    }

  for (i_IC=0;i_IC<TOTAL_IC;i_IC++){
    for (i_cell=0;i_cell<12;i_cell++){
      if ((((int16_t)cell_pu[i_IC][i_cell+1]-(int16_t)cell_pd[i_IC][i_cell+1]) < -400) && (CELL_ENABLE&(0x1<<i_cell))){
        fatal_err |= CELL_VOLT_UNDER;
        //LCD_Position(1u,0u);
        //LCD_PrintString("big ");
        return 1;
      }
      if (cell_pu[i_IC][0]==0){
        fatal_err |= CELL_VOLT_UNDER;
        //LCD_Position(1u,0u);
        //LCD_PrintString("eq 0");
        return 1;
      }
      if (cell_pd[i_IC][11]==0){
        fatal_err |= CELL_VOLT_UNDER;
        return 1;
      }
    }

  }
    return 0;

}// check_cells()


uint8_t get_cell_volt(){
    LTC68_ClearFIFO();
   // DEBUG_UART_PutString("Enter GET_CELL_VOLT\n");
    int error;
    uint8_t i=0;
    uint8_t ic=0;
    wakeup_sleep();
    LTC6804_adcv();
    CyDelay(10);
    wakeup_sleep();
    error = LTC6804_rdcv(0, TOTAL_IC,cell_codes); // Set to read back all cell voltage registers
    if (error == -1)
    {
        #ifdef DEBUG_LCD
            LCD_Position(0u,10u);
            LCD_PrintString("ERROR");
        #endif
       return 1;
    }
    
    
    for (ic=0;ic<TOTAL_IC;ic++){
        for (i=0;i<12;i++){
            if (CELL_ENABLE & (0x1<<i)){
                if (cell_codes[ic][i]>OVER_VOLTAGE){
                    #ifdef DEBUG_LCD
                        LCD_Position(1u,0u);
                        LCD_PrintHexUint8(i);
                        LCD_Position(1u,2u);
                        LCD_PrintString("OVER VOLTAGE");
                    #endif
                error_IC = ic;
                error_CHIP = i;
                fatal_err |= CELL_VOLT_OVER;
                return 1;
            }else if (cell_codes[ic][i]<UNDER_VOLTAGE){
                #ifdef DEBUG_LCD
                    LCD_Position(1u,0u);
                    LCD_PrintHexUint8(i);
                    LCD_Position(1u,2u);
                    LCD_PrintString("UNDER VOLTAGE");
                #endif
                error_IC = ic;
                error_CHIP = i;
                fatal_err |= CELL_VOLT_UNDER;
                return 1;
                }
            else{
                if (CAN_UPDATE_FLAG){
                    can_send_volt(ic,
                    i,
                    cell_codes);
                }
                CyDelay(20);
                }
            }
        }
    }

  /*  
    for (ic=0;ic<TOTAL_IC;ic++){
        for (i=0;i<12;i++){
            if (CELL_ENABLE & (0x1<<i)){
                        can_send_volt(ic,
                        i,
                        cell_codes);
                        CyDelay(10);
            }
        }
    }
    */
    
    return 0;
}// get_cell_volt()


uint8_t get_cell_temp(){
    int error;
    int i=0;
    wakeup_sleep();
    LTC6804_adax();
    CyDelay(6);  
    wakeup_sleep();
    error = LTC6804_rdaux(0,TOTAL_IC,aux_codes); // Set to read back all aux registers
    if (error == -1)
    {
        #ifdef DEBUG_LCD
        LCD_Position(0u,10u);
        LCD_PrintString("ERROR");
        #endif
        return 1;
    }
 
    for (i=0;i<12;i++){
        if (aux_codes[0][i]>OVER_TEMP){
            #ifdef DEBUG_LCD
            LCD_Position(1u,0u);
            LCD_PrintString("OVER TEMP");
            #endif
           // error_IC = ic;
           // error_TEMP = i;
            fatal_err |= PACK_TEMP_OVER;
            return 1;
        }else if (aux_codes[0][i]<UNDER_TEMP){
            #ifdef DEBUG_LCD
            LCD_Position(1u,0u);
            LCD_PrintString("UNDER TEMP");
            #endif
            warning_err |= PACK_TEMP_UNDER;
            return 1;
        }else{
            warning_err &= ~PACK_TEMP_UNDER;
                if (CAN_UPDATE_FLAG){
                    can_send_temp(0,aux_codes);
                }
            }
    }
   
    #ifdef DEBUG_LCD
    LCD_Position(1u,10u);
    print_cells(aux_codes[0][0]);
    LCD_Position(0u,10u);
    LCD_PrintString("OK");
    #endif
    return 0;
}// get_cell_temp()
//void balance_cells(){}// balance_cells()
