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

#ifndef EPOS_GLOBAL_H
#define EPOS_GLOBAL_H

#define EPOS_MOTOR_SENSOR_3CHAN 0x01
#define EPOS_MOTOR_SENSOR_2CHAN 0x02
#define EPOS_MOTOR_SENSOR_HALL 0x03

#define EPOS_MOTOR_MODE_POSITION 0xFF
#define EPOS_MOTOR_MODE_VELOCITY 0xFE
#define EPOS_MOTOR_MODE_PROFILE_POSITION 0x01
#define EPOS_MOTOR_MODE_PROFILE_VELOCITY 0x03
#define EPOS_MOTOR_MODE_HOMING 0x06
#define EPOS_MOTOR_MODE_CURRENT 0xFD

#define EPOS_MOTOR_HOME_POSITIVE_LIMIT_SWITCH 0x02
#define EPOS_MOTOR_HOME_NEGATIVE_LIMIT_SWITCH 0x01
#define EPOS_MOTOR_HOME_POSITIVE_CURRENT_THRESHOLD 0xFD
#define EPOS_MOTOR_HOME_NEGATIVE_CURRENT_THRESHOLD 0xFC

#define EPOS_MOTOR_ERROR_HISTORY 30

#endif
