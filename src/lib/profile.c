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

#include "profile.h"

int epos_profile_wait(epos_node_t* node, double timeout) {
  return epos_device_wait_status(&node->dev, EPOS_PROFILE_STATUS_REACHED,
    timeout);
}

int epos_profile_set_acceleration(epos_device_t* dev, unsigned int
    acceleration) {
  epos_device_write(dev, EPOS_PROFILE_INDEX_ACCELERATION, 0,
    (unsigned char*)&acceleration, sizeof(unsigned int));
  
  return dev->error.code;
}

int epos_profile_set_deceleration(epos_device_t* dev, unsigned int
    deceleration) {
  epos_device_write(dev, EPOS_PROFILE_INDEX_DECELERATION, 0,
    (unsigned char*)&deceleration, sizeof(unsigned int));
  
  return dev->error.code;
}

int epos_profile_set_type(epos_device_t* dev, epos_profile_type_t type) {
  epos_device_write(dev, EPOS_PROFILE_INDEX_TYPE, 0,
    (unsigned char*)&type, sizeof(short));
  
  return dev->error.code;
}
