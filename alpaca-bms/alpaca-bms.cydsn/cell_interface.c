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

uint8_t _send_byte(uint8_t msg){
}

/**
 * @wake all BMSs up, waiting for commands
 *
 * @param no input parameters.
 * @return 1 if everything is OK. 0 for hard failure.
 */
uint8_t  wake_up(){
    //1. Send a dummy byte. The activity on CSB and SCK will wake up the serial interface on device S1.
    _send_byte(DUMMY_BYTE);
    //2. Wait for the amount of time n • tWAKE in order to power up all devices S1, S2 and S3.
    CyDelay(CELL_TOTAL_NUMBER*tWAKE);
    //3. Send a second dummy byte.
    _send_byte(DUMMY_BYTE);
    //4. Wait for the amount of time n • tREADY
    return 1;
}



uint8_t check_chips(){
}// check_chips()

uint8_t check_cells(){
    int i=0;
    for (i=0;i<CELL_TOTAL_NUMBER;i++){
        
    }
    LTC68_PutArray(command_array, CELL_TOTAL_NUMBER);    
    return 1;
}// check_cells()


uint8_t get_cell_volt(){
//1. Pull CSB low
//2. Send ADCV command with MD[1:0] = 10 and DCP = 1 i.e. 0x03 0x70 and its PEC (0xAF 0x42)
//3. Pull CSB high
return 1;
}// get_cell_volt()


uint8_t get_cell_temp(){}// get_cell_temp()
//void balance_cells(){}// balance_cells()
