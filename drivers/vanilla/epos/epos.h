#ifndef _EPOS_H
#define _EPOS_H

/* *************************** */
/* DEFINES                     */
/* *************************** */

#define EPOS_OPERATION_MODE_POSITION 0xFF
#define EPOS_OPERATION_MODE_VELOCITY 0xFE
#define EPOS_OPERATION_MODE_PROFILE_POSITION 0x01
#define EPOS_OPERATION_MODE_PROFILE_VELOCITY 0x03
#define EPOS_OPERATION_MODE_HOMING 0x06
#define EPOS_OPERATION_MODE_CURRENT 0xFD

#define EPOS_HOMING_METHOD_POSITIVE_LIMIT_SWITCH 0x02
#define EPOS_HOMING_METHOD_NEGATIVE_LIMIT_SWITCH 0x01
#define EPOS_HOMING_METHOD_POSITIVE_CURRENT_THRESHOLD 0xFD
#define EPOS_HOMING_METHOD_NEGATIVE_CURRENT_THRESHOLD 0xFC

#define EPOS_WRITE_1_BYTE 0x2f
#define EPOS_WRITE_2_BYTE 0x2b
#define EPOS_WRITE_4_BYTE 0x23
#define EPOS_READ 0x40

#define EPOS_NUM_NODES 6

#define EPOS_ERROR_HISTORY 30

/* *************************** */
/* STRUCTURES                  */
/* *************************** */

typedef struct {
  unsigned short int current_epos;
  int velocity;
} epos_velocity_t;

typedef struct {
  int p_gain;
  int i_gain;
  int d_gain;
  int v_feedforward;
  int a_feedforward;
} epos_position_configuration_t;

typedef struct {
  int p_gain;
  int i_gain;
} epos_velocity_configuration_t;

typedef struct {
  unsigned int continuous_current_limit;
  unsigned int output_current_limit;
  unsigned int pole_pair_number;
  unsigned int max_speed_in_current_mode;
  unsigned int thermal_time_constant_winding;
} epos_motor_data_t;

typedef struct {
  int code;
  unsigned char reg;
  char *msg;
} epos_error_history_t;

typedef struct {
  int count;
  unsigned char reg;
  epos_error_history_t history;
} epos_error_device_t;

typedef struct {
  long int code;
  char *msg;
} epos_error_comm_t;

typedef struct {
  epos_error_device_t device;
  epos_error_comm_t comm;
} epos_error_t;

typedef struct {
  short hw_version;

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

  short digital_input;
  short control;
  short status;

  unsigned int maximum_following_error;
  epos_motor_data_t motor_data;

  int position_window_time;
  unsigned int position_window;

  epos_velocity_configuration_t velocity_config;  // velocity mode configuration
  epos_position_configuration_t position_config;  // position mode configuration

  int max_profile_velocity;

  epos_error_t error;                             // errors
} epos_node_read_t;

typedef struct {
  epos_node_read_t node[EPOS_NUM_NODES];
} epos_read_t;

typedef struct {
  int des_velocity;             // desired velocity
  int des_maxvelocity;          // desired maximum velocity
  int des_acceleration;         // desired acceleration
  int des_deceleration;         // desired deceleration
  int des_stopdeceleration;     // desired emergency stop deceleration
  int des_position;             // desired position
  epos_velocity_configuration_t velocity_config;  // velocity mode configuration
  epos_position_configuration_t position_config;  // position mode configuration

  int log_flag;                 // flag indicates data logging on/off
} epos_node_set_t;

typedef struct {
  epos_node_set_t number[EPOS_NUM_NODES];
} epos_set_t;

extern epos_read_t epos_read;
extern epos_set_t epos_set;

/* *************************** */
/* INIT / CONTROL OPERATIONS   */
/* *************************** */

void epos_switch_on(int id); 	    // turns the motor to SWITCH_ON state
void epos_enable_quick_stop(int id);
void epos_fault_reset(int id);
void epos_enable_operation(int id); // enables the motor operation
void epos_activate(int id);
void epos_activate_position(int id);
void epos_shutdown(int id); 	    // turns the motor to READY_TO_SWITCH_ON state
void epos_disable_voltage(int id);  // turns the motor to SWITCH_ON_DISABLED state
void epos_quick_stop(int id); 	    // turns the motor to QUICK_STOP_ACTIV state
void epos_start_homing_operation(int id);

/* *************************** */
/* SET OPERATIONS              */
/* *************************** */

void epos_set_mode_of_operation(int id, int mode);  // sets the control mode
void epos_set_profile_acceleration(int id, long int a);
void epos_set_profile_deceleration(int id, long int a);
void epos_set_profile_velocity(int id, long int v);
void epos_set_position_window(int id, unsigned int a);
void epos_set_position_window_time(int id,long int a);
void epos_set_motion_profile_type(int id, long int a);
void epos_set_maximum_following_error(int id, unsigned int a);
void epos_set_maximal_profile_velocity(int id, long int v);
void epos_set_quick_stop_deceleration(int id, long int v);
void epos_set_current_value(int id, short current);
void epos_set_position_control_parameter_set(int id, long int p_gain,
  long int i_gain, long int d_gain, long int v_feedward,
  long int a_feedforward);
void epos_set_velocity_control_parameter_set(int id, long int p_gain,
  long int i_gain);
void epos_set_target_velocity(int id, long int v);
void epos_set_target_position(int id, long int x);
void epos_set_velocity_mode_setting_value(int id, long int v);
void epos_set_position_mode_setting_value(int id, long int x);
void epos_set_controlword(int id, int val);
void epos_set_home_position(int id, long int x);
void epos_set_home_offset(int id, long int x);
void epos_set_homing_speed_switch_search(int id, long int v);
void epos_set_homing_speed_zero_search(int id, long int v);
void epos_set_homing_method(int id, int method);
void epos_set_software_minimal_position_limit(int id, long int x);
void epos_set_software_maximal_position_limit(int id, long int x);
void epos_set_continous_current_limit(int id, int i);
void epos_set_output_current_limit(int id, int i);
void epos_set_homing_current_threshold(int id, int i);
void epos_set_RS232_baudrate(int id, int val);

/* *************************** */
/* READ OPERATIONS             */
/* *************************** */

void epos_get_hardware_version(int id);
void epos_get_mode_of_operation(int id);          // sets the control mode
void epos_get_mode_of_operation_display(int id);
void epos_get_position_control_parameter_set(int id);
void epos_get_velocity_control_parameter_set(int id);
void epos_get_maximum_profile_velocity(int id);
void epos_get_velocity_mode_setting_value(int id);
void epos_get_position_mode_setting_value(int id);
void epos_get_digital_input(int id);
void epos_get_statusword(int id);
void epos_get_controlword(int id);
void epos_get_home_offset(int id);
void epos_get_homing_speed_switch_search(int id);
void epos_get_homing_speed_zero_search(int id);
void epos_get_homing_method(int id);
void epos_get_software_version(int id);
void epos_get_software_minimal_position_limit(int id);
void epos_get_software_maximal_position_limit(int id);
void epos_get_profile_acceleration(int id);
void epos_get_profile_deceleration(int id);
void epos_get_error_register(int id);
void epos_get_error_history(int id, int index);
void epos_get_error(int id);
void epos_get_actual_error_register(int id);
void epos_get_actual_error_history(int id);
void epos_get_continous_current_limit(int id);
void epos_get_output_current_limit(int id);

// position
void epos_get_actual_position(int id);

// velocity
void epos_get_actual_velocity(int id);
void epos_get_averaged_velocity(int id);
void epos_get_maximum_following_error(int id);

// current
void epos_get_current_value(int id);                    // commanding value
void epos_get_current_actual_value(int id);             // measured value
void epos_get_current_actual_value_averaged(int id);

#endif
