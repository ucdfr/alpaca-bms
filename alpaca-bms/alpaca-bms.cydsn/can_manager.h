#ifndef CAN_MANAGER_H
#define CAN_MANAGER_H

#include <project.h>
#include "data.h"
#include "LTC68041.h"

typedef enum{
    NO_ERROR=0x0000,
    CHARGEMODE=0x0001,
    PACK_TEMP_OVER=0x0002,
    STACK_FUSE_BROKEN=0x0004,
    PACK_TEMP_UNDER=0x0008,
    LOW_SOC = 0x0010,
    CRITICAL_SOC = 0x0020,
    IMBALANCE = 0x0040,
    COM_FAILURE = 0x0080,
    NEG_CONT_CLOSED = 0x0100,
    POS_CONT_CLOSED = 0x0200,
    ISO_FAULT = 0x0400,
    CELL_VOLT_OVER = 0x0800,
    CELL_VOLT_UNDER = 0x1000,
    CHARGE_HAULT = 0x2000,
    FULL = 0x4000,
    PRECHARGE_CLOSED = 0x8000
}BMS_STATUS;

    
    
    
void can_send_temp(uint8_t IC_index, uint16_t temp[6]);
void can_send_volt(uint8_t IC_index,
                    uint8_t cell_index,
                    uint16_t [TOTAL_IC][12]);
void can_send_current(uint8_t IC_index, uint16_t battery_current);
void can_send_status(uint8_t SOC_P,
                    uint8_t AH,
                    BMS_STATUS status,
                    uint16_t charge_cy,
                    uint16_t delta);
uint8_t can_test_send();
void can_init();

#endif // CAN_MANAGER_H
