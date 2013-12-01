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

short epos_sensor_types[] = {
  1,
  2,
  3,
};

short epos_sensor_polarities[] = {
  0,
  3,
};

void epos_sensor_init(epos_sensor_t* sensor, epos_device_t* dev,
    epos_sensor_type_t type, epos_sensor_polarity_t polarity,
    int num_pulses, epos_sensor_supervision_t supervision) {
  sensor->dev = dev;

  sensor->type = type;
  sensor->polarity = polarity;
  sensor->num_pulses = num_pulses;

  sensor->supervision = supervision;
}

void epos_sensor_destroy(epos_sensor_t* sensor) {
  sensor->dev = 0;
}

int epos_sensor_setup(epos_sensor_t* sensor) {
  if (!epos_sensor_set_type(sensor, sensor->type) &&
      !epos_sensor_set_polarity(sensor, sensor->polarity) &&
      !epos_sensor_set_pulses(sensor, sensor->num_pulses))
    epos_sensor_set_supervision(sensor, sensor->supervision);

  return sensor->dev->error.code;
}

epos_sensor_type_t epos_sensor_get_type(epos_sensor_t* sensor) {
  short type = 0;
  
  if (epos_device_read(sensor->dev, EPOS_SENSOR_INDEX_CONFIGURATION,
      EPOS_SENSOR_SUBINDEX_TYPE, (unsigned char*)&type, sizeof(short)) > 0) {
    int i;
    for (i = 0; i < sizeof(epos_sensor_types)/sizeof(short); ++i)
      if (epos_sensor_types[i] == type)
        return i;
  }
  else
    return -1;

  return type;
}

int epos_sensor_set_type(epos_sensor_t* sensor, epos_sensor_type_t type) {
  if (epos_device_write(sensor->dev, EPOS_SENSOR_INDEX_CONFIGURATION,
      EPOS_SENSOR_SUBINDEX_TYPE, (unsigned char*)&epos_sensor_types[type],
      sizeof(short)) > 0)
    sensor->type = type;

  return sensor->dev->error.code;
}

epos_sensor_polarity_t epos_sensor_get_polarity(epos_sensor_t* sensor) {
  short polarity = 0;
  
  if (epos_device_read(sensor->dev, EPOS_SENSOR_INDEX_CONFIGURATION,
      EPOS_SENSOR_SUBINDEX_POLARITY, (unsigned char*)&polarity,
      sizeof(short)) > 0) {
    int i;
    for (i = 0; i < sizeof(epos_sensor_polarities)/sizeof(short); ++i)
      if (epos_sensor_polarities[i] == polarity)
        return i;
  }
  else
    return -1;
  
  return polarity;
}

int epos_sensor_set_polarity(epos_sensor_t* sensor, epos_sensor_polarity_t
    polarity) {
  if (epos_device_write(sensor->dev, EPOS_SENSOR_INDEX_CONFIGURATION,
      EPOS_SENSOR_SUBINDEX_POLARITY,
      (unsigned char*)&epos_sensor_polarities[polarity], sizeof(short)) > 0)
    sensor->polarity = polarity;

  return sensor->dev->error.code;
}

int epos_sensor_get_pulses(epos_sensor_t* sensor) {
  int pulses = 0;

  if (sensor->dev->hardware_generation == 1) {
    short pulses_short;
    if (epos_device_read(sensor->dev, EPOS_SENSOR_INDEX_CONFIGURATION,
        EPOS_SENSOR_SUBINDEX_PULSES, (unsigned char*)&pulses_short,
        sizeof(short)) > 0)
      pulses = pulses_short;
  }
  else
    epos_device_read(sensor->dev, EPOS_SENSOR_INDEX_CONFIGURATION,
      EPOS_SENSOR_SUBINDEX_PULSES, (unsigned char*)&pulses, sizeof(int));

  return pulses;
}

int epos_sensor_set_pulses(epos_sensor_t* sensor, int num_pulses) {
  if (sensor->dev->hardware_generation == 1) {
    short pulses_short = num_pulses;;    
    epos_device_write(sensor->dev, EPOS_SENSOR_INDEX_CONFIGURATION,
      EPOS_SENSOR_SUBINDEX_PULSES, (unsigned char*)&pulses_short,
      sizeof(short));
  }
  else
    epos_device_write(sensor->dev, EPOS_SENSOR_INDEX_CONFIGURATION,
      EPOS_SENSOR_SUBINDEX_PULSES, (unsigned char*)&num_pulses, sizeof(int));

  if (!sensor->dev->error.code)
    sensor->num_pulses = num_pulses;

  return sensor->dev->error.code;
}

epos_sensor_supervision_t epos_sensor_get_supervision(epos_sensor_t* sensor) {
  short configuration = epos_device_get_configuration(sensor->dev);
  
  if (!sensor->dev->error.code)
    return configuration & 0x0003  ;
  else
    return -1;
}

int epos_sensor_set_supervision(epos_sensor_t* sensor,
    epos_sensor_supervision_t supervision) {
  short configuration = epos_device_get_configuration(sensor->dev);
  error_return(&sensor->dev->error);

  configuration = supervision | (0xFFFC & configuration);
  if (!epos_device_set_configuration(sensor->dev, configuration))
    sensor->supervision = supervision;

  return sensor->dev->error.code;
}

short epos_sensor_get_position(epos_sensor_t* sensor) {
  short pos = 0;
  epos_device_read(sensor->dev, EPOS_SENSOR_INDEX_POSITION, 0,
    (unsigned char*)&pos, sizeof(short));

  return pos;
}
