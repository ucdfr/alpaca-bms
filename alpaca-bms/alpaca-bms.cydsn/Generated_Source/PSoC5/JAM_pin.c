/*******************************************************************************
* File Name: JAM_pin.c  
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
#include "JAM_pin.h"

/* APIs are not generated for P15[7:6] on PSoC 5 */
#if !(CY_PSOC5A &&\
	 JAM_pin__PORT == 15 && ((JAM_pin__MASK & 0xC0) != 0))


/*******************************************************************************
* Function Name: JAM_pin_Write
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
void JAM_pin_Write(uint8 value) 
{
    uint8 staticBits = (JAM_pin_DR & (uint8)(~JAM_pin_MASK));
    JAM_pin_DR = staticBits | ((uint8)(value << JAM_pin_SHIFT) & JAM_pin_MASK);
}


/*******************************************************************************
* Function Name: JAM_pin_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  JAM_pin_DM_STRONG     Strong Drive 
*  JAM_pin_DM_OD_HI      Open Drain, Drives High 
*  JAM_pin_DM_OD_LO      Open Drain, Drives Low 
*  JAM_pin_DM_RES_UP     Resistive Pull Up 
*  JAM_pin_DM_RES_DWN    Resistive Pull Down 
*  JAM_pin_DM_RES_UPDWN  Resistive Pull Up/Down 
*  JAM_pin_DM_DIG_HIZ    High Impedance Digital 
*  JAM_pin_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void JAM_pin_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(JAM_pin_0, mode);
}


/*******************************************************************************
* Function Name: JAM_pin_Read
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
*  Macro JAM_pin_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 JAM_pin_Read(void) 
{
    return (JAM_pin_PS & JAM_pin_MASK) >> JAM_pin_SHIFT;
}


/*******************************************************************************
* Function Name: JAM_pin_ReadDataReg
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
uint8 JAM_pin_ReadDataReg(void) 
{
    return (JAM_pin_DR & JAM_pin_MASK) >> JAM_pin_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(JAM_pin_INTSTAT) 

    /*******************************************************************************
    * Function Name: JAM_pin_ClearInterrupt
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
    uint8 JAM_pin_ClearInterrupt(void) 
    {
        return (JAM_pin_INTSTAT & JAM_pin_MASK) >> JAM_pin_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 

#endif /* CY_PSOC5A... */

    
/* [] END OF FILE */
