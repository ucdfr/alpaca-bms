/*******************************************************************************
* File Name: int_WDT_set.c  
* Version 1.70
*
*  Description:
*   API for controlling the state of an interrupt.
*
*
*  Note:
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/


#include <cydevice_trm.h>
#include <CyLib.h>
#include <int_WDT_set.h>

#if !defined(int_WDT_set__REMOVED) /* Check for removal by optimization */

/*******************************************************************************
*  Place your includes, defines and code here 
********************************************************************************/
/* `#START int_WDT_set_intc` */

/* `#END` */

#ifndef CYINT_IRQ_BASE
#define CYINT_IRQ_BASE      16
#endif /* CYINT_IRQ_BASE */
#ifndef CYINT_VECT_TABLE
#define CYINT_VECT_TABLE    ((cyisraddress **) CYREG_NVIC_VECT_OFFSET)
#endif /* CYINT_VECT_TABLE */

/* Declared in startup, used to set unused interrupts to. */
CY_ISR_PROTO(IntDefaultHandler);


/*******************************************************************************
* Function Name: int_WDT_set_Start
********************************************************************************
*
* Summary:
*  Set up the interrupt and enable it.
*
* Parameters:  
*   None
*
* Return:
*   None
*
*******************************************************************************/
void int_WDT_set_Start(void)
{
    /* For all we know the interrupt is active. */
    int_WDT_set_Disable();

    /* Set the ISR to point to the int_WDT_set Interrupt. */
    int_WDT_set_SetVector(&int_WDT_set_Interrupt);

    /* Set the priority. */
    int_WDT_set_SetPriority((uint8)int_WDT_set_INTC_PRIOR_NUMBER);

    /* Enable it. */
    int_WDT_set_Enable();
}


/*******************************************************************************
* Function Name: int_WDT_set_StartEx
********************************************************************************
*
* Summary:
*  Set up the interrupt and enable it.
*
* Parameters:  
*   address: Address of the ISR to set in the interrupt vector table.
*
* Return:
*   None
*
*******************************************************************************/
void int_WDT_set_StartEx(cyisraddress address)
{
    /* For all we know the interrupt is active. */
    int_WDT_set_Disable();

    /* Set the ISR to point to the int_WDT_set Interrupt. */
    int_WDT_set_SetVector(address);

    /* Set the priority. */
    int_WDT_set_SetPriority((uint8)int_WDT_set_INTC_PRIOR_NUMBER);

    /* Enable it. */
    int_WDT_set_Enable();
}


/*******************************************************************************
* Function Name: int_WDT_set_Stop
********************************************************************************
*
* Summary:
*   Disables and removes the interrupt.
*
* Parameters:  
*
* Return:
*   None
*
*******************************************************************************/
void int_WDT_set_Stop(void)
{
    /* Disable this interrupt. */
    int_WDT_set_Disable();

    /* Set the ISR to point to the passive one. */
    int_WDT_set_SetVector(&IntDefaultHandler);
}


/*******************************************************************************
* Function Name: int_WDT_set_Interrupt
********************************************************************************
*
* Summary:
*   The default Interrupt Service Routine for int_WDT_set.
*
*   Add custom code between the coments to keep the next version of this file
*   from over writting your code.
*
* Parameters:  
*
* Return:
*   None
*
*******************************************************************************/
CY_ISR(int_WDT_set_Interrupt)
{
    /*  Place your Interrupt code here. */
    /* `#START int_WDT_set_Interrupt` */

    /* `#END` */
}


/*******************************************************************************
* Function Name: int_WDT_set_SetVector
********************************************************************************
*
* Summary:
*   Change the ISR vector for the Interrupt. Note calling int_WDT_set_Start
*   will override any effect this method would have had. To set the vector 
*   before the component has been started use int_WDT_set_StartEx instead.
*
* Parameters:
*   address: Address of the ISR to set in the interrupt vector table.
*
* Return:
*   None
*
*******************************************************************************/
void int_WDT_set_SetVector(cyisraddress address)
{
    cyisraddress * ramVectorTable;

    ramVectorTable = (cyisraddress *) *CYINT_VECT_TABLE;

    ramVectorTable[CYINT_IRQ_BASE + (uint32)int_WDT_set__INTC_NUMBER] = address;
}


/*******************************************************************************
* Function Name: int_WDT_set_GetVector
********************************************************************************
*
* Summary:
*   Gets the "address" of the current ISR vector for the Interrupt.
*
* Parameters:
*   None
*
* Return:
*   Address of the ISR in the interrupt vector table.
*
*******************************************************************************/
cyisraddress int_WDT_set_GetVector(void)
{
    cyisraddress * ramVectorTable;

    ramVectorTable = (cyisraddress *) *CYINT_VECT_TABLE;

    return ramVectorTable[CYINT_IRQ_BASE + (uint32)int_WDT_set__INTC_NUMBER];
}


/*******************************************************************************
* Function Name: int_WDT_set_SetPriority
********************************************************************************
*
* Summary:
*   Sets the Priority of the Interrupt. Note calling int_WDT_set_Start
*   or int_WDT_set_StartEx will override any effect this method 
*   would have had. This method should only be called after 
*   int_WDT_set_Start or int_WDT_set_StartEx has been called. To set 
*   the initial priority for the component use the cydwr file in the tool.
*
* Parameters:
*   priority: Priority of the interrupt. 0 - 7, 0 being the highest.
*
* Return:
*   None
*
*******************************************************************************/
void int_WDT_set_SetPriority(uint8 priority)
{
    *int_WDT_set_INTC_PRIOR = priority << 5;
}


/*******************************************************************************
* Function Name: int_WDT_set_GetPriority
********************************************************************************
*
* Summary:
*   Gets the Priority of the Interrupt.
*
* Parameters:
*   None
*
* Return:
*   Priority of the interrupt. 0 - 7, 0 being the highest.
*
*******************************************************************************/
uint8 int_WDT_set_GetPriority(void)
{
    uint8 priority;


    priority = *int_WDT_set_INTC_PRIOR >> 5;

    return priority;
}


/*******************************************************************************
* Function Name: int_WDT_set_Enable
********************************************************************************
*
* Summary:
*   Enables the interrupt.
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
void int_WDT_set_Enable(void)
{
    /* Enable the general interrupt. */
    *int_WDT_set_INTC_SET_EN = int_WDT_set__INTC_MASK;
}


/*******************************************************************************
* Function Name: int_WDT_set_GetState
********************************************************************************
*
* Summary:
*   Gets the state (enabled, disabled) of the Interrupt.
*
* Parameters:
*   None
*
* Return:
*   1 if enabled, 0 if disabled.
*
*******************************************************************************/
uint8 int_WDT_set_GetState(void)
{
    /* Get the state of the general interrupt. */
    return ((*int_WDT_set_INTC_SET_EN & (uint32)int_WDT_set__INTC_MASK) != 0u) ? 1u:0u;
}


/*******************************************************************************
* Function Name: int_WDT_set_Disable
********************************************************************************
*
* Summary:
*   Disables the Interrupt.
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
void int_WDT_set_Disable(void)
{
    /* Disable the general interrupt. */
    *int_WDT_set_INTC_CLR_EN = int_WDT_set__INTC_MASK;
}


/*******************************************************************************
* Function Name: int_WDT_set_SetPending
********************************************************************************
*
* Summary:
*   Causes the Interrupt to enter the pending state, a software method of
*   generating the interrupt.
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
void int_WDT_set_SetPending(void)
{
    *int_WDT_set_INTC_SET_PD = int_WDT_set__INTC_MASK;
}


/*******************************************************************************
* Function Name: int_WDT_set_ClearPending
********************************************************************************
*
* Summary:
*   Clears a pending interrupt.
*
* Parameters:
*   None
*
* Return:
*   None
*
*******************************************************************************/
void int_WDT_set_ClearPending(void)
{
    *int_WDT_set_INTC_CLR_PD = int_WDT_set__INTC_MASK;
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
