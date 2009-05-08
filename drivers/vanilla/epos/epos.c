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
#include "position.h"
#include "velocity.h"
#include "current.h"

const char* epos_errors[] = {
  "success",
  "error opening EPOS",
  "error closing EPOS",
  "command line format error",
};

param_t epos_default_params[] = {
  {EPOS_PARAMETER_ID, "0"},
  {EPOS_PARAMETER_RESET, "1"},
  {EPOS_PARAMETER_SENSOR_TYPE, "1"},
  {EPOS_PARAMETER_SENSOR_POLARITY, "0"},
  {EPOS_PARAMETER_SENSOR_PULSES, "1024"},
  {EPOS_PARAMETER_MOTOR_TYPE, "1"},
  {EPOS_PARAMETER_MOTOR_CURRENT, "2.0"},
  {EPOS_PARAMETER_GEAR_TRANSMISSION, "1.0"},
  {EPOS_PARAMETER_CONTROL_TYPE, "6"},
};

config_t epos_default_config = {
  epos_default_params,
  sizeof(epos_default_params)/sizeof(param_t),
};

void epos_init(epos_node_p node, can_device_p can_dev, config_p config) {
  config_init_default(&node->config, &epos_default_config);
  if (config)
    config_set(&node->config, config);

  if (!can_dev) {
    can_dev = malloc(sizeof(can_device_t));
    can_init(can_dev, 0);
  }

  epos_device_init(&node->dev, can_dev,
    config_get_int(&node->config, EPOS_PARAMETER_ID),
    config_get_int(&node->config, EPOS_PARAMETER_RESET));
  epos_sensor_init(&node->sensor, &node->dev,
    config_get_int(&node->config, EPOS_PARAMETER_SENSOR_TYPE),
    config_get_int(&node->config, EPOS_PARAMETER_SENSOR_POLARITY),
    config_get_int(&node->config, EPOS_PARAMETER_SENSOR_PULSES));
  epos_motor_init(&node->motor, &node->dev,
    config_get_int(&node->config, EPOS_PARAMETER_MOTOR_TYPE),
    config_get_float(&node->config, EPOS_PARAMETER_MOTOR_CURRENT));
  epos_gear_init(&node->gear, &node->sensor,
    config_get_float(&node->config, EPOS_PARAMETER_GEAR_TRANSMISSION));
  epos_control_init(&node->control, &node->dev,
    config_get_int(&node->config, EPOS_PARAMETER_CONTROL_TYPE));
}

void epos_init_arg(epos_node_p node, int argc, char **argv) {
  can_device_p can_dev = malloc(sizeof(can_device_t));
  can_init_arg(can_dev, argc, argv);

  config_t config;
  config_init_arg(&config, argc, argv, EPOS_CONFIG_ARG_PREFIX);
    
  epos_init(node, can_dev, &config);
}

void epos_destroy(epos_node_p node) {
  can_device_p can_dev = node->dev.can_dev;

  epos_control_destroy(&node->control);
  epos_gear_destroy(&node->gear);
  epos_motor_destroy(&node->motor);
  epos_sensor_destroy(&node->sensor);
  epos_device_destroy(&node->dev);

  if (!can_dev->num_references)
    can_destroy(can_dev);

  config_destroy(&node->config);
}

int epos_open(epos_node_p node) {
  if (!epos_device_open(&node->dev) &&
    !epos_sensor_setup(&node->sensor) &&
    !epos_motor_setup(&node->motor))
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

float epos_get_current(epos_node_p node) {
  return 1e-3*epos_current_get_average(&node->dev);
}
