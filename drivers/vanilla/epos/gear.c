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
#include <math.h>
#include <limits.h>
#include <float.h>

#include "gear.h"

void epos_gear_init(epos_gear_p gear, epos_sensor_p sensor, float
  transmission) {
  gear->sensor = sensor;
  gear->transmission = transmission;
}

void epos_gear_destroy(epos_gear_p gear) {
  gear->sensor = 0;
}

float epos_gear_to_angle(epos_gear_p gear, int position) {
  return 2.0*M_PI*position/(4.0*gear->sensor->num_pulses*gear->transmission);
}

int epos_gear_from_angle(epos_gear_p gear, float angle) {
  return clip(angle*4.0*gear->sensor->num_pulses*gear->transmission/(2.0*M_PI),
    INT_MIN, INT_MAX);
}

float epos_gear_to_angular_velocity(epos_gear_p gear, int velocity) {
  return 2.0*M_PI*velocity/(60.0*gear->transmission);
}

int epos_gear_from_angular_velocity(epos_gear_p gear, float angular_vel) {
  return clip(angular_vel*60.0*gear->transmission/(2.0*M_PI),
    INT_MIN, INT_MAX);
}

float epos_gear_to_angular_acceleration(epos_gear_p gear, int acceleration) {
  return 2.0*M_PI*acceleration/(60.0*gear->transmission);
}

int epos_gear_from_angular_acceleration(epos_gear_p gear, float angular_acc) {
  return clip(angular_acc*60.0*gear->transmission/(2.0*M_PI), 1, UINT_MAX);
}
