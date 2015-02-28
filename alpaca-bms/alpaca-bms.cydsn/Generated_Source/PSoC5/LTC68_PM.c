/*******************************************************************************
* File Name: LTC68_PM.c
* Version 2.40
*
* Description:
*  This file contains the setup, control and status commands to support
*  component operations in low power mode.
*
* Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "LTC68_PVT.h"

static LTC68_BACKUP_STRUCT LTC68_backup =
{
    LTC68_DISABLED,
    LTC68_BITCTR_INIT,
    #if(CY_UDB_V0)
        LTC68_TX_INIT_INTERRUPTS_MASK,
        LTC68_RX_INIT_INTERRUPTS_MASK
    #endif /* CY_UDB_V0 */
};


/*******************************************************************************
* Function Name: LTC68_SaveConfig
********************************************************************************
*
* Summary:
*  Saves SPIM configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  LTC68_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void LTC68_SaveConfig(void) 
{
    /* Store Status Mask registers */
    #if(CY_UDB_V0)
       LTC68_backup.cntrPeriod      = LTC68_COUNTER_PERIOD_REG;
       LTC68_backup.saveSrTxIntMask = LTC68_TX_STATUS_MASK_REG;
       LTC68_backup.saveSrRxIntMask = LTC68_RX_STATUS_MASK_REG;
    #endif /* (CY_UDB_V0) */
}


/*******************************************************************************
* Function Name: LTC68_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores SPIM configuration.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  LTC68_backup - used when non-retention registers are restored.
*
* Side Effects:
*  If this API is called without first calling SaveConfig then in the following
*  registers will be default values from Customizer:
*  LTC68_STATUS_MASK_REG and LTC68_COUNTER_PERIOD_REG.
*
*******************************************************************************/
void LTC68_RestoreConfig(void) 
{
    /* Restore the data, saved by SaveConfig() function */
    #if(CY_UDB_V0)
        LTC68_COUNTER_PERIOD_REG = LTC68_backup.cntrPeriod;
        LTC68_TX_STATUS_MASK_REG = ((uint8) LTC68_backup.saveSrTxIntMask);
        LTC68_RX_STATUS_MASK_REG = ((uint8) LTC68_backup.saveSrRxIntMask);
    #endif /* (CY_UDB_V0) */
}


/*******************************************************************************
* Function Name: LTC68_Sleep
********************************************************************************
*
* Summary:
*  Prepare SPIM Component goes to sleep.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  LTC68_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void LTC68_Sleep(void) 
{
    /* Save components enable state */
    LTC68_backup.enableState = ((uint8) LTC68_IS_ENABLED);

    LTC68_Stop();
    LTC68_SaveConfig();
}


/*******************************************************************************
* Function Name: LTC68_Wakeup
********************************************************************************
*
* Summary:
*  Prepare SPIM Component to wake up.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  LTC68_backup - used when non-retention registers are restored.
*  LTC68_txBufferWrite - modified every function call - resets to
*  zero.
*  LTC68_txBufferRead - modified every function call - resets to
*  zero.
*  LTC68_rxBufferWrite - modified every function call - resets to
*  zero.
*  LTC68_rxBufferRead - modified every function call - resets to
*  zero.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void LTC68_Wakeup(void) 
{
    LTC68_RestoreConfig();

    #if(LTC68_RX_SOFTWARE_BUF_ENABLED)
        LTC68_rxBufferFull  = 0u;
        LTC68_rxBufferRead  = 0u;
        LTC68_rxBufferWrite = 0u;
    #endif /* (LTC68_RX_SOFTWARE_BUF_ENABLED) */

    #if(LTC68_TX_SOFTWARE_BUF_ENABLED)
        LTC68_txBufferFull  = 0u;
        LTC68_txBufferRead  = 0u;
        LTC68_txBufferWrite = 0u;
    #endif /* (LTC68_TX_SOFTWARE_BUF_ENABLED) */

    /* Clear any data from the RX and TX FIFO */
    LTC68_ClearFIFO();

    /* Restore components block enable state */
    if(0u != LTC68_backup.enableState)
    {
        LTC68_Enable();
    }
}


/* [] END OF FILE */
