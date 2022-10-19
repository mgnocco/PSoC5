;
; File Name: cyfitteriar.inc
; 
; PSoC Creator  4.2
;
; Description:
; 
;
;-------------------------------------------------------------------------------
; Copyright (c) 2007-2018 Cypress Semiconductor.  All rights reserved.
; You may use this file only in accordance with the license, terms, conditions, 
; disclaimers, and limitations in the end user license agreement accompanying 
; the software package with which this file was provided.
;-------------------------------------------------------------------------------

#ifndef INCLUDED_CYFITTERIAR_INC
#define INCLUDED_CYFITTERIAR_INC
    INCLUDE cydeviceiar.inc
    INCLUDE cydeviceiar_trm.inc

/* USB_VDD */
USB_VDD__0__INTTYPE EQU CYREG_PICU1_INTTYPE4
USB_VDD__0__MASK EQU 0x10
USB_VDD__0__PC EQU CYREG_PRT1_PC4
USB_VDD__0__PORT EQU 1
USB_VDD__0__SHIFT EQU 4
USB_VDD__AG EQU CYREG_PRT1_AG
USB_VDD__AMUX EQU CYREG_PRT1_AMUX
USB_VDD__BIE EQU CYREG_PRT1_BIE
USB_VDD__BIT_MASK EQU CYREG_PRT1_BIT_MASK
USB_VDD__BYP EQU CYREG_PRT1_BYP
USB_VDD__CTL EQU CYREG_PRT1_CTL
USB_VDD__DM0 EQU CYREG_PRT1_DM0
USB_VDD__DM1 EQU CYREG_PRT1_DM1
USB_VDD__DM2 EQU CYREG_PRT1_DM2
USB_VDD__DR EQU CYREG_PRT1_DR
USB_VDD__INP_DIS EQU CYREG_PRT1_INP_DIS
USB_VDD__INTTYPE_BASE EQU CYDEV_PICU_INTTYPE_PICU1_BASE
USB_VDD__LCD_COM_SEG EQU CYREG_PRT1_LCD_COM_SEG
USB_VDD__LCD_EN EQU CYREG_PRT1_LCD_EN
USB_VDD__MASK EQU 0x10
USB_VDD__PORT EQU 1
USB_VDD__PRT EQU CYREG_PRT1_PRT
USB_VDD__PRTDSI__CAPS_SEL EQU CYREG_PRT1_CAPS_SEL
USB_VDD__PRTDSI__DBL_SYNC_IN EQU CYREG_PRT1_DBL_SYNC_IN
USB_VDD__PRTDSI__OE_SEL0 EQU CYREG_PRT1_OE_SEL0
USB_VDD__PRTDSI__OE_SEL1 EQU CYREG_PRT1_OE_SEL1
USB_VDD__PRTDSI__OUT_SEL0 EQU CYREG_PRT1_OUT_SEL0
USB_VDD__PRTDSI__OUT_SEL1 EQU CYREG_PRT1_OUT_SEL1
USB_VDD__PRTDSI__SYNC_OUT EQU CYREG_PRT1_SYNC_OUT
USB_VDD__PS EQU CYREG_PRT1_PS
USB_VDD__SHIFT EQU 4
USB_VDD__SLW EQU CYREG_PRT1_SLW

/* RS485_RX */
RS485_RX__0__INTTYPE EQU CYREG_PICU12_INTTYPE6
RS485_RX__0__MASK EQU 0x40
RS485_RX__0__PC EQU CYREG_PRT12_PC6
RS485_RX__0__PORT EQU 12
RS485_RX__0__SHIFT EQU 6
RS485_RX__AG EQU CYREG_PRT12_AG
RS485_RX__BIE EQU CYREG_PRT12_BIE
RS485_RX__BIT_MASK EQU CYREG_PRT12_BIT_MASK
RS485_RX__BYP EQU CYREG_PRT12_BYP
RS485_RX__DM0 EQU CYREG_PRT12_DM0
RS485_RX__DM1 EQU CYREG_PRT12_DM1
RS485_RX__DM2 EQU CYREG_PRT12_DM2
RS485_RX__DR EQU CYREG_PRT12_DR
RS485_RX__INP_DIS EQU CYREG_PRT12_INP_DIS
RS485_RX__INTTYPE_BASE EQU CYDEV_PICU_INTTYPE_PICU12_BASE
RS485_RX__MASK EQU 0x40
RS485_RX__PORT EQU 12
RS485_RX__PRT EQU CYREG_PRT12_PRT
RS485_RX__PRTDSI__DBL_SYNC_IN EQU CYREG_PRT12_DBL_SYNC_IN
RS485_RX__PRTDSI__OE_SEL0 EQU CYREG_PRT12_OE_SEL0
RS485_RX__PRTDSI__OE_SEL1 EQU CYREG_PRT12_OE_SEL1
RS485_RX__PRTDSI__OUT_SEL0 EQU CYREG_PRT12_OUT_SEL0
RS485_RX__PRTDSI__OUT_SEL1 EQU CYREG_PRT12_OUT_SEL1
RS485_RX__PRTDSI__SYNC_OUT EQU CYREG_PRT12_SYNC_OUT
RS485_RX__PS EQU CYREG_PRT12_PS
RS485_RX__SHIFT EQU 6
RS485_RX__SIO_CFG EQU CYREG_PRT12_SIO_CFG
RS485_RX__SIO_DIFF EQU CYREG_PRT12_SIO_DIFF
RS485_RX__SIO_HYST_EN EQU CYREG_PRT12_SIO_HYST_EN
RS485_RX__SIO_REG_HIFREQ EQU CYREG_PRT12_SIO_REG_HIFREQ
RS485_RX__SLW EQU CYREG_PRT12_SLW

/* RS485_TX */
RS485_TX__0__INTTYPE EQU CYREG_PICU12_INTTYPE7
RS485_TX__0__MASK EQU 0x80
RS485_TX__0__PC EQU CYREG_PRT12_PC7
RS485_TX__0__PORT EQU 12
RS485_TX__0__SHIFT EQU 7
RS485_TX__AG EQU CYREG_PRT12_AG
RS485_TX__BIE EQU CYREG_PRT12_BIE
RS485_TX__BIT_MASK EQU CYREG_PRT12_BIT_MASK
RS485_TX__BYP EQU CYREG_PRT12_BYP
RS485_TX__DM0 EQU CYREG_PRT12_DM0
RS485_TX__DM1 EQU CYREG_PRT12_DM1
RS485_TX__DM2 EQU CYREG_PRT12_DM2
RS485_TX__DR EQU CYREG_PRT12_DR
RS485_TX__INP_DIS EQU CYREG_PRT12_INP_DIS
RS485_TX__INTTYPE_BASE EQU CYDEV_PICU_INTTYPE_PICU12_BASE
RS485_TX__MASK EQU 0x80
RS485_TX__PORT EQU 12
RS485_TX__PRT EQU CYREG_PRT12_PRT
RS485_TX__PRTDSI__DBL_SYNC_IN EQU CYREG_PRT12_DBL_SYNC_IN
RS485_TX__PRTDSI__OE_SEL0 EQU CYREG_PRT12_OE_SEL0
RS485_TX__PRTDSI__OE_SEL1 EQU CYREG_PRT12_OE_SEL1
RS485_TX__PRTDSI__OUT_SEL0 EQU CYREG_PRT12_OUT_SEL0
RS485_TX__PRTDSI__OUT_SEL1 EQU CYREG_PRT12_OUT_SEL1
RS485_TX__PRTDSI__SYNC_OUT EQU CYREG_PRT12_SYNC_OUT
RS485_TX__PS EQU CYREG_PRT12_PS
RS485_TX__SHIFT EQU 7
RS485_TX__SIO_CFG EQU CYREG_PRT12_SIO_CFG
RS485_TX__SIO_DIFF EQU CYREG_PRT12_SIO_DIFF
RS485_TX__SIO_HYST_EN EQU CYREG_PRT12_SIO_HYST_EN
RS485_TX__SIO_REG_HIFREQ EQU CYREG_PRT12_SIO_REG_HIFREQ
RS485_TX__SLW EQU CYREG_PRT12_SLW

/* RS485_CTS */
RS485_CTS_Sync_ctrl_reg__0__MASK EQU 0x01
RS485_CTS_Sync_ctrl_reg__0__POS EQU 0
RS485_CTS_Sync_ctrl_reg__16BIT_CONTROL_AUX_CTL_REG EQU CYREG_B1_UDB10_11_ACTL
RS485_CTS_Sync_ctrl_reg__16BIT_CONTROL_CONTROL_REG EQU CYREG_B1_UDB10_11_CTL
RS485_CTS_Sync_ctrl_reg__16BIT_CONTROL_COUNT_REG EQU CYREG_B1_UDB10_11_CTL
RS485_CTS_Sync_ctrl_reg__16BIT_COUNT_CONTROL_REG EQU CYREG_B1_UDB10_11_CTL
RS485_CTS_Sync_ctrl_reg__16BIT_COUNT_COUNT_REG EQU CYREG_B1_UDB10_11_CTL
RS485_CTS_Sync_ctrl_reg__16BIT_MASK_MASK_REG EQU CYREG_B1_UDB10_11_MSK
RS485_CTS_Sync_ctrl_reg__16BIT_MASK_PERIOD_REG EQU CYREG_B1_UDB10_11_MSK
RS485_CTS_Sync_ctrl_reg__16BIT_PERIOD_MASK_REG EQU CYREG_B1_UDB10_11_MSK
RS485_CTS_Sync_ctrl_reg__16BIT_PERIOD_PERIOD_REG EQU CYREG_B1_UDB10_11_MSK
RS485_CTS_Sync_ctrl_reg__CONTROL_AUX_CTL_REG EQU CYREG_B1_UDB10_ACTL
RS485_CTS_Sync_ctrl_reg__CONTROL_REG EQU CYREG_B1_UDB10_CTL
RS485_CTS_Sync_ctrl_reg__CONTROL_ST_REG EQU CYREG_B1_UDB10_ST_CTL
RS485_CTS_Sync_ctrl_reg__COUNT_REG EQU CYREG_B1_UDB10_CTL
RS485_CTS_Sync_ctrl_reg__COUNT_ST_REG EQU CYREG_B1_UDB10_ST_CTL
RS485_CTS_Sync_ctrl_reg__MASK EQU 0x01
RS485_CTS_Sync_ctrl_reg__MASK_CTL_AUX_CTL_REG EQU CYREG_B1_UDB10_MSK_ACTL
RS485_CTS_Sync_ctrl_reg__PER_CTL_AUX_CTL_REG EQU CYREG_B1_UDB10_MSK_ACTL
RS485_CTS_Sync_ctrl_reg__PERIOD_REG EQU CYREG_B1_UDB10_MSK

/* RS_485_EN */
RS_485_EN__0__INTTYPE EQU CYREG_PICU12_INTTYPE4
RS_485_EN__0__MASK EQU 0x10
RS_485_EN__0__PC EQU CYREG_PRT12_PC4
RS_485_EN__0__PORT EQU 12
RS_485_EN__0__SHIFT EQU 4
RS_485_EN__AG EQU CYREG_PRT12_AG
RS_485_EN__BIE EQU CYREG_PRT12_BIE
RS_485_EN__BIT_MASK EQU CYREG_PRT12_BIT_MASK
RS_485_EN__BYP EQU CYREG_PRT12_BYP
RS_485_EN__DM0 EQU CYREG_PRT12_DM0
RS_485_EN__DM1 EQU CYREG_PRT12_DM1
RS_485_EN__DM2 EQU CYREG_PRT12_DM2
RS_485_EN__DR EQU CYREG_PRT12_DR
RS_485_EN__INP_DIS EQU CYREG_PRT12_INP_DIS
RS_485_EN__INTTYPE_BASE EQU CYDEV_PICU_INTTYPE_PICU12_BASE
RS_485_EN__MASK EQU 0x10
RS_485_EN__PORT EQU 12
RS_485_EN__PRT EQU CYREG_PRT12_PRT
RS_485_EN__PRTDSI__DBL_SYNC_IN EQU CYREG_PRT12_DBL_SYNC_IN
RS_485_EN__PRTDSI__OE_SEL0 EQU CYREG_PRT12_OE_SEL0
RS_485_EN__PRTDSI__OE_SEL1 EQU CYREG_PRT12_OE_SEL1
RS_485_EN__PRTDSI__OUT_SEL0 EQU CYREG_PRT12_OUT_SEL0
RS_485_EN__PRTDSI__OUT_SEL1 EQU CYREG_PRT12_OUT_SEL1
RS_485_EN__PRTDSI__SYNC_OUT EQU CYREG_PRT12_SYNC_OUT
RS_485_EN__PS EQU CYREG_PRT12_PS
RS_485_EN__SHIFT EQU 4
RS_485_EN__SIO_CFG EQU CYREG_PRT12_SIO_CFG
RS_485_EN__SIO_DIFF EQU CYREG_PRT12_SIO_DIFF
RS_485_EN__SIO_HYST_EN EQU CYREG_PRT12_SIO_HYST_EN
RS_485_EN__SIO_REG_HIFREQ EQU CYREG_PRT12_SIO_REG_HIFREQ
RS_485_EN__SLW EQU CYREG_PRT12_SLW

/* MOTOR_PORT */
MOTOR_PORT__0__INTTYPE EQU CYREG_PICU3_INTTYPE0
MOTOR_PORT__0__MASK EQU 0x01
MOTOR_PORT__0__PC EQU CYREG_PRT3_PC0
MOTOR_PORT__0__PORT EQU 3
MOTOR_PORT__0__SHIFT EQU 0
MOTOR_PORT__AG EQU CYREG_PRT3_AG
MOTOR_PORT__AMUX EQU CYREG_PRT3_AMUX
MOTOR_PORT__BIE EQU CYREG_PRT3_BIE
MOTOR_PORT__BIT_MASK EQU CYREG_PRT3_BIT_MASK
MOTOR_PORT__BYP EQU CYREG_PRT3_BYP
MOTOR_PORT__CTL EQU CYREG_PRT3_CTL
MOTOR_PORT__DM0 EQU CYREG_PRT3_DM0
MOTOR_PORT__DM1 EQU CYREG_PRT3_DM1
MOTOR_PORT__DM2 EQU CYREG_PRT3_DM2
MOTOR_PORT__DR EQU CYREG_PRT3_DR
MOTOR_PORT__INP_DIS EQU CYREG_PRT3_INP_DIS
MOTOR_PORT__INTTYPE_BASE EQU CYDEV_PICU_INTTYPE_PICU3_BASE
MOTOR_PORT__LCD_COM_SEG EQU CYREG_PRT3_LCD_COM_SEG
MOTOR_PORT__LCD_EN EQU CYREG_PRT3_LCD_EN
MOTOR_PORT__MASK EQU 0x01
MOTOR_PORT__PORT EQU 3
MOTOR_PORT__PRT EQU CYREG_PRT3_PRT
MOTOR_PORT__PRTDSI__CAPS_SEL EQU CYREG_PRT3_CAPS_SEL
MOTOR_PORT__PRTDSI__DBL_SYNC_IN EQU CYREG_PRT3_DBL_SYNC_IN
MOTOR_PORT__PRTDSI__OE_SEL0 EQU CYREG_PRT3_OE_SEL0
MOTOR_PORT__PRTDSI__OE_SEL1 EQU CYREG_PRT3_OE_SEL1
MOTOR_PORT__PRTDSI__OUT_SEL0 EQU CYREG_PRT3_OUT_SEL0
MOTOR_PORT__PRTDSI__OUT_SEL1 EQU CYREG_PRT3_OUT_SEL1
MOTOR_PORT__PRTDSI__SYNC_OUT EQU CYREG_PRT3_SYNC_OUT
MOTOR_PORT__PS EQU CYREG_PRT3_PS
MOTOR_PORT__SHIFT EQU 0
MOTOR_PORT__SLW EQU CYREG_PRT3_SLW

/* UART_RS485 */
UART_RS485_BUART_sRX_RxBitCounter__16BIT_CONTROL_AUX_CTL_REG EQU CYREG_B1_UDB09_10_ACTL
UART_RS485_BUART_sRX_RxBitCounter__16BIT_CONTROL_CONTROL_REG EQU CYREG_B1_UDB09_10_CTL
UART_RS485_BUART_sRX_RxBitCounter__16BIT_CONTROL_COUNT_REG EQU CYREG_B1_UDB09_10_CTL
UART_RS485_BUART_sRX_RxBitCounter__16BIT_COUNT_CONTROL_REG EQU CYREG_B1_UDB09_10_CTL
UART_RS485_BUART_sRX_RxBitCounter__16BIT_COUNT_COUNT_REG EQU CYREG_B1_UDB09_10_CTL
UART_RS485_BUART_sRX_RxBitCounter__16BIT_MASK_MASK_REG EQU CYREG_B1_UDB09_10_MSK
UART_RS485_BUART_sRX_RxBitCounter__16BIT_MASK_PERIOD_REG EQU CYREG_B1_UDB09_10_MSK
UART_RS485_BUART_sRX_RxBitCounter__16BIT_PERIOD_MASK_REG EQU CYREG_B1_UDB09_10_MSK
UART_RS485_BUART_sRX_RxBitCounter__16BIT_PERIOD_PERIOD_REG EQU CYREG_B1_UDB09_10_MSK
UART_RS485_BUART_sRX_RxBitCounter__CONTROL_AUX_CTL_REG EQU CYREG_B1_UDB09_ACTL
UART_RS485_BUART_sRX_RxBitCounter__CONTROL_REG EQU CYREG_B1_UDB09_CTL
UART_RS485_BUART_sRX_RxBitCounter__CONTROL_ST_REG EQU CYREG_B1_UDB09_ST_CTL
UART_RS485_BUART_sRX_RxBitCounter__COUNT_REG EQU CYREG_B1_UDB09_CTL
UART_RS485_BUART_sRX_RxBitCounter__COUNT_ST_REG EQU CYREG_B1_UDB09_ST_CTL
UART_RS485_BUART_sRX_RxBitCounter__MASK_CTL_AUX_CTL_REG EQU CYREG_B1_UDB09_MSK_ACTL
UART_RS485_BUART_sRX_RxBitCounter__PER_CTL_AUX_CTL_REG EQU CYREG_B1_UDB09_MSK_ACTL
UART_RS485_BUART_sRX_RxBitCounter__PERIOD_REG EQU CYREG_B1_UDB09_MSK
UART_RS485_BUART_sRX_RxBitCounter_ST__16BIT_STATUS_AUX_CTL_REG EQU CYREG_B1_UDB09_10_ACTL
UART_RS485_BUART_sRX_RxBitCounter_ST__16BIT_STATUS_REG EQU CYREG_B1_UDB09_10_ST
UART_RS485_BUART_sRX_RxBitCounter_ST__MASK_REG EQU CYREG_B1_UDB09_MSK
UART_RS485_BUART_sRX_RxBitCounter_ST__MASK_ST_AUX_CTL_REG EQU CYREG_B1_UDB09_MSK_ACTL
UART_RS485_BUART_sRX_RxBitCounter_ST__PER_ST_AUX_CTL_REG EQU CYREG_B1_UDB09_MSK_ACTL
UART_RS485_BUART_sRX_RxBitCounter_ST__STATUS_AUX_CTL_REG EQU CYREG_B1_UDB09_ACTL
UART_RS485_BUART_sRX_RxBitCounter_ST__STATUS_CNT_REG EQU CYREG_B1_UDB09_ST_CTL
UART_RS485_BUART_sRX_RxBitCounter_ST__STATUS_CONTROL_REG EQU CYREG_B1_UDB09_ST_CTL
UART_RS485_BUART_sRX_RxBitCounter_ST__STATUS_REG EQU CYREG_B1_UDB09_ST
UART_RS485_BUART_sRX_RxShifter_u0__16BIT_A0_REG EQU CYREG_B1_UDB10_11_A0
UART_RS485_BUART_sRX_RxShifter_u0__16BIT_A1_REG EQU CYREG_B1_UDB10_11_A1
UART_RS485_BUART_sRX_RxShifter_u0__16BIT_D0_REG EQU CYREG_B1_UDB10_11_D0
UART_RS485_BUART_sRX_RxShifter_u0__16BIT_D1_REG EQU CYREG_B1_UDB10_11_D1
UART_RS485_BUART_sRX_RxShifter_u0__16BIT_DP_AUX_CTL_REG EQU CYREG_B1_UDB10_11_ACTL
UART_RS485_BUART_sRX_RxShifter_u0__16BIT_F0_REG EQU CYREG_B1_UDB10_11_F0
UART_RS485_BUART_sRX_RxShifter_u0__16BIT_F1_REG EQU CYREG_B1_UDB10_11_F1
UART_RS485_BUART_sRX_RxShifter_u0__A0_A1_REG EQU CYREG_B1_UDB10_A0_A1
UART_RS485_BUART_sRX_RxShifter_u0__A0_REG EQU CYREG_B1_UDB10_A0
UART_RS485_BUART_sRX_RxShifter_u0__A1_REG EQU CYREG_B1_UDB10_A1
UART_RS485_BUART_sRX_RxShifter_u0__D0_D1_REG EQU CYREG_B1_UDB10_D0_D1
UART_RS485_BUART_sRX_RxShifter_u0__D0_REG EQU CYREG_B1_UDB10_D0
UART_RS485_BUART_sRX_RxShifter_u0__D1_REG EQU CYREG_B1_UDB10_D1
UART_RS485_BUART_sRX_RxShifter_u0__DP_AUX_CTL_REG EQU CYREG_B1_UDB10_ACTL
UART_RS485_BUART_sRX_RxShifter_u0__F0_F1_REG EQU CYREG_B1_UDB10_F0_F1
UART_RS485_BUART_sRX_RxShifter_u0__F0_REG EQU CYREG_B1_UDB10_F0
UART_RS485_BUART_sRX_RxShifter_u0__F1_REG EQU CYREG_B1_UDB10_F1
UART_RS485_BUART_sRX_RxShifter_u0__MSK_DP_AUX_CTL_REG EQU CYREG_B1_UDB10_MSK_ACTL
UART_RS485_BUART_sRX_RxShifter_u0__PER_DP_AUX_CTL_REG EQU CYREG_B1_UDB10_MSK_ACTL
UART_RS485_BUART_sRX_RxSts__1__MASK EQU 0x02
UART_RS485_BUART_sRX_RxSts__1__POS EQU 1
UART_RS485_BUART_sRX_RxSts__16BIT_STATUS_AUX_CTL_REG EQU CYREG_B0_UDB09_10_ACTL
UART_RS485_BUART_sRX_RxSts__16BIT_STATUS_REG EQU CYREG_B0_UDB09_10_ST
UART_RS485_BUART_sRX_RxSts__3__MASK EQU 0x08
UART_RS485_BUART_sRX_RxSts__3__POS EQU 3
UART_RS485_BUART_sRX_RxSts__4__MASK EQU 0x10
UART_RS485_BUART_sRX_RxSts__4__POS EQU 4
UART_RS485_BUART_sRX_RxSts__5__MASK EQU 0x20
UART_RS485_BUART_sRX_RxSts__5__POS EQU 5
UART_RS485_BUART_sRX_RxSts__MASK EQU 0x3A
UART_RS485_BUART_sRX_RxSts__MASK_REG EQU CYREG_B0_UDB09_MSK
UART_RS485_BUART_sRX_RxSts__STATUS_AUX_CTL_REG EQU CYREG_B0_UDB09_ACTL
UART_RS485_BUART_sRX_RxSts__STATUS_REG EQU CYREG_B0_UDB09_ST
UART_RS485_BUART_sTX_sCLOCK_TxBitClkGen__16BIT_A0_REG EQU CYREG_B0_UDB08_09_A0
UART_RS485_BUART_sTX_sCLOCK_TxBitClkGen__16BIT_A1_REG EQU CYREG_B0_UDB08_09_A1
UART_RS485_BUART_sTX_sCLOCK_TxBitClkGen__16BIT_D0_REG EQU CYREG_B0_UDB08_09_D0
UART_RS485_BUART_sTX_sCLOCK_TxBitClkGen__16BIT_D1_REG EQU CYREG_B0_UDB08_09_D1
UART_RS485_BUART_sTX_sCLOCK_TxBitClkGen__16BIT_DP_AUX_CTL_REG EQU CYREG_B0_UDB08_09_ACTL
UART_RS485_BUART_sTX_sCLOCK_TxBitClkGen__16BIT_F0_REG EQU CYREG_B0_UDB08_09_F0
UART_RS485_BUART_sTX_sCLOCK_TxBitClkGen__16BIT_F1_REG EQU CYREG_B0_UDB08_09_F1
UART_RS485_BUART_sTX_sCLOCK_TxBitClkGen__A0_A1_REG EQU CYREG_B0_UDB08_A0_A1
UART_RS485_BUART_sTX_sCLOCK_TxBitClkGen__A0_REG EQU CYREG_B0_UDB08_A0
UART_RS485_BUART_sTX_sCLOCK_TxBitClkGen__A1_REG EQU CYREG_B0_UDB08_A1
UART_RS485_BUART_sTX_sCLOCK_TxBitClkGen__D0_D1_REG EQU CYREG_B0_UDB08_D0_D1
UART_RS485_BUART_sTX_sCLOCK_TxBitClkGen__D0_REG EQU CYREG_B0_UDB08_D0
UART_RS485_BUART_sTX_sCLOCK_TxBitClkGen__D1_REG EQU CYREG_B0_UDB08_D1
UART_RS485_BUART_sTX_sCLOCK_TxBitClkGen__DP_AUX_CTL_REG EQU CYREG_B0_UDB08_ACTL
UART_RS485_BUART_sTX_sCLOCK_TxBitClkGen__F0_F1_REG EQU CYREG_B0_UDB08_F0_F1
UART_RS485_BUART_sTX_sCLOCK_TxBitClkGen__F0_REG EQU CYREG_B0_UDB08_F0
UART_RS485_BUART_sTX_sCLOCK_TxBitClkGen__F1_REG EQU CYREG_B0_UDB08_F1
UART_RS485_BUART_sTX_TxShifter_u0__16BIT_A0_REG EQU CYREG_B1_UDB08_09_A0
UART_RS485_BUART_sTX_TxShifter_u0__16BIT_A1_REG EQU CYREG_B1_UDB08_09_A1
UART_RS485_BUART_sTX_TxShifter_u0__16BIT_D0_REG EQU CYREG_B1_UDB08_09_D0
UART_RS485_BUART_sTX_TxShifter_u0__16BIT_D1_REG EQU CYREG_B1_UDB08_09_D1
UART_RS485_BUART_sTX_TxShifter_u0__16BIT_DP_AUX_CTL_REG EQU CYREG_B1_UDB08_09_ACTL
UART_RS485_BUART_sTX_TxShifter_u0__16BIT_F0_REG EQU CYREG_B1_UDB08_09_F0
UART_RS485_BUART_sTX_TxShifter_u0__16BIT_F1_REG EQU CYREG_B1_UDB08_09_F1
UART_RS485_BUART_sTX_TxShifter_u0__A0_A1_REG EQU CYREG_B1_UDB08_A0_A1
UART_RS485_BUART_sTX_TxShifter_u0__A0_REG EQU CYREG_B1_UDB08_A0
UART_RS485_BUART_sTX_TxShifter_u0__A1_REG EQU CYREG_B1_UDB08_A1
UART_RS485_BUART_sTX_TxShifter_u0__D0_D1_REG EQU CYREG_B1_UDB08_D0_D1
UART_RS485_BUART_sTX_TxShifter_u0__D0_REG EQU CYREG_B1_UDB08_D0
UART_RS485_BUART_sTX_TxShifter_u0__D1_REG EQU CYREG_B1_UDB08_D1
UART_RS485_BUART_sTX_TxShifter_u0__DP_AUX_CTL_REG EQU CYREG_B1_UDB08_ACTL
UART_RS485_BUART_sTX_TxShifter_u0__F0_F1_REG EQU CYREG_B1_UDB08_F0_F1
UART_RS485_BUART_sTX_TxShifter_u0__F0_REG EQU CYREG_B1_UDB08_F0
UART_RS485_BUART_sTX_TxShifter_u0__F1_REG EQU CYREG_B1_UDB08_F1
UART_RS485_BUART_sTX_TxSts__0__MASK EQU 0x01
UART_RS485_BUART_sTX_TxSts__0__POS EQU 0
UART_RS485_BUART_sTX_TxSts__1__MASK EQU 0x02
UART_RS485_BUART_sTX_TxSts__1__POS EQU 1
UART_RS485_BUART_sTX_TxSts__2__MASK EQU 0x04
UART_RS485_BUART_sTX_TxSts__2__POS EQU 2
UART_RS485_BUART_sTX_TxSts__3__MASK EQU 0x08
UART_RS485_BUART_sTX_TxSts__3__POS EQU 3
UART_RS485_BUART_sTX_TxSts__MASK EQU 0x0F
UART_RS485_BUART_sTX_TxSts__MASK_REG EQU CYREG_B1_UDB11_MSK
UART_RS485_BUART_sTX_TxSts__STATUS_AUX_CTL_REG EQU CYREG_B1_UDB11_ACTL
UART_RS485_BUART_sTX_TxSts__STATUS_REG EQU CYREG_B1_UDB11_ST
UART_RS485_IntClock__CFG0 EQU CYREG_CLKDIST_DCFG0_CFG0
UART_RS485_IntClock__CFG1 EQU CYREG_CLKDIST_DCFG0_CFG1
UART_RS485_IntClock__CFG2 EQU CYREG_CLKDIST_DCFG0_CFG2
UART_RS485_IntClock__CFG2_SRC_SEL_MASK EQU 0x07
UART_RS485_IntClock__INDEX EQU 0x00
UART_RS485_IntClock__PM_ACT_CFG EQU CYREG_PM_ACT_CFG2
UART_RS485_IntClock__PM_ACT_MSK EQU 0x01
UART_RS485_IntClock__PM_STBY_CFG EQU CYREG_PM_STBY_CFG2
UART_RS485_IntClock__PM_STBY_MSK EQU 0x01
UART_RS485_RXInternalInterrupt__INTC_CLR_EN_REG EQU CYREG_NVIC_CLRENA0
UART_RS485_RXInternalInterrupt__INTC_CLR_PD_REG EQU CYREG_NVIC_CLRPEND0
UART_RS485_RXInternalInterrupt__INTC_MASK EQU 0x01
UART_RS485_RXInternalInterrupt__INTC_NUMBER EQU 0
UART_RS485_RXInternalInterrupt__INTC_PRIOR_NUM EQU 7
UART_RS485_RXInternalInterrupt__INTC_PRIOR_REG EQU CYREG_NVIC_PRI_0
UART_RS485_RXInternalInterrupt__INTC_SET_EN_REG EQU CYREG_NVIC_SETENA0
UART_RS485_RXInternalInterrupt__INTC_SET_PD_REG EQU CYREG_NVIC_SETPEND0
UART_RS485_TXInternalInterrupt__INTC_CLR_EN_REG EQU CYREG_NVIC_CLRENA0
UART_RS485_TXInternalInterrupt__INTC_CLR_PD_REG EQU CYREG_NVIC_CLRPEND0
UART_RS485_TXInternalInterrupt__INTC_MASK EQU 0x02
UART_RS485_TXInternalInterrupt__INTC_NUMBER EQU 1
UART_RS485_TXInternalInterrupt__INTC_PRIOR_NUM EQU 7
UART_RS485_TXInternalInterrupt__INTC_PRIOR_REG EQU CYREG_NVIC_PRI_1
UART_RS485_TXInternalInterrupt__INTC_SET_EN_REG EQU CYREG_NVIC_SETENA0
UART_RS485_TXInternalInterrupt__INTC_SET_PD_REG EQU CYREG_NVIC_SETPEND0

/* FTDI_ENABLE */
FTDI_ENABLE__0__INTTYPE EQU CYREG_PICU1_INTTYPE2
FTDI_ENABLE__0__MASK EQU 0x04
FTDI_ENABLE__0__PC EQU CYREG_PRT1_PC2
FTDI_ENABLE__0__PORT EQU 1
FTDI_ENABLE__0__SHIFT EQU 2
FTDI_ENABLE__AG EQU CYREG_PRT1_AG
FTDI_ENABLE__AMUX EQU CYREG_PRT1_AMUX
FTDI_ENABLE__BIE EQU CYREG_PRT1_BIE
FTDI_ENABLE__BIT_MASK EQU CYREG_PRT1_BIT_MASK
FTDI_ENABLE__BYP EQU CYREG_PRT1_BYP
FTDI_ENABLE__CTL EQU CYREG_PRT1_CTL
FTDI_ENABLE__DM0 EQU CYREG_PRT1_DM0
FTDI_ENABLE__DM1 EQU CYREG_PRT1_DM1
FTDI_ENABLE__DM2 EQU CYREG_PRT1_DM2
FTDI_ENABLE__DR EQU CYREG_PRT1_DR
FTDI_ENABLE__INP_DIS EQU CYREG_PRT1_INP_DIS
FTDI_ENABLE__INTTYPE_BASE EQU CYDEV_PICU_INTTYPE_PICU1_BASE
FTDI_ENABLE__LCD_COM_SEG EQU CYREG_PRT1_LCD_COM_SEG
FTDI_ENABLE__LCD_EN EQU CYREG_PRT1_LCD_EN
FTDI_ENABLE__MASK EQU 0x04
FTDI_ENABLE__PORT EQU 1
FTDI_ENABLE__PRT EQU CYREG_PRT1_PRT
FTDI_ENABLE__PRTDSI__CAPS_SEL EQU CYREG_PRT1_CAPS_SEL
FTDI_ENABLE__PRTDSI__DBL_SYNC_IN EQU CYREG_PRT1_DBL_SYNC_IN
FTDI_ENABLE__PRTDSI__OE_SEL0 EQU CYREG_PRT1_OE_SEL0
FTDI_ENABLE__PRTDSI__OE_SEL1 EQU CYREG_PRT1_OE_SEL1
FTDI_ENABLE__PRTDSI__OUT_SEL0 EQU CYREG_PRT1_OUT_SEL0
FTDI_ENABLE__PRTDSI__OUT_SEL1 EQU CYREG_PRT1_OUT_SEL1
FTDI_ENABLE__PRTDSI__SYNC_OUT EQU CYREG_PRT1_SYNC_OUT
FTDI_ENABLE__PS EQU CYREG_PRT1_PS
FTDI_ENABLE__SHIFT EQU 2
FTDI_ENABLE__SLW EQU CYREG_PRT1_SLW

/* Miscellaneous */
BCLK__BUS_CLK__HZ EQU 24000000
BCLK__BUS_CLK__KHZ EQU 24000
BCLK__BUS_CLK__MHZ EQU 24
CYDEV_BOOTLOADER_APPLICATIONS EQU 1
CYDEV_BOOTLOADER_CHECKSUM_BASIC EQU 0
CYDEV_BOOTLOADER_CHECKSUM_CRC EQU 1
CYDEV_BOOTLOADER_IO_COMP_CUSTOM_IO EQU 0
CyBtldr_Custom_Interface EQU CYDEV_BOOTLOADER_IO_COMP_CUSTOM_IO
CYDEV_BOOTLOADER_IO_COMP_LAUNCHER_ONLY EQU 1
CyBtldr_LAUNCHER_ONLY EQU CYDEV_BOOTLOADER_IO_COMP_LAUNCHER_ONLY
CYDEV_BOOTLOADER_IO_COMP_UART_RS485 EQU 2
CyBtldr_UART_RS485 EQU CYDEV_BOOTLOADER_IO_COMP_UART_RS485
CYDEV_BOOTLOADER_IO_COMP EQU CYDEV_BOOTLOADER_IO_COMP_UART_RS485
CYDEV_CHIP_DIE_LEOPARD EQU 1
CYDEV_CHIP_DIE_PSOC4A EQU 18
CYDEV_CHIP_DIE_PSOC5LP EQU 2
CYDEV_CHIP_DIE_PSOC5TM EQU 3
CYDEV_CHIP_DIE_TMA4 EQU 4
CYDEV_CHIP_DIE_UNKNOWN EQU 0
CYDEV_CHIP_FAMILY_FM0P EQU 5
CYDEV_CHIP_FAMILY_FM3 EQU 6
CYDEV_CHIP_FAMILY_FM4 EQU 7
CYDEV_CHIP_FAMILY_PSOC3 EQU 1
CYDEV_CHIP_FAMILY_PSOC4 EQU 2
CYDEV_CHIP_FAMILY_PSOC5 EQU 3
CYDEV_CHIP_FAMILY_PSOC6 EQU 4
CYDEV_CHIP_FAMILY_UNKNOWN EQU 0
CYDEV_CHIP_FAMILY_USED EQU CYDEV_CHIP_FAMILY_PSOC5
CYDEV_CHIP_JTAG_ID EQU 0x2E161069
CYDEV_CHIP_MEMBER_3A EQU 1
CYDEV_CHIP_MEMBER_4A EQU 18
CYDEV_CHIP_MEMBER_4D EQU 13
CYDEV_CHIP_MEMBER_4E EQU 6
CYDEV_CHIP_MEMBER_4F EQU 19
CYDEV_CHIP_MEMBER_4G EQU 4
CYDEV_CHIP_MEMBER_4H EQU 17
CYDEV_CHIP_MEMBER_4I EQU 23
CYDEV_CHIP_MEMBER_4J EQU 14
CYDEV_CHIP_MEMBER_4K EQU 15
CYDEV_CHIP_MEMBER_4L EQU 22
CYDEV_CHIP_MEMBER_4M EQU 21
CYDEV_CHIP_MEMBER_4N EQU 10
CYDEV_CHIP_MEMBER_4O EQU 7
CYDEV_CHIP_MEMBER_4P EQU 20
CYDEV_CHIP_MEMBER_4Q EQU 12
CYDEV_CHIP_MEMBER_4R EQU 8
CYDEV_CHIP_MEMBER_4S EQU 11
CYDEV_CHIP_MEMBER_4T EQU 9
CYDEV_CHIP_MEMBER_4U EQU 5
CYDEV_CHIP_MEMBER_4V EQU 16
CYDEV_CHIP_MEMBER_5A EQU 3
CYDEV_CHIP_MEMBER_5B EQU 2
CYDEV_CHIP_MEMBER_6A EQU 24
CYDEV_CHIP_MEMBER_FM3 EQU 28
CYDEV_CHIP_MEMBER_FM4 EQU 29
CYDEV_CHIP_MEMBER_PDL_FM0P_TYPE1 EQU 25
CYDEV_CHIP_MEMBER_PDL_FM0P_TYPE2 EQU 26
CYDEV_CHIP_MEMBER_PDL_FM0P_TYPE3 EQU 27
CYDEV_CHIP_MEMBER_UNKNOWN EQU 0
CYDEV_CHIP_MEMBER_USED EQU CYDEV_CHIP_MEMBER_5B
CYDEV_CHIP_DIE_EXPECT EQU CYDEV_CHIP_MEMBER_USED
CYDEV_CHIP_DIE_ACTUAL EQU CYDEV_CHIP_DIE_EXPECT
CYDEV_CHIP_REV_LEOPARD_ES1 EQU 0
CYDEV_CHIP_REV_LEOPARD_ES2 EQU 1
CYDEV_CHIP_REV_LEOPARD_ES3 EQU 3
CYDEV_CHIP_REV_LEOPARD_PRODUCTION EQU 3
CYDEV_CHIP_REV_PSOC4A_ES0 EQU 17
CYDEV_CHIP_REV_PSOC4A_PRODUCTION EQU 17
CYDEV_CHIP_REV_PSOC5LP_ES0 EQU 0
CYDEV_CHIP_REV_PSOC5LP_PRODUCTION EQU 0
CYDEV_CHIP_REV_PSOC5TM_ES0 EQU 0
CYDEV_CHIP_REV_PSOC5TM_ES1 EQU 1
CYDEV_CHIP_REV_PSOC5TM_PRODUCTION EQU 1
CYDEV_CHIP_REV_TMA4_ES EQU 17
CYDEV_CHIP_REV_TMA4_ES2 EQU 33
CYDEV_CHIP_REV_TMA4_PRODUCTION EQU 17
CYDEV_CHIP_REVISION_3A_ES1 EQU 0
CYDEV_CHIP_REVISION_3A_ES2 EQU 1
CYDEV_CHIP_REVISION_3A_ES3 EQU 3
CYDEV_CHIP_REVISION_3A_PRODUCTION EQU 3
CYDEV_CHIP_REVISION_4A_ES0 EQU 17
CYDEV_CHIP_REVISION_4A_PRODUCTION EQU 17
CYDEV_CHIP_REVISION_4D_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_4E_CCG2_NO_USBPD EQU 0
CYDEV_CHIP_REVISION_4E_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_4F_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_4F_PRODUCTION_256DMA EQU 0
CYDEV_CHIP_REVISION_4F_PRODUCTION_256K EQU 0
CYDEV_CHIP_REVISION_4G_ES EQU 17
CYDEV_CHIP_REVISION_4G_ES2 EQU 33
CYDEV_CHIP_REVISION_4G_PRODUCTION EQU 17
CYDEV_CHIP_REVISION_4H_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_4I_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_4J_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_4K_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_4L_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_4M_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_4N_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_4O_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_4P_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_4Q_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_4R_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_4S_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_4T_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_4U_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_4V_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_5A_ES0 EQU 0
CYDEV_CHIP_REVISION_5A_ES1 EQU 1
CYDEV_CHIP_REVISION_5A_PRODUCTION EQU 1
CYDEV_CHIP_REVISION_5B_ES0 EQU 0
CYDEV_CHIP_REVISION_5B_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_6A_ES EQU 17
CYDEV_CHIP_REVISION_6A_NO_UDB EQU 33
CYDEV_CHIP_REVISION_6A_PRODUCTION EQU 33
CYDEV_CHIP_REVISION_FM3_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_FM4_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_PDL_FM0P_TYPE1_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_PDL_FM0P_TYPE2_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_PDL_FM0P_TYPE3_PRODUCTION EQU 0
CYDEV_CHIP_REVISION_USED EQU CYDEV_CHIP_REVISION_5B_PRODUCTION
CYDEV_CHIP_REV_EXPECT EQU CYDEV_CHIP_REVISION_USED
CYDEV_CONFIG_FASTBOOT_ENABLED EQU 1
CYDEV_CONFIG_UNUSED_IO_AllowButWarn EQU 0
CYDEV_CONFIG_UNUSED_IO_AllowWithInfo EQU 1
CYDEV_CONFIG_UNUSED_IO_Disallowed EQU 2
CYDEV_CONFIG_UNUSED_IO EQU CYDEV_CONFIG_UNUSED_IO_Disallowed
CYDEV_CONFIGURATION_COMPRESSED EQU 1
CYDEV_CONFIGURATION_DMA EQU 0
CYDEV_CONFIGURATION_ECC EQU 1
CYDEV_CONFIGURATION_IMOENABLED EQU CYDEV_CONFIG_FASTBOOT_ENABLED
CYDEV_CONFIGURATION_MODE_COMPRESSED EQU 0
CYDEV_CONFIGURATION_MODE EQU CYDEV_CONFIGURATION_MODE_COMPRESSED
CYDEV_CONFIGURATION_MODE_DMA EQU 2
CYDEV_CONFIGURATION_MODE_UNCOMPRESSED EQU 1
CYDEV_DEBUG_ENABLE_MASK EQU 0x20
CYDEV_DEBUG_ENABLE_REGISTER EQU CYREG_MLOGIC_DEBUG
CYDEV_DEBUGGING_DPS_Disable EQU 3
CYDEV_DEBUGGING_DPS EQU CYDEV_DEBUGGING_DPS_Disable
CYDEV_DEBUGGING_DPS_JTAG_4 EQU 1
CYDEV_DEBUGGING_DPS_JTAG_5 EQU 0
CYDEV_DEBUGGING_DPS_SWD EQU 2
CYDEV_DEBUGGING_DPS_SWD_SWV EQU 6
CYDEV_DEBUGGING_ENABLE EQU 1
CYDEV_DEBUGGING_XRES EQU 0
CYDEV_DMA_CHANNELS_AVAILABLE EQU 24
CYDEV_ECC_ENABLE EQU 0
CYDEV_HEAP_SIZE EQU 0x800
CYDEV_INSTRUCT_CACHE_ENABLED EQU 1
CYDEV_INTR_RISING EQU 0x00000003
CYDEV_IS_EXPORTING_CODE EQU 0
CYDEV_IS_IMPORTING_CODE EQU 0
CYDEV_PROJ_TYPE EQU 1
CYDEV_PROJ_TYPE_BOOTLOADER EQU 1
CYDEV_PROJ_TYPE_LAUNCHER EQU 5
CYDEV_PROJ_TYPE_LOADABLE EQU 2
CYDEV_PROJ_TYPE_LOADABLEANDBOOTLOADER EQU 4
CYDEV_PROJ_TYPE_MULTIAPPBOOTLOADER EQU 3
CYDEV_PROJ_TYPE_STANDARD EQU 0
CYDEV_PROTECTION_ENABLE EQU 0
CYDEV_STACK_SIZE EQU 0x0800
CYDEV_USE_BUNDLED_CMSIS EQU 1
CYDEV_VARIABLE_VDDA EQU 0
CYDEV_VDDA_MV EQU 5000
CYDEV_VDDD_MV EQU 5000
CYDEV_VDDIO0_MV EQU 5000
CYDEV_VDDIO1_MV EQU 5000
CYDEV_VDDIO2_MV EQU 5000
CYDEV_VDDIO3_MV EQU 5000
CYDEV_VIO0_MV EQU 5000
CYDEV_VIO1_MV EQU 5000
CYDEV_VIO2_MV EQU 5000
CYDEV_VIO3_MV EQU 5000
CYIPBLOCK_ARM_CM3_VERSION EQU 0
CYIPBLOCK_P3_ANAIF_VERSION EQU 0
CYIPBLOCK_P3_CAN_VERSION EQU 0
CYIPBLOCK_P3_CAPSENSE_VERSION EQU 0
CYIPBLOCK_P3_COMP_VERSION EQU 0
CYIPBLOCK_P3_DECIMATOR_VERSION EQU 0
CYIPBLOCK_P3_DFB_VERSION EQU 0
CYIPBLOCK_P3_DMA_VERSION EQU 0
CYIPBLOCK_P3_DRQ_VERSION EQU 0
CYIPBLOCK_P3_DSM_VERSION EQU 0
CYIPBLOCK_P3_EMIF_VERSION EQU 0
CYIPBLOCK_P3_I2C_VERSION EQU 0
CYIPBLOCK_P3_LCD_VERSION EQU 0
CYIPBLOCK_P3_LPF_VERSION EQU 0
CYIPBLOCK_P3_OPAMP_VERSION EQU 0
CYIPBLOCK_P3_PM_VERSION EQU 0
CYIPBLOCK_P3_SCCT_VERSION EQU 0
CYIPBLOCK_P3_TIMER_VERSION EQU 0
CYIPBLOCK_P3_USB_VERSION EQU 0
CYIPBLOCK_P3_VIDAC_VERSION EQU 0
CYIPBLOCK_P3_VREF_VERSION EQU 0
CYIPBLOCK_S8_GPIO_VERSION EQU 0
CYIPBLOCK_S8_IRQ_VERSION EQU 0
CYIPBLOCK_S8_SAR_VERSION EQU 0
CYIPBLOCK_S8_SIO_VERSION EQU 0
CYIPBLOCK_S8_UDB_VERSION EQU 0
DMA_CHANNELS_USED__MASK0 EQU 0x00000000
CYDEV_BOOTLOADER_ENABLE EQU 1

#endif /* INCLUDED_CYFITTERIAR_INC */