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

short epos_motor_types[] = {
   1,
  10,
  11,
};

void epos_motor_init(epos_motor_t* motor, epos_device_t* dev,
    epos_motor_type_t type, float max_current) {
  motor->dev = dev;

  motor->type = type;
  motor->max_cont_current = 0.5*max_current;
  motor->max_out_current = max_current;
  motor->num_poles = 1;
}

void epos_motor_destroy(epos_motor_t* motor) {
  motor->dev = 0;
}

int epos_motor_setup(epos_motor_t* motor) {
  if (!epos_motor_set_type(motor, motor->type) &&
      !epos_motor_set_max_continuous_current(motor,
        motor->max_cont_current*1e3) &&
    epos_motor_set_max_output_current(motor, motor->max_out_current*1e3));

  return motor->dev->error.code;
}

epos_motor_type_t epos_motor_get_type(epos_motor_t* motor) {
  short type = 0;
  
  if (epos_device_read(motor->dev, EPOS_MOTOR_INDEX_TYPE, 0,
      (unsigned char*)&type, sizeof(short)) > 0) {
    int i;
    for (i = 0; i < sizeof(epos_motor_types)/sizeof(short); ++i)
      if (epos_motor_types[i] == type)
        return i;
  }
  else
    return -1;
    
  return type;
}

int epos_motor_set_type(epos_motor_t* motor, epos_motor_type_t type) {
  if (epos_device_write(motor->dev, EPOS_MOTOR_INDEX_TYPE, 0,
      (unsigned char*)&epos_motor_types[type], sizeof(short)) > 0)
    motor->type = type;

  return motor->dev->error.code;
}

short epos_motor_get_max_continuous_current(epos_motor_t* motor) {
  short current = 0;
  epos_device_read(motor->dev, EPOS_MOTOR_INDEX_DATA,
    EPOS_MOTOR_SUBINDEX_MAX_CONTINUOUS_CURRENT, (unsigned char*)&current,
    sizeof(short));

  return current;
}

int epos_motor_set_max_continuous_current(epos_motor_t* motor, short current) {
  if (epos_device_write(motor->dev, EPOS_MOTOR_INDEX_DATA,
      EPOS_MOTOR_SUBINDEX_MAX_CONTINUOUS_CURRENT, (unsigned char*)&current,
      sizeof(short)) > 0)
    motor->max_cont_current = current*1e-3;

  return motor->dev->error.code;
}

short epos_motor_get_max_output_current(epos_motor_t* motor) {
  short current = 0;
  epos_device_read(motor->dev, EPOS_MOTOR_INDEX_DATA,
    EPOS_MOTOR_SUBINDEX_MAX_OUTPUT_CURRENT, (unsigned char*)&current,
    sizeof(short));

  return current;
}

int epos_motor_set_max_output_current(epos_motor_t* motor, short current) {
  if (epos_device_write(motor->dev, EPOS_MOTOR_INDEX_DATA,
      EPOS_MOTOR_SUBINDEX_MAX_OUTPUT_CURRENT, (unsigned char*)&current,
      sizeof(short)) > 0)
    motor->max_out_current = current*1e-3;

  return motor->dev->error.code;
}

short epos_motor_get_num_poles(epos_motor_t* motor) {
  short num_poles = 0;
  epos_device_read(motor->dev, EPOS_MOTOR_INDEX_DATA,
    EPOS_MOTOR_SUBINDEX_NUM_POLES, (unsigned char*)&num_poles,
    sizeof(short));

  return num_poles;
}

int epos_motor_set_num_poles(epos_motor_t* motor, short num_poles) {
  if (epos_device_write(motor->dev, EPOS_MOTOR_INDEX_DATA,
      EPOS_MOTOR_SUBINDEX_NUM_POLES, (unsigned char*)&num_poles,
      sizeof(short)) > 0)
    motor->num_poles = num_poles;

  return motor->dev->error.code;
}
