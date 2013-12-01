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

#include "control.h"

char epos_control_modes[] = {
   6,
   3,
   1,
  -1,
  -2,
  -3,
  -4,
  -5,
  -6,
};

void epos_control_init(epos_control_t* control, epos_device_t* dev,
    epos_control_mode_t mode) {
  control->dev = dev;
  control->mode = mode;
}

void epos_control_destroy(epos_control_t* control) {
  if (control->dev)
    control->dev = 0;
}

epos_control_mode_t epos_control_get_mode(epos_control_t* control) {
  unsigned char mode = 0;
  
  if (epos_device_read(control->dev, EPOS_CONTROL_INDEX_MODE_DISPLAY, 0,
      &mode, 1) > 0) {
    int i;
    for (i = 0; i < sizeof(epos_control_modes); ++i)
      if (epos_control_modes[i] == mode)
        return i;
  }
  else
    return -1;
    
  return mode;
}

int epos_control_set_mode(epos_control_t* control, epos_control_mode_t mode) {
  if (epos_device_write(control->dev, EPOS_CONTROL_INDEX_MODE, 0,
      (unsigned char*)&epos_control_modes[mode], 1) > 0)
    control->mode = mode;

  return control->dev->error.code;
}

int epos_control_start(epos_control_t* control) {
  if (!epos_device_set_control(control->dev,
      EPOS_DEVICE_CONTROL_ENABLE_OPERATION))
    timer_sleep(EPOS_CONTROL_START_SLEEP);

  return control->dev->error.code;
}

int epos_control_stop(epos_control_t* control) {
  return epos_device_set_control(control->dev,
    EPOS_DEVICE_CONTROL_QUICK_STOP);
}
