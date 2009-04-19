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

#include "velocity_profile.h"
#include "gear.h"

void epos_velocity_profile_init(epos_velocity_profile_p profile,
  float target_value, float acceleration, float deceleration,
  epos_profile_type_t type) {
  profile->target_value = target_value;
  profile->acceleration = acceleration;
  profile->deceleration = deceleration;

  profile->type = type;
}

int epos_velocity_profile_start(epos_node_p node, epos_velocity_profile_p
  profile) {
  int result;
  int vel = epos_gear_from_angular_velocity(&node->gear,
    profile->target_value);
  unsigned int acc = abs(epos_gear_from_angular_acceleration(&node->gear,
    profile->acceleration));
  unsigned int dec = abs(epos_gear_from_angular_acceleration(&node->gear,
    profile->deceleration));

  if (!(result = epos_control_set_type(&node->control, epos_profile_vel)) &&
    !(result = epos_profile_set_acceleration(&node->dev, acc)) &&
    !(result = epos_profile_set_deceleration(&node->dev, dec)) &&
    !(result = epos_profile_set_type(&node->dev, profile->type)) &&
    !(result = epos_control_start(&node->control)))
    result = epos_velocity_profile_set_target(&node->dev, vel);

  return result;
}

int epos_velocity_profile_stop(epos_node_p node) {
  return epos_control_stop(&node->control);
}

int epos_velocity_profile_set_target(epos_device_p dev, int velocity) {
  return epos_device_write(dev, EPOS_VELOCITY_PROFILE_INDEX_TARGET, 0,
    (unsigned char*)&velocity, sizeof(int));
}
