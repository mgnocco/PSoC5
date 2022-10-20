/*******************************************************************************
* File Name: TOAST_4B.h  
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

#if !defined(CY_PINS_TOAST_4B_H) /* Pins TOAST_4B_H */
#define CY_PINS_TOAST_4B_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "TOAST_4B_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 TOAST_4B__PORT == 15 && ((TOAST_4B__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    TOAST_4B_Write(uint8 value);
void    TOAST_4B_SetDriveMode(uint8 mode);
uint8   TOAST_4B_ReadDataReg(void);
uint8   TOAST_4B_Read(void);
void    TOAST_4B_SetInterruptMode(uint16 position, uint16 mode);
uint8   TOAST_4B_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the TOAST_4B_SetDriveMode() function.
     *  @{
     */
        #define TOAST_4B_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define TOAST_4B_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define TOAST_4B_DM_RES_UP          PIN_DM_RES_UP
        #define TOAST_4B_DM_RES_DWN         PIN_DM_RES_DWN
        #define TOAST_4B_DM_OD_LO           PIN_DM_OD_LO
        #define TOAST_4B_DM_OD_HI           PIN_DM_OD_HI
        #define TOAST_4B_DM_STRONG          PIN_DM_STRONG
        #define TOAST_4B_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define TOAST_4B_MASK               TOAST_4B__MASK
#define TOAST_4B_SHIFT              TOAST_4B__SHIFT
#define TOAST_4B_WIDTH              1u

/* Interrupt constants */
#if defined(TOAST_4B__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in TOAST_4B_SetInterruptMode() function.
     *  @{
     */
        #define TOAST_4B_INTR_NONE      (uint16)(0x0000u)
        #define TOAST_4B_INTR_RISING    (uint16)(0x0001u)
        #define TOAST_4B_INTR_FALLING   (uint16)(0x0002u)
        #define TOAST_4B_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define TOAST_4B_INTR_MASK      (0x01u) 
#endif /* (TOAST_4B__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define TOAST_4B_PS                     (* (reg8 *) TOAST_4B__PS)
/* Data Register */
#define TOAST_4B_DR                     (* (reg8 *) TOAST_4B__DR)
/* Port Number */
#define TOAST_4B_PRT_NUM                (* (reg8 *) TOAST_4B__PRT) 
/* Connect to Analog Globals */                                                  
#define TOAST_4B_AG                     (* (reg8 *) TOAST_4B__AG)                       
/* Analog MUX bux enable */
#define TOAST_4B_AMUX                   (* (reg8 *) TOAST_4B__AMUX) 
/* Bidirectional Enable */                                                        
#define TOAST_4B_BIE                    (* (reg8 *) TOAST_4B__BIE)
/* Bit-mask for Aliased Register Access */
#define TOAST_4B_BIT_MASK               (* (reg8 *) TOAST_4B__BIT_MASK)
/* Bypass Enable */
#define TOAST_4B_BYP                    (* (reg8 *) TOAST_4B__BYP)
/* Port wide control signals */                                                   
#define TOAST_4B_CTL                    (* (reg8 *) TOAST_4B__CTL)
/* Drive Modes */
#define TOAST_4B_DM0                    (* (reg8 *) TOAST_4B__DM0) 
#define TOAST_4B_DM1                    (* (reg8 *) TOAST_4B__DM1)
#define TOAST_4B_DM2                    (* (reg8 *) TOAST_4B__DM2) 
/* Input Buffer Disable Override */
#define TOAST_4B_INP_DIS                (* (reg8 *) TOAST_4B__INP_DIS)
/* LCD Common or Segment Drive */
#define TOAST_4B_LCD_COM_SEG            (* (reg8 *) TOAST_4B__LCD_COM_SEG)
/* Enable Segment LCD */
#define TOAST_4B_LCD_EN                 (* (reg8 *) TOAST_4B__LCD_EN)
/* Slew Rate Control */
#define TOAST_4B_SLW                    (* (reg8 *) TOAST_4B__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define TOAST_4B_PRTDSI__CAPS_SEL       (* (reg8 *) TOAST_4B__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define TOAST_4B_PRTDSI__DBL_SYNC_IN    (* (reg8 *) TOAST_4B__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define TOAST_4B_PRTDSI__OE_SEL0        (* (reg8 *) TOAST_4B__PRTDSI__OE_SEL0) 
#define TOAST_4B_PRTDSI__OE_SEL1        (* (reg8 *) TOAST_4B__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define TOAST_4B_PRTDSI__OUT_SEL0       (* (reg8 *) TOAST_4B__PRTDSI__OUT_SEL0) 
#define TOAST_4B_PRTDSI__OUT_SEL1       (* (reg8 *) TOAST_4B__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define TOAST_4B_PRTDSI__SYNC_OUT       (* (reg8 *) TOAST_4B__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(TOAST_4B__SIO_CFG)
    #define TOAST_4B_SIO_HYST_EN        (* (reg8 *) TOAST_4B__SIO_HYST_EN)
    #define TOAST_4B_SIO_REG_HIFREQ     (* (reg8 *) TOAST_4B__SIO_REG_HIFREQ)
    #define TOAST_4B_SIO_CFG            (* (reg8 *) TOAST_4B__SIO_CFG)
    #define TOAST_4B_SIO_DIFF           (* (reg8 *) TOAST_4B__SIO_DIFF)
#endif /* (TOAST_4B__SIO_CFG) */

/* Interrupt Registers */
#if defined(TOAST_4B__INTSTAT)
    #define TOAST_4B_INTSTAT            (* (reg8 *) TOAST_4B__INTSTAT)
    #define TOAST_4B_SNAP               (* (reg8 *) TOAST_4B__SNAP)
    
	#define TOAST_4B_0_INTTYPE_REG 		(* (reg8 *) TOAST_4B__0__INTTYPE)
#endif /* (TOAST_4B__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_TOAST_4B_H */


/* [] END OF FILE */
