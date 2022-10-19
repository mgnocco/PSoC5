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
 
    static uint16 counter_tension_func = DIV_INIT_VALUE;
    char info_[2500] = "";
    
    uint8 MOTOR_IDX = 0;
    uint8 SECOND_MOTOR_IDX = 1;
    
    MY_TIMER_REG_Write(0x00);
    timer_value0 = (uint32)MY_TIMER_ReadCounter();

    // Start ADC Conversion, SOC = 1

    ADC_SOC_Write(0x01); 
    
    // Check Interrupt 

    if (interrupt_flag){
        interrupt_flag = FALSE;
        interrupt_manager();
    }
       
    //---------------------------------- Control Motor
    // Control MOTOR_IDX motor (always active) according to motor PWM frequency
    motor_control_generic(MOTOR_IDX);
        
    
    // Check Interrupt 

    if (interrupt_flag){
        interrupt_flag = FALSE;
        interrupt_manager();
    }
    
    // Control 2nd motor (if necessary) according to motor PWM frequency
    motor_control_generic(SECOND_MOTOR_IDX);

    // Check Interrupt 

    if (interrupt_flag){
        interrupt_flag = FALSE;
        interrupt_manager();
    }
           

    //---------------------------------- Read conversion buffer - LOCK function

    analog_read_end();

    if (interrupt_flag){
        interrupt_flag = FALSE;
        interrupt_manager();
    }
    
     
    //---------------------------------- EMG history Update
    if (c_mem.exp.record_EMG_history_on_SD){
        
        update_EMG_history();

        // Check Interrupt 

        if (interrupt_flag){
            interrupt_flag = FALSE;
            interrupt_manager();
        }
    }

    
    //---------------------------------- Control Cycles Counter

    if (pos_reconstruct[c_mem.motor[0].encoder_line]){      // Once Motor 0 encoder line reading is ready
        cycles_counter_update();                            // Need at least one encoder to work
    }

    // Check Cycles Interrupt 
    
    if (cycles_interrupt_flag){
        cycles_interrupt_flag = FALSE;

        // Cycles are written only every 120 seconds (CYCLES_TIMER interrupt)
        // to save EEPROM writings (1M maximum)
        if(can_write) {
            cycles_status = PREPARE_DATA;
            
            //Update time variable
            g_mem.cnt.total_runtime = g_mem.cnt.total_runtime + 120;  // Add 120 seconds.
            
            if (c_mem.exp.read_exp_port_flag == EXP_SD_RTC) {
                
                store_RTC_current_time();
                
                //Write in SD card
                prepare_SD_info(info_);
                FS_Write(pFile, info_, strlen(info_));
                
                if (c_mem.exp.record_EMG_history_on_SD){
                    char EMG_history_info_[15000] = "";
                    strcpy(EMG_history_info_, "");
                    prepare_SD_EMG_history(EMG_history_info_);
                    FS_Write(pEMGHFile, EMG_history_info_, strlen(EMG_history_info_));
                }
            }
        }
        
        // Deactivate the motor just for the time data are written in the EEPROM
        if (cycles_status == WRITE_CYCLES || cycles_status == WAIT_QUERY){
            // Deactivate motors
            enable_motor(0, 0x00); 
            if (g_mem.dev.use_2nd_motor_flag == TRUE) {
                enable_motor(1, 0x00); 
            }
        }
        else {
            // Activate/Deactivate motors
            enable_motor(0, g_ref[0].onoff); 
            if (g_mem.dev.use_2nd_motor_flag == TRUE) {
                enable_motor(1, g_ref[1].onoff); 
            }
        }
    }
    
    // Check Interrupt 
    
    if (interrupt_flag){
        interrupt_flag = FALSE;
        interrupt_manager();
    }
    
    
    //---------------------------------- Check battery
/*
    // Divider 10, freq = 500 Hz
    if (counter_tension_func == CALIBRATION_DIV) {
        battery_management();   
        counter_tension_func = 0;
    }
    counter_tension_func++;

    // Check Interrupt 
    
    if (interrupt_flag){
        interrupt_flag = FALSE;
        interrupt_manager();
    }
*/   
    //---------------------------------- Read IMUs
    if (c_mem.imu.read_imu_flag) {
        ReadAllIMUs();      // IMU reading is atomic, no RS485 request is handled
        
        if (interrupt_flag){
            interrupt_flag = FALSE;
            interrupt_manager();
        }
    }
   
    //---------------------------------- Update States
    
    // Load k-1 state
    memcpy( &g_adc_measOld, &g_adc_meas, sizeof(g_adc_meas) );
    memcpy( &g_measOld, &g_meas, sizeof(g_meas) );
    memcpy( &g_refOld, &g_ref, sizeof(g_ref) );

    // Load k+1 state        
    memcpy( &g_ref, &g_refNew, sizeof(g_ref) );
    memcpy( &g_imu, &g_imuNew, sizeof(g_imu) );
                
    if (interrupt_flag){
        interrupt_flag = FALSE;
        interrupt_manager();
    }

    timer_value = (uint16)MY_TIMER_ReadCounter();
    cycle_time = ((float)(timer_value0 - timer_value)/1000000.0);
    MY_TIMER_REG_Write(0x01);   // reset timer

}

//==============================================================================
//                                                       COMPUTE MOTOR REFERENCE
//==============================================================================
void compute_reference(uint8 motor_idx, struct st_ref* st_ref_p, struct st_ref* st_refOld_p) {
    
    int32 CYDATA handle_value;
    int32 CYDATA err_emg_1, err_emg_2;
    struct st_motor* MOT = &c_mem.motor[motor_idx];      // SoftHand default motor
    uint8 ENC_L = MOT->encoder_line;          // Associated encoder line
    
    static uint8 current_emg[NUM_OF_MOTORS] = {0, 0};   // 0 NONE
                                                        // 1 EMG 1
                                                        // 2 EMG 2
                                                        // wait for both to get down
    
    err_emg_1 = g_adc_meas.emg[0] - c_mem.emg.emg_threshold[0];
    err_emg_2 = g_adc_meas.emg[1] - c_mem.emg.emg_threshold[1];
    
     // =========================== POSITION INPUT ==============================            
    switch(MOT->input_mode) {
        case INPUT_MODE_ENCODER3:

            // Calculate handle value based on position of handle in the
            // sensor chain and multiplication factor between handle and motor position
            if (c_mem.enc[ENC_L].double_encoder_on_off) 
                handle_value = (g_meas[ENC_L].pos[2] * c_mem.enc[ENC_L].motor_handle_ratio) + MOT->pos_lim_inf;
            else
                handle_value = (g_meas[ENC_L].pos[1] * c_mem.enc[ENC_L].motor_handle_ratio) + MOT->pos_lim_inf;
            

            // Read handle and use it as reference for the motor
            if (((handle_value - st_refOld_p->pos) > MOT->max_step_pos) && (MOT->max_step_pos != 0))
                st_ref_p->pos += MOT->max_step_pos;
            else {
                if (((handle_value - st_refOld_p->pos) < MOT->max_step_neg) && (MOT->max_step_neg != 0))
                    st_ref_p->pos += MOT->max_step_neg;
                else
                    st_ref_p->pos = handle_value;
            }
            break;
        
        case INPUT_MODE_JOYSTICK:
            
            if (c_mem.dev.use_2nd_motor_flag == FALSE){
                // Code for single motor devices. Use only up/down direction to give speed reference
                st_ref_p->pos = st_refOld_p->pos;
                if(!(g_adc_meas.joystick[0] > 700)) {
                    int32 CYDATA err_joy_1 = g_adc_meas.joystick[0] - c_mem.JOY_spec.joystick_threshold;

                    if(g_adc_meas.joystick[0] > c_mem.JOY_spec.joystick_threshold) {     //both motors wind the wire around the pulley
                        st_ref_p->pos += ((int32) err_joy_1 * c_mem.JOY_spec.joystick_closure_speed) / (1024 - c_mem.JOY_spec.joystick_threshold);
                    }
                    else {
                        err_joy_1 = g_adc_meas.joystick[0] + c_mem.JOY_spec.joystick_threshold;
                        if(g_adc_meas.joystick[0] < -c_mem.JOY_spec.joystick_threshold) {  //both motors unroll the wire around the pulley
                            st_ref_p->pos += ((int32) err_joy_1 * c_mem.JOY_spec.joystick_closure_speed) / (1024 - c_mem.JOY_spec.joystick_threshold);
                        }
                    }
                }
                else {  //The button is pressed and the motor reference is gradually set to zero 
                    st_ref_p->pos -= (int32) g_mem.JOY_spec.joystick_closure_speed;
                }
            }
            else {
                // Code for two motors devices. Use both direction to give speed references
                if (c_mem.dev.dev_type == SOFTHAND_2_MOTORS){
                    compute_SoftHand_2_motors_joystick_reference(motor_idx, st_ref_p, st_refOld_p);
                }
            }
            break;
            
        case INPUT_MODE_EMG_PROPORTIONAL:
            if (err_emg_1 > 0)
                st_ref_p->pos = (err_emg_1 * g_mem.motor[motor_idx].pos_lim_sup) / (1024 - c_mem.emg.emg_threshold[0]);
            else
                st_ref_p->pos = 0;
            break;
        
        case INPUT_MODE_EMG_PROPORTIONAL_NC:
            if (err_emg_1 > 0)
                st_ref_p->pos = g_mem.motor[motor_idx].pos_lim_sup - (err_emg_1 * g_mem.motor[motor_idx].pos_lim_sup) / (1024 - c_mem.emg.emg_threshold[0]);
            else
                st_ref_p->pos = g_mem.motor[motor_idx].pos_lim_sup;
            break;

        case INPUT_MODE_EMG_INTEGRAL:
            st_ref_p->pos = st_refOld_p->pos;
            if (err_emg_1 > 0) {
                st_ref_p->pos = st_refOld_p->pos + (err_emg_1 * (int)g_mem.emg.emg_speed[0] * 2) / (1024 - c_mem.emg.emg_threshold[0]);
            }
            if (err_emg_2 > 0) {
                st_ref_p->pos = st_refOld_p->pos - (err_emg_2 * (int)g_mem.emg.emg_speed[1] * 2) / (1024 - c_mem.emg.emg_threshold[1]);
            }
            break;

        case INPUT_MODE_EMG_FCFS:
            st_ref_p->pos = st_refOld_p->pos;
            if (c_mem.dev.dev_type != SOFTHAND_2_MOTORS){
                switch (current_emg[motor_idx]) {
                    case 0:
                        // Look for the first EMG passing the threshold
                        if (err_emg_1 > 0 && err_emg_1 > err_emg_2) {
                            current_emg[motor_idx] = 1;
                            break;
                        }
                        if (err_emg_2 > 0 && err_emg_2 > err_emg_1) {
                            current_emg[motor_idx] = 2;
                            break;
                        }
                        break;

                    case 1:
                        // EMG 1 is first
                        if (err_emg_1 < 0) {
                            current_emg[motor_idx] = 0;
                            break;
                        }
                        st_ref_p->pos = st_refOld_p->pos + (err_emg_1 * g_mem.emg.emg_speed[0] << 2) / (1024 - c_mem.emg.emg_threshold[0]);
                        break;

                    case 2:
                        // EMG 2 is first
                        if (err_emg_2 < 0) {
                            current_emg[motor_idx] = 0;
                            break;
                        }
                        st_ref_p->pos = st_refOld_p->pos - (err_emg_2 * g_mem.emg.emg_speed[1] << 2) / (1024 - c_mem.emg.emg_threshold[1]);
                        break;

                    default:
                        break;
                }
            }
            else{ // case SOFTHAND_2_MOTORS
                //compute reference using a FSM for choosing the right sinergy to activate
                compute_SoftHand_2_motors_emg_reference(motor_idx, st_ref_p, st_refOld_p, err_emg_1, err_emg_2);
            }
            break;

        case INPUT_MODE_EMG_FCFS_ADV:
            st_ref_p->pos = st_refOld_p->pos;
            switch (current_emg[motor_idx]) {
                // Look for the first EMG passing the threshold
                case 0:
                    if (err_emg_1 > 0 && err_emg_1 > err_emg_2) {
                        current_emg[motor_idx] = 1;
                        break;
                    }
                    if (err_emg_2 > 0 && err_emg_2 > err_emg_1) {
                        current_emg[motor_idx] = 2;
                        break;
                    }
                    break;

                // EMG 1 is first
                case 1:
                    // If both signals are under threshold go back to status 0
                    if ((err_emg_1 < 0) && (err_emg_2 < 0)) {
                        current_emg[motor_idx] = 0;
                        break;
                    }
                    // but if the current signal come back over threshold, continue using it
                    if (err_emg_1 > 0) 
                        st_ref_p->pos = st_refOld_p->pos + (err_emg_1 * g_mem.emg.emg_speed[0] << 2) / (1024 - c_mem.emg.emg_threshold[0]);
                    
                    break;

                // EMG 2 is first
                case 2:
                    // If both signals are under threshold go back to status 0
                    if ((err_emg_1 < 0) && (err_emg_2 < 0)) {
                        current_emg[motor_idx] = 0;
                        break;
                    }
                    // but if the current signal come back over threshold, continue using it
                    if (err_emg_2 > 0) {
                        st_ref_p->pos = st_refOld_p->pos - (err_emg_2 * c_mem.emg.emg_speed[1] << 2) / (1024 - c_mem.emg.emg_threshold[1]);
                    }
                    break;

                default:
                    break;
            }
            break;

        default:
            break;
    }

    // Position limit saturation
    if (MOT->pos_lim_flag) {
        if (st_ref_p->pos < MOT->pos_lim_inf) 
            st_ref_p->pos = MOT->pos_lim_inf;
        if (st_ref_p->pos > MOT->pos_lim_sup) 
            st_ref_p->pos = MOT->pos_lim_sup;
    }
    
    // SAFETY
    if (battery_low_SoC == TRUE) {
        //Reopen the terminal device before disabling motor
        st_ref_p->pos = 0;
    }      
    
       
    if (c_mem.SH.rest_position_flag == TRUE) {
        if (rest_enabled == 1){
            // Change position reference to drive motor to rest position smoothly
            st_ref_p->pos = rest_pos_curr_ref;
        }
        
        if (forced_open == 1) {
            // Open the SoftHand as EMG PROPORTIONAL input mode 
            if (err_emg_2 > 0)
                st_ref_p->pos = g_mem.SH.rest_pos - (err_emg_2 * g_mem.SH.rest_pos) / (1024 - c_mem.emg.emg_threshold[1]);
            else {
                st_ref_p->pos = g_mem.SH.rest_pos;
                forced_open = 0;
            }
        }
    }
}


//==============================================================================
//                                  COMPUTE SOFTHAND 2 MOTORS JOYSTICK REFERENCE
//==============================================================================
void compute_SoftHand_2_motors_joystick_reference(uint8 motor_idx, struct st_ref* st_ref_p, struct st_ref* st_refOld_p){
    
    if (c_mem.motor[0].input_mode == INPUT_MODE_JOYSTICK && c_mem.motor[1].input_mode == INPUT_MODE_JOYSTICK){  //only if both motor inputs are in joystick mode
        st_ref_p->pos = st_refOld_p->pos;
    
        if(!(g_adc_meas.joystick[0] > 700)) {
             
            int32 CYDATA err_joy_1 = 0;
            int32 CYDATA err_joy_2 = 0;
        
            if(g_adc_meas.joystick[0] > c_mem.JOY_spec.joystick_threshold) {     //both motors wind the wire around the pulley
                err_joy_1 = g_adc_meas.joystick[0] - c_mem.JOY_spec.joystick_threshold;
                st_ref_p->pos += ((int32) err_joy_1 * c_mem.JOY_spec.joystick_closure_speed) / (1024 - c_mem.JOY_spec.joystick_threshold);
            }
            else {
                if(g_adc_meas.joystick[0] < -c_mem.JOY_spec.joystick_threshold) {  //both motors unroll the wire around the pulley
                    err_joy_1 = g_adc_meas.joystick[0] + c_mem.JOY_spec.joystick_threshold;
                    st_ref_p->pos += ((int32) err_joy_1 * c_mem.JOY_spec.joystick_closure_speed) / (1024 - c_mem.JOY_spec.joystick_threshold);               
                }
            }

            if(g_adc_meas.joystick[1] > c_mem.JOY_spec.joystick_threshold) {    //The wire is winded around the first pulley and unwinded from the second one
                err_joy_2 = g_adc_meas.joystick[1] - c_mem.JOY_spec.joystick_threshold;
                if (motor_idx == 0){
                    st_ref_p->pos += ((int32) err_joy_2 * c_mem.JOY_spec.joystick_closure_speed) / (1024 - c_mem.JOY_spec.joystick_threshold);   
                }
                if (motor_idx == 1){
                    st_ref_p->pos -= ((int32) err_joy_2 * c_mem.JOY_spec.joystick_closure_speed) / (1024 - c_mem.JOY_spec.joystick_threshold);   
                }          
            }
            else {
                if(g_adc_meas.joystick[1] < -c_mem.JOY_spec.joystick_threshold) {  //The wire is unwinded from the first pulley and winded on the second one
                    
                    err_joy_2 = g_adc_meas.joystick[1] + c_mem.JOY_spec.joystick_threshold;
                    if (motor_idx == 0){
                        st_ref_p->pos += ((int32) err_joy_2 * c_mem.JOY_spec.joystick_closure_speed) / (1024 - c_mem.JOY_spec.joystick_threshold);   
                    }
                    if (motor_idx == 1){
                        st_ref_p->pos -= ((int32) err_joy_2 * c_mem.JOY_spec.joystick_closure_speed) / (1024 - c_mem.JOY_spec.joystick_threshold);   
                    }
                }
            }
        }
        else {  //The button is pressed and the hand is opened firstly making the position difference
                //equal to zero, then the position sum is gradually set to zero 
            int32 pos_diff = (g_meas[c_mem.motor[0].encoder_line].pos[0] - g_meas[c_mem.motor[0].encoder_line].pos[1]) >> c_mem.enc[c_mem.motor[0].encoder_line].res[0];
            int32 pos_sum = (g_meas[c_mem.motor[0].encoder_line].pos[0] + g_meas[c_mem.motor[0].encoder_line].pos[1]) >> c_mem.enc[c_mem.motor[0].encoder_line].res[0];

            if(pos_diff > 500 || pos_diff < -500) {
                if(SIGN(pos_diff) == 1) {
                    if (motor_idx == 1){
                        st_ref_p->pos += c_mem.JOY_spec.joystick_closure_speed;
                    }
                    if (motor_idx == 0){
                        st_ref_p->pos -= c_mem.JOY_spec.joystick_closure_speed / 2;
                    }                    
                }
                else {
                    if (motor_idx == 0){
                        st_ref_p->pos += c_mem.JOY_spec.joystick_closure_speed;
                    }
                    if (motor_idx == 1){
                        st_ref_p->pos -= c_mem.JOY_spec.joystick_closure_speed / 2;
                    }
                }
                
            }
            else {
                if(pos_sum > 0) {
                    if (motor_idx == 0){
                        st_ref_p->pos -= c_mem.JOY_spec.joystick_closure_speed * 2;
                    }
                    if (motor_idx == 1){
                        st_ref_p->pos -= c_mem.JOY_spec.joystick_closure_speed * 2;
                    }
                    
                    if (st_ref_p->pos < 0){
                        st_ref_p->pos = 0;
                    }
                }
            }
        }
    }           
}


//==============================================================================
//                                       COMPUTE SOFTHAND 2 MOTORS EMG REFERENCE
//==============================================================================
void compute_SoftHand_2_motors_emg_reference(uint8 motor_idx, struct st_ref* st_ref_p, struct st_ref* st_refOld_p,
    int32 err_emg_1, int32 err_emg_2) {
    
    //Compute reference using a FSM for choosing the right sinergy to activate (call this routine only once per cycle)
    static uint8 fsm_state;     
    if (motor_idx == 0){        //update the fsm only at motor 0 compute reference call and hold the value for motor 1 reference computation
        fsm_state = emg_activation_velocity_fsm();
    }
       
    switch (fsm_state){
        case RELAX_STATE: case TIMER_STATE:
             
            st_ref_p->pos = st_refOld_p->pos;
    
            break;
        
        case MOVE_SLOW_ACT: // First sinergy movement (related to default slow activation)

            if (err_emg_1 > 0 && err_emg_1 > err_emg_2) {   //both motors wind the wire around the pulley and use emg1 error
               st_ref_p->pos = st_refOld_p->pos + (err_emg_1 * g_mem.emg.emg_speed[0] << 2) / (1024 - c_mem.emg.emg_threshold[0]);                       
            }
            if (err_emg_2 > 0 && err_emg_2 > err_emg_1) {   //both motors unroll the wire around the pulley and use emg2 error
               st_ref_p->pos = st_refOld_p->pos - (err_emg_2 * g_mem.emg.emg_speed[1] << 2) / (1024 - c_mem.emg.emg_threshold[1]);
            }
            
            break;
            
        case MOVE_FAST_ACT: // Second sinergy movement (related to default fast activation)
            
            if (err_emg_1 > 0 && err_emg_1 > err_emg_2) {   //(Pinch) The wire is winded around the first pulley and unwinded from the second one
                if (motor_idx == 0){
                    st_ref_p->pos = st_refOld_p->pos + (err_emg_1 * g_mem.emg.emg_speed[0] << 2) / (1024 - c_mem.emg.emg_threshold[0]);   
                }
                if (motor_idx == 1){
                    st_ref_p->pos = st_refOld_p->pos - (err_emg_1 * g_mem.emg.emg_speed[0] << 2) / (1024 - c_mem.emg.emg_threshold[0]);  
                }   
            }
            if (err_emg_2 > 0 && err_emg_2 > err_emg_1) {   //(Index point) The wire is unwinded from the first pulley and winded on the second one
                if (motor_idx == 0){
                    st_ref_p->pos = st_refOld_p->pos - (err_emg_2 * g_mem.emg.emg_speed[1] << 2) / (1024 - c_mem.emg.emg_threshold[1]);   
                }
                if (motor_idx == 1){
                    st_ref_p->pos = st_refOld_p->pos + (err_emg_2 * g_mem.emg.emg_speed[1] << 2) / (1024 - c_mem.emg.emg_threshold[1]);  
                }
            }
            
            break;
    }
 
}

//==============================================================================
//                                                         MOTOR CONTROL GENERIC
//==============================================================================
void motor_control_generic(uint8 idx) {
    
    int32 CYDATA pwm_input = 0;
    
    struct st_motor* MOT = &c_mem.motor[idx]; // Motor struct
    
    // ======================= UPDATE MOTOR REFERENCE ==========================            
    //NOT NEEDED compute_reference(idx, &g_ref[idx], &g_refOld[idx]);
    
   
    switch(MOT->control_mode) {
        // ======================= CURRENT AND POSITION CONTROL =======================
        case CURR_AND_POS_CONTROL:
        break;
        // ============================== POSITION CONTROL =====================
        case CONTROL_ANGLE:
        break;    
        // ========================== CURRENT CONTROL ==========================
        case CONTROL_CURRENT:    
        break;
        
        // ================= DIRECT PWM CONTROL ================================
        case CONTROL_PWM:

            pwm_input = g_ref[idx].pwm;

            if (pwm_input != 0){
                // PWM duty cycle is always fixed to 50%
                switch(MOT->motor_pwm_frequency){
                    case FREQ_30HZ:
                        pwm_input = 4999;   //period 9999
                    break;
                    case FREQ_100HZ:
                        pwm_input = 1499;   //period 2999
                    break;
                    case FREQ_300HZ:
                        pwm_input = 499;    //period 999
                    break;
                    case FREQ_1000HZ:
                        pwm_input = 149;    //period 299
                    break;
                    case FREQ_3000HZ:
                        pwm_input = 49;     //period 99
                    break;
                }
            }
            else {
                pwm_input = 0;
            }
            
        break;
            
    }

    // Set motor direction and write pwm value
    switch (idx) {
        case 0:         // Motor 1
            // DRIVER_MC33887 standard
            PWM_MOTORS_WriteCompare1(abs(pwm_input));    
            break;
        case 1:         // Motor 2
            
            // DRIVER_MC33887 standard
            PWM_MOTORS_WriteCompare2(abs(pwm_input)); 
            break;
        default:
            break;
    }
    
}

//==============================================================================
//                                                           ENCODER READING SPI
//==============================================================================

void encoder_reading_SPI(uint8 n_line, uint8 assoc_motor) {

    uint8 CYDATA index = 0;
    
    // Encoder Variables  
    uint8 jj;
     
    uint32 aux_encoder[NUM_OF_SENSORS];
    int16 tmp_value_encoder;
    int32 tmp_value_encoder_32;
    int32 value_encoder;
    int32 speed_encoder;
    int32 value_diff;
	int8 init_rot = 0;

    static int32 last_value_encoder[N_ENCODER_LINE_MAX][NUM_OF_SENSORS];
    static int32 comp_value_encoder[N_ENCODER_LINE_MAX][NUM_OF_SENSORS];
    static uint8 error[N_ENCODER_LINE_MAX][NUM_OF_SENSORS];
    
    static CYBIT only_first_time[N_ENCODER_LINE_MAX] = {TRUE, TRUE};
    static CYBIT safe_startup_motor_activation[N_ENCODER_LINE_MAX] = {FALSE, FALSE};
    static uint8 one_time_execute[N_ENCODER_LINE_MAX] = {0, 0};
    static uint32 count_startup_motor[N_ENCODER_LINE_MAX] = {0, 0};

    static int32 v_value[N_ENCODER_LINE_MAX][N_ENCODERS];   //last value for velocity
    static int32 vv_value[N_ENCODER_LINE_MAX][N_ENCODERS];  //last last value for velocity
    static int32 vvv_value[N_ENCODER_LINE_MAX][N_ENCODERS];  //last last last value for velocity
    
    if (reset_last_value_flag[n_line]) {
        for (jj = N_ENCODERS; jj--;) 
            last_value_encoder[n_line][jj] = 0;
        
        reset_last_value_flag[n_line] = 0;
    }

    //======================================================     reading sensors
    for (index = 0; index < N_ENCODERS; index++) {
        aux_encoder[index] = 0;
    }   
    
    ReadEncoderLine(N_Encoder_Line_Connected[n_line] , n_line);         //CS0 for right hand
                                                                        //CS1 for left hand
    for (int j = 0; j < N_ENCODERS; j++) {    
        
        // As default, index=0 reads SoftHand Pro encoder positioned on screw, while index=1 reads encoder positioned on gear    
        index = c_mem.enc[n_line].Enc_idx_use_for_control[j];       // take encoder idx used for motor control
        
        if (Encoder_Check[n_line][index] > 15){  // check on encoder data
            aux_encoder[j] = (uint32)Encoder_Value[n_line][index];     // 00000000000000[20] XXXXXXXXXXXX[12]
        }
    }
    
    // S = SIGN BIT
    // X = ENCODER VALUE BIT
    // 0 = 0 BIT
    // C = CONTROL BIT
    
    for (index = 0; index < N_ENCODERS; index++) {
        
        data_encoder_raw[n_line][index] = aux_encoder[index];
        
        tmp_value_encoder = (int16)(aux_encoder[index] - (uint16)g_mem.enc[n_line].m_off[index]);
        if (tmp_value_encoder < 0){
            tmp_value_encoder = tmp_value_encoder + 4096;   //SSSS[4] XXXXXXXXXXXX[12] worst case range [-4096, 4095]
        }       // Range [0, 4096]

        if (tmp_value_encoder >= 2048) {
           tmp_value_encoder = tmp_value_encoder - 4096;    //SSSS[4] XXXXXXXXXXXX[12] range [-2048, 2047]
        }       // Range [-2048, 2047]

        tmp_value_encoder_32 = (((int32)(tmp_value_encoder)) << 4);     //SSSSSSSSSSSSSSSS[16] XXXXXXXXXXXX[12] 0000[4] range [-32768, 32767]
        comp_value_encoder[n_line][index] = tmp_value_encoder_32;

        // Initialize last_value_encoder
        if (only_first_time[n_line]) {
            last_value_encoder[n_line][index] = tmp_value_encoder_32;
            if (index == 2)
                only_first_time[n_line] = 0;
        }

        // Take care of rotations
        value_diff = tmp_value_encoder_32 - last_value_encoder[n_line][index];     // worst case SSSSSSSSSSSSSSS[15] XXXXXXXXXXXXX[13] 0000[4]

        // ====================== 1 TURN ======================
        // -32768                    0                    32767 -32768                   0                     32767
        // |-------------------------|-------------------------|-------------------------|-------------------------|
        //              |                         |      |           |      |                         |
        //           -16384                     16383    |           |   -16384                     16383
        //                                               |           |
        //                                           24575           -24576
        //                                               |___________|
        //                                                   49152

        // if we are in the right interval, take care of rotation
        // and update the variable only if the difference between
        // one measure and another is less than 1/4 of turn

        // Considering we are sampling at 1kHz, this means that our shaft needs
        // to go slower than 1/4 turn every ms -> 1 turn every 4ms
        // equal to 250 turn/s -> 15000 RPM

        if (value_diff > 49152)
            g_meas[n_line].rot[index]--;
        else{ 
            if (value_diff < -49152)
                g_meas[n_line].rot[index]++;
            else{
                if (abs(value_diff) > 16384) { // if two measures are too far
                    error[n_line][index]++;
                                
                    if (error[n_line][index] < 10)
                        // Discard
                        return;
                }
            }
        }

        error[n_line][index] = 0;
        
        last_value_encoder[n_line][index] = tmp_value_encoder_32;

        value_encoder = (int32)tmp_value_encoder_32;   // SSSSSSSSSSSSSSSS[16] XXXXXXXXXXXX[12] 0000[4]

        value_encoder += ((int32)g_meas[n_line].rot[index] << 16);    
        
        if (c_mem.enc[n_line].m_mult[index] != 1.0) {
            value_encoder *= c_mem.enc[n_line].m_mult[index];
        }

        if (c_mem.dev.dev_type == SOFTHAND_PRO) {
            // Right / Left hand turn
            if (index == 0) {
                if (c_mem.dev.right_left == RIGHT_HAND){
                    value_encoder *= -1;        
                }
            }
        } 
        
        g_meas[n_line].pos[index] = value_encoder;
    
        speed_encoder = (int16)filter((11*value_encoder - 18* v_value[n_line][index] + 9 * vv_value[n_line][index] -2 * vvv_value[n_line][index]), &filt_vel[index]);

        //Update current speed
        speed_encoder = speed_encoder / (6*cycle_time);
        g_meas[n_line].vel[index] = speed_encoder;

        // update old velocity and acceleration values
        vvv_value[n_line][index] = vv_value[n_line][index];
        vv_value[n_line][index] = v_value[n_line][index];
        v_value[n_line][index] = value_encoder;

        // wait at least 5 * max_num_of_error (10) + 5 = 55 cycles to reconstruct the right turn
        if (pos_reconstruct[n_line] == FALSE){
            if (one_time_execute[n_line] < 54) 
                one_time_execute[n_line]++;
            else {

                //Double encoder translation
                if (c_mem.enc[n_line].double_encoder_on_off){
                    init_rot = calc_turns_fcn(comp_value_encoder[n_line][0],comp_value_encoder[n_line][1], 
                                c_mem.enc[n_line].gears_params[0], c_mem.enc[n_line].gears_params[1], c_mem.enc[n_line].gears_params[2]);


                    if (c_mem.enc[n_line].m_mult[0] < 0)
                        init_rot = -init_rot;
                    
                    g_meas[n_line].rot[0] = (int8)init_rot;
                }    

                if (c_mem.enc[n_line].m_mult[0] != 1.0)
                    g_meas[n_line].pos[0] /= c_mem.enc[n_line].m_mult[0];
                
                value_encoder += ((int32)init_rot << 16); 
       
                if (c_mem.enc[n_line].m_mult[0] != 1.0) {
                    value_encoder *= c_mem.enc[n_line].m_mult[0];
                }

                if (c_mem.dev.dev_type == SOFTHAND_PRO) {
                    // Right / Left hand turn
                    if (c_mem.dev.right_left == RIGHT_HAND){
                        value_encoder *= -1;        
                    }
                }
         
                g_meas[n_line].pos[0] = value_encoder;
                
                g_refNew[assoc_motor].pos = g_meas[n_line].pos[0];

                // If necessary activate motor
    			safe_startup_motor_activation[n_line] = TRUE;
                // Activate the motor associated to this encoder line
                g_refNew[assoc_motor].onoff = c_mem.motor[assoc_motor].activ;
                enable_motor(assoc_motor, g_refNew[assoc_motor].onoff);                
                
                pos_reconstruct[n_line] = TRUE;
            }
        }
    } 

	
	// Wait for 35+SAFE_STARTUP_MOTOR_READINGS cycles before starting motors
    if (safe_startup_motor_activation[n_line]){
        count_startup_motor[n_line]++;
        if (count_startup_motor[n_line] >= (uint32)SAFE_STARTUP_MOTOR_READINGS) {                            
            g_refNew[assoc_motor].pos = 0;
            
            // Activate the motor associated to this encoder line
            g_refNew[assoc_motor].onoff = c_mem.motor[assoc_motor].activ;
            enable_motor(assoc_motor, g_refNew[assoc_motor].onoff); 
                        
            safe_startup_motor_activation[n_line] = FALSE;
        }
    }
 
}

//==============================================================================
//                                                           ANALOG MEASUREMENTS
//==============================================================================

void analog_read_end() {

    /* =========================================================================
    /   Ideal formulation to calculate tension and current
    /
    /   tension = ((read_value_mV - offset) * 101) / gain -> [mV]
    /   current = ((read_value_mV - offset) * 375) / (gain * resistor) -> [mA]
    /
    /   Definition:
    /   read_value_mV = counts_read / 0.819 -> conversion from counts to mV
    /   offset = 2000 -> hardware amplifier bias in mV
    /   gain = 8.086 -> amplifier gain
    /   resistor = 18 -> resistor of voltage divider in KOhm 
    /
    /   Real formulation: tradeoff in good performance and accurancy, ADC_buf[] 
    /   and offset unit of measurement is counts, instead dev_tension and
    /   g_meas.curr[] are converted in properly unit.
    /  =========================================================================
    */

    int32 CYDATA i_aux;

    static uint16 emg_counter_1 = 0;
    static uint16 emg_counter_2 = 0;
    static uint16 UD_counter = 0;
    static uint16 LR_counter = 0;
    static int32 UD_mean_value;
    static int32 LR_mean_value;
	static uint8 first_tension_valid = TRUE;
    static int32 detect_power_cycle_prev = -3;
    static uint16 count = 0;
    static uint32 v_count = 0;
    static uint8 idx = 0;
    
    
    // Wait for conversion end
    
    while(!ADC_STATUS_Read()){
        
        if (interrupt_flag){
            interrupt_flag = FALSE;
            interrupt_manager();
        }
    }
    
#ifdef AIR_CHAMBERS_FB_FW
    if (c_mem.dev.dev_type == AIR_CHAMBERS_FB){
        // Read pressure in any case
        g_fb_meas.pressure  = (int32)(ADC_buf[0] -1540);    //0 - 4096  
        g_fb_meas.pressure = (((float)g_fb_meas.pressure/4096.0)/0.004);       // datasheet transfer function ticks->kPa
        if (g_fb_meas.pressure < 0) g_fb_meas.pressure = 0;
    }
#endif
 
    // Convert tension read
    if (g_mem.dev.dev_type == AIR_CHAMBERS_FB){
        dev_tension[0] = 5000;
    }
    else {
        dev_tension[0] =  ((int32)(ADC_buf[0] - 1621) * 1990) >> 7;
    }
    
    // Read also 2nd power tension (if necessary)
    if (NUM_OF_ANALOG_INPUTS > 4) {
        dev_tension[1] = ((int32)(ADC_buf[4] - 1621) * 1990) >> 7;
    }
    
    if (interrupt_flag){
        interrupt_flag = FALSE;                                                                                                                                                                                                                                                                                                                                                                                                                                                 
        interrupt_manager();
    }
    
    // Update cycle power value
    detect_power_cycle = filter(dev_tension[0]/6000, &filt_detect_pc);
    if (detect_power_cycle_prev < 0 && detect_power_cycle >= 0){    // Only positive difference is valid to update power cycles
        g_mem.cnt.power_cycles++;       // New power cycle (update value)   
    }
    detect_power_cycle_prev = detect_power_cycle;
  
    
    // VOLTAGE READING
    // Once firmware starts, first_tension_valid flag is set to TRUE while tension_valid status is FALSE
    // Step 1) Wait for battery voltage stabilization and filter convergence for 1000 cycles (v_count counter), after that tension_valid flag is set to TRUE
    // Step 2) Wait for another 1000 cycles (count counter) to decide which is full charge power tension, than set first_tension_valid to FALSE
    // Low voltage condition) Whenever dev_tension ADC value is under 7000 mV, tension_valid flag is set to FALSE and dev_tension assigned to 5000 mV, then Step 1 has to be repeated
    
	if (first_tension_valid && tension_valid) {     // Voltage reading (Step 2)
        count = count + 1;
        
        if (count == 1000){
            for (idx = 0; idx < NUM_OF_MOTORS; idx++) {
                if (dev_tension[idx] < 9000) {   // 8 V case
                    pow_tension[idx] = 8000;
                }
                else {      // 12 V - 24 V cases
                    if (dev_tension[idx] < 13000) {
                        pow_tension[idx] = 12000;
                    }
                    else
                        pow_tension[idx] = 24000;
                }
            }
            first_tension_valid = FALSE;
        }
    }

    // Until there is no valid input tension repeat this measurement

    if (dev_tension[0] < 6500 && (NUM_OF_ANALOG_INPUTS<=4 || dev_tension[1] < 6500)) {       // Voltage reading (Low voltage condition)
        // PSoC is powered through uUSB
        
        tension_valid = FALSE;
            
        if (interrupt_flag){
            interrupt_flag = FALSE;
            interrupt_manager();
        }
        
        for (idx = 0; idx < NUM_OF_MOTORS; idx++){
            if (c_mem.emg.emg_calibration_flag) {
                if ((c_mem.motor[idx].input_mode == INPUT_MODE_EMG_PROPORTIONAL) ||
                    (c_mem.motor[idx].input_mode == INPUT_MODE_EMG_INTEGRAL) ||
                    (c_mem.motor[idx].input_mode == INPUT_MODE_EMG_FCFS) ||
                    (c_mem.motor[idx].input_mode == INPUT_MODE_EMG_FCFS_ADV) ||
                    (c_mem.motor[idx].input_mode == INPUT_MODE_EMG_PROPORTIONAL_NC)) {
                    g_ref[idx].onoff = 0x00;
                    enable_motor(idx, g_ref[idx].onoff); 
                    
                }
            }

            // Assign dev_tension and reset current
            dev_tension[idx] = 5000;
            g_meas[g_mem.motor[idx].encoder_line].curr = 0;
        }
        
        // Reset emg
        for (idx = 0; idx < NUM_OF_INPUT_EMGS; idx++){
            g_adc_meas.emg[idx] = 0;
        }
        for (idx = 0; c_mem.exp.read_ADC_sensors_port_flag && idx < NUM_OF_ADDITIONAL_EMGS; idx++){
            g_adc_meas.add_emg[idx] = 0;
        }
        
    }
    else {
        // PSoC is powered through battery or power source
        // (at least > 6.88 V (86% of 8 V) that is minimum charge of smallest battery (2 cells @ 2000 mAh Ossur))
        
        // Read ADC sampled value of power source tension and motor current
        
        if (v_count == 1000 && !tension_valid){     // Voltage reading (Step 1)
            // After 1000 v_count cycles, dev_tension_f is stable
            tension_valid = TRUE;   
            count = 0;
            v_count = 0;            
        }
        else {  
            // wait for battery voltage stabilization
            if (v_count < 1000) {
                v_count = v_count + 1;
            }
            for(idx = 0; idx < NUM_OF_MOTORS; idx++) {
                dev_tension_f[idx] = filter(dev_tension[idx], &filt_v[idx]);
            }
        }

        for (idx = 0; idx < NUM_OF_MOTORS; idx++){
      //      if(g_mem.motor[idx].activate_pwm_rescaling)        //pwm rescaling is activated
      //          pwm_limit_search(idx);                 //only for 12V motor
        }
        
        // Filter and Set currents
        g_meas[g_mem.motor[0].encoder_line].curr = filter((int16) (((int32)(ADC_buf[1] - 1648) * 22634) >> 13) * (int32)pwm_sign[0], &filt_i[0]);
        

        // Calculate current estimation and put it in the second part of the current measurement array;
		g_meas[g_mem.motor[0].encoder_line].estim_curr = (int16) filter(((int32) g_meas[g_mem.motor[0].encoder_line].curr) - curr_estim(0, g_meas[g_mem.motor[0].encoder_line].pos[0] >> g_mem.enc[g_mem.motor[0].encoder_line].res[0], g_meas[g_mem.motor[0].encoder_line].vel[0] >> g_mem.enc[g_mem.motor[0].encoder_line].res[0], g_ref[0].pos >> g_mem.enc[g_mem.motor[0].encoder_line].res[0]), &filt_curr_diff[0]);
        
        // Read also 2nd power current (if necessary)
        if (NUM_OF_ANALOG_INPUTS > 4) {
            // Filter and Set currents
            g_meas[g_mem.motor[1].encoder_line].curr = filter((int16) (((int32)(ADC_buf[5] - 1648) * 22634) >> 13) * (int32)pwm_sign[1], &filt_i[1]);
            
            g_meas[g_mem.motor[1].encoder_line].estim_curr = (int16) filter(((int32) g_meas[g_mem.motor[1].encoder_line].curr) - curr_estim(1, g_meas[g_mem.motor[1].encoder_line].pos[0] >> g_mem.enc[g_mem.motor[1].encoder_line].res[0], g_meas[g_mem.motor[1].encoder_line].vel[0] >> g_mem.enc[g_mem.motor[1].encoder_line].res[0], g_ref[1].pos >> g_mem.enc[g_mem.motor[1].encoder_line].res[0]), &filt_curr_diff[1]);
        }
    
        // Check Interrupt 
    
        if (interrupt_flag){
            interrupt_flag = FALSE;
            interrupt_manager();
        }

    }
    
    // Read EMG (always even if the board is not powered)
        
    // if calibration is not needed go to "normal execution"
    if (!c_mem.emg.emg_calibration_flag){
        emg_1_status = NORMAL; // normal execution
        emg_2_status = NORMAL; // normal execution
    }

    // EMG 1 calibration state machine
   
    // Calibration state machine
    switch(emg_1_status) {
        case NORMAL: // normal execution
            
            if (g_mem.dev.dev_type != AIR_CHAMBERS_FB && g_mem.dev.dev_type != OTBK_ACT_WRIST_MS){
                i_aux = ((int32)(ADC_buf[2 + c_mem.emg.switch_emg] - 1639) * 87) >> 5;  //map range to 0-4096
            }
            else {  // Use additional ADC channels, so the signal does not pass through AVAGO isolators
                i_aux = (int32)(ADC_buf[2 + c_mem.emg.switch_emg]);
            }
            
            if (i_aux < 0) 
                i_aux = 0;
            i_aux = filter(i_aux, &filt_emg[0]);
            i_aux = (i_aux << 10) / g_mem.emg.emg_max_value[0];

            if (interrupt_flag){
                interrupt_flag = FALSE;
                interrupt_manager();
            }
            //Saturation
            if (i_aux < 0)
                i_aux = 0;
            else 
                if (i_aux > 1024) 
                    i_aux = 1024;
            
            g_adc_meas.emg[0] = i_aux;

            if (interrupt_flag){
                interrupt_flag = FALSE;
                interrupt_manager();
            }
            
            break;

        case RESET: // reset variables
            emg_counter_1 = 0;
            g_mem.emg.emg_max_value[0] = 0;
            emg_1_status = DISCARD; // goto next status  
            break;

        case DISCARD: // discard first EMG_SAMPLE_TO_DISCARD samples
            emg_counter_1++;
            if (emg_counter_1 == EMG_SAMPLE_TO_DISCARD) {
                emg_counter_1 = 0;          // reset counter
                LED_control(1);
				
                if (interrupt_flag){
                    interrupt_flag = FALSE;
                    interrupt_manager();
                }
                
                emg_1_status = SUM_AND_MEAN;    // sum and mean status
            }
            break;

        case SUM_AND_MEAN: // sum first SAMPLES_FOR_EMG_MEAN samples
            // NOTE max(value)*SAMPLES_FOR_EMG_MEAN must fit in 32bit
            emg_counter_1++;
            if (ADC_buf[2 + c_mem.emg.switch_emg] < 0) 
                ADC_buf[2 + c_mem.emg.switch_emg] = 0;
            g_mem.emg.emg_max_value[0] += filter((int32)ADC_buf[2 + c_mem.emg.switch_emg], &filt_emg[0]);
            
            if (interrupt_flag){
                interrupt_flag = FALSE;
                interrupt_manager();
            }
            
            if (emg_counter_1 == SAMPLES_FOR_EMG_MEAN) {
                g_mem.emg.emg_max_value[0] = g_mem.emg.emg_max_value[0] / SAMPLES_FOR_EMG_MEAN; // calc mean

                if (interrupt_flag){
                    interrupt_flag = FALSE;
                    interrupt_manager();
                }                    
                
                LED_control(0);
				
                emg_counter_1 = 0;          // reset counter

                emg_1_status = NORMAL;      // goto normal execution
            }
            break;

        default:
            break;
    }

    if (interrupt_flag){
        interrupt_flag = FALSE;
        interrupt_manager();
    }
    // EMG 2 calibration state machine
    switch(emg_2_status) {
        case NORMAL: // normal execution
        
            if (g_mem.dev.dev_type != AIR_CHAMBERS_FB && g_mem.dev.dev_type != OTBK_ACT_WRIST_MS){
                i_aux = ((int32)(ADC_buf[3 - c_mem.emg.switch_emg] - 1639) * 87) >> 5;  //map range to 0-4096
            }
            else {  // Use additional ADC channels, so the signal does not pass through AVAGO isolators
                i_aux = (int32)(ADC_buf[3 - c_mem.emg.switch_emg]);
            }

            if (i_aux < 0)
                i_aux = 0;
            i_aux = filter(i_aux, &filt_emg[1]);
            i_aux = (i_aux << 10) / g_mem.emg.emg_max_value[1];

            if (interrupt_flag){
                interrupt_flag = FALSE;
                interrupt_manager();
            }
            
            if (i_aux < 0) 
                i_aux = 0;
            else 
                if (i_aux > 1024)
                    i_aux = 1024;
            
            g_adc_meas.emg[1] = i_aux;

            break;

        case RESET: // reset variables
            emg_counter_2 = 0;
            g_mem.emg.emg_max_value[1] = 0;

            if (interrupt_flag){
                interrupt_flag = FALSE;
                interrupt_manager();
            }
            
            emg_2_status = WAIT; // wait for EMG 1 calibration
            break;

        case DISCARD: // discard first EMG_SAMPLE_TO_DISCARD samples
            emg_counter_2++;
            if (emg_counter_2 == EMG_SAMPLE_TO_DISCARD) {
                emg_counter_2 = 0;          // reset counter
                LED_control(1);

                if (interrupt_flag){
                    interrupt_flag = FALSE;
                    interrupt_manager();
                }
                
                emg_2_status = SUM_AND_MEAN;           // sum and mean status
            }
            break;

        case SUM_AND_MEAN: // sum first SAMPLES_FOR_EMG_MEAN samples
            // NOTE max(value)*SAMPLES_FOR_EMG_MEAN must fit in 32bit
            emg_counter_2++;
            if (ADC_buf[3 - c_mem.emg.switch_emg] < 0)
                ADC_buf[3 - c_mem.emg.switch_emg] = 0;
            g_mem.emg.emg_max_value[1] += filter((int32)ADC_buf[3 - c_mem.emg.switch_emg], &filt_emg[1]);

            if (interrupt_flag){
                interrupt_flag = FALSE;
                interrupt_manager();
            }
            
            if (emg_counter_2 == SAMPLES_FOR_EMG_MEAN) {
                g_mem.emg.emg_max_value[1] = g_mem.emg.emg_max_value[1] / SAMPLES_FOR_EMG_MEAN; // calc mean
                LED_control(0);
                emg_counter_2 = 0;          // reset counter
            
                if (interrupt_flag){
                    interrupt_flag = FALSE;
                    interrupt_manager();
                }
                
                emg_2_status = WAIT_EoC;    // goto end of calibration wait
            }
            break;

        case WAIT: // wait for EMG calibration to be done
            if (emg_1_status == NORMAL)
                emg_2_status = DISCARD;           // goto discard sample
            break;

        case WAIT_EoC:  // wait for end of calibration procedure (only for LED visibility reasons)
            emg_counter_2++;
            if (emg_counter_2 == EMG_SAMPLE_TO_DISCARD) {
                emg_counter_2 = 0;          // reset counter
                
                if (interrupt_flag){
                    interrupt_flag = FALSE;
                    interrupt_manager();
                }
                
                for (idx = 0; idx < NUM_OF_MOTORS; idx++) {
                    if ((c_mem.motor[idx].input_mode == INPUT_MODE_EMG_PROPORTIONAL) ||
                        (c_mem.motor[idx].input_mode == INPUT_MODE_EMG_INTEGRAL) ||
                        (c_mem.motor[idx].input_mode == INPUT_MODE_EMG_FCFS) ||
                        (c_mem.motor[idx].input_mode == INPUT_MODE_EMG_FCFS_ADV) ||
                        (c_mem.motor[idx].input_mode == INPUT_MODE_EMG_PROPORTIONAL_NC)) {
                        if (c_mem.motor[idx].control_mode == CONTROL_ANGLE) {
                            g_ref[idx].pos = g_meas[g_mem.motor[idx].encoder_line].pos[0];
                        }
                        g_ref[idx].onoff = c_mem.motor[idx].activ;
                        enable_motor(idx, g_ref[idx].onoff); 
                    }
                }
                    
                memStore(0);        // Store emg_max_value
                    
                emg_2_status = NORMAL;           // goto normal execution
            }
            break;
        default:
            break;
    }
        
    if (interrupt_flag){
        interrupt_flag = FALSE;
        interrupt_manager();
    }
    
    if (c_mem.motor[0].input_mode == INPUT_MODE_JOYSTICK || c_mem.motor[1].input_mode == INPUT_MODE_JOYSTICK){
        // Read joystick
        
        switch (joy_UD_status) {
            case NORMAL:
                i_aux = (int32)(ADC_buf[2]);
                // Remap the analog reading from -1024 to 1024.  
                i_aux = (int32) (((float) (i_aux - UD_mean_value) / UD_mean_value) * c_mem.JOY_spec.joystick_gains[1]); 
                
                if (interrupt_flag){
                    interrupt_flag = FALSE;
                    interrupt_manager();
                }

                //Saturation
                if (i_aux < -1024) 
                    i_aux = -1024;
                if (i_aux > 1024)
                    i_aux = 1024;

                g_adc_meas.joystick[1] = (int16) i_aux;

                if (interrupt_flag){
                    interrupt_flag = FALSE;
                    interrupt_manager();
                }

            break;

            case RESET: // reset variables
                UD_counter = 0;
                UD_mean_value = 0;
                joy_UD_status = WAIT; // go to waiting status

            break;

            case DISCARD: // discard first EMG_SAMPLE_TO_DISCARD samples
                UD_counter++;
                if (UD_counter == JOYSTICK_SAMPLE_TO_DISCARD) {
                    UD_counter = 0;                     // reset counter

                    if (interrupt_flag){
                        interrupt_flag = FALSE;
                        interrupt_manager();
                    }

                    joy_UD_status = SUM_AND_MEAN;           // sum and mean status
                }

            break;

            case SUM_AND_MEAN: // sum first SAMPLES_FOR_EMG_MEAN samples
                // NOTE max(value)*SAMPLES_FOR_EMG_MEAN must fit in 32bit
                UD_counter++;
                UD_mean_value += (int32)(ADC_buf[2]);        // No filter

                if (interrupt_flag){
                    interrupt_flag = FALSE;
                    interrupt_manager();
                }

                if (UD_counter == SAMPLES_FOR_JOYSTICK_MEAN) {
                    UD_mean_value = UD_mean_value / SAMPLES_FOR_JOYSTICK_MEAN; // calc mean

                    if (interrupt_flag){
                        interrupt_flag = FALSE;
                        interrupt_manager();
                    }

                    UD_counter = 0;          // reset counter
                    joy_UD_status = NORMAL;           // goto normal execution
                }
            break;

            case WAIT: case WAIT_EoC: // wait for both EMG calibrations to be done
                if (emg_1_status == NORMAL && emg_2_status == NORMAL)
                    joy_UD_status = DISCARD;           // goto discard sample
            break;
        }

        if (interrupt_flag){
            interrupt_flag = FALSE;
            interrupt_manager();
        }
            
        switch (joy_LR_status) {
            case NORMAL:
                i_aux = (int32)(ADC_buf[3]);

                i_aux = (int32) (((float) (i_aux - LR_mean_value) / LR_mean_value) * c_mem.JOY_spec.joystick_gains[0]); 
                
                if (interrupt_flag){
                    interrupt_flag = FALSE;
                    interrupt_manager();
                }

                //Saturation
                if (i_aux < -1024)
                    i_aux = -1024;
                if (i_aux > 1024)
                    i_aux = 1024;

                if (interrupt_flag){
                    interrupt_flag = FALSE;
                    interrupt_manager();
                }

                g_adc_meas.joystick[0] = (int16) i_aux;

            break;

            case RESET: // reset variables
                LR_counter = 0;
                LR_mean_value = 0;
                joy_LR_status = WAIT; // goes waiting for all conversions to be done
            break;

            case DISCARD: // discard first EMG_SAMPLE_TO_DISCARD samples8
                LR_counter++;
                if (LR_counter == JOYSTICK_SAMPLE_TO_DISCARD) {
                    LR_counter = 0;                     // reset counter

                    if (interrupt_flag){
                        interrupt_flag = FALSE;
                        interrupt_manager();
                    }

                    joy_LR_status = SUM_AND_MEAN;           // sum and mean status
                }
            break;

            case SUM_AND_MEAN: // sum first SAMPLES_FOR_EMG_MEAN samples
                // NOTE max(value)*SAMPLES_FOR_EMG_MEAN must fit in 32bit
                LR_counter++;
                LR_mean_value += (int32)(ADC_buf[3]);
                if (LR_counter == SAMPLES_FOR_JOYSTICK_MEAN) {
                    LR_mean_value = LR_mean_value / SAMPLES_FOR_JOYSTICK_MEAN; // calc mean
                    
                    if (interrupt_flag){
                        interrupt_flag = FALSE;
                        interrupt_manager();
                    }

                    LR_counter = 0;               // reset counter
                    joy_LR_status = NORMAL;           // goto normal execution
                }
            break;

            case WAIT: case WAIT_EoC:
                if(emg_1_status == NORMAL && emg_2_status == NORMAL && joy_UD_status == NORMAL)
                    joy_LR_status = DISCARD;
            break;
        }
       
        if (interrupt_flag){
            interrupt_flag = FALSE;
            interrupt_manager();
        }
    }
    
    // Read also EMG additional sensors port
    if (NUM_OF_ANALOG_INPUTS > 6) {
        
        for (idx = 0; idx < NUM_OF_ADDITIONAL_EMGS; idx++){
            
            i_aux = (int32)(ADC_buf[6 + idx]);
            if (i_aux < 0) 
                i_aux = 0;
            i_aux = filter(i_aux, &filt_emg[2+idx]);
            i_aux = (i_aux << 10) / 1024;

            if (interrupt_flag){
                interrupt_flag = FALSE;
                interrupt_manager();
            }
            //Saturation
            if (i_aux < 0)
                i_aux = 0;
            else 
                if (i_aux > 1024) 
                    i_aux = 1024;
            
            g_adc_meas.add_emg[idx] = i_aux;

            if (interrupt_flag){
                interrupt_flag = FALSE;
                interrupt_manager();
            }
        }
    }
    
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