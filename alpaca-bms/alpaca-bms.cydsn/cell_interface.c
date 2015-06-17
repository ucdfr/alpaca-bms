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



extern volatile uint8_t CAN_UPDATE_FLAG;
extern volatile BMS_STATUS fatal_err;
extern volatile BMS_STATUS warning_err;
extern volatile uint8_t error_IC;
extern volatile uint8_t error_CHIP;
volatile uint8_t error_voltage_count=0;
volatile uint8_t error_temperature_count=0;
volatile BATTERYPACK mypack;


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



uint8_t get_cell_volt(){
    LTC68_ClearFIFO();
   // DEBUG_UART_PutString("Enter GET_CELL_VOLT\n");
    int error;
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
    
    //get information
    update_volt(cell_codes);
    
    
    //check error
    if (mypack.status!=NOMRAL){
        if (mypack.bad_cell_index>0){
            return 1;
        }
    }
    
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
 

    //get information
    update_temp(*aux_codes[6]);

    //check error
    //check error
    if (mypack.status!=NOMRAL){
        if (mypack.bad_temp_index>0){
            return 1;
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


void update_volt(uint16_t cell_codes[TOTAL_IC][12]){
    uint8_t cell=0;
    uint8_t ic=0;
    uint32_t stack_volt=0;
    uint8_t stack=0;
    uint16_t voltage;

    //log in voltage data
    for (ic=0;ic<TOTAL_IC;ic++){
        for (cell=0;cell<12;cell++){
            if (CELL_ENABLE & (0x1<<cell)){
                mypack.cell[ic/4][ic%4][cell].value16=cell_codes[ic][cell];
            }
        }
    }

    //update stack voltage
    for (stack =0; stack<3;stack++){
        ic=0;
        cell=0;
        stack_volt=0;
        for (ic=0;ic<4;ic++){
            cell=0;
            for (cell=0;cell<7;cell++){
                stack_volt =stack_volt+mypack.cell[stack][ic][cell].value16;
            }
        }
        mypack.stack[stack].value32=stack_volt;
    }

    //update pack voltage
    stack_volt=0;
    for (stack =0;stack<3;stack++){
        stack_volt = stack_volt + mypack.stack[stack].value32;
    }
    mypack.pack.value32=stack_volt;


    //update healthy status
    for (stack = 0; stack <3;stack ++){
        for (ic =0; ic< 4;ic++){
            for (cell=0;cell<7;cell++){
                voltage = mypack.cell[stack][ic][cell].value16;
                if (voltage>OVER_VOLTAGE){
                    mypack.cell[stack][ic][cell].bad_counter++;
                    mypack.cell[stack][ic][cell].bad=1;
                }else if (voltage < UNDER_VOLTAGE){
                    mypack.cell[stack][ic][cell].bad_counter++;
                    mypack.cell[stack][ic][cell].bad=0;
                }else{
                    if (mypack.cell[stack][ic][cell].bad_counter>0){
                        mypack.cell[stack][ic][cell].bad_counter--;
                    }
                   
                }

                //check faulty cell
                if (mypack.cell[stack][ic][cell].bad_counter>ERROR_VOLTAGE_LIMIT){
                    mypack.bad_cell[mypack.bad_cell_index].stack=stack;
                    mypack.bad_cell[mypack.bad_cell_index].ic=ic;
                    mypack.bad_cell[mypack.bad_cell_index].cell=cell;
                    mypack.bad_cell[mypack.bad_cell_index].error=mypack.cell[stack][ic][cell].bad;
                    if (mypack.bad_cell_index<255){
                        mypack.bad_cell_index++;
                    }else{
                        mypack.bad_cell_index=255;
                    }
                    mypack.status = FAULT;
                    if (mypack.bad_cell[mypack.bad_cell_index].error){
                        fatal_err |= CELL_VOLT_OVER;
                    }else{
                        fatal_err |= CELL_VOLT_UNDER;  
                    }
                }
            }
        }
    }



}
                


void update_temp(uint16_t aux_codes[TOTAL_IC][6]){
    uint8_t cell=0;
    uint8_t ic=0;
    uint8_t stack=0;
    uint16_t temp;

    //log in temp data
    for (ic=0;ic<TOTAL_IC;ic++){
        for (cell=0;cell<5;cell++){
            mypack.temp[ic/4][ic*5+cell].value8=temp_transfer(aux_codes[ic][cell]);        
        }
    }



    //update healthy status
    for (stack = 0; stack <3;stack ++){
        for (cell=0;cell<20;cell++){
            temp = mypack.temp[stack][cell].value8;
            if (temp>OVER_TEMP){
                mypack.temp[stack][cell].bad_counter++;
                mypack.temp[stack][cell].bad=1;
            }else if (temp < UNDER_TEMP){
                mypack.temp[stack][cell].bad_counter++;
                mypack.temp[stack][cell].bad=0;
            }else{
                if (mypack.temp[stack][cell].bad_counter>0){
                    mypack.temp[stack][cell].bad_counter--;
                }
                
            }

            //check faulty temp
            if (mypack.temp[stack][cell].bad_counter>ERROR_TEMPERATURE_LIMIT){
                mypack.bad_temp[mypack.bad_temp_index].stack=stack;
                mypack.bad_temp[mypack.bad_temp_index].cell=cell;
                mypack.bad_temp[mypack.bad_temp_index].error=mypack.temp[stack][cell].bad;
                if (mypack.bad_temp_index<255){
                    mypack.bad_temp_index++;
                }else{
                    mypack.bad_temp_index=255;
                }
                mypack.status = FAULT;
                if (mypack.bad_cell[mypack.bad_temp_index].error){
                    fatal_err |= PACK_TEMP_OVER;
                }else{
                    fatal_err |= PACK_TEMP_UNDER;  
                }
            }
        }
    }
}




void mypack_init(){
    mypack.status = NOMRAL;
    mypack.bad_cell_index =0;
    mypack.bad_temp_index =0;
    mypack.fuse_fault=NORMAL;
    mypack.voltage =0;
}

void check_stack_fuse(){
    if ((mypack.stack[0].value32 - mypack.stack[1].value32 > STACK_VOLT_DIFF_LIMIT) && (mypack.stack[0].value32 - mypack.stack[2].value32 > STACK_VOLT_DIFF_LIMIT)){
            mypack.status = FAULT;
            fatal_err |= STACK_FUSE_BROKEN;
            mypack.fuse_fault=STACK0;
        }else if ((mypack.stack[1].value32 - mypack.stack[0].value32 > STACK_VOLT_DIFF_LIMIT) && (mypack.stack[0].value32 - mypack.stack[2].value32 > STACK_VOLT_DIFF_LIMIT)){
            mypack.status = FAULT;
            fatal_err |= STACK_FUSE_BROKEN;
            mypack.fuse_fault=STACK1;
        }else if ((mypack.stack[2].value32 - mypack.stack[1].value32 > STACK_VOLT_DIFF_LIMIT) && (mypack.stack[0].value32 - mypack.stack[1].value32 > STACK_VOLT_DIFF_LIMIT)){
            mypack.status = FAULT;
            fatal_err |= STACK_FUSE_BROKEN;
            mypack.fuse_fault=STACK2;
        }
}



uint8_t temp_transfer(uint16_t raw){
    //translate raw reading to C temperature
    return (0xff & raw);
}
//void balance_cells(){}// balance_cells()
