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

#include "current.h"

void epos_current_init(epos_current_t* current, float target_value) {
  current->target_value = target_value;
}

int epos_current_setup(epos_node_t* node, const epos_current_config_t*
    config) {
  if (!epos_current_set_p_gain(&node->dev, config->p_gain))
    epos_current_set_i_gain(&node->dev, config->i_gain);

  return node->dev.error.code;
}

int epos_current_start(epos_node_t* node, const epos_current_t* current) {
  short curr = current->target_value*1e3;

  if (!epos_control_set_mode(&node->control, epos_control_current) &&
      !epos_control_start(&node->control))
    epos_current_set_demand(&node->dev, curr);

  return node->dev.error.code;
}

int epos_current_stop(epos_node_t* node) {
  return epos_control_stop(&node->control);
}

short epos_current_get_actual(epos_device_t* dev) {
  short current = 0;
  epos_device_read(dev, EPOS_CURRENT_INDEX_ACTUAL_VALUE, 0,
    (unsigned char*)&current, sizeof(short));

  return current;
}

short epos_current_get_average(epos_device_t* dev) {
  short current = 0;
  epos_device_read(dev, EPOS_CURRENT_INDEX_AVERAGE_VALUE, 0,
    (unsigned char*)&current, sizeof(short));

  return current;
}

int epos_current_set_demand(epos_device_t* dev, short current) {
  epos_device_write(dev, EPOS_CURRENT_INDEX_SETTING_VALUE, 0,
    (unsigned char*)&current, sizeof(short));
  
  return dev->error.code;
}

short epos_current_get_demand(epos_device_t* dev) {
  short current = 0;
  epos_device_read(dev, EPOS_CURRENT_INDEX_SETTING_VALUE, 0,
    (unsigned char*)&current, sizeof(short));

  return current;
}

int epos_current_set_p_gain(epos_device_t* dev, short p_gain) {
  epos_device_write(dev, EPOS_CURRENT_INDEX_CONTROL_PARAMETERS,
    EPOS_CURRENT_SUBINDEX_P_GAIN, (unsigned char*)&p_gain, sizeof(short));

  return dev->error.code;
}

int epos_current_set_i_gain(epos_device_t* dev, short i_gain) {
  epos_device_write(dev, EPOS_CURRENT_INDEX_CONTROL_PARAMETERS,
    EPOS_CURRENT_SUBINDEX_I_GAIN, (unsigned char*)&i_gain, sizeof(short));
  
  return dev->error.code;
}
