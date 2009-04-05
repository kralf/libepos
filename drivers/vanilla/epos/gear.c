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

#include "gear.h"

const char* epos_gear_errors[] = {
  "success",
  "error initializing EPOS gear",
  "error closing EPOS gear",
};

int epos_gear_init(epos_sensor_p sensor, epos_gear_p gear, float
  transmission) {
  gear->sensor = sensor;
  gear->transmission = transmission;

  return EPOS_GEAR_ERROR_NONE;
}

int epos_gear_close(epos_gear_p gear) {
  if (gear->sensor) {
    gear->sensor = 0;
    return EPOS_GEAR_ERROR_NONE;
  }
  else
    return EPOS_GEAR_ERROR_CLOSE;
}

float epos_gear_steps_to_angle(epos_gear_p gear, int num_steps) {
  return 2.0*M_PI*num_steps/gear->sensor->num_pulses/gear->transmission;
}

int epos_gear_angle_to_steps(epos_gear_p gear, float angle) {
  return angle*gear->transmission*gear->sensor->num_pulses/(2.0*M_PI);
}
