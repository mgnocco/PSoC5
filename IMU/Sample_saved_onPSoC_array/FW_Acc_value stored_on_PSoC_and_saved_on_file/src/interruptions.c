// ----------------------------------------------------------------------------
// BSD 3-Clause License

// Copyright (c) 2016, qbrobotics
// Copyright (c) 2017-2020, Centro "E.Piaggio"
// All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.

// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.

// * Neither the name of the copyright holder nor the names of its
//   contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// POSSIBILITY OF SUCH DAMAGE.
// ----------------------------------------------------------------------------

/**
* \file         interruptions.c
*
* \brief        Interruption handling and firmware core functions
* \date         March 20th, 2020
* \author       _Centro "E.Piaggio"_
* \copyright    (C) 2012-2016 qbrobotics. All rights reserved.
* \copyright    (C) 2017-2020 Centro "E.Piaggio". All rights reserved.
*/


//=================================================================     includes
#include "interruptions.h"

//==============================================================================
//                                                            RS485 RX INTERRUPT
//==============================================================================
// Processing RS-485 data frame:
//
// - 0:     Waits for beginning characters
// - 1:     Waits for ID;
// - 2:     Data length;
// - 3:     Receive all bytes;
// - 4:     Wait for another device end of transmission;
//
//==============================================================================

// PWM values needed to obtain 11.5 V given a certain input tension
// Numbers are sperimentally calculated //[index] (millivolts)
static const uint8 pwm_preload_values[29] = {100,    //0 (11500)
                                              83,
                                              78,
                                              76,
                                              74,
                                              72,    //5 (14000)
                                              70,
                                              68,
                                              67,
                                              65,
                                              64,    //10 (16500)
                                              63,
                                              62,
                                              61,
                                              60,
                                              59,    //15 (19000)
                                              58,
                                              57,
                                              56,
                                              56,
                                              55,    //20 (21500)
                                              54,
                                              54,
                                              53,
                                              52,
                                              52,    //25 (24000)
                                              52,
                                              51,
                                              51};   //28 (25500)

//==============================================================================
//                                                            RS485 RX INTERRUPT
//==============================================================================

CY_ISR(ISR_RS485_RX_ExInterrupt) {

    // Set RS485 flag
    
    interrupt_flag = TRUE;
     
}

//==============================================================================
//                                                        CYCLES TIMER INTERRUPT
//==============================================================================
CY_ISR(ISR_CYCLES_Handler){

    // Set cycles interrupt flag
    
    cycles_interrupt_flag = TRUE;
    
    CYCLES_TIMER_STATUS;

}

//==============================================================================
//                                                             INTERRUPT MANAGER
//==============================================================================
// Manage interrupt from RS485 
//==============================================================================
// Processing RS-485 data frame:
//
// - WAIT_START:    Waits for beginning characters;
// - WAIT_ID:       Waits for ID;
// - WAIT_LENGTH:   Data length;
// - RECEIVE:       Receive all bytes;
// - UNLOAD:        Wait for another device end of transmission;
//
//==============================================================================

void interrupt_manager(){

    
    //===========================================     local variables definition

    static uint8 CYDATA state = WAIT_START;                      // state
    
    //------------------------------------------------- local data packet
    static uint8 CYDATA data_packet_index;
    static uint8 CYDATA data_packet_length;
    static uint8 data_packet_buffer[128];                     
    static uint8 CYDATA rx_queue[3];                    // last 2 bytes received
    //-------------------------------------------------

    uint8 CYDATA    rx_data;                            // RS485 UART rx data
    CYBIT           rx_data_type = TRUE;                       // my id?
    uint8 CYDATA    package_count = 0;                     

    //======================================================     receive routine
    
    while(UART_RS485_GetRxBufferSize() && (package_count < 100)){
        // 100 - estimated maximum number of packets we can read without blocking firmware execution

        // Get next char
        rx_data = UART_RS485_GetChar();
        
        switch (state) {
            //-----     wait for frame start     -------------------------------
            case WAIT_START:
            
                rx_queue[0] = rx_queue[1];
                rx_queue[1] = rx_queue[2];
                rx_queue[2] = rx_data;
                
                // Check for header configuration package
                if ((rx_queue[1] == 58) && (rx_queue[2] == 58)) {
                    rx_queue[0] = 0;
                    rx_queue[1] = 0;
                    rx_queue[2] = 0;
                    state       = WAIT_ID;                    
                }else
                    if ((rx_queue[0] == 63) &&      //ASCII - ?
                        (rx_queue[1] == 13) &&      //ASCII - CR
                        (rx_queue[2] == 10))        //ASCII - LF)
                        infoGet(INFO_ALL);
                break;

            //-----     wait for id     ----------------------------------------
            case  WAIT_ID:

                // packet is for my ID or is broadcast
#ifdef MASTER_FW
                if (rx_data == c_mem.dev.id || rx_data == 0 || (c_mem.MS.slave_comm_active && rx_data == c_mem.MS.slave_ID))
                   rx_data_type = FALSE;
                else                //packet is for others
                    rx_data_type = TRUE;
#else
                if (rx_data == c_mem.dev.id || rx_data == 0)
                    rx_data_type = FALSE;
                else                //packet is for others
                    rx_data_type = TRUE;
#endif                
                data_packet_length = 0;
                state = WAIT_LENGTH;
                break;

            //-----     wait for length     ------------------------------------
            case  WAIT_LENGTH:

                data_packet_length = rx_data;
                // check validity of pack length
                if (data_packet_length <= 1) {
                    data_packet_length = 0;
                    state = WAIT_START;
                } else if (data_packet_length > 128) {
                    data_packet_length = 0;
                    state = WAIT_START;
                } else {
                    data_packet_index = 0;
                    
                    if(rx_data_type == FALSE)
                        state = RECEIVE;          // packet for me or broadcast
                    else
                        state = UNLOAD;           // packet for others
                }
                break;

            //-----     receiving     -------------------------------------------
            case RECEIVE:

                data_packet_buffer[data_packet_index] = rx_data;
                data_packet_index++;
                
                // check end of transmission
                if (data_packet_index >= data_packet_length) {
                    // verify if frame ID corresponded to the device ID
                    if (rx_data_type == FALSE) {
                        // copying data from buffer to global packet
                        memcpy(g_rx.buffer, data_packet_buffer, data_packet_length);
                        g_rx.length = data_packet_length;
                        g_rx.ready  = 1;
                        commProcess();
                    }
                    
                    data_packet_index  = 0;
                    data_packet_length = 0;
                    state              = WAIT_START;
                    package_count++;
                
                }
                break;

            //-----     other device is receving     ---------------------------
            case UNLOAD:
                if (!(--data_packet_length)) {
                    data_packet_index  = 0;
                    data_packet_length = 0;
                    RS485_CTS_Write(1);
                    RS485_CTS_Write(0);
                    state              = WAIT_START;
                    package_count++;
                }
                break;
        }
    }
}
//==============================================================================
//                                                            FUNCTION SCHEDULER
//==============================================================================
// Call all the function with the right frequency
//==============================================================================
// Base frequency 5000 Hz (110 us - max. 200 us cycle time)
//==============================================================================

void function_scheduler(void) {
// t0 = (float)MY_TIMER_ReadCounter();
    static uint16 counter_calibration = DIV_INIT_VALUE;
    static uint16 counter_tension_func = DIV_INIT_VALUE;
    char info[2500];
       strcpy(info,"");
//t1 = (float)MY_TIMER_ReadCounter();
 //   uint8 MOTOR_IDX = 0;
  //  uint8 SECOND_MOTOR_IDX = 1;
    
    //MY_TIMER_REG_Write(0x00);
   //timer_value0 = (uint32)MY_TIMER_ReadCounter();
    
     // Check Interrupt 
     //t0 = (float)MY_TIMER_ReadCounter();
        //---------------------------------- Read IMUs
    if (c_mem.imu.read_imu_flag) {
            ReadAllIMUs();      // IMU reading is atomic, no RS485 request is handled
            if (interrupt_flag){
            interrupt_flag = FALSE;
            interrupt_manager();
        }
    }
     
   // memcpy( &g_imu, &g_imuNew, sizeof(g_imu) );
          
   
        
    if (interrupt_flag){
        interrupt_flag = FALSE;
        interrupt_manager();
    }
 //t1 = (float)MY_TIMER_ReadCounter();
   }


//==============================================================================
//                                                         CYCLES COUNTER UPDATE
//==============================================================================

void cycles_counter_update() {
	static uint8 pos_cycle_status = STATE_INACTIVE;
    static uint8 emg_cycle_status[2] = {STATE_INACTIVE, STATE_INACTIVE};
    static uint8 motion_status[2] = {STATE_INACTIVE, STATE_INACTIVE};
    static uint8 emg_excess_status[2] = {STATE_INACTIVE, STATE_INACTIVE};
    static uint8 rest_cycle_status = STATE_INACTIVE;
    static int32 bin_threshold = 250;
    static int32 exc_act_thr = 980; //0.95*1024
    static int32 thr_pos = 0;
    static int32 max_pos = 0;
    uint8 i, bin_st, bin_max, bin_1, bin_2;
    int32 curr_pos = 0, curr_off = 0, curr_ref = 0;
    int32 step;
    static uint32 timer_value_s, timer_value_e;
    static uint32 timer_exc_s[2], timer_exc_e[2];
    static int32 start_emg_pos[2] = {0,0};

    curr_pos = (int32)(g_meas[g_mem.motor[0].encoder_line].pos[0] >> g_mem.enc[g_mem.motor[0].encoder_line].res[0]);
        
    // State machine - Evaluate position counter update
    switch (pos_cycle_status){
        case STATE_INACTIVE:
            if ((g_mem.motor[0].input_mode != INPUT_MODE_EMG_PROPORTIONAL_NC && pwm_sign[0] == 1) || (g_mem.motor[0].input_mode == INPUT_MODE_EMG_PROPORTIONAL_NC && pwm_sign[0] == -1)){
                thr_pos = curr_pos; 
                curr_off = (max_pos>thr_pos)?(max_pos-thr_pos):(thr_pos-max_pos);
                if (curr_off > 20){    // it has to be a sensible movement to update counter (to avoid noisy measurements sum)  
                    g_mem.cnt.wire_disp = g_mem.cnt.wire_disp + curr_off;     //sum opening track
                }
                pos_cycle_status = STATE_ACTIVE;
            }
            break;
        case STATE_ACTIVE:
            if ((g_mem.motor[0].input_mode != INPUT_MODE_EMG_PROPORTIONAL_NC && pwm_sign[0] == -1) || (g_mem.motor[0].input_mode == INPUT_MODE_EMG_PROPORTIONAL_NC && pwm_sign[0] == 1)){
                max_pos = curr_pos;
                curr_off = (max_pos>thr_pos)?(max_pos-thr_pos):(thr_pos-max_pos);
                if (curr_off > 20){    // it has to be a sensible movement to update counter (to avoid noisy measurements sum)  
                   g_mem.cnt.wire_disp = g_mem.cnt.wire_disp + curr_off;     //sum closure track
                }
                pos_cycle_status = COUNTER_INC;
            }
            break;
        case COUNTER_INC:

            curr_off = (max_pos>thr_pos)?(max_pos-thr_pos):(thr_pos-max_pos);
            if (curr_off > bin_threshold){
                //update position histogram
                step = ((int32)(g_mem.motor[0].pos_lim_sup >> g_mem.enc[g_mem.motor[0].encoder_line].res[0]) / 10);
                bin_st  = (uint8)(thr_pos/step);
                bin_max = (uint8)(max_pos/step);
                
                // Bin computation valid for both NO and NC working
                bin_1 = (bin_st<bin_max)?bin_st:bin_max;
                bin_2 = (bin_st<bin_max)?bin_max:bin_st;    
                for (i=bin_1; i<= bin_2 && i <10; i++){
                    //position_hist counts how many times the SoftHand stays in bin while moving
                    g_mem.cnt.position_hist[i] = g_mem.cnt.position_hist[i] + 1;
                }
                
                //update current histogram (only positive current measures)
                step = ((int32)(g_mem.motor[0].current_limit) / 4);
                if (g_mem.motor[0].not_revers_motor_flag == TRUE)
                    curr_ref = g_meas[g_mem.motor[0].encoder_line].hold_curr; 
                else
                    curr_ref = g_meas[g_mem.motor[0].encoder_line].curr;

                if (curr_ref < 0){
                    curr_ref = -curr_ref;       // Invert sign to have a positive current reading in this function
                }
                
                if (curr_ref > g_mem.motor[0].current_limit)
                    g_mem.cnt.current_hist[3] = g_mem.cnt.current_hist[3] + 1; 
                else
                    g_mem.cnt.current_hist[(uint8)(curr_ref/step)] = g_mem.cnt.current_hist[(uint8)(curr_ref/step)] + 1;
            }
            pos_cycle_status = STATE_INACTIVE;
            break;
    }
    
    // State machine - Evaluate EMG activation counter update
    for (i=0; i<2 && emg_1_status == NORMAL && emg_2_status == NORMAL; i++){
        switch (emg_cycle_status[i]){
            case STATE_INACTIVE:
                if (g_adc_meas.emg[i] > g_mem.emg.emg_threshold[i]){
                    emg_cycle_status[i] = STATE_ACTIVE;
                }
                break;
            case STATE_ACTIVE:
                if (g_adc_meas.emg[i] < g_mem.emg.emg_threshold[i]-10){                    
                    emg_cycle_status[i] = COUNTER_INC;
                }
                break;
            case COUNTER_INC:
                g_mem.cnt.emg_act_counter[i] = g_mem.cnt.emg_act_counter[i] + 1;
                emg_cycle_status[i] = STATE_INACTIVE;
                break;
        }
    }
    
    // State machine - Evaluate Motion counter update
    for (i=0; i<2 && emg_1_status == NORMAL && emg_2_status == NORMAL; i++){
        switch (motion_status[i]){
            case STATE_INACTIVE:
                if (g_adc_meas.emg[i] > g_mem.emg.emg_threshold[i]){
                    start_emg_pos[i] = curr_pos;
                    motion_status[i] = STATE_ACTIVE;
                }
                break;
            case STATE_ACTIVE:
                if (g_adc_meas.emg[i] < g_mem.emg.emg_threshold[i]-10){                    
                    if (abs(start_emg_pos[i] - curr_pos) > 200){     // it has to be a sensible movement to update counter (only if produces motor movement)
                        motion_status[i] = COUNTER_INC;
                    }
                    else {
                        motion_status[i] = STATE_INACTIVE;
                    }
                }
                break;
            case COUNTER_INC:
                g_mem.cnt.motion_counter[i] = g_mem.cnt.motion_counter[i] + 1;
                motion_status[i] = STATE_INACTIVE;
                break;
        }
    }
    
    // State machine - Evaluate EMG excessive activity counter update
    for (i=0; i<2 && emg_1_status == NORMAL && emg_2_status == NORMAL; i++){
        switch (emg_excess_status[i]){
            case STATE_INACTIVE:
                if (g_adc_meas.emg[i] > exc_act_thr){
                    timer_exc_s[i] = (uint32)CYCLES_TIMER_ReadCounter();
                    emg_excess_status[i] = STATE_ACTIVE;
                }
                break;
            case STATE_ACTIVE:
                if (g_adc_meas.emg[i] < exc_act_thr-10){
                    timer_exc_e[i] = (uint32)CYCLES_TIMER_ReadCounter();
                    if (timer_exc_s[i] < timer_exc_e[i]) {
                        timer_exc_s[i] = timer_exc_s[i] + (uint32)6000;
                    }
                    if (((float)(timer_exc_s[i] - timer_exc_e[i])/50.0) > 4.0){      //50 timers ticks per second
                        emg_excess_status[i] = COUNTER_INC;
                    }
                    else {
                        emg_excess_status[i] = STATE_INACTIVE;
                    }
                }
                break;
            case COUNTER_INC:
                g_mem.cnt.excessive_signal_activity[i] = g_mem.cnt.excessive_signal_activity[i] + 1;
                emg_excess_status[i] = STATE_INACTIVE;
                break;
        }
    }
    
    // State machine - Evaluate rest counter update
    switch (rest_cycle_status){
        case STATE_INACTIVE:
            if (rest_enabled){
                timer_value_s = (uint32)CYCLES_TIMER_ReadCounter();
                rest_cycle_status = STATE_ACTIVE;
            }
            break;
        case STATE_ACTIVE:
            if (!rest_enabled){
                timer_value_e = (uint32)CYCLES_TIMER_ReadCounter();
                if (timer_value_s < timer_value_e) {
                    timer_value_s = timer_value_s + (uint32)6000;
                }
                rest_cycle_status = COUNTER_INC;
            }
            break;
        case COUNTER_INC: 
            g_mem.cnt.total_time_rest = g_mem.cnt.total_time_rest + (uint32)((timer_value_s - timer_value_e)/50);
            g_mem.cnt.rest_counter = g_mem.cnt.rest_counter + 1;
            rest_cycle_status = STATE_INACTIVE;
            break;
    }
          
    // This function writes rows [row_start, row_end] on EEPROM
    save_cycles_eeprom();
    
}

//==============================================================================
//                                                            SAVE CYCLES EEPROM
//==============================================================================

void save_cycles_eeprom() {

    cystatus status;
    static uint8 row_number;
    uint8 row_start = 1;
    uint8* addr_start   = (uint8*)&g_mem.cnt.emg_act_counter[0];    //Data at beginning of the row 1
    uint8* addr_start_c = (uint8*)&c_mem.cnt.emg_act_counter[0];
    uint8 row_end   = row_start + EEPROM_COUNTERS_ROWS - 1;
    static uint8* m_addr = NULL; 

    // This part of code writes rows [row_start, row_end] on EEPROM    
    switch(cycles_status) {
        case PREPARE_DATA:
            // Store data in c_mem structure to have consistent counters
            memcpy( addr_start_c, addr_start, EEPROM_BYTES_ROW*EEPROM_COUNTERS_ROWS );            
            m_addr = addr_start;
            row_number = row_start;
            cycles_status = WRITE_CYCLES;
            break;
            
        case WRITE_CYCLES:
            EEPROM_UpdateTemperature();     //Check temperature of chip before writing
            status = EEPROM_StartWrite((uint8*) m_addr, row_number);           
            if(status == CYRET_STARTED || status == CYRET_SUCCESS) {
                cycles_status = WAIT_QUERY;
                can_write = FALSE;
            }
            break;
            
        case WAIT_QUERY:
            status = EEPROM_Query();
            if(status == CYRET_SUCCESS) {
                if (row_number < row_end) {
                    m_addr = m_addr + EEPROM_BYTES_ROW;
                    row_number = row_number + 1;
                    cycles_status = WRITE_CYCLES;
                }
                else {
                    cycles_status = WRITE_END;
                }
            }
            break;
           
        case WRITE_END:
            can_write = TRUE;            
            cycles_status = NONE;
            
            // Restore data saved in c_mem structure
            memcpy( addr_start, addr_start_c, EEPROM_BYTES_ROW*EEPROM_COUNTERS_ROWS );
            break;
            
        case NONE:
            break;
    }
    
}
/* [] END OF FILE */