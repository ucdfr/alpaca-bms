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



#ifndef CELL_INTERFACE_H
#define CELL_INTERFACE_H

    #include <stdint.h>
    #include <project.h>
    #include "can_manager.h"
   
    #define ERROR_VOLTAGE_LIMIT (3u)
    #define ERROR_TEMPERATURE_LIMIT (3u)
    #define FUSE_BAD_LIMIT (3u)
    
    #define CELL_ENABLE (0x1cf)
    #define OVER_VOLTAGE (42000u)
    #define UNDER_VOLTAGE (20000u)
    #define STACK_VOLT_DIFF_LIMIT (10000u)
    #define CRITICAL_TEMP_L (2000u)          //0.2V
    #define CRITICAL_TEMP_H (30000u)             //1.0213V  10213
    #define BAD_THERM_LIMIT (8u)
    

    //#define DEBUG_LCD 0

    #define OVER_TEMP (90u)             //now it just for debug purpose
    #define UNDER_TEMP (0u)

typedef enum {
  NOMRAL =0,
  WARNING =1,
  FAULT =2,
}BMS_HEALTH;


typedef struct {
  uint16_t value16;
  uint32_t value32;
  uint8_t bad;
  uint8_t bad_counter;
  uint16_t vcc;
}BAT_VOLT;

typedef struct {
  uint16_t value16;
  uint8_t bad;
  uint8_t bad_counter;
}BAT_TEMP;


typedef struct{
  uint8_t stack;
  uint8_t ic;
  uint8_t cell;
  uint8_t error;
  uint16_t value16;
}BAT_ERROR;

typedef enum{
  NORMAL = 0,
  STACK0 = 1,
  STACK1 = 2,
  STACK2 = 3
}STACK_ID;

typedef struct 
{
  BMS_HEALTH status;
  uint8_t bad_cell_index;
  uint8_t bad_temp_index;
  BAT_ERROR bad_cell[255];
  BAT_ERROR bad_temp[255];
  BAT_VOLT cell[3][4][7];
  BAT_TEMP temp[3][6];
  BAT_VOLT stack[3];
  BAT_VOLT pack;
  STACK_ID fuse_fault;
  uint32_t voltage;
  int16_t current;
}BATTERYPACK;









/**
 * @initialize. In case need to setup anything, write them in it.
 *
 * @param no input parameters.
 * @return 1 if everything is OK. 0 for hard failure.
 */
void  bms_init();




/**
 * @check config register
 *
 * @param no input parameters.
 * @return 1 if everything is OK. 0 for hard failure.
 */
void check_cfg();


/**
 * @wake all BMSs up, waiting for commands
 *
 * @param no input parameters.
 * @return 1 if everything is OK. 0 for hard failure.
 */
void  wake_up();

/**
 * @check if chips are exist without and error
 *
 * @param no input parameters.
 * @return 1 if everything is OK. 0 for hard failure.
 */
void check_chips();


/**
 * @check if cells are existed 
 *
 * @param no input parameters.
 * @return 1 if everything is OK. 0 for hard failure.
 */
uint8_t check_cells();

/**
 * @check every cells if voltages are in safe range
 *
 * @param no input parameters.
 * @return 1 if everything is OK. 0 for hard failure.
 */
uint8_t get_cell_volt();

/**
 * @check every cells if temperature are in safe range
 *
 * @param no input parameters.
 * @return 1 if everything is OK. 0 for hard failure.
 */
uint8_t get_cell_temp();

/**
 * @balance each cell
 *
 * @param no input parameters.
 * @return 1 if everything is OK. 0 for hard failure.
 */
//void balance_cells();

/**
 * @update voltage and detect error
 *
 * @param 1 input parameters, which is raw cell_codes.
 * @return NULL.
 */
void update_volt(uint16_t cell_codes[TOTAL_IC][12]);

/**
 * @update temperature and detect error
 *
 * @param 1 input parameters, which is raw aux_codes.
 * @return NULL.
 */
void update_temp(uint16_t aux_codes[TOTAL_IC][6]);

/**
 * @initial mypack
 *
 * @param no input parameters.
 * @return NULL.
 */
void mypack_init();

/**
 * @check is fuse is broken
 *
 * @param no input parameters. (use global mypack)
 * @return NULL.
 */
void check_stack_fuse();

uint8_t temp_transfer(uint16_t);

void voltage_compensation();

#endif // CELL_INTERFACE_H
