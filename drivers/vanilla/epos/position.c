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

void epos_position_init(epos_position_p position, float angle) {
  epos_position_init_limits(position, angle, -FLT_MAX, FLT_MAX);
}

void epos_position_init_limits(epos_position_p position, float angle,
  float min_angle, float max_angle) {
  position->angle = angle;

  position->min_angle = min_angle;
  position->max_angle = max_angle;
}

int epos_position_start(epos_node_p node, epos_position_p position) {
  int result;
  int num_steps = epos_gear_angle_to_steps(&node->gear, position->angle);
  int min_steps = epos_gear_angle_to_steps(&node->gear, position->min_angle);
  int max_steps = epos_gear_angle_to_steps(&node->gear, position->max_angle);

  if (!(result = epos_control_set_type(&node->control, epos_position)) &&
    !(result = epos_position_set_limits(&node->dev, min_steps, max_steps)) &&
    !(result = epos_position_set_demand(&node->dev, num_steps)))
    return epos_control_start(&node->control);
  else
    return result;
}

int epos_position_stop(epos_node_p node) {
  return epos_control_stop(&node->control);
}

int epos_position_set_limits(epos_device_p dev, int min_steps, int max_steps) {
  int result;

  if (!(result = epos_device_write(dev, EPOS_POSITION_INDEX_SOFTWARE_LIMIT,
    EPOS_POSITION_SUBINDEX_NEG_LIMIT, (unsigned char*)&min_steps, sizeof(int))))
    return epos_device_write(dev, EPOS_POSITION_INDEX_SOFTWARE_LIMIT,
    EPOS_POSITION_SUBINDEX_POS_LIMIT, (unsigned char*)&max_steps, sizeof(int));
  else
    return result;
}

int epos_position_get_actual(epos_device_p dev) {
  int num_steps;
  epos_device_read(dev, EPOS_POSITION_INDEX_ACTUAL_VALUE, 0,
    (unsigned char*)&num_steps, sizeof(int));

  return num_steps;
}

int epos_position_set_demand(epos_device_p dev, int num_steps) {
  return epos_device_write(dev, EPOS_POSITION_INDEX_SETTING_VALUE, 0,
    (unsigned char*)&num_steps, sizeof(int));
}

int epos_position_get_demand(epos_device_p dev) {
  int num_steps;
  epos_device_read(dev, EPOS_POSITION_INDEX_DEMAND_VALUE, 0,
    (unsigned char*)&num_steps, sizeof(int));

  return num_steps;
}
