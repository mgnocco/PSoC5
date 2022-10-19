/*******************************************************************************
* File Name: RESET_FF_PM.c
* Version 1.80
*
* Description:
*  This file contains the setup, control, and status commands to support 
*  the component operation in the low power mode. 
*
* Note:
*
********************************************************************************
* Copyright 2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "RESET_FF.h"

/* Check for removal by optimization */
#if !defined(RESET_FF_Sync_ctrl_reg__REMOVED)

static RESET_FF_BACKUP_STRUCT  RESET_FF_backup = {0u};

    
/*******************************************************************************
* Function Name: RESET_FF_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the control register value.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void RESET_FF_SaveConfig(void) 
{
    RESET_FF_backup.controlState = RESET_FF_Control;
}


/*******************************************************************************
* Function Name: RESET_FF_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the control register value.
*
* Parameters:
*  None
*
* Return:
*  None
*
*
*******************************************************************************/
void RESET_FF_RestoreConfig(void) 
{
     RESET_FF_Control = RESET_FF_backup.controlState;
}


/*******************************************************************************
* Function Name: RESET_FF_Sleep
********************************************************************************
*
* Summary:
*  Prepares the component for entering the low power mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void RESET_FF_Sleep(void) 
{
    RESET_FF_SaveConfig();
}


/*******************************************************************************
* Function Name: RESET_FF_Wakeup
********************************************************************************
*
* Summary:
*  Restores the component after waking up from the low power mode.
*
* Parameters:
*  None
*
* Return:
*  None
*
*******************************************************************************/
void RESET_FF_Wakeup(void)  
{
    RESET_FF_RestoreConfig();
}

#endif /* End check for removal by optimization */


/* [] END OF FILE */
