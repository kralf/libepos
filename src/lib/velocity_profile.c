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

#include <timer/timer.h>

#include "velocity_profile.h"

#include "gear.h"
#include "macros.h"

void epos_velocity_profile_init(epos_velocity_profile_t* profile,
    float target_value, float acceleration, float deceleration,
    epos_profile_type_t type) {
  profile->target_value = target_value;
  profile->acceleration = acceleration;
  profile->deceleration = deceleration;

  profile->type = type;

  profile->start_value = 0.0;
  profile->start_time = 0.0;
}

int epos_velocity_profile_start(epos_node_t* node, epos_velocity_profile_t*
    profile) {
  int vel = epos_gear_from_angular_velocity(&node->gear,
    profile->target_value);
  unsigned int acc = abs(epos_gear_from_angular_acceleration(&node->gear,
    profile->acceleration));
  unsigned int dec = abs(epos_gear_from_angular_acceleration(&node->gear,
    profile->deceleration));
  short control = EPOS_VELOCITY_PROFILE_CONTROL_SET;

  if (!epos_control_set_mode(&node->control, epos_control_profile_vel) &&
      !epos_profile_set_acceleration(&node->dev, acc) &&
      !epos_profile_set_deceleration(&node->dev, dec) &&
      !epos_profile_set_type(&node->dev, profile->type) &&
      !epos_control_start(&node->control) &&
      !epos_velocity_profile_set_target(&node->dev, vel)) {
    profile->start_value = epos_node_get_velocity(node);
    error_return(&node->dev.error);
  
    timer_start(&profile->start_time);
    epos_device_set_control(&node->dev, control);
    timer_correct(&profile->start_time);
  }

  return node->dev.error.code;
}

int epos_velocity_profile_stop(epos_node_t* node) {
  return epos_control_stop(&node->control);
}

epos_profile_value_t epos_velocity_profile_eval(const epos_velocity_profile_t*
    profile, double time) {
  epos_profile_value_t values;
  
  float v_0 = profile->start_value;
  float v_1 = profile->target_value;
  double t = time-profile->start_time;
  
  if (t > 0.0) {
    float v = v_1-v_0;
    float a = (v_1 > v_0) ? fabs(profile->acceleration) :
      -fabs(profile->acceleration);
    
    if (profile->type == epos_profile_sinusoidal) {
      double t_a = 0.5*M_PI*v/a;

      if (t <= t_a) {
        values.position = v_0*t+0.5*v*(t-0.5*v/a*sin(2.0*a/v*t));
        values.velocity = v_0+0.5*v*(1.0-cos(2.0*a/v*t));
        values.acceleration = a*sin(2.0*a/v*t);
      }
      else {
        values.position = v_0*t_a+0.5*v*(t_a-0.5*v/a*sin(2.0*a/v*t_a));
        values.velocity = v_1;
        values.acceleration = 0.0;
      }
    }
    else {
      double t_a = v/a;

      if (t <= t_a) {
        values.position = (v_0+0.5*a*t)*t;
        values.velocity = v_0+a*t;
        values.acceleration = a;
      }
      else {
        values.position = (v_0+0.5*a*t_a)*t_a;
        values.velocity = v_1;
        values.acceleration = 0.0;
      }
    }
  }
  else {
    values.position = 0.0;
    values.velocity = v_0;
    values.acceleration = 0.0;
  }

  return values;
}

int epos_velocity_profile_set_target(epos_device_t* dev, int velocity) {
  epos_device_write(dev, EPOS_VELOCITY_PROFILE_INDEX_TARGET, 0,
    (unsigned char*)&velocity, sizeof(int));

  return dev->error.code;
}
