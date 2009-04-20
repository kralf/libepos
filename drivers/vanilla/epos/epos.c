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
  "error initializing EPOS",
  "error closing EPOS",
  "command line format error",
};

epos_parameter_t epos_default_parameters[] = {
  {"node-id", "0"},
  {"node-reset", "1"},
  {"enc-type", "1"},
  {"enc-polarity", "0"},
  {"enc-pulses", "1024"},
  {"motor-type", "1"},
  {"motor-current", "2.0"},
  {"gear-trans", "1.0"},
  {"control-type", "6"},
};

int epos_init(epos_node_p node, epos_parameter_t parameters[], ssize_t
  num_parameters) {
  ssize_t num_defp = sizeof(epos_default_parameters)/sizeof(epos_parameter_t);
  ssize_t num_devp = 0;
  can_parameter_t devp[num_parameters];
  node->parameters = malloc(sizeof(epos_default_parameters));
  memcpy(node->parameters, epos_default_parameters,
    sizeof(epos_default_parameters));
  int i, j;

  for (i = 0; i < num_parameters; ++i) {
    if (!strncmp(parameters[i].name, EPOS_ARG_DEVICE,
    strlen(EPOS_ARG_DEVICE))) {
      strcpy(devp[num_devp].name, &parameters[i].name[strlen(EPOS_ARG_DEVICE)]);
      strcpy(devp[num_devp].value, parameters[i].value);
      ++num_devp;
    }
    else {
      for (j = 0; j < num_defp; ++j)
        if (!strcmp(parameters[i].name, node->parameters[j].name)) {
        strcpy(node->parameters[j].value, parameters[i].value);
        break;
      }
    }
  }

  if (!epos_device_init(&node->dev,
      atoi(node->parameters[EPOS_PARAMETER_ID].value),
      atoi(node->parameters[EPOS_PARAMETER_RESET].value), devp, num_devp) &&
    !epos_sensor_init(&node->dev, &node->sensor,
      atoi(node->parameters[EPOS_PARAMETER_SENSOR_TYPE].value),
      atoi(node->parameters[EPOS_PARAMETER_SENSOR_POLARITY].value),
      atoi(node->parameters[EPOS_PARAMETER_SENSOR_PULSES].value)) &&
    !epos_motor_init(&node->dev, &node->motor,
      atoi(node->parameters[EPOS_PARAMETER_MOTOR_TYPE].value),
      atof(node->parameters[EPOS_PARAMETER_MOTOR_CURRENT].value)) &&
    !epos_gear_init(&node->sensor, &node->gear,
      atof(node->parameters[EPOS_PARAMETER_GEAR_TRANSMISSION].value)) &&
    !epos_control_init(&node->dev, &node->control,
      atoi(node->parameters[EPOS_PARAMETER_CONTROL_TYPE].value))) {
    return EPOS_ERROR_NONE;
  }
  else {
    free(node->parameters);
    node->parameters = 0;

    return EPOS_ERROR_INIT;
  }
}

int epos_init_arg(epos_node_p node, int argc, char **argv) {
  ssize_t num_parameters = 0;
  epos_parameter_t parameters[argc-1];
  int i;

  for (i = 1; i < argc; ++i)
    if ((argv[i][0] == '-') && (argv[i][1] == '-')) {
    char* param = &argv[i][2];
    char* split = strchr(param, '=');
    if (split) {
      char name[64];
      char value[64];
      strncpy(parameters[num_parameters].name, param, split-param);
      parameters[num_parameters].name[split-param] = 0;
      strcpy(parameters[num_parameters].value, &split[1]);
      ++num_parameters;
    }
    else
      return EPOS_ERROR_FORMAT;
  }

  return epos_init(node, parameters, num_parameters);  
}

int epos_close(epos_node_p node) {
  if (!epos_control_close(&node->control) &&
    !epos_gear_close(&node->gear) &&
    !epos_motor_close(&node->motor) &&
    !epos_sensor_close(&node->sensor) &&
    !epos_device_close(&node->dev)) {
    free(node->parameters);
    node->parameters = 0;

    return EPOS_ERROR_NONE;
  }
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
