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

#include "position_profile.h"
#include "gear.h"

void epos_position_profile_init(epos_position_profile_p profile,
  float target_value, float velocity, float acceleration, float deceleration,
  epos_profile_type_t type) {
  profile->target_value = target_value;
  profile->velocity = velocity;
  profile->acceleration = acceleration;
  profile->deceleration = deceleration;

  profile->relative = 0;
  profile->type = type;
}

int epos_position_profile_start(epos_node_p node, epos_position_profile_p
  profile) {
  int result;
  int pos = epos_gear_from_angle(&node->gear, profile->target_value);
  unsigned int vel = abs(epos_gear_from_angular_velocity(&node->gear,
    profile->velocity));
  unsigned int acc = abs(epos_gear_from_angular_acceleration(&node->gear,
    profile->acceleration));
  unsigned int dec = abs(epos_gear_from_angular_acceleration(&node->gear,
    profile->deceleration));
  short control = (profile->relative) ?
    EPOS_POSITION_PROFILE_CONTROL_SET_RELATIVE :
    EPOS_POSITION_PROFILE_CONTROL_SET_ABSOLUTE;

  if (!(result = epos_control_set_type(&node->control, epos_profile_pos)) &&
    !(result = epos_position_profile_set_target(&node->dev, pos)) &&
    !(result = epos_position_profile_set_velocity(&node->dev, vel)) &&
    !(result = epos_profile_set_acceleration(&node->dev, acc)) &&
    !(result = epos_profile_set_deceleration(&node->dev, dec)) &&
    !(result = epos_profile_set_type(&node->dev, profile->type)) &&
    !(result = epos_control_start(&node->control)))
    return epos_device_set_control(&node->dev, control);
  else
    return result;
}

int epos_position_profile_stop(epos_node_p node) {
  return epos_control_stop(&node->control);
}

int epos_position_profile_set_target(epos_device_p dev, int position) {
  return epos_device_write(dev, EPOS_POSITION_PROFILE_INDEX_TARGET, 0,
    (unsigned char*)&position, sizeof(int));
}

int epos_position_profile_set_velocity(epos_device_p dev, unsigned int
  velocity) {
  return epos_device_write(dev, EPOS_POSITION_PROFILE_INDEX_VELOCITY, 0,
    (unsigned char*)&velocity, sizeof(unsigned int));
}
