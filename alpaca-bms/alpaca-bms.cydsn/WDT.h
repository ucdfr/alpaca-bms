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

#ifndef WDT_H
#define WDT_H
    
#include <stdbool.h>

void WDT_init();
void WDT_clear();
bool WDT_should_clear();

#endif

/* [] END OF FILE */
