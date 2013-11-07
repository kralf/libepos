/***************************************************************************
 *   Copyright (C) 2008 by Ralf Kaestner                                   *
 *   ralf.kaestner@gmail.com                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <stdio.h>
#include <string.h>

#include "epos.h"

#include "global.h"
#include "home.h"
#include "position.h"
#include "velocity.h"
#include "current.h"

const char* epos_errors[] = {
  "Success",
  "Configuration error",
  "Error opening EPOS",
  "Error closing EPOS",
  "Error homing EPOS",
};

config_param_t epos_default_params[] = {
  {EPOS_PARAMETER_DEVICE_NODE_ID,
    config_param_type_int,
    "0",
    "[0, 127]",
    "Node identifier of the connected EPOS device or zero for broadcast"},
  {EPOS_PARAMETER_DEVICE_RESET,
    config_param_type_bool,
    "false",
    "false|true",
    "Reset EPOS device on initialization"},
  {EPOS_PARAMETER_SENSOR_TYPE,
    config_param_type_enum,
    "3chan",
    "3chan|2chan|hall",
    "Type of position sensor, where 'Nchan' refers to an N-channel "
    "incremental encoder, and 'hall' indicates a Hall effect sensor"},
  {EPOS_PARAMETER_SENSOR_POLARITY,
    config_param_type_enum,
    "normal",
    "normal|inverted",
    "Polarity of the position sensor, where 'normal' means normal "
    "polarity, and 'inverted' indicates inverse polarity"},
  {EPOS_PARAMETER_SENSOR_PULSES,
    config_param_type_int,
    "1024",
    "[16, 2500000]",
    "Number of position sensor pulses per revolution in [ticks]"},
  {EPOS_PARAMETER_SENSOR_SUPERVISION,
    config_param_type_enum,
    "full",
    "full|hardware|software|none",
    "Type of position sensor supervision, indicating that bad working "
    "conditions will be detected by 'hardware', 'software', both for 'full', "
    "and not at all for 'none'"},
  {EPOS_PARAMETER_MOTOR_TYPE,
    config_param_type_enum,
    "dc",
    "dc|ec_sin|ec_block",
    "Type of the connected motor, where 'dc' refers to brushed DC motor, "
    "and 'ec_sin' or 'ec_block' indicate a sinus or block commutated EC "
    "motor, respectively"},
  {EPOS_PARAMETER_MOTOR_CURRENT,
    config_param_type_float,
    "2.0",
    "[0.0, inf)",
    "Maximum motor output current in [A]"},
  {EPOS_PARAMETER_GEAR_TRANSMISSION,
    config_param_type_float,
    "1.0",
    "(-inf, inf)",
    "Transmission ratio of the gears"},
  {EPOS_PARAMETER_CONTROL_MODE,
    config_param_type_enum,
    "homing",
    "homing|profile_velocity|profile_position|position|velocity|current|"
    "diagnostic|master_enc|step_dir",
    "Operation mode of the controller as documented by the EPOS firmware "
    "specification"},
  {EPOS_PARAMETER_HOME_METHOD,
    config_param_type_enum,
    "pos_current_index",
    "neg_switch_index|pos_switch_index|neg_switch|pos_switch|neg_limit_index|"
    "pos_limit_index|neg_limit|pos_limit|pos_current_index|neg_current_index|"
    "pos_current|neg_current|pos_index|neg_index",
    "Homing method applied by the controller as documented by the EPOS "
    "firmware specification"},
  {EPOS_PARAMETER_HOME_TYPE,
    config_param_type_enum,
    "linear",
    "linear|sinusoidal",
    "Type of motion profile used for homing, which may either correspond "
    "to 'linear' or 'sinusoidal' trajectories"},
  {EPOS_PARAMETER_HOME_CURRENT,
    config_param_type_float,
    "1.0",
    "[0.0, inf)",
    "Current threshold used for homing in [A]"},
  {EPOS_PARAMETER_HOME_VELOCITY,
    config_param_type_float,
    "10.0",
    "[0.0, inf)",
    "Switch and zero search velocity during homing in [rad/s]"},
  {EPOS_PARAMETER_HOME_ACCELERATION,
    config_param_type_float,
    "10.0",
    "[0.0, inf)",
    "Homing acceleration in [rad/s^2]"},
  {EPOS_PARAMETER_HOME_OFFSET,
    config_param_type_float,
    "0.0",
    "(-inf, inf)",
    "Home offset from the switch in [rad]"},
  {EPOS_PARAMETER_HOME_POSITION,
    config_param_type_float,
    "0.0",
    "(-inf, inf)",
    "Home position in [rad]"},
};

config_t epos_default_config = {
  epos_default_params,
  sizeof(epos_default_params)/sizeof(config_param_t),
};

void epos_init(epos_node_p node, can_device_p can_dev) {
  epos_init_config(node, can_dev, &epos_default_config);  
}

int epos_init_config(epos_node_p node, can_device_p can_dev, config_p config) {
  if (!can_dev) {
    can_dev = malloc(sizeof(can_device_t));
    can_init(can_dev);
  }

  config_init_copy(&node->config, &epos_default_config);
  if ((config != &epos_default_config) && config_set(&node->config, config))
    return EPOS_ERROR_CONFIG;

  epos_device_init(&node->dev, can_dev,
    config_get_int(&node->config, EPOS_PARAMETER_DEVICE_NODE_ID),
    config_get_bool(&node->config, EPOS_PARAMETER_DEVICE_RESET));
  epos_sensor_init(&node->sensor, &node->dev,
    config_get_int(&node->config, EPOS_PARAMETER_SENSOR_TYPE),
    config_get_int(&node->config, EPOS_PARAMETER_SENSOR_POLARITY),
    config_get_int(&node->config, EPOS_PARAMETER_SENSOR_PULSES),
    config_get_int(&node->config, EPOS_PARAMETER_SENSOR_SUPERVISION));
  epos_motor_init(&node->motor, &node->dev,
    config_get_int(&node->config, EPOS_PARAMETER_MOTOR_TYPE),
    config_get_float(&node->config, EPOS_PARAMETER_MOTOR_CURRENT));
  epos_gear_init(&node->gear, &node->sensor,
    config_get_float(&node->config, EPOS_PARAMETER_GEAR_TRANSMISSION));
  epos_input_init(&node->input, &node->dev);
  epos_control_init(&node->control, &node->dev,
    config_get_int(&node->config, EPOS_PARAMETER_CONTROL_MODE));
  
  return EPOS_ERROR_NONE;
}

int epos_init_config_parse(epos_node_p node, config_parser_p parser,
    const char* prefix, int argc, char **argv, config_parser_exit_t exit) {
  can_device_p can_dev = malloc(sizeof(can_device_t));
  
  config_p config = &config_parser_add_option_group(parser,
    &epos_default_config, prefix ? prefix : EPOS_ARG_PREFIX, "EPOS options", 
    "These options control the settings for the EPOS node and all maxon "
    "hardware connected to it.")->options;
  
  if (can_init_config_parse(can_dev, parser, 0, argc, argv, exit)) {
    free(can_dev);
    return EPOS_ERROR_CONFIG;
  }

  return epos_init_config(node, can_dev, config);
}

void epos_destroy(epos_node_p node) {
  can_device_p can_dev = node->dev.can_dev;

  epos_control_destroy(&node->control);
  epos_gear_destroy(&node->gear);
  epos_motor_destroy(&node->motor);
  epos_sensor_destroy(&node->sensor);
  epos_input_destroy(&node->input);
  epos_device_destroy(&node->dev);

  if (!can_dev->num_references)
    can_destroy(can_dev);

  config_destroy(&node->config);
}

int epos_open(epos_node_p node) {
  if (!epos_device_open(&node->dev) &&
    !epos_motor_setup(&node->motor) &&
    !epos_sensor_setup(&node->sensor) &&
    !epos_input_setup(&node->input))
    return EPOS_ERROR_NONE;
  else
    return EPOS_ERROR_OPEN;
}

int epos_close(epos_node_p node) {
  if (!epos_device_close(&node->dev))
    return EPOS_ERROR_NONE;
  else
    return EPOS_ERROR_CLOSE;
}

float epos_get_position(epos_node_p node) {
  int pos = epos_position_get_actual(&node->dev);
  return epos_gear_to_angle(&node->gear, pos);
}

float epos_get_velocity(epos_node_p node) {
  int vel = epos_velocity_get_average(&node->dev);
  return epos_gear_to_angular_velocity(&node->gear, vel);
}

float epos_get_acceleration(epos_node_p node) {
  int vel = epos_velocity_get_average(&node->dev);
  return epos_gear_to_angular_velocity(&node->gear, vel);
}

float epos_get_current(epos_node_p node) {
  return 1e-3*epos_current_get_average(&node->dev);
}

int epos_home(epos_node_p node, double timeout) {
  epos_home_t home;
  epos_home_init(&home,
    config_get_int(&node->config, EPOS_PARAMETER_HOME_METHOD),
    config_get_float(&node->config, EPOS_PARAMETER_HOME_CURRENT),
    deg_to_rad(config_get_float(&node->config, EPOS_PARAMETER_HOME_VELOCITY)),
    deg_to_rad(config_get_float(&node->config, 
      EPOS_PARAMETER_HOME_ACCELERATION)),
    deg_to_rad(config_get_float(&node->config, EPOS_PARAMETER_HOME_POSITION)));
  home.type = config_get_int(&node->config, EPOS_PARAMETER_HOME_TYPE);
  home.offset = deg_to_rad(config_get_float(&node->config, 
    EPOS_PARAMETER_HOME_OFFSET));

  if (!epos_home_start(node, &home)) {
    epos_home_wait(node, timeout);
    return EPOS_ERROR_NONE;
  }
  else
    return EPOS_ERROR_HOME;
}
