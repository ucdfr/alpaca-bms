/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#include <project.h>
#include <stdio.h>
#include <stdbool.h>

static volatile bool WDT_flag = false;

CY_ISR(WDT_int)
{
    WDT_flag = true;
}


void WDT_init() {
	CyWdtStart(CYWDT_1024_TICKS,CYWDT_LPMODE_NOCHANGE);
    WDT_flag = false;
    WDT_ISR_StartEx(WDT_int);
    WDT_timer_Start();
}

void WDT_clear() {
    WDT_flag = false;
	CyWdtClear();
}

/* [] END OF FILE */
