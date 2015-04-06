/*******************************************************************************
* File Name: vplus.c  
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
#include "vplus.h"

/* APIs are not generated for P15[7:6] on PSoC 5 */
#if !(CY_PSOC5A &&\
	 vplus__PORT == 15 && ((vplus__MASK & 0xC0) != 0))


/*******************************************************************************
* Function Name: vplus_Write
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
void vplus_Write(uint8 value) 
{
    uint8 staticBits = (vplus_DR & (uint8)(~vplus_MASK));
    vplus_DR = staticBits | ((uint8)(value << vplus_SHIFT) & vplus_MASK);
}


/*******************************************************************************
* Function Name: vplus_SetDriveMode
********************************************************************************
*
* Summary:
*  Change the drive mode on the pins of the port.
* 
* Parameters:  
*  mode:  Change the pins to one of the following drive modes.
*
*  vplus_DM_STRONG     Strong Drive 
*  vplus_DM_OD_HI      Open Drain, Drives High 
*  vplus_DM_OD_LO      Open Drain, Drives Low 
*  vplus_DM_RES_UP     Resistive Pull Up 
*  vplus_DM_RES_DWN    Resistive Pull Down 
*  vplus_DM_RES_UPDWN  Resistive Pull Up/Down 
*  vplus_DM_DIG_HIZ    High Impedance Digital 
*  vplus_DM_ALG_HIZ    High Impedance Analog 
*
* Return: 
*  None
*
*******************************************************************************/
void vplus_SetDriveMode(uint8 mode) 
{
	CyPins_SetPinDriveMode(vplus_0, mode);
}


/*******************************************************************************
* Function Name: vplus_Read
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
*  Macro vplus_ReadPS calls this function. 
*  
*******************************************************************************/
uint8 vplus_Read(void) 
{
    return (vplus_PS & vplus_MASK) >> vplus_SHIFT;
}


/*******************************************************************************
* Function Name: vplus_ReadDataReg
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
uint8 vplus_ReadDataReg(void) 
{
    return (vplus_DR & vplus_MASK) >> vplus_SHIFT;
}


/* If Interrupts Are Enabled for this Pins component */ 
#if defined(vplus_INTSTAT) 

    /*******************************************************************************
    * Function Name: vplus_ClearInterrupt
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
    uint8 vplus_ClearInterrupt(void) 
    {
        return (vplus_INTSTAT & vplus_MASK) >> vplus_SHIFT;
    }

#endif /* If Interrupts Are Enabled for this Pins component */ 

#endif /* CY_PSOC5A... */

    
/* [] END OF FILE */
