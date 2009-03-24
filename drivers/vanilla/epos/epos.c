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

const char* epos_errors[] = {
  "success",
  "error initializing EPOS",
  "error closing EPOS",
  "command line format error",
};

epos_parameter_t epos_parameters[] = {
  {"id", "0"},
  {"enc-type", "1"},
  {"enc-polarity", "0"},
  {"enc-pulses", "1024"},
  {"motor-type", "1"},
  {"motor-current", "1000"},
  {"control-type", "6"},
};

int epos_init(epos_node_p node, int node_id) {
  sprintf(epos_parameters[EPOS_PARAMETER_ID].value, "%d", node_id);
  return epos_init_arg(node, 0, 0);
}

int epos_init_arg(epos_node_p node, int argc, char **argv) {
  ssize_t num_devp = 0;
  ssize_t num_eposp = sizeof(epos_parameters)/sizeof(epos_parameter_t);
  can_parameter_t dev_parameters[argc-1];
  int i, j;

  for (i = 1; i < argc; ++i)
    if ((argv[i][0] == '-') && (argv[i][1] == '-')) {
    char* param = &argv[i][2];
    char* split = strchr(param, '=');
    if (split) {
      char name[64];
      char value[64];
      strncpy(name, param, split-param);
      name[split-param] = 0;
      strcpy(value, &split[1]);

      if (!strncmp(name, EPOS_ARG_DEVICE, strlen(EPOS_ARG_DEVICE))) {
        strcpy(dev_parameters[num_devp].name, &name[strlen(EPOS_ARG_DEVICE)]);
        strcpy(dev_parameters[num_devp].value, value);
        ++num_devp;
      }
      else {
        for (j = 0; j < num_eposp; ++j)
          if (!strcmp(name, epos_parameters[j].name)) {
          strcpy(epos_parameters[j].value, value);
          break;
        }
      }
    }
    else
      return EPOS_ERROR_FORMAT;
  }

  if (!epos_device_init(&node->dev,
      atoi(epos_parameters[EPOS_PARAMETER_ID].value), dev_parameters,
      num_devp) &&
    !epos_sensor_init(&node->dev, &node->sensor,
      atoi(epos_parameters[EPOS_PARAMETER_SENSOR_TYPE].value),
      atoi(epos_parameters[EPOS_PARAMETER_SENSOR_POLARITY].value),
      atoi(epos_parameters[EPOS_PARAMETER_SENSOR_PULSES].value)) &&
    !epos_motor_init(&node->dev, &node->motor,
      atoi(epos_parameters[EPOS_PARAMETER_MOTOR_TYPE].value),
      atoi(epos_parameters[EPOS_PARAMETER_MOTOR_CURRENT].value)) &&
    !epos_control_init(&node->dev, &node->control,
      atoi(epos_parameters[EPOS_PARAMETER_CONTROL_TYPE].value)))
    return EPOS_ERROR_NONE;
  else
    return EPOS_ERROR_INIT;
}

int epos_close(epos_node_p node) {
  if (!epos_control_close(&node->control) &&
    !epos_motor_close(&node->motor) &&
    !epos_sensor_close(&node->sensor) &&
    !epos_device_close(&node->dev))
    return EPOS_ERROR_NONE;
  else
    return EPOS_ERROR_CLOSE;
}
