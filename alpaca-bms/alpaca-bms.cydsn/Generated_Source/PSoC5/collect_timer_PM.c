/*******************************************************************************
* File Name: collect_timer_PM.c
* Version 2.70
*
*  Description:
*     This file provides the power management source code to API for the
*     Timer.
*
*   Note:
*     None
*
*******************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "collect_timer.h"

static collect_timer_backupStruct collect_timer_backup;


/*******************************************************************************
* Function Name: collect_timer_SaveConfig
********************************************************************************
*
* Summary:
*     Save the current user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  collect_timer_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void collect_timer_SaveConfig(void) 
{
    #if (!collect_timer_UsingFixedFunction)
        collect_timer_backup.TimerUdb = collect_timer_ReadCounter();
        collect_timer_backup.InterruptMaskValue = collect_timer_STATUS_MASK;
        #if (collect_timer_UsingHWCaptureCounter)
            collect_timer_backup.TimerCaptureCounter = collect_timer_ReadCaptureCount();
        #endif /* Back Up capture counter register  */

        #if(!collect_timer_UDB_CONTROL_REG_REMOVED)
            collect_timer_backup.TimerControlRegister = collect_timer_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: collect_timer_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  collect_timer_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void collect_timer_RestoreConfig(void) 
{   
    #if (!collect_timer_UsingFixedFunction)

        collect_timer_WriteCounter(collect_timer_backup.TimerUdb);
        collect_timer_STATUS_MASK =collect_timer_backup.InterruptMaskValue;
        #if (collect_timer_UsingHWCaptureCounter)
            collect_timer_SetCaptureCount(collect_timer_backup.TimerCaptureCounter);
        #endif /* Restore Capture counter register*/

        #if(!collect_timer_UDB_CONTROL_REG_REMOVED)
            collect_timer_WriteControlRegister(collect_timer_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: collect_timer_Sleep
********************************************************************************
*
* Summary:
*     Stop and Save the user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  collect_timer_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void collect_timer_Sleep(void) 
{
    #if(!collect_timer_UDB_CONTROL_REG_REMOVED)
        /* Save Counter's enable state */
        if(collect_timer_CTRL_ENABLE == (collect_timer_CONTROL & collect_timer_CTRL_ENABLE))
        {
            /* Timer is enabled */
            collect_timer_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            collect_timer_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    collect_timer_Stop();
    collect_timer_SaveConfig();
}


/*******************************************************************************
* Function Name: collect_timer_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration
*
* Parameters:
*  void
*
* Return:
*  void
*
* Global variables:
*  collect_timer_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void collect_timer_Wakeup(void) 
{
    collect_timer_RestoreConfig();
    #if(!collect_timer_UDB_CONTROL_REG_REMOVED)
        if(collect_timer_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                collect_timer_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */
