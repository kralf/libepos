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

#include "sensor.h"

const char* epos_sensor_errors[] = {
  "success",
  "error setting EPOS sensor parameters",
};

void epos_sensor_init(epos_sensor_p sensor, epos_device_p dev,
  epos_sensor_type_t type, epos_sensor_polarity_t polarity, short num_pulses) {
  sensor->dev = dev;

  sensor->type = type;
  sensor->polarity = polarity;
  sensor->num_pulses = num_pulses;
}

void epos_sensor_destroy(epos_sensor_p sensor) {
  sensor->dev = 0;
}

int epos_sensor_setup(epos_sensor_p sensor) {
  if (!epos_sensor_set_type(sensor, sensor->type) &&
    !epos_sensor_set_polarity(sensor, sensor->polarity) &&
    !epos_sensor_set_pulses(sensor, sensor->num_pulses))
    return EPOS_SENSOR_ERROR_NONE;
  else
    return EPOS_SENSOR_ERROR_SETUP;
}

epos_sensor_type_t epos_sensor_get_type(epos_sensor_p sensor) {
  short type;
  epos_device_read(sensor->dev, EPOS_SENSOR_INDEX_CONFIGURATION,
    EPOS_SENSOR_SUBINDEX_TYPE, (unsigned char*)&type, sizeof(short));

  return type;
}

int epos_sensor_set_type(epos_sensor_p sensor, epos_sensor_type_t type) {
  short t = type;
  int result = epos_device_write(sensor->dev, EPOS_SENSOR_INDEX_CONFIGURATION,
    EPOS_SENSOR_SUBINDEX_TYPE, (unsigned char*)&t, sizeof(short));

  if (!result)
    sensor->type = type;

  return result;
}

epos_sensor_polarity_t epos_sensor_get_polarity(epos_sensor_p sensor) {
  short polarity;
  epos_device_read(sensor->dev, EPOS_SENSOR_INDEX_CONFIGURATION,
    EPOS_SENSOR_SUBINDEX_POLARITY, (unsigned char*)&polarity, sizeof(short));

  return polarity;
}

int epos_sensor_set_polarity(epos_sensor_p sensor, epos_sensor_polarity_t
  polarity) {
  short p = polarity;
  int result = epos_device_write(sensor->dev, EPOS_SENSOR_INDEX_CONFIGURATION,
    EPOS_SENSOR_SUBINDEX_POLARITY, (unsigned char*)&p, sizeof(short));

  if (!result)
    sensor->polarity = polarity;

  return result;
}

short epos_sensor_get_pulses(epos_sensor_p sensor) {
  short pulses;
  epos_device_read(sensor->dev, EPOS_SENSOR_INDEX_CONFIGURATION,
    EPOS_SENSOR_SUBINDEX_PULSES, (unsigned char*)&pulses, sizeof(short));

  return pulses;
}

int epos_sensor_set_pulses(epos_sensor_p sensor, short num_pulses) {
  int result = epos_device_write(sensor->dev, EPOS_SENSOR_INDEX_CONFIGURATION,
    EPOS_SENSOR_SUBINDEX_PULSES, (unsigned char*)&num_pulses, sizeof(short));

  if (!result)
    sensor->num_pulses = num_pulses;

  return result;
}

short epos_sensor_get_position(epos_sensor_p sensor) {
  short pos;
  epos_device_read(sensor->dev, EPOS_SENSOR_INDEX_POSITION, 0,
    (unsigned char*)&pos, sizeof(short));

  return pos;
}
