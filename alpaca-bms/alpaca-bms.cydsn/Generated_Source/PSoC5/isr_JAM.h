/*******************************************************************************
* File Name: isr_JAM.h
* Version 1.70
*
*  Description:
*   Provides the function definitions for the Interrupt Controller.
*
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/
#if !defined(CY_ISR_isr_JAM_H)
#define CY_ISR_isr_JAM_H


#include <cytypes.h>
#include <cyfitter.h>

/* Interrupt Controller API. */
void isr_JAM_Start(void);
void isr_JAM_StartEx(cyisraddress address);
void isr_JAM_Stop(void);

CY_ISR_PROTO(isr_JAM_Interrupt);

void isr_JAM_SetVector(cyisraddress address);
cyisraddress isr_JAM_GetVector(void);

void isr_JAM_SetPriority(uint8 priority);
uint8 isr_JAM_GetPriority(void);

void isr_JAM_Enable(void);
uint8 isr_JAM_GetState(void);
void isr_JAM_Disable(void);

void isr_JAM_SetPending(void);
void isr_JAM_ClearPending(void);


/* Interrupt Controller Constants */

/* Address of the INTC.VECT[x] register that contains the Address of the isr_JAM ISR. */
#define isr_JAM_INTC_VECTOR            ((reg32 *) isr_JAM__INTC_VECT)

/* Address of the isr_JAM ISR priority. */
#define isr_JAM_INTC_PRIOR             ((reg8 *) isr_JAM__INTC_PRIOR_REG)

/* Priority of the isr_JAM interrupt. */
#define isr_JAM_INTC_PRIOR_NUMBER      isr_JAM__INTC_PRIOR_NUM

/* Address of the INTC.SET_EN[x] byte to bit enable isr_JAM interrupt. */
#define isr_JAM_INTC_SET_EN            ((reg32 *) isr_JAM__INTC_SET_EN_REG)

/* Address of the INTC.CLR_EN[x] register to bit clear the isr_JAM interrupt. */
#define isr_JAM_INTC_CLR_EN            ((reg32 *) isr_JAM__INTC_CLR_EN_REG)

/* Address of the INTC.SET_PD[x] register to set the isr_JAM interrupt state to pending. */
#define isr_JAM_INTC_SET_PD            ((reg32 *) isr_JAM__INTC_SET_PD_REG)

/* Address of the INTC.CLR_PD[x] register to clear the isr_JAM interrupt. */
#define isr_JAM_INTC_CLR_PD            ((reg32 *) isr_JAM__INTC_CLR_PD_REG)


#endif /* CY_ISR_isr_JAM_H */


/* [] END OF FILE */
