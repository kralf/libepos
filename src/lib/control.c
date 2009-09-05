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

#include "control.h"

void epos_control_init(epos_control_p control, epos_device_p dev,
  epos_control_type_t type) {
  control->dev = dev;
}

void epos_control_destroy(epos_control_p control) {
  if (control->dev)
    control->dev = 0;
}

epos_control_type_t epos_control_get_type(epos_control_p control) {
  char type;
  epos_device_read(control->dev, EPOS_CONTROL_INDEX_MODE_DISPLAY, 0, &type, 1);

  return type;
}

int epos_control_set_type(epos_control_p control, epos_control_type_t type) {
  char t = type;
  int result = epos_device_write(control->dev, EPOS_CONTROL_INDEX_MODE, 0,
    &t, 1);

  if (!result)
    control->type = type;

  return result;
}

int epos_control_start(epos_control_p control) {
  int result = epos_device_set_control(control->dev,
    EPOS_DEVICE_CONTROL_ENABLE_OPERATION);

  if (!result)
    timer_sleep(EPOS_CONTROL_START_SLEEP);

  return result;
}

int epos_control_stop(epos_control_p control) {
  return epos_device_set_control(control->dev,
    EPOS_DEVICE_CONTROL_QUICK_STOP);
}
