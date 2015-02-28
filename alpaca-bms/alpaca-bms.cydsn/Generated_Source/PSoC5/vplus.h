/*******************************************************************************
* File Name: vplus.h  
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

#if !defined(CY_PINS_vplus_H) /* Pins vplus_H */
#define CY_PINS_vplus_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "vplus_aliases.h"

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5A)
    #error Component cy_pins_v2_10 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5A) */

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 vplus__PORT == 15 && ((vplus__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

void    vplus_Write(uint8 value) ;
void    vplus_SetDriveMode(uint8 mode) ;
uint8   vplus_ReadDataReg(void) ;
uint8   vplus_Read(void) ;
uint8   vplus_ClearInterrupt(void) ;


/***************************************
*           API Constants        
***************************************/

/* Drive Modes */
#define vplus_DM_ALG_HIZ         PIN_DM_ALG_HIZ
#define vplus_DM_DIG_HIZ         PIN_DM_DIG_HIZ
#define vplus_DM_RES_UP          PIN_DM_RES_UP
#define vplus_DM_RES_DWN         PIN_DM_RES_DWN
#define vplus_DM_OD_LO           PIN_DM_OD_LO
#define vplus_DM_OD_HI           PIN_DM_OD_HI
#define vplus_DM_STRONG          PIN_DM_STRONG
#define vplus_DM_RES_UPDWN       PIN_DM_RES_UPDWN

/* Digital Port Constants */
#define vplus_MASK               vplus__MASK
#define vplus_SHIFT              vplus__SHIFT
#define vplus_WIDTH              1u


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define vplus_PS                     (* (reg8 *) vplus__PS)
/* Data Register */
#define vplus_DR                     (* (reg8 *) vplus__DR)
/* Port Number */
#define vplus_PRT_NUM                (* (reg8 *) vplus__PRT) 
/* Connect to Analog Globals */                                                  
#define vplus_AG                     (* (reg8 *) vplus__AG)                       
/* Analog MUX bux enable */
#define vplus_AMUX                   (* (reg8 *) vplus__AMUX) 
/* Bidirectional Enable */                                                        
#define vplus_BIE                    (* (reg8 *) vplus__BIE)
/* Bit-mask for Aliased Register Access */
#define vplus_BIT_MASK               (* (reg8 *) vplus__BIT_MASK)
/* Bypass Enable */
#define vplus_BYP                    (* (reg8 *) vplus__BYP)
/* Port wide control signals */                                                   
#define vplus_CTL                    (* (reg8 *) vplus__CTL)
/* Drive Modes */
#define vplus_DM0                    (* (reg8 *) vplus__DM0) 
#define vplus_DM1                    (* (reg8 *) vplus__DM1)
#define vplus_DM2                    (* (reg8 *) vplus__DM2) 
/* Input Buffer Disable Override */
#define vplus_INP_DIS                (* (reg8 *) vplus__INP_DIS)
/* LCD Common or Segment Drive */
#define vplus_LCD_COM_SEG            (* (reg8 *) vplus__LCD_COM_SEG)
/* Enable Segment LCD */
#define vplus_LCD_EN                 (* (reg8 *) vplus__LCD_EN)
/* Slew Rate Control */
#define vplus_SLW                    (* (reg8 *) vplus__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define vplus_PRTDSI__CAPS_SEL       (* (reg8 *) vplus__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define vplus_PRTDSI__DBL_SYNC_IN    (* (reg8 *) vplus__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define vplus_PRTDSI__OE_SEL0        (* (reg8 *) vplus__PRTDSI__OE_SEL0) 
#define vplus_PRTDSI__OE_SEL1        (* (reg8 *) vplus__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define vplus_PRTDSI__OUT_SEL0       (* (reg8 *) vplus__PRTDSI__OUT_SEL0) 
#define vplus_PRTDSI__OUT_SEL1       (* (reg8 *) vplus__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define vplus_PRTDSI__SYNC_OUT       (* (reg8 *) vplus__PRTDSI__SYNC_OUT) 


#if defined(vplus__INTSTAT)  /* Interrupt Registers */

    #define vplus_INTSTAT                (* (reg8 *) vplus__INTSTAT)
    #define vplus_SNAP                   (* (reg8 *) vplus__SNAP)

#endif /* Interrupt Registers */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_vplus_H */


/* [] END OF FILE */
