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
#include <float.h>

#include "position.h"
#include "gear.h"

void epos_position_init(epos_position_t* position, float target_value) {
  epos_position_init_limits(position, target_value, -FLT_MAX, FLT_MAX,
    FLT_MAX);
}

void epos_position_init_limits(epos_position_t* position, float target_value,
  float min_value, float max_value, float max_error) {
  position->target_value = target_value;

  position->min_value = min_value;
  position->max_value = max_value;
  position->max_error = max_error;
}

int epos_position_setup(epos_node_t* node, const epos_position_config_t*
    config) {
  if (!epos_position_set_p_gain(&node->dev, config->p_gain) &&
      !epos_position_set_i_gain(&node->dev, config->i_gain) &&
      !epos_position_set_d_gain(&node->dev, config->d_gain) &&
      !epos_position_set_velocity_factor(&node->dev,config->vel_factor))
    epos_position_set_acceleration_factor(&node->dev, config->acc_factor);

  return node->dev.error.code;
}

int epos_position_start(epos_node_t* node, const epos_position_t* position) {
  int pos = epos_gear_from_angle(&node->gear, position->target_value);
  int min_pos = epos_gear_from_angle(&node->gear, position->min_value);
  int max_pos = epos_gear_from_angle(&node->gear, position->max_value);
  unsigned int max_error = abs(epos_gear_from_angle(&node->gear,
    position->max_error));

  if (!epos_control_set_mode(&node->control, epos_control_position) &&
      !epos_position_set_limits(&node->dev, min_pos, max_pos) &&
      !epos_position_set_max_error(&node->dev, max_error) &&
      !epos_control_start(&node->control))
    epos_position_set_demand(&node->dev, pos);

  return node->dev.error.code;
}

int epos_position_stop(epos_node_t* node) {
  return epos_control_stop(&node->control);
}

int epos_position_update(epos_node_t* node, epos_position_t* position) {
  int pos = epos_gear_from_angle(&node->gear, position->target_value);
  return epos_position_set_demand(&node->dev, pos);
}

int epos_position_set_limits(epos_device_t* dev, int min_pos, int max_pos) {
  if (!epos_device_write(dev, EPOS_POSITION_INDEX_SOFTWARE_LIMIT,
      EPOS_POSITION_SUBINDEX_NEG_LIMIT, (unsigned char*)&min_pos,
      sizeof(int)))
    epos_device_write(dev, EPOS_POSITION_INDEX_SOFTWARE_LIMIT,
      EPOS_POSITION_SUBINDEX_POS_LIMIT, (unsigned char*)&max_pos,
      sizeof(int));

  return dev->error.code;
}

int epos_position_set_max_error(epos_device_t* dev, unsigned int max_error) {
  epos_device_write(dev, EPOS_POSITION_INDEX_MAX_FOLLOWING_ERROR, 0,
    (unsigned char*)&max_error, sizeof(unsigned int));
  
  return dev->error.code;
}

int epos_position_get_actual(epos_device_t* dev) {
  int pos = 0;
  epos_device_read(dev, EPOS_POSITION_INDEX_ACTUAL_VALUE, 0,
    (unsigned char*)&pos, sizeof(int));

  return pos;
}

int epos_position_set_demand(epos_device_t* dev, int position) {
  epos_device_write(dev, EPOS_POSITION_INDEX_SETTING_VALUE, 0,
    (unsigned char*)&position, sizeof(int));
  
  return dev->error.code;
}

int epos_position_get_demand(epos_device_t* dev) {
  int pos = 0;
  epos_device_read(dev, EPOS_POSITION_INDEX_DEMAND_VALUE, 0,
    (unsigned char*)&pos, sizeof(int));

  return pos;
}

int epos_position_set_p_gain(epos_device_t* dev, short p_gain) {
  epos_device_write(dev, EPOS_POSITION_INDEX_CONTROL_PARAMETERS,
    EPOS_POSITION_SUBINDEX_P_GAIN, (unsigned char*)&p_gain, sizeof(short));
  
  return dev->error.code;
}

int epos_position_set_i_gain(epos_device_t* dev, short i_gain) {
  epos_device_write(dev, EPOS_POSITION_INDEX_CONTROL_PARAMETERS,
    EPOS_POSITION_SUBINDEX_I_GAIN, (unsigned char*)&i_gain, sizeof(short));
  
  return dev->error.code;
}

int epos_position_set_d_gain(epos_device_t* dev, short d_gain) {
  epos_device_write(dev, EPOS_POSITION_INDEX_CONTROL_PARAMETERS,
    EPOS_POSITION_SUBINDEX_D_GAIN, (unsigned char*)&d_gain, sizeof(short));
  
  return dev->error.code;
}

int epos_position_set_velocity_factor(epos_device_t* dev, short vel_factor) {
  epos_device_write(dev, EPOS_POSITION_INDEX_CONTROL_PARAMETERS,
    EPOS_POSITION_SUBINDEX_VELOCITY_FACTOR, (unsigned char*)&vel_factor,
    sizeof(short));
  
  return dev->error.code;
}

int epos_position_set_acceleration_factor(epos_device_t* dev, short
    acc_factor) {
  epos_device_write(dev, EPOS_POSITION_INDEX_CONTROL_PARAMETERS,
    EPOS_POSITION_SUBINDEX_ACCELERATION_FACTOR, (unsigned char*)&acc_factor,
    sizeof(short));
  
  return dev->error.code;
}
