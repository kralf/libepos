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

void epos_position_init(epos_position_p position, float target_value) {
  epos_position_init_limits(position, target_value, -FLT_MAX, FLT_MAX);
}

void epos_position_init_limits(epos_position_p position, float target_value,
  float min_value, float max_value) {
  position->target_value = target_value;

  position->min_value = min_value;
  position->max_value = max_value;
}

void epos_position_setup(epos_node_p node, epos_position_config_p config) {
  int result;

  /** TODO: Hier geht's weiter! */
}

int epos_position_start(epos_node_p node, epos_position_p position) {
  int result;
  int pos = epos_gear_from_angle(&node->gear, position->target_value);
  int min_pos = epos_gear_from_angle(&node->gear, position->min_value);
  int max_pos = epos_gear_from_angle(&node->gear, position->max_value);

  if (!(result = epos_control_set_type(&node->control, epos_position)) &&
    !(result = epos_position_set_limits(&node->dev, min_pos, max_pos)) &&
    !(result = epos_control_start(&node->control)))
    return epos_position_set_demand(&node->dev, pos);
  else
    return result;
}

int epos_position_stop(epos_node_p node) {
  return epos_control_stop(&node->control);
}

int epos_position_set_limits(epos_device_p dev, int min_pos, int max_pos) {
  int result;

  if (!(result = epos_device_write(dev, EPOS_POSITION_INDEX_SOFTWARE_LIMIT,
    EPOS_POSITION_SUBINDEX_NEG_LIMIT, (unsigned char*)&min_pos, sizeof(int))))
    return epos_device_write(dev, EPOS_POSITION_INDEX_SOFTWARE_LIMIT,
    EPOS_POSITION_SUBINDEX_POS_LIMIT, (unsigned char*)&max_pos, sizeof(int));
  else
    return result;
}

int epos_position_get_actual(epos_device_p dev) {
  int pos;
  epos_device_read(dev, EPOS_POSITION_INDEX_ACTUAL_VALUE, 0,
    (unsigned char*)&pos, sizeof(int));

  return pos;
}

int epos_position_set_demand(epos_device_p dev, int pos) {
  return epos_device_write(dev, EPOS_POSITION_INDEX_SETTING_VALUE, 0,
    (unsigned char*)&pos, sizeof(int));
}

int epos_position_get_demand(epos_device_p dev) {
  int pos;
  epos_device_read(dev, EPOS_POSITION_INDEX_DEMAND_VALUE, 0,
    (unsigned char*)&pos, sizeof(int));

  return pos;
}
