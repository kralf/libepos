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

#ifndef EPOS_GEAR_H
#define EPOS_GEAR_H

#include "sensor.h"

/** \brief Predefined EPOS gear error codes
  */
#define EPOS_GEAR_ERROR_NONE                    0
#define EPOS_GEAR_ERROR_INIT                    1
#define EPOS_GEAR_ERROR_CLOSE                   2

/** \brief Predefined EPOS gear error descriptions
  */
extern const char* epos_gear_errors[];

/** \brief Structure defining an EPOS gear assembly
  */
typedef struct epos_gear_t {
  epos_sensor_p sensor;            //!< The EPOS position sensor of the gear.

  float transmission;              //!< The gear transmission factor.
} epos_gear_t, *epos_gear_p;

/** \brief Initialize an EPOS gear assembly
  * \param[in] sensor The EPOS position sensor the gear assembly is
  *   connected to.
  * \param[in] gear The EPOS gear assembly to be initialized.
  * \param[in] transmission The transmission factor of the EPOS gear to be
  *   initialized.
  * \return The resulting error code.
  */
int epos_gear_init(
  epos_sensor_p sensor,
  epos_gear_p gear,
  float transmission);

/** \brief Close an EPOS gear assembly
  * \param[in] gear The EPOS gear assembly to be closed.
  * \return The resulting error code.
  */
int epos_gear_close(
  epos_gear_p gear);

/** \brief Convert EPOS position sensor steps to angle
  * \param[in] gear The EPOS gear assembly to be used for conversion.
  * \param[in] num_steps The number of EPOS position sensor steps to be
  *   converted into an angle.
  * \return The angle corresponding to the specified number of steps in [rad].
  */
float epos_gear_steps_to_angle(
  epos_gear_p gear,
  int num_steps);

/** \brief Convert an angle to EPOS position sensor steps
  * \param[in] gear The EPOS gear assembly to be used for conversion.
  * \param[in] angle The angle to be converted into EPOS position sensor
  *   steps in [rad].
  * \return The number of steps corresponding to the specified angle.
  */
int epos_gear_angle_to_steps(
  epos_gear_p gear,
  float angle);

#endif
