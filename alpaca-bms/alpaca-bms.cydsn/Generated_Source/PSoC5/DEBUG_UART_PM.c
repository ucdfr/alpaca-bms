/*******************************************************************************
* File Name: DEBUG_UART_PM.c
* Version 2.40
*
* Description:
*  This file provides Sleep/WakeUp APIs functionality.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "DEBUG_UART.h"


/***************************************
* Local data allocation
***************************************/

static DEBUG_UART_BACKUP_STRUCT  DEBUG_UART_backup =
{
    /* enableState - disabled */
    0u,
};



/*******************************************************************************
* Function Name: DEBUG_UART_SaveConfig
********************************************************************************
*
* Summary:
*  This function saves the component configuration and nonretention registers.
*  It also saves the current component parameter values, as defined in the
*  Configure dialog or as modified by appropriate APIs. This function is called
*  by the DEBUG_UART_Sleep() function.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  DEBUG_UART_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
* Side Effects:
*  All nonretention registers except FIFO are saved to RAM
*
*******************************************************************************/
void DEBUG_UART_SaveConfig(void)
{
    #if(DEBUG_UART_CONTROL_REG_REMOVED == 0u)
        DEBUG_UART_backup.cr = DEBUG_UART_CONTROL_REG;
    #endif /* End DEBUG_UART_CONTROL_REG_REMOVED */
}


/*******************************************************************************
* Function Name: DEBUG_UART_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the user configuration of nonretention registers.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  DEBUG_UART_backup - used when non-retention registers are restored.
*
* Reentrant:
*  No.
*
* Side Effects:
*  All nonretention registers except FIFO loaded from RAM. This function should
*  be called only after DEBUG_UART_SaveConfig() is called, otherwise
*  incorrect data will be loaded into the registers.
*
*******************************************************************************/
void DEBUG_UART_RestoreConfig(void)
{
    #if(DEBUG_UART_CONTROL_REG_REMOVED == 0u)
        DEBUG_UART_CONTROL_REG = DEBUG_UART_backup.cr;
    #endif /* End DEBUG_UART_CONTROL_REG_REMOVED */
}


/*******************************************************************************
* Function Name: DEBUG_UART_Sleep
********************************************************************************
*
* Summary:
*  This is the preferred API to prepare the component for sleep. 
*  The DEBUG_UART_Sleep() API saves the current component state. Then it
*  calls the DEBUG_UART_Stop() function and calls 
*  DEBUG_UART_SaveConfig() to save the hardware configuration.
*  Call the DEBUG_UART_Sleep() function before calling the CyPmSleep() 
*  or the CyPmHibernate() function. 
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  DEBUG_UART_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void DEBUG_UART_Sleep(void)
{
    #if(DEBUG_UART_RX_ENABLED || DEBUG_UART_HD_ENABLED)
        if((DEBUG_UART_RXSTATUS_ACTL_REG  & DEBUG_UART_INT_ENABLE) != 0u)
        {
            DEBUG_UART_backup.enableState = 1u;
        }
        else
        {
            DEBUG_UART_backup.enableState = 0u;
        }
    #else
        if((DEBUG_UART_TXSTATUS_ACTL_REG  & DEBUG_UART_INT_ENABLE) !=0u)
        {
            DEBUG_UART_backup.enableState = 1u;
        }
        else
        {
            DEBUG_UART_backup.enableState = 0u;
        }
    #endif /* End DEBUG_UART_RX_ENABLED || DEBUG_UART_HD_ENABLED*/

    DEBUG_UART_Stop();
    DEBUG_UART_SaveConfig();
}


/*******************************************************************************
* Function Name: DEBUG_UART_Wakeup
********************************************************************************
*
* Summary:
*  This is the preferred API to restore the component to the state when 
*  DEBUG_UART_Sleep() was called. The DEBUG_UART_Wakeup() function
*  calls the DEBUG_UART_RestoreConfig() function to restore the 
*  configuration. If the component was enabled before the 
*  DEBUG_UART_Sleep() function was called, the DEBUG_UART_Wakeup()
*  function will also re-enable the component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  DEBUG_UART_backup - used when non-retention registers are restored.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void DEBUG_UART_Wakeup(void)
{
    DEBUG_UART_RestoreConfig();
    #if( (DEBUG_UART_RX_ENABLED) || (DEBUG_UART_HD_ENABLED) )
        DEBUG_UART_ClearRxBuffer();
    #endif /* End (DEBUG_UART_RX_ENABLED) || (DEBUG_UART_HD_ENABLED) */
    #if(DEBUG_UART_TX_ENABLED || DEBUG_UART_HD_ENABLED)
        DEBUG_UART_ClearTxBuffer();
    #endif /* End DEBUG_UART_TX_ENABLED || DEBUG_UART_HD_ENABLED */

    if(DEBUG_UART_backup.enableState != 0u)
    {
        DEBUG_UART_Enable();
    }
}


/* [] END OF FILE */
