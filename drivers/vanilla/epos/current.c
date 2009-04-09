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

void epos_current_init(epos_current_p current, float target_value) {
  current->target_value = target_value;
}

int epos_current_setup(epos_node_p node, epos_current_config_p config) {
  int result;

  if (!(result = epos_current_set_p_gain(&node->dev, config->p_gain)))
    return epos_current_set_i_gain(&node->dev, config->i_gain);
  else
    return result;
}

int epos_current_start(epos_node_p node, epos_current_p current) {
  int result;
  short curr = current->target_value*1e3;

  if (!(result = epos_control_set_type(&node->control, epos_current)) &&
    !(result = epos_control_start(&node->control)))
    return epos_current_set_demand(&node->dev, curr);
  else
    return result;
}

int epos_current_stop(epos_node_p node) {
  return epos_control_stop(&node->control);
}

short epos_current_get_actual(epos_device_p dev) {
  short current;
  epos_device_read(dev, EPOS_CURRENT_INDEX_ACTUAL_VALUE, 0,
    (unsigned char*)&current, sizeof(short));

  return current;
}

short epos_current_get_average(epos_device_p dev) {
  short current;
  epos_device_read(dev, EPOS_CURRENT_INDEX_AVERAGE_VALUE, 0,
    (unsigned char*)&current, sizeof(short));

  return current;
}

int epos_current_set_demand(epos_device_p dev, short current) {
  return epos_device_write(dev, EPOS_CURRENT_INDEX_SETTING_VALUE, 0,
    (unsigned char*)&current, sizeof(short));
}

int epos_current_set_p_gain(epos_device_p dev, short p_gain) {
  return epos_device_write(dev, EPOS_CURRENT_INDEX_CONTROL_PARAMETERS,
    EPOS_CURRENT_SUBINDEX_P_GAIN, (unsigned char*)&p_gain, sizeof(short));
}

int epos_current_set_i_gain(epos_device_p dev, short i_gain) {
  return epos_device_write(dev, EPOS_CURRENT_INDEX_CONTROL_PARAMETERS,
    EPOS_CURRENT_SUBINDEX_I_GAIN, (unsigned char*)&i_gain, sizeof(short));
}
