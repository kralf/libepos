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

#include "velocity.h"
#include "gear.h"

void epos_velocity_init(epos_velocity_p velocity, float target_value) {
  velocity->target_value = target_value;
}

int epos_velocity_setup(epos_node_p node, epos_velocity_config_p config) {
  int result;

  if (!(result = epos_velocity_set_p_gain(&node->dev, config->p_gain)))
    return epos_velocity_set_i_gain(&node->dev, config->i_gain);
  else
    return result;
}

int epos_velocity_start(epos_node_p node, epos_velocity_p velocity) {
  int result;
  int vel = epos_gear_from_angular_velocity(&node->gear,
    velocity->target_value);

  if (!(result = epos_control_set_type(&node->control, epos_velocity)) &&
    !(result = epos_control_start(&node->control)))
    return epos_velocity_set_demand(&node->dev, vel);
  else
    return result;
}

int epos_velocity_stop(epos_node_p node) {
  return epos_control_stop(&node->control);
}

int epos_velocity_get_actual(epos_device_p dev) {
  int vel;
  epos_device_read(dev, EPOS_VELOCITY_INDEX_ACTUAL_VALUE, 0,
    (unsigned char*)&vel, sizeof(int));

  return vel;
}

int epos_velocity_get_average(epos_device_p dev) {
  int vel;
  epos_device_read(dev, EPOS_VELOCITY_INDEX_AVERAGE_VALUE, 0,
    (unsigned char*)&vel, sizeof(int));

  return vel;
}

int epos_velocity_set_demand(epos_device_p dev, int velocity) {
  return epos_device_write(dev, EPOS_VELOCITY_INDEX_SETTING_VALUE, 0,
    (unsigned char*)&velocity, sizeof(int));
}

int epos_velocity_get_demand(epos_device_p dev) {
  int vel;
  epos_device_read(dev, EPOS_VELOCITY_INDEX_DEMAND_VALUE, 0,
    (unsigned char*)&vel, sizeof(int));

  return vel;
}

int epos_velocity_set_p_gain(epos_device_p dev, short p_gain) {
  return epos_device_write(dev, EPOS_VELOCITY_INDEX_CONTROL_PARAMETERS,
    EPOS_VELOCITY_SUBINDEX_P_GAIN, (unsigned char*)&p_gain, sizeof(short));
}

int epos_velocity_set_i_gain(epos_device_p dev, short i_gain) {
  return epos_device_write(dev, EPOS_VELOCITY_INDEX_CONTROL_PARAMETERS,
    EPOS_VELOCITY_SUBINDEX_I_GAIN, (unsigned char*)&i_gain, sizeof(short));
}
