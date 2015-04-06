/*******************************************************************************
* File Name: vminus.c  
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
#include "vminus.h"

/* APIs are not generated for P15[7:6] on PSoC 5 */
#if !(CY_PSOC5A &&\
	 vminus__PORT == 15 && ((vminus__MASK & 0xC0) != 0))


/*******************************************************************************
* Function Name: vminus_Write
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
void vminus_Write(uint8 value) 
{
    uint8 staticBits = (vminus_DR & (uint8)(~vminus_MASK));
    vminus_DR = staticBits | ((uint8)(value << vminus_SHIFT) & vminus_MASK);
}


/*******************************************************************************
* Function Name: vminus_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  vminus_DM_STRONG     Strong Drive 
*  vminus_DM_OD_HI      Open Drain, Drives High 
*  vminus_DM_OD_LO      Open Drain, Drives Low 
*  vminus_DM_RES_UP     Resistive Pull Up 
*  vminus_DM_RES_DWN    Resistive Pull Down 
*  vminus_DM_RES_UPDWN  Resistive Pull Up/Down 
*  vminus_DM_DIG_HIZ    High Impedance Digital 
*  vminus_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void vminus_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(vminus_0, mode);
}


/*******************************************************************************
* Function Name: vminus_Read
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
*  Macro vminus_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 vminus_Read(void) 
{
    return (vminus_PS & vminus_MASK) >> vminus_SHIFT;
}


/*******************************************************************************
* Function Name: vminus_ReadDataReg
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
uint8 vminus_ReadDataReg(void) 
{
    return (vminus_DR & vminus_MASK) >> vminus_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(vminus_INTSTAT) 

    /*******************************************************************************
    * Function Name: vminus_ClearInterrupt
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
    uint8 vminus_ClearInterrupt(void) 
    {
        return (vminus_INTSTAT & vminus_MASK) >> vminus_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 

#endif /* CY_PSOC5A... */

    
/* [] END OF FILE */
