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

#include "home.h"

void epos_home_init(epos_home_p home, epos_home_method_t method, short current,
  int velocity, int position) {
  home->method = method;

  home->current = current;
  home->switch_vel = velocity;
  home->zero_vel = velocity;

  home->offset = 0;
  home->position = position;
}

int epos_home_start(epos_node_p node, epos_home_p home) {
  int result;
  if (!(result = epos_control_set_type(&node->control, epos_homing)) &&
    !(result = epos_home_set_method(&node->dev, home->method)) &&
    !(result = epos_home_set_current_threshold(&node->dev, home->current)) &&
    !(result = epos_home_set_switch_search_velocity(&node->dev,
      home->switch_vel)) &&
    !(result = epos_home_set_zero_search_velocity(&node->dev,
      home->zero_vel)) &&
    !(result = epos_home_set_offset(&node->dev, home->offset)) &&
    !(result = epos_home_set_position(&node->dev, home->position)) &&
    !(result = epos_control_start(&node->control)))
    return epos_device_set_control(&node->dev, EPOS_HOME_CONTROL_START);
  else
    return result;
}

int epos_home_stop(epos_node_p node) {
  int result;
  if (!(result = epos_device_set_control(&node->dev, EPOS_HOME_CONTROL_HALT)))
    return epos_control_stop(&node->control);
  else  
    return result;
}

int epos_home_wait(epos_node_p node, double timeout) {
  return epos_device_wait_status(&node->dev,
    EPOS_DEVICE_STATUS_HOMING_ATTAINED, timeout);
}

int epos_home_set_method(epos_device_p dev, epos_home_method_t method) {
  unsigned char m = method;
  return epos_device_write(dev, EPOS_HOME_INDEX_METHOD, 0, &m, 1);
}

int epos_home_set_current_threshold(epos_device_p dev, short current) {
  return epos_device_write(dev, EPOS_HOME_INDEX_CURRENT_THRESHOLD, 0,
    (unsigned char*)&current, sizeof(short));
}

int epos_home_set_switch_search_velocity(epos_device_p dev, int velocity) {
  return epos_device_write(dev, EPOS_HOME_INDEX_VELOCITIES,
    EPOS_HOME_SUBINDEX_SWITCH_SEARCH_VELOCITY, (unsigned char*)&velocity,
    sizeof(int));
}

int epos_home_set_zero_search_velocity(epos_device_p dev, int velocity) {
  return epos_device_write(dev, EPOS_HOME_INDEX_VELOCITIES,
    EPOS_HOME_SUBINDEX_ZERO_SEARCH_VELOCITY, (unsigned char*)&velocity,
    sizeof(int));
}

int epos_home_set_offset(epos_device_p dev, int offset) {
  return epos_device_write(dev, EPOS_HOME_INDEX_OFFSET, 0,
    (unsigned char*)&offset, sizeof(int));
}

int epos_home_set_position(epos_device_p dev, int position) {
  return epos_device_write(dev, EPOS_HOME_INDEX_POSITION, 0,
    (unsigned char*)&position, sizeof(int));
}
