/*******************************************************************************
* File Name: current.h  
* Version 2.10
*
* Description:
*  This file containts Control Register function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_current_H) /* Pins current_H */
#define CY_PINS_current_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "current_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v2_10 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 current__PORT == 15 && ((current__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    current_Write(uint8 value) ;
void    current_SetDriveMode(uint8 mode) ;
uint8   current_ReadDataReg(void) ;
uint8   current_Read(void) ;
uint8   current_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define current_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define current_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define current_DM_RES_UP          PIN_DM_RES_UP
#define current_DM_RES_DWN         PIN_DM_RES_DWN
#define current_DM_OD_LO           PIN_DM_OD_LO
#define current_DM_OD_HI           PIN_DM_OD_HI
#define current_DM_STRONG          PIN_DM_STRONG
#define current_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define current_MASK               current__MASK
#define current_SHIFT              current__SHIFT
#define current_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define current_PS                     (* (reg8 *) current__PS)
/* Data Register */
#define current_DR                     (* (reg8 *) current__DR)
/* Port Number */
#define current_PRT_NUM                (* (reg8 *) current__PRT) 
/* Connect to Analog Globals */                                                  
#define current_AG                     (* (reg8 *) current__AG)                       
/* Analog MUX bux enable */
#define current_AMUX                   (* (reg8 *) current__AMUX) 
/* Bidirectional Enable */                                                        
#define current_BIE                    (* (reg8 *) current__BIE)
/* Bit-mask for Aliased Register Access */
#define current_BIT_MASK               (* (reg8 *) current__BIT_MASK)
/* Bypass Enable */
#define current_BYP                    (* (reg8 *) current__BYP)
/* Port wide control signals */                                                   
#define current_CTL                    (* (reg8 *) current__CTL)
/* Drive Modes */
#define current_DM0                    (* (reg8 *) current__DM0) 
#define current_DM1                    (* (reg8 *) current__DM1)
#define current_DM2                    (* (reg8 *) current__DM2) 
/* Input Buffer Disable Override */
#define current_INP_DIS                (* (reg8 *) current__INP_DIS)
/* LCD Common or Segment Drive */
#define current_LCD_COM_SEG            (* (reg8 *) current__LCD_COM_SEG)
/* Enable Segment LCD */
#define current_LCD_EN                 (* (reg8 *) current__LCD_EN)
/* Slew Rate Control */
#define current_SLW                    (* (reg8 *) current__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define current_PRTDSI__CAPS_SEL       (* (reg8 *) current__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define current_PRTDSI__DBL_SYNC_IN    (* (reg8 *) current__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define current_PRTDSI__OE_SEL0        (* (reg8 *) current__PRTDSI__OE_SEL0) 
#define current_PRTDSI__OE_SEL1        (* (reg8 *) current__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define current_PRTDSI__OUT_SEL0       (* (reg8 *) current__PRTDSI__OUT_SEL0) 
#define current_PRTDSI__OUT_SEL1       (* (reg8 *) current__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define current_PRTDSI__SYNC_OUT       (* (reg8 *) current__PRTDSI__SYNC_OUT) 


#if defined(current__INTSTAT)  /* Interrupt Registers */

    #define current_INTSTAT                (* (reg8 *) current__INTSTAT)
    #define current_SNAP                   (* (reg8 *) current__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_current_H */


/* [] END OF FILE */
