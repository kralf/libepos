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

/** \file gear.h
  * \brief EPOS gear functions
  */

/** \brief Structure defining an EPOS gear assembly
  */
typedef struct epos_gear_t {
  epos_sensor_t* sensor;           //!< The EPOS position sensor of the gear.

  float transmission;              //!< The gear transmission factor.
} epos_gear_t;

/** \brief Initialize an EPOS gear assembly
  * \param[in] gear The EPOS gear assembly to be initialized.
  * \param[in] sensor The EPOS position sensor the gear assembly is
  *   connected to.
  * \param[in] transmission The transmission factor of the EPOS gear to be
  *   initialized.
  */
void epos_gear_init(
  epos_gear_t* gear,
  epos_sensor_t* sensor,
  float transmission);

/** \brief Destroy an EPOS gear assembly
  * \param[in] gear The EPOS gear assembly to be destroyed.
  */
void epos_gear_destroy(
  epos_gear_t* gear);

/** \brief Convert EPOS position units to radian space angle
  * \param[in] gear The EPOS gear assembly to be used for conversion.
  * \param[in] position The number of EPOS position units to be converted
  *   into an angle.
  * \return The angle corresponding to the specified number of EPOS
  *   position units in [rad].
  */
float epos_gear_to_angle(
  const epos_gear_t* gear,
  int position);

/** \brief Convert radian space angle to EPOS position units
  * \param[in] gear The EPOS gear assembly to be used for conversion.
  * \param[in] angle The angle to be converted into EPOS position
  *   units in [rad].
  * \return The number of EPOS position units corresponding to the
  *   specified angle.
  */
int epos_gear_from_angle(
  const epos_gear_t* gear,
  float angle);

/** \brief Convert EPOS velocity units to radian space angular velocity
  * \param[in] gear The EPOS gear assembly to be used for conversion.
  * \param[in] velocity The number of EPOS velocity units to be converted
  *   into an angular velocity.
  * \return The angular velocity corresponding to the specified number of
  *   EPOS velocity units in [rad/s].
  */
float epos_gear_to_angular_velocity(
  const epos_gear_t* gear,
  int velocity);

/** \brief Convert radian space angular velocity to EPOS velocity units
  * \param[in] gear The EPOS gear assembly to be used for conversion.
  * \param[in] angular_vel The angular velocity to be converted into EPOS
  *   velocity units in [rad/s].
  * \return The number of EPOS velocity units corresponding to the
  *   specified angular velocity.
  */
int epos_gear_from_angular_velocity(
  const epos_gear_t* gear,
  float angular_vel);

/** \brief Convert EPOS acceleration units to radian space angular acceleration
  * \param[in] gear The EPOS gear assembly to be used for conversion.
  * \param[in] acceleration The number of EPOS acceleration units to be
  *   converted into an angular acceleration.
  * \return The angular acceleration corresponding to the specified number of
  *   EPOS velocity units in [rad/s^2].
  */
float epos_gear_to_angular_acceleration(
  const epos_gear_t* gear,
  int acceleration);

/** \brief Convert radian space angular acceleration to EPOS acceleration units
  * \param[in] gear The EPOS gear assembly to be used for conversion.
  * \param[in] angular_acc The angular acceleration to be converted into EPOS
  *   acceleration units in [rad/s^2].
  * \return The number of EPOS acceleration units corresponding to the
  *   specified angular acceleration.
  */
int epos_gear_from_angular_acceleration(
  const epos_gear_t* gear,
  float angular_acc);

#endif
