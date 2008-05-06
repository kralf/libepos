#ifndef _EPOS_H
#define _EPOS_H

#include "can.h"

/* *************************** */
/* DEFINES                     */
/* *************************** */

#define OPERATION_MODE_PROFILE_VELOCITY 0x3
#define OPERATION_MODE_PROFILE_POSITION 0x1
#define OPERATION_MODE_CURRENT 0xFD

#define WRITE_1_BYTE 0x2f
#define WRITE_2_BYTE 0x2b
#define WRITE_4_BYTE 0x23
#define READ 0x40

#define NUMBER_OF_EPOS 6

#define MAXERRORHISTORY 30

/* *************************** */
/* STRUCTURES                  */
/* *************************** */

typedef struct EPOS_VELOCITY {
  unsigned short int current_epos;
  int velocity;
} EPOS_VELOCITY;

typedef struct EPOS_POSITION_CONFIG_STR {
  int p_gain;
  int i_gain;
  int d_gain;
  int v_feedforward;
  int a_feedforward;
} EPOS_POSITION_CONFIG_STR;

typedef struct EPOS_VELOCITY_CONFIG_STR {
  int p_gain;
  int i_gain;
} EPOS_VELOCITY_CONFIG_STR;

typedef struct EPOS_MOTOR_DATA_STR {
  unsigned int continuous_current_limit;
  unsigned int output_current_limit;
  unsigned int pole_pair_number;
  unsigned int max_speed_in_current_mode;
  unsigned int thermal_time_constant_winding;
} EPOS_MOTOR_DATA_STR;

typedef struct EPOS_ERROR_HISTORY{
  int code;
  unsigned char reg;
  char *msg;
} EPOS_ERROR_HISTORY;

typedef struct EPOS_ERROR_DEVICE{
  int count;
  unsigned char reg;
  EPOS_ERROR_HISTORY history[5];
} EPOS_ERROR_DEVICE;

typedef struct EPOS_ERROR_COMM{
  long int code;
  char *msg;
} EPOS_ERROR_COMM;

typedef struct EPOS_ERROR {
  EPOS_ERROR_DEVICE device;
  EPOS_ERROR_COMM comm;
} EPOS_ERROR;

typedef struct EPOS_READ{
  int sensed_velocity;
  int motion_profile_type;
  int actual_velocity;
  int actual_velocity_avg;
  short actual_current;
  short actual_current_avg;
  int actual_position;
  char operation_mode;
  char operation_mode_display;
  short current_value;

  unsigned int maximum_following_error;
  EPOS_MOTOR_DATA_STR motor_data;

  int position_window_time;
  unsigned int position_window;

  EPOS_VELOCITY_CONFIG_STR velocity_config;     // velocity mode configuration
  EPOS_POSITION_CONFIG_STR position_config;     // position mode configuration

  int max_profile_velocity;

  EPOS_ERROR error;                             // errors
}EPOS_READ;

typedef struct ALL_EPOS_READ {
  EPOS_READ number[NUMBER_OF_EPOS];
} ALL_EPOS_READ;

typedef struct EPOS_SET{
  int des_velocity;             // desired velocity
  int des_maxvelocity;          // desired maximum velocity
  int des_acceleration;         // desired acceleration
  int des_deceleration;         // desired deceleration
  int des_stopdeceleration;     // desired emergency stop deceleration
  int des_position;             // desired position
  EPOS_VELOCITY_CONFIG_STR velocity_config;     // velocity mode configuration
  EPOS_POSITION_CONFIG_STR position_config;     // position mode configuration

  int log_flag;                 // flag indicates data logging on/off
}EPOS_SET;

typedef struct ALL_EPOS_SET {
  EPOS_SET number[NUMBER_OF_EPOS];
} ALL_EPOS_SET;

ALL_EPOS_READ epos_read;
ALL_EPOS_SET epos_set;

/* *************************** */
/* INIT / CONTROL OPERATIONS   */
/* *************************** */

void switch_on(int id); 	// turns the motor to SWITCH_ON state
void enable_quick_stop(int id);
void fault_reset(int id);
void enable_operation(int id);	// enables the motor operation
void activate(int id);
void activate_position(int id);
void shutdown(int id); 		// turns the motor to READY_TO_SWITCH_ON state
void disable_voltage(int id); 	// turns the motor to SWITCH_ON_DISABLED state
void quick_stop(int id); 	// turns the motor to QUICK_STOP_ACTIV state
void start_homing_operation(int id);

/* *************************** */
/* SET OPERATIONS              */
/* *************************** */

void set_mode_of_operation(int id, int mode); 	     // sets the control mode
void set_profile_acceleration(int id, long int a);
void set_profile_deceleration(int id, long int a);
void set_profile_velocity(int id, long int v);
void set_position_window(int id, unsigned int a);
void set_position_window_time(int id,long int a);
void set_motion_profile_type(int id, long int a);
void set_maximum_following_error(int id, unsigned int a);
void set_maximal_profile_velocity(int id, long int v);
void set_quick_stop_deceleration(int id, long int v);
void set_current_value(int id, short current);
void set_position_control_parameter_set(int id, long int p_gain,
  long int i_gain, long int d_gain, long int v_feedward,
  long int a_feedforward);
void set_velocity_control_parameter_set(int id, long int p_gain,
  long int i_gain);
void set_target_velocity(int id, long int v);
void set_target_position(int id, long int x);
void set_velocity_mode_setting_value(int id, long int v);
void set_position_mode_setting_value(int id, long int x);
void set_controlword(int id, int val);
void set_home_offset(int id, long int x);
void set_homing_speed_switch_search(int id, long int v);
void set_homing_speed_zero_search(int id, long int v);
void set_homing_method(int id, int method);
void set_software_minimal_position_limit(int id, long int x);
void set_software_maximal_position_limit(int id, long int x);
void set_continous_current_limit(int id, int i);
void set_output_current_limit(int id, int i);
void set_homing_current_threshold(int id, int i);
void set_RS232_baudrate(int id, int val);

/* *************************** */
/* READ OPERATIONS             */
/* *************************** */

void get_mode_of_operation(int id); 		      // sets the control mode
void get_mode_of_operation_display(int id);
void get_position_control_parameter_set(int id);
void get_velocity_control_parameter_set(int id);
void get_maximum_profile_velocity(int id);
void get_velocity_mode_setting_value(int id);
void get_position_mode_setting_value(int id);
void get_statusword(int id);
void get_controlword(int id);
void get_home_offset(int id);
void get_homing_speed_switch_search(int id);
void get_homing_speed_zero_search(int id);
void get_homing_method(int id);
void get_software_version(int id);
void get_software_minimal_position_limit(int id);
void get_software_maximal_position_limit(int id);
void get_profile_acceleration(int id);
void get_profile_deceleration(int id);
void get_error_register(int id);
void get_error_history(int id, int index);
void get_error(int id);
void get_actual_error_register(int id);
void get_actual_error_history(int id);
void get_continous_current_limit(int id);
void get_output_current_limit(int id);

// position
void get_actual_position(int id);

// velocity
void get_actual_velocity(int id);
void get_averaged_velocity(int id);
void get_maximum_following_error(int id);

// current
void get_current_value(int id);			      // commanding value
void get_current_actual_value(int id);		      // measured value
void get_current_actual_value_averaged(int id);

#endif
