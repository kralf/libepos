#include <stdio.h>

#undef ASL_DEBUG

#include <can.h>
#include <pdebug.h>

#include "epos.h"

epos_error_history_t error_history[EPOS_ERROR_HISTORY] = {
  { 0x0000, 0x00, "No error." },
  { 0x1000, 0x01, "Generic error." },
  { 0x2310, 0x02, "Over current error." },
  { 0x3210, 0x04, "Over voltage error." },
  { 0x3220, 0x04, "Under voltage." },
  { 0x4210, 0x08, "Over temperature." },
  { 0x5113, 0x04, "Supply voltage (+5V) too low." },
  { 0x6100, 0x20, "Internal software error." },
  { 0x6320, 0x20, "Software parameter error." },
  { 0x7320, 0x20, "Sensor position error." },
  { 0x8110, 0x10, "CAN overrun error (objects lost)." },
  { 0x8111, 0x10, "CAN overrun error." },
  { 0x8120, 0x10, "CAN passive mode error." },
  { 0x8130, 0x10, "CAN life guard error." },
  { 0x8150, 0x10, "CAN transmit COD-ID collision." },
  { 0x81FD, 0x10, "CAN bus off." },
  { 0x81FE, 0x10, "CAN Rx queue overrun." },
  { 0x81FF, 0x10, "CAN Tx queue overrun." },
  { 0x8210, 0x10, "CAN PDO length error." },
  { 0x8611, 0x20, "Following error." },
  { 0xFF01, 0x80, "Hall sensor error." },
  { 0xFF02, 0x80, "Index processing error." },
  { 0xFF03, 0x80, "Encoder resolution error." },
  { 0xFF04, 0x80, "Hallsensor not found error." },
  { 0xFF06, 0x80, "Negative limit error." },
  { 0xFF07, 0x80, "Positive limit error." },
  { 0xFF08, 0x80, "Hall angle detection error." },
  { 0xFF09, 0x80, "Software position limit error." },
  { 0xFF0A, 0x80, "Position sensor breach." },
  { 0xFF0B, 0x20, "System overloaded." }
};

/* *************************** */
/* INIT / CONTROL OPERATIONS   */
/* *************************** */

/* turns the motor to EPOS_READY_TO_SWITCH_ON state */
void epos_shutdown(int id)
{
  PDEBUG("requesting shutdown\n");
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_WRITE_2_BYTE;	//0x2b
  message.content[1]= 0x40;
  message.content[2]= 0x60;
  message.content[3]= 0x00;
  message.content[4]= 0x06;
  message.content[5]= 0x00;
  message.content[6]= 0x00;
  message.content[7]= 0x00;
  can_send_message(&message);
}

/* turns the motor to SWITCH_ON_DISABLED state */
void epos_quick_stop(int id)
{
  PDEBUG("requesting disable voltage\n");
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_WRITE_2_BYTE;
  message.content[1]= 0x40;
  message.content[2]= 0x60;
  message.content[3]= 0x00;
  message.content[4]= 0x02;
  message.content[5]= 0x00;
  message.content[6]= 0x00;
  message.content[7]= 0x00;
  can_send_message(&message);
}

/* FAULT_RESET */
void epos_faultreset(int id)
{
  PDEBUG("requesting fault reset\n");
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_WRITE_2_BYTE;
  message.content[1]= 0x40;
  message.content[2]= 0x60;
  message.content[3]= 0x00;
  message.content[4]= 0xFF;
  message.content[5]= 0x00;
  message.content[6]= 0x00;
  message.content[7]= 0x00;
  can_send_message(&message);
}
/* turns the motor to SWITCH_ON state */
void epos_switch_on(int id)
{
  PDEBUG("requesting switch on\n");
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_WRITE_2_BYTE;
  message.content[1]= 0x40;
  message.content[2]= 0x60;
  message.content[3]= 0x00;
  message.content[4]= 0x07;
  message.content[5]= 0x00;
  message.content[6]= 0x00;
  message.content[7]= 0x00;
  can_send_message(&message);
}

/* enables the motor operation */
void epos_enable_operation(int id)
{
  PDEBUG("activating...\n");
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_WRITE_2_BYTE;
  message.content[1]= 0x40;
  message.content[2]= 0x60;
  message.content[3]= 0x00;
  message.content[4]= 0x0F;
  message.content[5]= 0x00;
  message.content[6]= 0x00;
  message.content[7]= 0x00;
  can_send_message(&message);
}

void epos_enable_quick_stop(int id)
{
  PDEBUG("requesting quick stop\n");
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_WRITE_2_BYTE;
  message.content[1]= 0x40;
  message.content[2]= 0x60;
  message.content[3]= 0x00;
  message.content[4]= 0x0F;
  message.content[5]= 0x01;
  message.content[6]= 0x00;
  message.content[7]= 0x00;
  can_send_message(&message);
}

/* activate new velocity demand */
void epos_activate(int id)
{
  epos_enable_operation(id);
}

/* activate new position demand */
void epos_activate_position(int id)
/* sending control word suiting position mode
   bit 4: Assume target position
   bit 5: execute now
   bit 6: 0:abs value */
{
  PDEBUG("activate position demand\n ");
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_WRITE_2_BYTE;
  message.content[1]= 0x40;
  message.content[2]= 0x60;
  message.content[3]= 0x00;
  message.content[4]= 0x3f;
  message.content[5]= 0x00;
  message.content[6]= 0x00;
  message.content[7]= 0x00;
  can_send_message(&message);
}

/* turns the motor to SWITCH_ON_DISABLED state */
void epos_disable_voltage(int id)
{
  PDEBUG("requesting disable voltage\n");
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_WRITE_2_BYTE;
  message.content[1]= 0x40;
  message.content[2]= 0x60;
  message.content[3]= 0x00;
  message.content[4]= 0x00;
  message.content[5]= 0x00;
  message.content[6]= 0x00;
  message.content[7]= 0x00;
  can_send_message(&message);
}

/* turns the motor to SWITCH_ON_DISABLED state */
void epos_fault_reset(int id)
{
  PDEBUG("requesting fault reset\n");
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_WRITE_2_BYTE;
  message.content[1]= 0x40;
  message.content[2]= 0x60;
  message.content[3]= 0x00;
  message.content[4]= 0x80;
  message.content[5]= 0x00;
  message.content[6]= 0x00;
  message.content[7]= 0x00;
  can_send_message(&message);
}

void epos_start_homing_operation(int id)
{
  PDEBUG("start homing operation...\n");
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_WRITE_2_BYTE;
  message.content[1]= 0x40;
  message.content[2]= 0x60;
  message.content[3]= 0x00;
  message.content[4]= 0x1F;
  message.content[5]= 0x00;
  message.content[6]= 0x00;
  message.content[7]= 0x00;
  can_send_message(&message);
}

/* *************************** */
/* SET OPERATIONS              */
/* *************************** */

/* sets the control mode */
void epos_set_mode_of_operation(int id, int mode)
{
  PDEBUG("set mode of operation to 0x%x\n",mode);
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_WRITE_1_BYTE;
  message.content[1]= 0x60;
  message.content[2]= 0x60;
  message.content[3]= 0x00;
  message.content[4]= mode;
  message.content[5]= 0x00;
  message.content[6]= 0x00;
  message.content[7]= 0x00;
  can_send_message(&message);
}

void epos_set_target_velocity(int id, long int v)
{
  //PDEBUG("[libepos] set target velocity to ");
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_WRITE_4_BYTE;
  message.content[1]= 0xff;
  message.content[2]= 0x60;
  message.content[3]= 0x00;
  message.content[4]= (v & 0x000000ff);
  message.content[5]= ((v & 0x0000ff00)>>8);
  message.content[6]= ((v & 0x00ff0000)>>16);
  message.content[7]= ((v & 0xff000000)>>24);
  //PDEBUG_SNIP("%x (%i)\n",v,v);
  can_send_message(&message);
}

void epos_set_target_position(int id, long int x)
{
  //PDEBUG("set target position to ");
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_WRITE_4_BYTE;
  message.content[1]= 0x7a;
  message.content[2]= 0x60;
  message.content[3]= 0x00;
  message.content[4]= (x & 0x000000ff);
  message.content[5]= ((x & 0x0000ff00)>>8);
  message.content[6]= ((x & 0x00ff0000)>>16);
  message.content[7]= ((x & 0xff000000)>>24);
  //PDEBUG_SNIP("%d\n",x);
  can_send_message(&message);
}

void epos_set_profile_velocity(int id,long int v)
{
  PDEBUG("set profile velocity to ");
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_WRITE_4_BYTE;
  message.content[1]= 0x81;
  message.content[2]= 0x60;
  message.content[3]= 0x00;
  message.content[4]= (v & 0x000000ff);
  message.content[5]= ((v & 0x0000ff00)>>8);
  message.content[6]= ((v & 0x00ff0000)>>16);
  message.content[7]= ((v & 0xff000000)>>24);
  PDEBUG_SNIP("%ld\n",v);
  can_send_message(&message);
}

void epos_set_profile_acceleration(int id,long int a)
{
  PDEBUG("set profile acceleration to ");
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_WRITE_4_BYTE;
  message.content[1]= 0x83;
  message.content[2]= 0x60;
  message.content[3]= 0x00;
  message.content[4]= (a & 0x000000ff);
  message.content[5]= ((a & 0x0000ff00)>>8);
  message.content[6]= ((a & 0x00ff0000)>>16);
  message.content[7]= ((a & 0xff000000)>>24);
  PDEBUG_SNIP("%lx\n",a);
  can_send_message(&message);
}

void epos_set_profile_deceleration(int id, long int a)
{
  PDEBUG("set profile deceleration to ");
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_WRITE_4_BYTE;
  message.content[1]= 0x84;
  message.content[2]= 0x60;
  message.content[3]= 0x00;
  message.content[4]= (a & 0x000000ff);
  message.content[5]= ((a & 0x0000ff00)>>8);
  message.content[6]= ((a & 0x00ff0000)>>16);
  message.content[7]= ((a & 0xff000000)>>24);
  PDEBUG_SNIP("%lx\n",a);
  can_send_message(&message);
}

void epos_set_position_window(int id, unsigned int a)
{
  PDEBUG("set position window to ");
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_WRITE_4_BYTE;
  message.content[1]= 0x67;
  message.content[2]= 0x60;
  message.content[3]= 0x00;
  message.content[4]= (a & 0x000000ff);
  message.content[5]= ((a & 0x0000ff00)>>8);
  message.content[6]= ((a & 0x00ff0000)>>16);
  message.content[7]= ((a & 0xff000000)>>24);
  PDEBUG_SNIP("%x\n",a);
  can_send_message(&message);
}

void epos_set_position_window_time(int id,long int a)
{
  PDEBUG("set position window time to ");
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_WRITE_2_BYTE;
  message.content[1]= 0x68;
  message.content[2]= 0x60;
  message.content[3]= 0x00;
  message.content[4]= (a & 0x000000ff);
  message.content[5]= ((a & 0x0000ff00)>>8);
  message.content[6]= 0;
  message.content[7]= 0;
  PDEBUG_SNIP("%lx\n",a);
  can_send_message(&message);
}

void epos_set_maximum_following_error(int id, unsigned int a)
{
  PDEBUG("set maximum following error to ");
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_WRITE_4_BYTE;
  message.content[1]= 0x65;
  message.content[2]= 0x60;
  message.content[3]= 0x00;
  message.content[4]= (a & 0x000000ff);
  message.content[5]= ((a & 0x0000ff00)>>8);
  message.content[6]= ((a & 0x00ff0000)>>16);
  message.content[7]= ((a & 0xff000000)>>24);
  PDEBUG_SNIP("%x\n",a);
  can_send_message(&message);
}

void epos_set_motion_profile_type(int id, long int a)
{
  PDEBUG("set motion profile type to ");
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_WRITE_2_BYTE;
  message.content[1]= 0x86;
  message.content[2]= 0x60;
  message.content[3]= 0x00;
  message.content[4]= (a & 0x000000ff);
  message.content[5]= ((a & 0x0000ff00)>>8);
  message.content[6]= 0;
  message.content[7]= 0;
  PDEBUG_SNIP("%lx\n",a);
  can_send_message(&message);
}

void epos_set_motor_data(int id, epos_motor_data_t motor_data)
{
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_WRITE_2_BYTE;
  message.content[1]= 0x10;
  message.content[2]= 0x64;
  message.content[3]= 0x01;
  message.content[4]= (motor_data.continuous_current_limit & 0x000000ff);
  message.content[5]= ((motor_data.continuous_current_limit & 0x0000ff00)>>8);
  message.content[6]= 0x00;
  message.content[7]= 0x00;
  can_send_message(&message);
  can_read_message();

  message.content[0]= EPOS_WRITE_2_BYTE;
  message.content[1]= 0x10;
  message.content[2]= 0x64;
  message.content[3]= 0x02;
  message.content[4]= (motor_data.output_current_limit & 0x000000ff);
  message.content[5]= ((motor_data.output_current_limit & 0x0000ff00)>>8);
  message.content[6]= 0x00;
  message.content[7]= 0x00;
  can_send_message(&message);
  can_read_message();

  message.content[0]= EPOS_WRITE_1_BYTE;
  message.content[1]= 0x10;
  message.content[2]= 0x64;
  message.content[3]= 0x03;
  message.content[4]= (motor_data.pole_pair_number & 0x000000ff);
  message.content[5]= 0x00;
  message.content[6]= 0x00;
  message.content[7]= 0x00;
  can_send_message(&message);
  can_read_message();

  message.content[0]= EPOS_WRITE_2_BYTE;
  message.content[1]= 0x10;
  message.content[2]= 0x64;
  message.content[3]= 0x04;
  message.content[4]= (motor_data.max_speed_in_current_mode & 0x000000ff);
  message.content[5]= ((motor_data.max_speed_in_current_mode & 0x0000ff00)>>8);
  message.content[6]= 0x00;
  message.content[7]= 0x00;
  can_send_message(&message);
  can_read_message();

  message.content[0]= EPOS_WRITE_2_BYTE;
  message.content[1]= 0x10;
  message.content[2]= 0x64;
  message.content[3]= 0x04;
  message.content[4]= (motor_data.thermal_time_constant_winding & 0x000000ff);
  message.content[5]= ((motor_data.thermal_time_constant_winding & 0x0000ff00)>>8);
  message.content[6]= 0x00;
  message.content[7]= 0x00;
  can_send_message(&message);

}

void epos_set_position_control_parameter_set(int id, long int p_gain,
  long int i_gain, long int d_gain, long int v_feedforward,
  long int a_feedforward)
{
  PDEBUG("set position control parameter set\n");
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_WRITE_2_BYTE;
  message.content[1]= 0xFB;
  message.content[2]= 0x60;
  message.content[3]= 0x01;
  message.content[4]= (p_gain & 0x000000ff);
  message.content[5]= ((p_gain & 0x0000ff00)>>8);
  message.content[6]= 0x00;
  message.content[7]= 0x00;
  can_send_message(&message);
  PDEBUG("p_gain: %lx\n",p_gain);
  can_read_message();

  message.content[0]= EPOS_WRITE_2_BYTE;
  message.content[1]= 0xFB;
  message.content[2]= 0x60;
  message.content[3]= 0x02;
  message.content[4]= (i_gain & 0x000000ff);
  message.content[5]= ((i_gain & 0x0000ff00)>>8);
  message.content[6]= 0x00;
  message.content[7]= 0x00;
  can_send_message(&message);
  PDEBUG("i_gain: %lx\n",i_gain);
  can_read_message();

  message.content[0]= EPOS_WRITE_2_BYTE;
  message.content[1]= 0xFB;
  message.content[2]= 0x60;
  message.content[3]= 0x03;
  message.content[4]= (d_gain & 0x000000ff);
  message.content[5]= ((d_gain & 0x0000ff00)>>8);
  message.content[6]= 0x00;
  message.content[7]= 0x00;
  can_send_message(&message);
  PDEBUG("d_gain %lx\n",d_gain);
  can_read_message();

  message.content[0]= EPOS_WRITE_2_BYTE;
  message.content[1]= 0xFB;
  message.content[2]= 0x60;
  message.content[3]= 0x04;
  message.content[4]= (v_feedforward & 0x000000ff);
  message.content[5]= ((v_feedforward & 0x0000ff00)>>8);
  message.content[6]= 0x00;
  message.content[7]= 0x00;
  can_send_message(&message);
  PDEBUG("v_feedforward %lx\n",v_feedforward);
  can_read_message();

  message.content[0]= EPOS_WRITE_2_BYTE;
  message.content[1]= 0xFB;
  message.content[2]= 0x60;
  message.content[3]= 0x05;
  message.content[4]= (a_feedforward & 0x000000ff);
  message.content[5]= ((a_feedforward & 0x0000ff00)>>8);
  message.content[6]= 0x00;
  message.content[7]= 0x00;
  can_send_message(&message);
  PDEBUG("a_feedforward %lx\n",a_feedforward);

}

void epos_set_velocity_control_parameter_set(int id, long int p_gain,
  long int i_gain)
{
  PDEBUG("set velocity control parameter set\n");
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_WRITE_2_BYTE;
  message.content[1]= 0xF9;
  message.content[2]= 0x60;
  message.content[3]= 0x01;
  message.content[4]= (p_gain & 0x000000ff);
  message.content[5]= ((p_gain & 0x0000ff00)>>8);
  message.content[6]= 0x00;
  message.content[7]= 0x00;
  can_send_message(&message);
  PDEBUG("p_gain: %lx\n",p_gain);
  can_read_message();

  message.content[0]= EPOS_WRITE_2_BYTE;
  message.content[1]= 0xF9;
  message.content[2]= 0x60;
  message.content[3]= 0x02;
  message.content[4]= (i_gain & 0x000000ff);
  message.content[5]= ((i_gain & 0x0000ff00)>>8);
  message.content[6]= 0x00;
  message.content[7]= 0x00;
  can_send_message(&message);
  PDEBUG("i_gain: %lx\n",i_gain);
  can_read_message();
}

void epos_set_maximal_profile_velocity(int id, long int v)
{
  //PDEBUG("[libepos] set maximal profile velocity to ");
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_WRITE_4_BYTE;
  message.content[1]= 0x7f;
  message.content[2]= 0x60;
  message.content[3]= 0x00;
  message.content[4]= (v & 0x000000ff);
  message.content[5]= ((v & 0x0000ff00)>>8);
  message.content[6]= ((v & 0x00ff0000)>>16);
  message.content[7]= ((v & 0xff000000)>>24);
  //PDEBUG_SNIP("%lx (%i)\n",v,v);
  can_send_message(&message);
}

void epos_set_quick_stop_deceleration(int id, long int v)
{
  PDEBUG("set quick strop deceleration to ");
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_WRITE_4_BYTE;
  message.content[1]= 0x85;
  message.content[2]= 0x60;
  message.content[3]= 0x00;
  message.content[4]= (v & 0x000000ff);
  message.content[5]= ((v & 0x0000ff00)>>8);
  message.content[6]= ((v & 0x00ff0000)>>16);
  message.content[7]= ((v & 0xff000000)>>24);
  PDEBUG_SNIP("%lx\n",v);
  can_send_message(&message);
}

void epos_set_current_value(int id, short current)
{
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_WRITE_2_BYTE;
  message.content[1]= 0x30;
  message.content[2]= 0x20;
  message.content[3]= 0x00;
  message.content[4]= (current & 0x00ff);
  message.content[5]= ((current & 0xff00)>>8);
  message.content[6]= 0x00;
  message.content[7]= 0x00;
  can_send_message(&message);
}

void epos_set_velocity_mode_setting_value(int id, long int v)
{
  PDEBUG("set velocity mode setting value to ");
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_WRITE_4_BYTE;
  message.content[1]= 0x6B;
  message.content[2]= 0x20;
  message.content[3]= 0x00;
  message.content[4]= (v & 0x000000ff);
  message.content[5]= ((v & 0x0000ff00)>>8);
  message.content[6]= ((v & 0x00ff0000)>>16);
  message.content[7]= ((v & 0xff000000)>>24);
  PDEBUG_SNIP("%d\n",v);
  can_send_message(&message);
}

void epos_set_position_mode_setting_value(int id, long int x)
{
  PDEBUG("set position mode setting value to ");
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_WRITE_4_BYTE;
  message.content[1]= 0x62;
  message.content[2]= 0x20;
  message.content[3]= 0x00;
  message.content[4]= (x & 0x000000ff);
  message.content[5]= ((x & 0x0000ff00)>>8);
  message.content[6]= ((x & 0x00ff0000)>>16);
  message.content[7]= ((x & 0xff000000)>>24);
  PDEBUG_SNIP("%ld\n",x);
  can_send_message(&message);
}

void epos_set_controlword(int id, int val)
{
  PDEBUG("set controlword to ");
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_WRITE_2_BYTE;
  message.content[1]= 0x40;
  message.content[2]= 0x60;
  message.content[3]= 0x00;
  message.content[4]= (val & 0x000000ff);
  message.content[5]= ((val & 0x0000ff00)>>8);
  message.content[6]= 0;
  message.content[7]= 0;
  PDEBUG_SNIP("%x\n",val);
  can_send_message(&message);
}

void epos_set_home_position(int id, long int x)
{
  PDEBUG("set home position to ");
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_WRITE_4_BYTE;
  message.content[1]= 0x81;
  message.content[2]= 0x20;
  message.content[3]= 0x00;
  message.content[4]= (x & 0x000000ff);
  message.content[5]= ((x & 0x0000ff00)>>8);
  message.content[6]= ((x & 0x00ff0000)>>16);
  message.content[7]= ((x & 0xff000000)>>24);
  PDEBUG_SNIP("%ld\n",x);
  can_send_message(&message);
}

void epos_set_home_offset(int id, long int x)
{
  PDEBUG("set home offset position to ");
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_WRITE_4_BYTE;
  message.content[1]= 0x7C;
  message.content[2]= 0x60;
  message.content[3]= 0x00;
  message.content[4]= (x & 0x000000ff);
  message.content[5]= ((x & 0x0000ff00)>>8);
  message.content[6]= ((x & 0x00ff0000)>>16);
  message.content[7]= ((x & 0xff000000)>>24);
  PDEBUG_SNIP("%ld\n",x);
  can_send_message(&message);
}

void epos_set_homing_speed_switch_search(int id, long int v)
{
  PDEBUG("set homing speed switch search to ");
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_WRITE_4_BYTE;
  message.content[1]= 0x99;
  message.content[2]= 0x60;
  message.content[3]= 0x01;
  message.content[4]= (v & 0x000000ff);
  message.content[5]= ((v & 0x0000ff00)>>8);
  message.content[6]= ((v & 0x00ff0000)>>16);
  message.content[7]= ((v & 0xff000000)>>24);
  PDEBUG_SNIP("%ld\n",v);
  can_send_message(&message);
}

void epos_set_homing_speed_zero_search(int id, long int v)
{
  PDEBUG("set homing speed zero search to ");
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_WRITE_4_BYTE;
  message.content[1]= 0x99;
  message.content[2]= 0x60;
  message.content[3]= 0x02;
  message.content[4]= (v & 0x000000ff);
  message.content[5]= ((v & 0x0000ff00)>>8);
  message.content[6]= ((v & 0x00ff0000)>>16);
  message.content[7]= ((v & 0xff000000)>>24);
  PDEBUG_SNIP("%ld\n",v);
  can_send_message(&message);
}

void epos_set_homing_method(int id, int method)
{
  PDEBUG("set homing method to ");
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_WRITE_1_BYTE;
  message.content[1]= 0x98;
  message.content[2]= 0x60;
  message.content[3]= 0x00;
  message.content[4]= method;
  message.content[5]= 0x00;
  message.content[6]= 0x00;
  message.content[7]= 0x00;
  PDEBUG_SNIP("%d\n",method);
  can_send_message(&message);
}

void epos_set_software_minimal_position_limit(int id, long int x)
{
  PDEBUG("set software minimal position limit to ");
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_WRITE_4_BYTE;
  message.content[1]= 0x7D;
  message.content[2]= 0x60;
  message.content[3]= 0x01;
  message.content[4]= (x & 0x000000ff);
  message.content[5]= ((x & 0x0000ff00)>>8);
  message.content[6]= ((x & 0x00ff0000)>>16);
  message.content[7]= ((x & 0xff000000)>>24);
  PDEBUG_SNIP("%ld\n",x);
  can_send_message(&message);
}

void epos_set_software_maximal_position_limit(int id, long int x)
{
  PDEBUG("set software maximal position limit to ");
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_WRITE_4_BYTE;
  message.content[1]= 0x7D;
  message.content[2]= 0x60;
  message.content[3]= 0x02;
  message.content[4]= (x & 0x000000ff);
  message.content[5]= ((x & 0x0000ff00)>>8);
  message.content[6]= ((x & 0x00ff0000)>>16);
  message.content[7]= ((x & 0xff000000)>>24);
  PDEBUG_SNIP("%ld\n",x);
  can_send_message(&message);
}

void epos_set_continous_current_limit(int id, int i)
{
  PDEBUG("set continous current limit to ");
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_WRITE_2_BYTE;
  message.content[1]= 0x10;
  message.content[2]= 0x64;
  message.content[3]= 0x01;
  message.content[4]= (i & 0x000000ff);
  message.content[5]= ((i & 0x0000ff00)>>8);
  message.content[6]= 0;
  message.content[7]= 0;
  PDEBUG_SNIP("%x\n",val);
  can_send_message(&message);
}

void epos_set_output_current_limit(int id, int i)
{
  PDEBUG("set output current limit to ");
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_WRITE_2_BYTE;
  message.content[1]= 0x10;
  message.content[2]= 0x64;
  message.content[3]= 0x02;
  message.content[4]= (i & 0x000000ff);
  message.content[5]= ((i & 0x0000ff00)>>8);
  message.content[6]= 0;
  message.content[7]= 0;
  PDEBUG_SNIP("%x\n",i);
  can_send_message(&message);
}

void epos_set_homing_current_threshold(int id, int i)
{
  PDEBUG("set homing current threshold to ");
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_WRITE_2_BYTE;
  message.content[1]= 0x80;
  message.content[2]= 0x20;
  message.content[3]= 0x00;
  message.content[4]= (i & 0x000000ff);
  message.content[5]= ((i & 0x0000ff00)>>8);
  message.content[6]= 0;
  message.content[7]= 0;
  PDEBUG_SNIP("%x\n",i);
  can_send_message(&message);
}

void epos_set_RS232_baudrate(int id, int val)
{
  PDEBUG("set RS232 baudrate to no. ");
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_WRITE_2_BYTE;
  message.content[1]= 0x02;
  message.content[2]= 0x20;
  message.content[3]= 0x00;
  message.content[4]= (val & 0x000000ff);
  message.content[5]= ((val & 0x0000ff00)>>8);
  message.content[6]= 0;
  message.content[7]= 0;
  PDEBUG_SNIP("%x\n",val);
  can_send_message(&message);
}

/* *************************** */
/* EPOS_READ OPERATIONS             */
/* *************************** */

/* gets the control mode */
void epos_get_mode_of_operation(int id)
{
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_READ;
  message.content[1]= 0x60;
  message.content[2]= 0x60;
  message.content[3]= 0x00;
  message.content[4]= 0x00;
  message.content[5]= 0x00;
  message.content[6]= 0x00;
  message.content[7]= 0x00;
  can_send_message(&message);
}

void epos_get_velocity_control_parameter_set(int id)
{
  PDEBUG("ask for velocity control parameter set\n");
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_READ;
  message.content[1]= 0xF9;
  message.content[2]= 0x60;
  message.content[3]= 0x01;
  message.content[4]= 0x00;
  message.content[5]= 0x00;
  message.content[6]= 0x00;
  message.content[7]= 0x00;
  can_send_message(&message);
  can_read_message();

  message.content[0]= EPOS_READ;
  message.content[1]= 0xF9;
  message.content[2]= 0x60;
  message.content[3]= 0x02;
  message.content[4]= 0x00;
  message.content[5]= 0x00;
  message.content[6]= 0x00;
  message.content[7]= 0x00;
  can_send_message(&message);
}

void epos_get_actual_position(int id)
{
  //PDEBUG("ask for actual position\n");
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_READ;
  message.content[1]= 0x64;
  message.content[2]= 0x60;
  message.content[3]= 0x00;
  message.content[4]= 0x00;
  message.content[5]= 0x00;
  message.content[6]= 0x00;
  message.content[7]= 0x00;
  can_send_message(&message);
}

void epos_get_actual_velocity(int id)
{
  //PDEBUG("ask for actual velocity\n");
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_READ;
  message.content[1]= 0x6C;
  message.content[2]= 0x60;
  message.content[3]= 0x00;
  message.content[4]= 0x00;
  message.content[5]= 0x00;
  message.content[6]= 0x00;
  message.content[7]= 0x00;
  can_send_message(&message);
}

void epos_get_actual_velocity_avg(int id)
{
  //PDEBUG("ask for averaged velocity\n");
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_READ;
  message.content[1]= 0x28;
  message.content[2]= 0x20;
  message.content[3]= 0x00;
  message.content[4]= 0x00;
  message.content[5]= 0x00;
  message.content[6]= 0x00;
  message.content[7]= 0x00;
  can_send_message(&message);
}

void epos_get_maximum_following_error(int id)
{
  PDEBUG("ask maximum following error\n");
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_READ;
  message.content[1]= 0x65;
  message.content[2]= 0x60;
  message.content[3]= 0x00;
  message.content[4]= 0x00;
  message.content[5]= 0x00;
  message.content[6]= 0x00;
  message.content[7]= 0x00;
  can_send_message(&message);
}

void epos_get_motion_profile_type(int id)
{
  //PDEBUG("ask for motion profile type\n");
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_READ;
  message.content[1]= 0x86;
  message.content[2]= 0x60;
  message.content[3]= 0x00;
  message.content[4]= 0x00;
  message.content[5]= 0x00;
  message.content[6]= 0x00;
  message.content[7]= 0x00;
  can_send_message(&message);
}

void epos_get_motor_data(int id)
{
  //PDEBUG("ask for motor data \n");
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_READ;
  message.content[1]= 0x10;
  message.content[2]= 0x64;
  message.content[3]= 0x01;
  message.content[4]= 0x00;
  message.content[5]= 0x00;
  message.content[6]= 0x00;
  message.content[7]= 0x00;
  can_send_message(&message);
  can_read_message();

  message.content[0]= EPOS_READ;
  message.content[1]= 0x10;
  message.content[2]= 0x64;
  message.content[3]= 0x02;
  message.content[4]= 0x00;
  message.content[5]= 0x00;
  message.content[6]= 0x00;
  message.content[7]= 0x00;
  can_send_message(&message);
  can_read_message();

  message.content[0]= EPOS_READ;
  message.content[1]= 0x10;
  message.content[2]= 0x64;
  message.content[3]= 0x03;
  message.content[4]= 0x00;
  message.content[5]= 0x00;
  message.content[6]= 0x00;
  message.content[7]= 0x00;
  can_send_message(&message);
  can_read_message();

  message.content[0]= EPOS_READ;
  message.content[1]= 0x10;
  message.content[2]= 0x64;
  message.content[3]= 0x04;
  message.content[4]= 0x00;
  message.content[5]= 0x00;
  message.content[6]= 0x00;
  message.content[7]= 0x00;
  can_send_message(&message);
  can_read_message();

  message.content[0]= EPOS_READ;
  message.content[1]= 0x10;
  message.content[2]= 0x64;
  message.content[3]= 0x05;
  message.content[4]= 0x00;
  message.content[5]= 0x00;
  message.content[6]= 0x00;
  message.content[7]= 0x00;
  can_send_message(&message);
}

void epos_get_position_control_parameter_set(int id)
{
  //PDEBUG("ask for position control parameter set\n");
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_READ;
  message.content[1]= 0xFB;
  message.content[2]= 0x60;
  message.content[3]= 0x01;
  message.content[4]= 0x00;
  message.content[5]= 0x00;
  message.content[6]= 0x00;
  message.content[7]= 0x00;
  can_send_message(&message);
  can_read_message();

  message.content[0]= EPOS_READ;
  message.content[1]= 0xFB;
  message.content[2]= 0x60;
  message.content[3]= 0x02;
  message.content[4]= 0x00;
  message.content[5]= 0x00;
  message.content[6]= 0x00;
  message.content[7]= 0x00;
  can_send_message(&message);
  can_read_message();

  message.content[0]= EPOS_READ;
  message.content[1]= 0xFB;
  message.content[2]= 0x60;
  message.content[3]= 0x03;
  message.content[4]= 0x00;
  message.content[5]= 0x00;
  message.content[6]= 0x00;
  message.content[7]= 0x00;
  can_send_message(&message);
  can_read_message();

  message.content[0]= EPOS_READ;
  message.content[1]= 0xFB;
  message.content[2]= 0x60;
  message.content[3]= 0x04;
  message.content[4]= 0x00;
  message.content[5]= 0x00;
  message.content[6]= 0x00;
  message.content[7]= 0x00;
  can_send_message(&message);
  can_read_message();

  message.content[0]= EPOS_READ;
  message.content[1]= 0xFB;
  message.content[2]= 0x60;
  message.content[3]= 0x05;
  message.content[4]= 0x00;
  message.content[5]= 0x00;
  message.content[6]= 0x00;
  message.content[7]= 0x00;
  can_send_message(&message);

}

void epos_get_current_actual_value(int id)
{
  //PDEBUG("ask for actual current value (%i)\n", id);
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_READ;
  message.content[1]= 0x78;
  message.content[2]= 0x60;
  message.content[3]= 0x00;
  message.content[4]= 0x00;
  message.content[5]= 0x00;
  message.content[6]= 0x00;
  message.content[7]= 0x00;
  can_send_message(&message);
}


void epos_get_current_actual_value_averaged(int id)
{
  //PDEBUG("ask for actual current value averaged\n");
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_READ;
  message.content[1]= 0x27;
  message.content[2]= 0x20;
  message.content[3]= 0x00;
  message.content[4]= 0x00;
  message.content[5]= 0x00;
  message.content[6]= 0x00;
  message.content[7]= 0x00;
  can_send_message(&message);
}

void epos_get_mode_of_operation_display(int id)
{
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_READ;
  message.content[1]= 0x61;
  message.content[2]= 0x60;
  message.content[3]= 0x00;
  message.content[4]= 0x00;
  message.content[5]= 0x00;
  message.content[6]= 0x00;
  message.content[7]= 0x00;
  can_send_message(&message);
}

void epos_get_current_value(int id)
{
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_READ;
  message.content[1]= 0x30;
  message.content[2]= 0x20;
  message.content[3]= 0x00;
  message.content[4]= 0x00;
  message.content[5]= 0x00;
  message.content[6]= 0x00;
  message.content[7]= 0x00;
  can_send_message(&message);
}

void epos_get_maximum_profile_velocity(int id)
{
  //PDEBUG("ask for actual position\n");
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_READ;
  message.content[1]= 0x7F;
  message.content[2]= 0x60;
  message.content[3]= 0x00;
  message.content[4]= 0x00;
  message.content[5]= 0x00;
  message.content[6]= 0x00;
  message.content[7]= 0x00;
  can_send_message(&message);
}

void epos_get_velocity_mode_setting_value(int id)
{
  PDEBUG("ask for actual velocity mode setting value\n");
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_READ;
  message.content[1]= 0x6B;
  message.content[2]= 0x20;
  message.content[3]= 0x00;
  message.content[4]= 0x00;
  message.content[5]= 0x00;
  message.content[6]= 0x00;
  message.content[7]= 0x00;
  can_send_message(&message);
}

void epos_get_position_mode_setting_value(int id)
{
  PDEBUG("ask for actual position mode setting value\n");
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_READ;
  message.content[1]= 0x62;
  message.content[2]= 0x20;
  message.content[3]= 0x00;
  message.content[4]= 0x00;
  message.content[5]= 0x00;
  message.content[6]= 0x00;
  message.content[7]= 0x00;
  can_send_message(&message);
}

void epos_get_digital_input(int id)
{
  PDEBUG("ask for digital input functionalities\n");
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_READ;
  message.content[1]= 0x71;
  message.content[2]= 0x20;
  message.content[3]= 0x01;
  message.content[4]= 0x00;
  message.content[5]= 0x00;
  message.content[6]= 0x00;
  message.content[7]= 0x00;
  can_send_message(&message);
}

void epos_get_statusword(int id)
{
  PDEBUG("ask for actual statusword\n");
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_READ;
  message.content[1]= 0x41;
  message.content[2]= 0x60;
  message.content[3]= 0x00;
  message.content[4]= 0x00;
  message.content[5]= 0x00;
  message.content[6]= 0x00;
  message.content[7]= 0x00;
  can_send_message(&message);
}

void epos_get_controlword(int id)
{
  PDEBUG("ask for actual controlword\n");
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_READ;
  message.content[1]= 0x40;
  message.content[2]= 0x60;
  message.content[3]= 0x00;
  message.content[4]= 0x00;
  message.content[5]= 0x00;
  message.content[6]= 0x00;
  message.content[7]= 0x00;
  can_send_message(&message);
}

void epos_get_home_offset(int id)
{
  PDEBUG("ask for actual home offset\n");
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_READ;
  message.content[1]= 0x7C;
  message.content[2]= 0x60;
  message.content[3]= 0x00;
  message.content[4]= 0x00;
  message.content[5]= 0x00;
  message.content[6]= 0x00;
  message.content[7]= 0x00;
  can_send_message(&message);
}

void epos_get_homing_speed_switch_search(int id)
{
  PDEBUG("ask for homing speed switch search\n");
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_READ;
  message.content[1]= 0x99;
  message.content[2]= 0x60;
  message.content[3]= 0x01;
  message.content[4]= 0x00;
  message.content[5]= 0x00;
  message.content[6]= 0x00;
  message.content[7]= 0x00;
  can_send_message(&message);
}

void epos_get_homing_speed_zero_search(int id)
{
  PDEBUG("ask for homing speed zero search\n");
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_READ;
  message.content[1]= 0x99;
  message.content[2]= 0x60;
  message.content[3]= 0x02;
  message.content[4]= 0x00;
  message.content[5]= 0x00;
  message.content[6]= 0x00;
  message.content[7]= 0x00;
  can_send_message(&message);
}

void epos_get_homing_method(int id)
{
  PDEBUG("ask for homing method\n");
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_READ;
  message.content[1]= 0x98;
  message.content[2]= 0x60;
  message.content[3]= 0x00;
  message.content[4]= 0x00;
  message.content[5]= 0x00;
  message.content[6]= 0x00;
  message.content[7]= 0x00;
  can_send_message(&message);
}

void epos_get_software_version(int id)
{
  //PDEBUG("ask for actual software version\n");
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_READ;
  message.content[1]= 0x03;
  message.content[2]= 0x20;
  message.content[3]= 0x01;
  message.content[4]= 0x00;
  message.content[5]= 0x00;
  message.content[6]= 0x00;
  message.content[7]= 0x00;
  can_send_message(&message);
}

void epos_get_software_minimal_position_limit(int id)
{
  PDEBUG("ask for actual software minimal position limit\n");
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_READ;
  message.content[1]= 0x7D;
  message.content[2]= 0x60;
  message.content[3]= 0x01;
  message.content[4]= 0x00;
  message.content[5]= 0x00;
  message.content[6]= 0x00;
  message.content[7]= 0x00;
  can_send_message(&message);
}

void epos_get_software_maximal_position_limit(int id)
{
  PDEBUG("ask for actual software maximal position limit\n");
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_READ;
  message.content[1]= 0x7D;
  message.content[2]= 0x60;
  message.content[3]= 0x02;
  message.content[4]= 0x00;
  message.content[5]= 0x00;
  message.content[6]= 0x00;
  message.content[7]= 0x00;
  can_send_message(&message);
}

void epos_get_profile_acceleration(int id)
{
  PDEBUG("ask for actual profile acceleration\n");
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_READ;
  message.content[1]= 0x83;
  message.content[2]= 0x60;
  message.content[3]= 0x00;
  message.content[4]= 0x00;
  message.content[5]= 0x00;
  message.content[6]= 0x00;
  message.content[7]= 0x00;
  can_send_message(&message);
}

void epos_get_profile_deceleration(int id)
{
  PDEBUG("ask for actual profile deceleration\n");
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_READ;
  message.content[1]= 0x84;
  message.content[2]= 0x60;
  message.content[3]= 0x00;
  message.content[4]= 0x00;
  message.content[5]= 0x00;
  message.content[6]= 0x00;
  message.content[7]= 0x00;
  can_send_message(&message);
}

void epos_get_error_register(int id)
{
  PDEBUG("ask for actual error register\n");
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_READ;
  message.content[1]= 0x01;
  message.content[2]= 0x10;
  message.content[3]= 0x00;
  message.content[4]= 0x00;
  message.content[5]= 0x00;
  message.content[6]= 0x00;
  message.content[7]= 0x00;
  can_send_message(&message);
}

void epos_get_error_history(int id, int index)
{
  PDEBUG("ask for actual error history\n");
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_READ;
  message.content[1]= 0x03;
  message.content[2]= 0x10;
  message.content[3]= (index & 0x00ff);
  message.content[4]= 0x00;
  message.content[5]= 0x00;
  message.content[6]= 0x00;
  message.content[7]= 0x00;
  can_send_message(&message);
}

void epos_get_error(int id)
{
  char index;
  PDEBUG("ask for error\n");

  epos_get_error_register(id);

  for(index=0;index<6;index++) epos_get_error_history(id, index);

}

void epos_get_continous_current_limit(int id)
{
  PDEBUG("ask for actual continous current limit\n");
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_READ;
  message.content[1]= 0x10;
  message.content[2]= 0x64;
  message.content[3]= 0x01;
  message.content[4]= 0x00;
  message.content[5]= 0x00;
  message.content[6]= 0x00;
  message.content[7]= 0x00;
  can_send_message(&message);
}

void epos_get_output_current_limit(int id)
{
  PDEBUG("ask for actual output current limit\n");
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_READ;
  message.content[1]= 0x10;
  message.content[2]= 0x64;
  message.content[3]= 0x02;
  message.content[4]= 0x00;
  message.content[5]= 0x00;
  message.content[6]= 0x00;
  message.content[7]= 0x00;
  can_send_message(&message);
}

void epos_get_RS232_baudrate(int id)
{
  PDEBUG("ask for actual RS232 baudrate\n");
  can_message_t message;

  message.id = 0x600+id;
  message.content[0]= EPOS_READ;
  message.content[1]= 0x02;
  message.content[2]= 0x20;
  message.content[3]= 0x00;
  message.content[4]= 0x00;
  message.content[5]= 0x00;
  message.content[6]= 0x00;
  message.content[7]= 0x00;
  can_send_message(&message);
}

void can_read_message_handler(const can_message_t* message)
{
  int i=0;
  int errorcode;

  PDEBUG("mhm... something to read... ");
  PDEBUG_SNIP("id: %lx Data: ",message->id);
  PDEBUG_SNIP("%x ",message->content[0]);
  PDEBUG_SNIP("%x ",message->content[1]);
  PDEBUG_SNIP("%x ",message->content[2]);
  PDEBUG_SNIP("%x ",message->content[3]);
  PDEBUG_SNIP("%x ",message->content[4]);
  PDEBUG_SNIP("%x ",message->content[5]);
  PDEBUG_SNIP("%x ",message->content[6]);
  PDEBUG_SNIP("%x \n",message->content[7]);

  if ((message->id > 0x80) && (message->id < 0x100)) {
    //EMERGENCY object
    PDEBUG("ERROR ERROR ERROR ERROR ERROR ERROR ERROR ERROR\n");
    int errorfound=0;

    errorcode = (message->content[0]+(message->content[1]<<8));

    for (i = 0; i < EPOS_ERROR_HISTORY; i++) {
      if(errorcode == error_history[i].code)
      {
        printf("Node %d error 0x%04X: %s\n", message->id-0x80,
          error_history[i].code, error_history[i].msg);
        epos_read.node[message->id-0x80].error.device.history.code =
          error_history[i].code;
        epos_read.node[message->id-0x80].error.device.history.reg =
          error_history[i].reg;
        epos_read.node[message->id-0x80].error.device.history.msg =
          error_history[i].msg;
        errorfound = 1;
      }
    }
  }

  if (message->content[0] == 0x80)
  //We have an error message --> print for debug
  { //TODO: add id to be shure message is not for LSS
      PDEBUG("mhm... configuration error... ");
      PDEBUG_SNIP("id: %lx Data: ",message->id);
      PDEBUG_SNIP("%x ",message->content[0]);
      PDEBUG_SNIP("%x ",message->content[1]);
      PDEBUG_SNIP("%x ",message->content[2]);
      PDEBUG_SNIP("%x ",message->content[3]);
      PDEBUG_SNIP("%x ",message->content[4]);
      PDEBUG_SNIP("%x ",message->content[5]);
      PDEBUG_SNIP("%x ",message->content[6]);
      PDEBUG_SNIP("%x \n",message->content[7]);
  }
  else
  {

	if ((message->id >= 0x581) && (message->id <= (0x581+EPOS_NUM_NODES-1)))
	{

 	  if ((message->content[1]==0x7F)
	      && (message->content[2]==0x60)
	      && (message->content[3]==0x00))
	    {
	      //PDEBUG("[libepos] received maximum profile velocity value ");
	      //PDEBUG_SNIP("byte 0: %x value: ",message->content[0]);
	      epos_read.node[(message->id - 0x581)].max_profile_velocity
		=message->content[4]
		+(message->content[5]<<8)
		+(message->content[6]<<16)
		+(message->content[7]<<24);
	      //PDEBUG("0x%x (%i)\n",epos_read.node[(message->id - 0x581)].max_profile_velocity, epos_read.node[(message->id - 0x581)].max_profile_velocity);
	    }

	  if ((message->content[1]==0x64)
	      && (message->content[2]==0x60)
	      && (message->content[3]==0x00))
	    {
	      //PDEBUG("received position actual value ");
	      epos_read.node[(message->id - 0x581)].actual_position //epos_read.node[(message->id - 0x581)]
		=message->content[4]
		+(message->content[5]<<8)
		+(message->content[6]<<16)
		+(message->content[7]<<24);

	      //PDEBUG_SNIP("ox%x\n",epos_read.node[(message->id - 0x581)].actual_position);
	    }
	  if ((message->content[1]==0x69)
	      && (message->content[2]==0x60)
	      && (message->content[3]==0x00))
	    {
	      //PDEBUG("received velocity sensor actual value ");
	      epos_read.node[(message->id - 0x581)].sensed_velocity
		=message->content[4]
		+(message->content[5]<<8)
		+(message->content[6]<<16)
		+(message->content[7]<<24);
	      //PDEBUG_SNIP("0x%x\n",epos_read.node[(message->id - 0x581)].sensed_velocity);
	    }

	  if ((message->content[1]==0x6C)
	      && (message->content[2]==0x60)
	      && (message->content[3]==0x00))
	    {
	      //PDEBUG("received velocity actual value \n");
	      epos_read.node[(message->id - 0x581)].actual_velocity
		=message->content[4]
		+(message->content[5]<<8)
		+(message->content[6]<<16)
		+(message->content[7]<<24);
	    }
	  if ((message->content[1]==0x28)
	      && (message->content[2]==0x20)
	      && (message->content[3]==0x00))
	    {
	      //PDEBUG("received velocity averaged value ");
	      epos_read.node[(message->id - 0x581)].actual_velocity
		=message->content[4]
		+(message->content[5]<<8)
		+(message->content[6]<<16)
		+(message->content[7]<<24);
	      //PDEBUG_SNIP("0x%x\n",epos_read.node[(message->id - 0x581)].actual_velocity_avg);
	    }

	  if ((message->content[1]==0x67)
	      && (message->content[2]==0x60)
	      && (message->content[3]==0x00))
	    {
	      //PDEBUG("received position window value ");
	      //PDEBUG_SNIP("byte 0: %x value: ",message->content[0]);
	      epos_read.node[(message->id - 0x581)].position_window
		=message->content[4]
		+(message->content[5]<<8)
		+(message->content[6]<<16)
		+(message->content[7]<<24);
	      //PDEBUG_SNIP("0x%x\n",epos_read.node[(message->id - 0x581)].position_window);
	    }

	  if ((message->content[1]==0x65)
	      && (message->content[2]==0x60)
	      && (message->content[3]==0x00))
	    {
	      //PDEBUG("received maximum following error value ");
	      //PDEBUG_SNIP("byte 0: %x value: ",message->content[0]);
	      epos_read.node[(message->id - 0x581)].maximum_following_error
		=message->content[4]
		+(message->content[5]<<8)
		+(message->content[6]<<16)
		+(message->content[7]<<24);
	      //PDEBUG_SNIP("0x%x\n",epos_read.node[(message->id - 0x581)].maximum_following_error);
	    }
	  if ((message->content[1]==0x68)
	      && (message->content[2]==0x60)
	      && (message->content[3]==0x00))
	    {
	      //PDEBUG("received position window time value ");
	      //PDEBUG_SNIP("byte 0: %x value: ",message->content[0]);
	      epos_read.node[(message->id - 0x581)].position_window_time
		=message->content[4]
		+(message->content[5]<<8);
	      //PDEBUG_SNIP("0x%x\n",epos_read.node[(message->id - 0x581)].position_window_time);
	    }
	  if ((message->content[1]==0x86)
	      && (message->content[2]==0x60)
	      && (message->content[3]==0x00))
	    {
	      //PDEBUG("received motion profile type ");
	      //PDEBUG_SNIP("byte 0: %x value: ",message->content[0]);
	      epos_read.node[(message->id - 0x581)].motion_profile_type
		=message->content[4]
		+(message->content[5]<<8);
	      //PDEBUG_SNIP("0x%x\n",epos_read.node[(message->id - 0x581)].motion_profile_type);
	    }

 	  if ((message->content[1]==0x78)
	      && (message->content[2]==0x60)
	      && (message->content[3]==0x00))
	    {
		epos_read.node[(message->id - 0x581)].actual_current
		=message->content[4]
		+(message->content[5]<<8);
	    }

	  if ((message->content[1]==0x27)
	      && (message->content[2]==0x20)
	      && (message->content[3]==0x00))
	    {

		epos_read.node[(message->id - 0x581)].actual_current_avg
		=message->content[4]
		+(message->content[5]<<8);
	    }

	  if ((message->content[1]==0x60)
	      && (message->content[2]==0x60)
	      && (message->content[3]==0x00))
	    {
		epos_read.node[(message->id - 0x581)].operation_mode
		=message->content[4];
	    }

	  if ((message->content[1]==0x61)
	      && (message->content[2]==0x60)
	      && (message->content[3]==0x00))
	    {
		epos_read.node[(message->id - 0x581)].operation_mode_display
		=message->content[4];
	    }

          if ((message->content[1]==0x40)
              && (message->content[2]==0x60)
              && (message->content[3]==0x00))
            {
                epos_read.node[(message->id - 0x581)].control
                =message->content[4]
                +(message->content[5]<<8);
            }

          if ((message->content[1]==0x71)
              && (message->content[2]==0x20)
              && (message->content[3]==0x01))
            {
                epos_read.node[(message->id - 0x581)].digital_input
                =message->content[4]
                +(message->content[5]<<8);
            }

          if ((message->content[1]==0x41)
              && (message->content[2]==0x60)
              && (message->content[3]==0x00))
            {
                epos_read.node[(message->id - 0x581)].status
                =message->content[4]
                +(message->content[5]<<8);
            }

	  if ((message->content[1]==0x30)
	      && (message->content[2]==0x20)
	      && (message->content[3]==0x00))
	    {
		epos_read.node[(message->id - 0x581)].current_value
		=message->content[4]
		+(message->content[5]<<8);
	    }


	  if ((message->content[1]==0xFB)
	      && (message->content[2]==0x60))
	    {
	      //PDEBUG("received position config ");
	      if (message->content[3]==0x01)
		{
		  epos_read.node[(message->id - 0x581)].position_config.p_gain
		    =message->content[4]
		    +(message->content[5]<<8)
		    +(0<<16)
		    +(0<<24);
		  //PDEBUG_SNIP("0x%x\n",epos_read.node[(message->id - 0x581)].position_config.p_gain);
		}

	      if (message->content[3]==0x02)
		{
		  epos_read.node[(message->id - 0x581)].position_config.i_gain
		    =message->content[4]
		    +(message->content[5]<<8)
		    +(0<<16)
		    +(0<<24);
		  //PDEBUG_SNIP("0x%x\n",epos_read.node[(message->id - 0x581)].position_config.i_gain);
		}
	      if (message->content[3]==0x03)
		{
		  epos_read.node[(message->id - 0x581)].position_config.d_gain
		    =message->content[4]
		    +(message->content[5]<<8)
		    +(0<<16)
		    +(0<<24);
		  //PDEBUG_SNIP("0x%x\n",epos_read.node[(message->id - 0x581)].position_config.d_gain);
		}
	      if (message->content[3]==0x04)
		{
		  epos_read.node[(message->id - 0x581)].position_config.v_feedforward
		    =message->content[4]
		    +(message->content[5]<<8)
		    +(0<<16)
		    +(0<<24);
		  //PDEBUG_SNIP("0x%x\n",epos_read.node[(message->id - 0x581)].position_config.v_feedforward);
		}

	      if (message->content[3]==0x05)
		{
		  epos_read.node[(message->id - 0x581)].position_config.a_feedforward
		    =message->content[4]
		    +(message->content[5]<<8)
		    +(0<<16)
		    +(0<<24);
		  //PDEBUG_SNIP("0x%x\n",epos_read.node[(message->id - 0x581)].position_config.a_feedforward);
		}

	    }

	   if ((message->content[1]==0xF9)
	      && (message->content[2]==0x60))
	    {
	      //PDEBUG("received velocity config ");
	      if (message->content[3]==0x01)
		{
		  epos_read.node[(message->id - 0x581)].velocity_config.p_gain
		    =message->content[4]
		    +(message->content[5]<<8)
		    +(0<<16)
		    +(0<<24);
		  //PDEBUG_SNIP("0x%x\n",epos_read.node[(message->id - 0x581)].velocity_config.p_gain);
		}

	      if (message->content[3]==0x02)
		{
		  epos_read.node[(message->id - 0x581)].velocity_config.i_gain
		    =message->content[4]
		    +(message->content[5]<<8)
		    +(0<<16)
		    +(0<<24);
		  //PDEBUG_SNIP("0x%x\n",epos_read.node[(message->id - 0x581)].velocity_config.i_gain);
		}
	    }

	  if ((message->content[1]==0x10)
	      && (message->content[2]==0x64))
	    {
	      //PDEBUG("got motor data ");
	      if (message->content[3]==0x01)
		{
		  epos_read.node[(message->id - 0x581)].motor_data.continuous_current_limit
		    =message->content[4]
		    +(message->content[5]<<8)
		    +(0<<16)
		    +(0<<24);
		  //PDEBUG_SNIP("Continuous current limit: 0x%x\n",epos_read.node[(message->id - 0x581)].motor_data.continuous_current_limit);
		}
	      if (message->content[3]==0x02)
		{
		  epos_read.node[(message->id - 0x581)].motor_data.output_current_limit
		    =message->content[4]
		    +(message->content[5]<<8)
		    +(0<<16)
		    +(0<<24);
		  //PDEBUG_SNIP("Output current limit: 0x%x\n",epos_read.node[(message->id - 0x581)].motor_data.output_current_limit);
		}
	        if (message->content[3]==0x03)
		{
		  epos_read.node[(message->id - 0x581)].motor_data.pole_pair_number
		    =message->content[4]
		    +(0<<8)
		    +(0<<16)
		    +(0<<24);
		  //PDEBUG_SNIP("Pole Pair Number: 0x%x\n",epos_read.node[(message->id - 0x581)].motor_data.pole_pair_number);
		}
		if (message->content[3]==0x04)
		{
		  epos_read.node[(message->id - 0x581)].motor_data.max_speed_in_current_mode
		    =message->content[4]
		    +(message->content[5]<<8)
		    +(0<<16)
		    +(0<<24);
		  //PDEBUG_SNIP("Max speed in current mode: 0x%x\n",epos_read.node[(message->id - 0x581)].motor_data.max_speed_in_current_mode);
		}
		if (message->content[3]==0x05)
		{
		  epos_read.node[(message->id - 0x581)].motor_data.thermal_time_constant_winding
		    =message->content[4]
		    +(message->content[5]<<8)
		    +(0<<16)
		    +(0<<24);
		  //PDEBUG_SNIP("Thermal time constant winding: 0x%x\n",epos_read.node[(message->id - 0x581)].motor_data.thermal_time_constant_winding);
		}
	    }

		if ((message->content[1]==0x01)
	      	&& (message->content[2]==0x10)
			&& (message->content[3]==0x00))
	    {
	    	epos_read.node[(message->id - 0x581)].error.device.reg
		    =message->content[4];
		  	//PDEBUG_SNIP("0x%x\n",epos_read.node[(message->id - 0x581)].error.device.reg);
		}


		if ((message->content[1]==0x03)
	      && (message->content[2]==0x10))
	    {
	      PDEBUG("received error history ");
		  //printf("message->content[3]: %d\n", message->content[3]);
	      if (message->content[3]==0x00)
		  {
		    epos_read.node[(message->id - 0x581)].error.device.count
		    =message->content[4];
		  //PDEBUG_SNIP("0x%x\n",epos_read.node[(message->id - 0x581)].error.device.node);
		  }
	  	  else
		  {
			errorcode = ((message->content[4]
						+(message->content[5]<<8)
						+(message->content[6]<<16)
						+(message->content[7]<<24)) & 0x0000FFFF);

			//printf("errorcode: %d\n",errorcode);

			for(i=0;i<EPOS_ERROR_HISTORY;i++)
			{
			  if(errorcode == error_history[i].code)
			  {
                        printf("%s => Node %d error 0x%04X: %s\n",
                          __FILE__, message->id - 0x581, error_history[i].code,
                          error_history[i].msg);
		    	epos_read.node[(message->id - 0x581)].error.device.history.code =
                          error_history[i].code;
		    	epos_read.node[(message->id - 0x581)].error.device.history.reg =
                          error_history[i].reg;
		    	epos_read.node[(message->id - 0x581)].error.device.history.msg =
                          error_history[i].msg;
			  }

			}

		  }
	    }

	  }
    }
}



