/*******************************************************************************
* File Name: TOAST_2B.h  
* Version 2.20
*
* Description:
*  This file contains the Alias definitions for Per-Pin APIs in cypins.h. 
*  Information on using these APIs can be found in the System Reference Guide.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_TOAST_2B_ALIASES_H) /* Pins TOAST_2B_ALIASES_H */
#define CY_PINS_TOAST_2B_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"


/***************************************
*              Constants        
***************************************/
#define TOAST_2B_0			(TOAST_2B__0__PC)
#define TOAST_2B_0_INTR	((uint16)((uint16)0x0001u << TOAST_2B__0__SHIFT))

#define TOAST_2B_INTR_ALL	 ((uint16)(TOAST_2B_0_INTR))

#endif /* End Pins TOAST_2B_ALIASES_H */


/* [] END OF FILE */
