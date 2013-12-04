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
#include <string.h>
#include <math.h>

#include <timer/timer.h>

#include "interpolated_position.h"

#include "gear.h"
#include "macros.h"

const char* epos_interpolated_position_errors[] = {
  "Success",
  "Profile undefined at value",
};

void epos_interpolated_position_init(epos_interpolated_position_t* profile,
    const epos_interpolated_position_knot_t* knots, size_t num_knots) {
  if (knots && num_knots) {
    profile->knots = malloc(num_knots*
      sizeof(epos_interpolated_position_knot_t));
    memcpy(profile->knots, knots, num_knots*
      sizeof(epos_interpolated_position_knot_t));
    
    profile->num_knots = num_knots;
  }
  else {
    profile->knots = 0;
    profile->num_knots = 0;
  }
  
  profile->start_knot.time = 0.0;
  profile->start_knot.position = 0.0;
  profile->start_knot.velocity = 0.0;
}

void epos_interpolated_position_init_spline(epos_interpolated_position_t*
    profile, const spline_t* spline) {
  if (spline->num_knots > 1) {
    profile->knots = malloc((spline->num_knots-1)*
      sizeof(epos_interpolated_position_knot_t));
    
    size_t i;
    for (i = 0; i < spline->num_knots; ++i) {
      if (i) {
        profile->knots[i-1].time = spline->knots[i].x;
        profile->knots[i-1].position = spline->knots[i].y;
        profile->knots[i-1].velocity = spline_knot_eval(
          &spline->knots[i-1], &spline->knots[i], 
          spline_eval_type_first_derivative, spline->knots[i].x);
      }
      else {
        profile->start_knot.time = spline->knots[0].x;
        profile->start_knot.position = spline->knots[0].y;
        profile->start_knot.velocity = spline_knot_eval(
          &spline->knots[0], &spline->knots[1], 
          spline_eval_type_first_derivative, spline->knots[0].x);
      }
    }
    
    profile->num_knots = spline->num_knots-1;
  }
  else {
    profile->start_knot.time = 0.0;
    profile->start_knot.position = 0.0;
    profile->start_knot.velocity = 0.0;
    
    profile->knots = 0;
    profile->num_knots = 0;
  }  
}

void epos_interpolated_position_destroy(epos_interpolated_position_t*
    profile) {
  if (profile->num_knots) {
    free(profile->knots);
    
    profile->knots = 0;
    profile->num_knots = 0;
  }
}

int epos_interpolated_position_start(epos_node_t* node,
    epos_interpolated_position_t* profile) {
  return EPOS_DEVICE_ERROR_NONE;
}

int epos_interpolated_position_stop(epos_node_t* node) {
  return epos_control_stop(&node->control);
}

ssize_t epos_interpolated_position_find_segment(const
    epos_interpolated_position_t* profile, double time) {
  return epos_interpolated_position_find_segment_bisect(profile, time,
    0, profile->num_knots > 0 ? profile->num_knots : 0);
}

ssize_t epos_interpolated_position_find_segment_bisect(const
    epos_interpolated_position_t* profile, double time, size_t
    index_min, size_t index_max) {
  if (profile->num_knots) {
    size_t i = (index_min < profile->num_knots) ? index_min : 
      profile->num_knots-1;
    size_t j = (index_max <= profile->num_knots) ? index_max : 
      profile->num_knots;
      
    if ((j > i) && (time >= i ? profile->knots[i-1].time :
        profile->start_knot.time) && (time <= profile->knots[j-1].time)) {    
      while (j-i > 1) {
        size_t k = (i+j) >> 1;
        if (time < k ? profile->knots[k-1].time : profile->start_knot.time)
          j = k;
        else
          i = k;
      }
      
      return i;
    }
  }

  return -EPOS_INTERPOLATED_POSITION_ERROR_UNDEFINED;
}

ssize_t epos_interpolated_position_find_segment_linear(const
    epos_interpolated_position_t* profile, double time, size_t index_start) {
  if (profile->num_knots && (time >= profile->start_knot.time) &&
      (time <= profile->knots[profile->num_knots-1].time)) {
    size_t i = (index_start <= profile->num_knots) ? index_start : 
      profile->num_knots;
      
    while (1) {
      if (time >= i ? profile->knots[i-1].time : profile->start_knot.time) {
        if (time <= profile->knots[i].time)
          return i;
        else
          ++i;
      }
      else
        --i;
    }
  }
  
  return -EPOS_INTERPOLATED_POSITION_ERROR_UNDEFINED;
}

epos_profile_value_t epos_interpolated_position_eval_segment(const
    epos_interpolated_position_t* profile, size_t index, double time) {
  epos_profile_value_t values = {NAN, NAN, NAN};
  
  if (index <= profile->num_knots) {
    double t_j = index ? profile->knots[index-1].time :
      profile->start_knot.time;
    double t_i = profile->knots[index].time;
    
    if ((time >= t_j) && (time <= t_i)) {
      float p_j = (index > 0) ? profile->knots[index-1].position :
        profile->start_knot.position;
      float v_j = (index > 0) ? profile->knots[index-1].velocity :
        profile->start_knot.velocity;
      float p_i = profile->knots[index].position;
      float v_i = profile->knots[index].velocity;
      
      double a = (-2.0*(p_i-p_j)+(t_i-t_j)*(v_i+v_j))/cub(t_i-t_j);
      double b = (3.0*(p_i-p_j)-(t_i-t_j)*(v_i+2.0*v_j))/sqr(t_i-t_j);
      double c = v_j;
      double d = p_j;
      double t = time-t_j;
      
      values.position = a*cub(t)+b*sqr(t)+c*t+d;
      values.velocity = 3.0*a*sqr(t)+2.0*b*t+c;
      values.acceleration = 6.0*a*t+2.0*b;
    }
  }
  
  return values;
}

epos_profile_value_t epos_interpolated_position_eval(const
    epos_interpolated_position_t* profile, double time) {
  return epos_interpolated_position_eval_bisect(profile, time, 0,
    profile->num_knots ? profile->num_knots : 0);
}

epos_profile_value_t epos_interpolated_position_eval_bisect(const
    epos_interpolated_position_t* profile, double time, size_t
    index_min, size_t index_max) {
  ssize_t i;
  
  if ((i = epos_interpolated_position_find_segment_bisect(profile, time,
      index_min, index_max)) >= 0)
    return epos_interpolated_position_eval_segment(profile, i, time);
  else {
    epos_profile_value_t values = {NAN, NAN, NAN};
    return values;
  }
}

epos_profile_value_t epos_interpolated_position_eval_linear(const
    epos_interpolated_position_t* profile, double time, size_t* index) {
  ssize_t i;
  
  if ((i = epos_interpolated_position_find_segment_linear(profile, time,
      *index)) >= 0) {
    *index = i;
    return epos_interpolated_position_eval_segment(profile, i, time);
  }
  else {
    epos_profile_value_t values = {NAN, NAN, NAN};
    return values;
  }
}
