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

#include "motor.h"

const char* epos_motor_errors[] = {
  "success",
  "error initializing EPOS motor",
  "error closing EPOS motor",
};

int epos_motor_init(epos_device_p dev, epos_motor_p motor, epos_motor_type_t
  type, float max_current) {
  motor->dev = dev;

  if (epos_motor_set_type(motor, type) ||
    epos_motor_set_max_continuous_current(motor, 0.5*max_current*1e3) ||
    epos_motor_set_max_output_current(motor, max_current*1e3)) {
    motor->dev = 0;
    return EPOS_MOTOR_ERROR_INIT;
  }
  else
    return EPOS_MOTOR_ERROR_NONE;
}

int epos_motor_close(epos_motor_p motor) {
  if (motor->dev) {
    motor->dev = 0;
    return EPOS_MOTOR_ERROR_NONE;
  }
  else
    return EPOS_MOTOR_ERROR_CLOSE;
}

epos_motor_type_t epos_motor_get_type(epos_motor_p motor) {
  short type;
  epos_device_read(motor->dev, EPOS_MOTOR_INDEX_TYPE, 0,
    (unsigned char*)&type, sizeof(short));

  return type;
}

int epos_motor_set_type(epos_motor_p motor, epos_motor_type_t type) {
  short t = type;
  int result = epos_device_write(motor->dev, EPOS_MOTOR_INDEX_TYPE, 0,
    (unsigned char*)&t, sizeof(short));

  if (!result)
    motor->type = type;

  return result;
}

short epos_motor_get_max_continuous_current(epos_motor_p motor) {
  short current;
  epos_device_read(motor->dev, EPOS_MOTOR_INDEX_DATA,
    EPOS_MOTOR_SUBINDEX_MAX_CONTINUOUS_CURRENT, (unsigned char*)&current,
    sizeof(short));

  return current;
}

int epos_motor_set_max_continuous_current(epos_motor_p motor, short current) {
  int result = epos_device_write(motor->dev, EPOS_MOTOR_INDEX_DATA,
    EPOS_MOTOR_SUBINDEX_MAX_CONTINUOUS_CURRENT, (unsigned char*)&current,
    sizeof(short));

  if (!result)
    motor->max_cont_current = current;

  return result;
}

short epos_motor_get_max_output_current(epos_motor_p motor) {
  short current;
  epos_device_read(motor->dev, EPOS_MOTOR_INDEX_DATA,
    EPOS_MOTOR_SUBINDEX_MAX_OUTPUT_CURRENT, (unsigned char*)&current,
    sizeof(short));

  return current;
}

int epos_motor_set_max_output_current(epos_motor_p motor, short current) {
  int result = epos_device_write(motor->dev, EPOS_MOTOR_INDEX_DATA,
    EPOS_MOTOR_SUBINDEX_MAX_OUTPUT_CURRENT, (unsigned char*)&current,
    sizeof(short));

  if (!result)
    motor->max_out_current = current;

  return result;
}
