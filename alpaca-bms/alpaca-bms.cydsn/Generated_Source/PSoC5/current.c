/*******************************************************************************
* File Name: current.c  
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
#include "current.h"

/* APIs are not generated for P15[7:6] on PSoC 5 */
#if !(CY_PSOC5A &&\
	 current__PORT == 15 && ((current__MASK & 0xC0) != 0))


/*******************************************************************************
* Function Name: current_Write
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
void current_Write(uint8 value) 
{
    uint8 staticBits = (current_DR & (uint8)(~current_MASK));
    current_DR = staticBits | ((uint8)(value << current_SHIFT) & current_MASK);
}


/*******************************************************************************
* Function Name: current_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  current_DM_STRONG     Strong Drive 
*  current_DM_OD_HI      Open Drain, Drives High 
*  current_DM_OD_LO      Open Drain, Drives Low 
*  current_DM_RES_UP     Resistive Pull Up 
*  current_DM_RES_DWN    Resistive Pull Down 
*  current_DM_RES_UPDWN  Resistive Pull Up/Down 
*  current_DM_DIG_HIZ    High Impedance Digital 
*  current_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void current_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(current_0, mode);
}


/*******************************************************************************
* Function Name: current_Read
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
*  Macro current_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 current_Read(void) 
{
    return (current_PS & current_MASK) >> current_SHIFT;
}


/*******************************************************************************
* Function Name: current_ReadDataReg
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
uint8 current_ReadDataReg(void) 
{
    return (current_DR & current_MASK) >> current_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(current_INTSTAT) 

    /*******************************************************************************
    * Function Name: current_ClearInterrupt
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
    uint8 current_ClearInterrupt(void) 
    {
        return (current_INTSTAT & current_MASK) >> current_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 

#endif /* CY_PSOC5A... */

    
/* [] END OF FILE */
