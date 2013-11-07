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
  short control = EPOS_VELOCITY_PROFILE_CONTROL_SET;

  if (!(result = epos_control_set_mode(&node->control, 
      epos_control_profile_vel)) &&
    !(result = epos_profile_set_acceleration(&node->dev, acc)) &&
    !(result = epos_profile_set_deceleration(&node->dev, dec)) &&
    !(result = epos_profile_set_type(&node->dev, profile->type)) &&
    !(result = epos_control_start(&node->control)) &&
    !(result = epos_velocity_profile_set_target(&node->dev, vel))) {
    profile->start_value = epos_get_velocity(node);
    timer_start(&profile->start_time);
    result = epos_device_set_control(&node->dev, control);
    timer_correct(&profile->start_time);
  }

  return result;
}

int epos_velocity_profile_stop(epos_node_p node) {
  return epos_control_stop(&node->control);
}

float epos_velocity_profile_eval(epos_velocity_profile_p profile, double
    time) {
  float v_0 = profile->start_value;
  float v_1 = profile->target_value;
  float d_v = 0.0;
  double t = time-profile->start_time;

  if (t > 0.0) {
    float a = profile->acceleration;
    float d = profile->deceleration;

    if (profile->type == epos_profile_sinusoidal) {
      double t_a = 0.0;
      double t_d = 0.0;
      
      if (v_1 > v_0) {
        t_a = 0.5*M_PI*(v_1-max(v_0, 0.0))/a;
        t_d = 0.5*M_PI*min(v_0, 0.0)/d;
      }
      else {
        t_d = 0.5*M_PI*(v_0-max(v_1, 0.0))/d;
        t_a = 0.5*M_PI*min(v_1, 0.0)/a;
      }

      if (t < t_d)
        d_v = 0.5*(v_1-v_0)*(1.0-cos(2.0*d/(v_0-v_1)*t));
      else if (t < t_d+t_a)
        d_v = 0.5*(v_1-v_0)*(1.0-cos(2.0*a/(v_1-v_0)*(t-t_d)));
      else
        d_v = v_1;
      
//       if (v_1 > v_0) {
//         double t_a = 0.5*M_PI*(v_1-v_0)/a;
// 
//         if (t < t_a)
//           d_v = 0.5*(v_1-v_0)*(1.0-cos(2.0*a/(v_1-v_0)*t));
//         else
//           d_v = v_1;
//       }
//       else {
//         double t_d = 0.5*M_PI*(v_0-v_1)/d;
// 
//         if (t < t_d)
//           d_v = 0.5*(v_1-v_0)*(1.0-cos(2.0*d/(v_0-v_1)*t));
//         else
//           d_v = v_1;        
//       }
    }
    else {
      if (v_1 > v_0) {
        double t_a = (v_1-v_0)/a;

        if (t < t_a)
          d_v = a*t;
        else
          d_v = v_1;
      }
      else {
        double t_d = (v_0-v_1)/d;

        if (t < t_d)
          d_v = -d*t;
        else
          d_v = v_1;        
      }
    }
  }

  return v_0+d_v;
}

int epos_velocity_profile_set_target(epos_device_p dev, int velocity) {
  return epos_device_write(dev, EPOS_VELOCITY_PROFILE_INDEX_TARGET, 0,
    (unsigned char*)&velocity, sizeof(int));
}
