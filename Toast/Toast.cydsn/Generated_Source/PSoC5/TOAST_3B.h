/*******************************************************************************
* File Name: TOAST_3B.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_TOAST_3B_H) /* Pins TOAST_3B_H */
#define CY_PINS_TOAST_3B_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "TOAST_3B_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 TOAST_3B__PORT == 15 && ((TOAST_3B__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    TOAST_3B_Write(uint8 value);
void    TOAST_3B_SetDriveMode(uint8 mode);
uint8   TOAST_3B_ReadDataReg(void);
uint8   TOAST_3B_Read(void);
void    TOAST_3B_SetInterruptMode(uint16 position, uint16 mode);
uint8   TOAST_3B_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the TOAST_3B_SetDriveMode() function.
     *  @{
     */
        #define TOAST_3B_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define TOAST_3B_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define TOAST_3B_DM_RES_UP          PIN_DM_RES_UP
        #define TOAST_3B_DM_RES_DWN         PIN_DM_RES_DWN
        #define TOAST_3B_DM_OD_LO           PIN_DM_OD_LO
        #define TOAST_3B_DM_OD_HI           PIN_DM_OD_HI
        #define TOAST_3B_DM_STRONG          PIN_DM_STRONG
        #define TOAST_3B_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define TOAST_3B_MASK               TOAST_3B__MASK
#define TOAST_3B_SHIFT              TOAST_3B__SHIFT
#define TOAST_3B_WIDTH              1u

/* Interrupt constants */
#if defined(TOAST_3B__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in TOAST_3B_SetInterruptMode() function.
     *  @{
     */
        #define TOAST_3B_INTR_NONE      (uint16)(0x0000u)
        #define TOAST_3B_INTR_RISING    (uint16)(0x0001u)
        #define TOAST_3B_INTR_FALLING   (uint16)(0x0002u)
        #define TOAST_3B_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define TOAST_3B_INTR_MASK      (0x01u) 
#endif /* (TOAST_3B__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define TOAST_3B_PS                     (* (reg8 *) TOAST_3B__PS)
/* Data Register */
#define TOAST_3B_DR                     (* (reg8 *) TOAST_3B__DR)
/* Port Number */
#define TOAST_3B_PRT_NUM                (* (reg8 *) TOAST_3B__PRT) 
/* Connect to Analog Globals */                                                  
#define TOAST_3B_AG                     (* (reg8 *) TOAST_3B__AG)                       
/* Analog MUX bux enable */
#define TOAST_3B_AMUX                   (* (reg8 *) TOAST_3B__AMUX) 
/* Bidirectional Enable */                                                        
#define TOAST_3B_BIE                    (* (reg8 *) TOAST_3B__BIE)
/* Bit-mask for Aliased Register Access */
#define TOAST_3B_BIT_MASK               (* (reg8 *) TOAST_3B__BIT_MASK)
/* Bypass Enable */
#define TOAST_3B_BYP                    (* (reg8 *) TOAST_3B__BYP)
/* Port wide control signals */                                                   
#define TOAST_3B_CTL                    (* (reg8 *) TOAST_3B__CTL)
/* Drive Modes */
#define TOAST_3B_DM0                    (* (reg8 *) TOAST_3B__DM0) 
#define TOAST_3B_DM1                    (* (reg8 *) TOAST_3B__DM1)
#define TOAST_3B_DM2                    (* (reg8 *) TOAST_3B__DM2) 
/* Input Buffer Disable Override */
#define TOAST_3B_INP_DIS                (* (reg8 *) TOAST_3B__INP_DIS)
/* LCD Common or Segment Drive */
#define TOAST_3B_LCD_COM_SEG            (* (reg8 *) TOAST_3B__LCD_COM_SEG)
/* Enable Segment LCD */
#define TOAST_3B_LCD_EN                 (* (reg8 *) TOAST_3B__LCD_EN)
/* Slew Rate Control */
#define TOAST_3B_SLW                    (* (reg8 *) TOAST_3B__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define TOAST_3B_PRTDSI__CAPS_SEL       (* (reg8 *) TOAST_3B__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define TOAST_3B_PRTDSI__DBL_SYNC_IN    (* (reg8 *) TOAST_3B__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define TOAST_3B_PRTDSI__OE_SEL0        (* (reg8 *) TOAST_3B__PRTDSI__OE_SEL0) 
#define TOAST_3B_PRTDSI__OE_SEL1        (* (reg8 *) TOAST_3B__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define TOAST_3B_PRTDSI__OUT_SEL0       (* (reg8 *) TOAST_3B__PRTDSI__OUT_SEL0) 
#define TOAST_3B_PRTDSI__OUT_SEL1       (* (reg8 *) TOAST_3B__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define TOAST_3B_PRTDSI__SYNC_OUT       (* (reg8 *) TOAST_3B__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(TOAST_3B__SIO_CFG)
    #define TOAST_3B_SIO_HYST_EN        (* (reg8 *) TOAST_3B__SIO_HYST_EN)
    #define TOAST_3B_SIO_REG_HIFREQ     (* (reg8 *) TOAST_3B__SIO_REG_HIFREQ)
    #define TOAST_3B_SIO_CFG            (* (reg8 *) TOAST_3B__SIO_CFG)
    #define TOAST_3B_SIO_DIFF           (* (reg8 *) TOAST_3B__SIO_DIFF)
#endif /* (TOAST_3B__SIO_CFG) */

/* Interrupt Registers */
#if defined(TOAST_3B__INTSTAT)
    #define TOAST_3B_INTSTAT            (* (reg8 *) TOAST_3B__INTSTAT)
    #define TOAST_3B_SNAP               (* (reg8 *) TOAST_3B__SNAP)
    
	#define TOAST_3B_0_INTTYPE_REG 		(* (reg8 *) TOAST_3B__0__INTTYPE)
#endif /* (TOAST_3B__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_TOAST_3B_H */


/* [] END OF FILE */
