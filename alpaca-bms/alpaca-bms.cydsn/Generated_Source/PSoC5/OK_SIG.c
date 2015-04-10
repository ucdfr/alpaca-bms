/*******************************************************************************
* File Name: OK_SIG.c  
* Version 2.10
*
* Description:
*  This file contains API to enable firmware control of a Pins component.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "OK_SIG.h"

/* APIs are not generated for P15[7:6] on PSoC 5 */
#if !(CY_PSOC5A &&\
	 OK_SIG__PORT == 15 && ((OK_SIG__MASK & 0xC0) != 0))


/*******************************************************************************
* Function Name: OK_SIG_Write
********************************************************************************
*
* Summary:
*  Assign a new value to the digital port's data output register.  
*
* Parameters:  
*  prtValue:  The value to be assigned to the Digital Port. 
*
* Return: 
*  None
*  
*******************************************************************************/
void OK_SIG_Write(uint8 value) 
{
    uint8 staticBits = (OK_SIG_DR & (uint8)(~OK_SIG_MASK));
    OK_SIG_DR = staticBits | ((uint8)(value << OK_SIG_SHIFT) & OK_SIG_MASK);
}


/*******************************************************************************
* Function Name: OK_SIG_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  OK_SIG_DM_STRONG     Strong Drive 
*  OK_SIG_DM_OD_HI      Open Drain, Drives High 
*  OK_SIG_DM_OD_LO      Open Drain, Drives Low 
*  OK_SIG_DM_RES_UP     Resistive Pull Up 
*  OK_SIG_DM_RES_DWN    Resistive Pull Down 
*  OK_SIG_DM_RES_UPDWN  Resistive Pull Up/Down 
*  OK_SIG_DM_DIG_HIZ    High Impedance Digital 
*  OK_SIG_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void OK_SIG_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(OK_SIG_0, mode);
}


/*******************************************************************************
* Function Name: OK_SIG_Read
********************************************************************************
*
* Summary:
*  Read the current value on the pins of the Digital Port in right justified 
*  form.
*
* Parameters:  
*  None
*
* Return: 
*  Returns the current value of the Digital Port as a right justified number
*  
* Note:
*  Macro OK_SIG_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 OK_SIG_Read(void) 
{
    return (OK_SIG_PS & OK_SIG_MASK) >> OK_SIG_SHIFT;
}


/*******************************************************************************
* Function Name: OK_SIG_ReadDataReg
********************************************************************************
*
* Summary:
*  Read the current value assigned to a Digital Port's data output register
*
* Parameters:  
*  None 
*
* Return: 
*  Returns the current value assigned to the Digital Port's data output register
*  
*******************************************************************************/
uint8 OK_SIG_ReadDataReg(void) 
{
    return (OK_SIG_DR & OK_SIG_MASK) >> OK_SIG_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(OK_SIG_INTSTAT) 

    /*******************************************************************************
    * Function Name: OK_SIG_ClearInterrupt
    ********************************************************************************
    * Summary:
    *  Clears any active interrupts attached to port and returns the value of the 
    *  interrupt status register.
    *
    * Parameters:  
    *  None 
    *
    * Return: 
    *  Returns the value of the interrupt status register
    *  
    *******************************************************************************/
    uint8 OK_SIG_ClearInterrupt(void) 
    {
        return (OK_SIG_INTSTAT & OK_SIG_MASK) >> OK_SIG_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 

#endif /* CY_PSOC5A... */

    
/* [] END OF FILE */
