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

#ifndef EPOS_MOTOR_H
#define EPOS_MOTOR_H

#include "device.h"

/** \brief Predefined EPOS motor constants
  */
#define EPOS_MOTOR_INDEX_TYPE                         0x6402
#define EPOS_MOTOR_INDEX_DATA                         0x6410
#define EPOS_MOTOR_SUBINDEX_MAX_CONTINUOUS_CURRENT    0x01
#define EPOS_MOTOR_SUBINDEX_MAX_OUTPUT_CURRENT        0x02

/** \brief Predefined EPOS motor error codes
  */
#define EPOS_MOTOR_ERROR_NONE                         0
#define EPOS_MOTOR_ERROR_INIT                         1
#define EPOS_MOTOR_ERROR_CLOSE                        2

/** \brief Predefined EPOS motor error descriptions
  */
extern const char* epos_motor_errors[];

/** \brief EPOS motor types
  */
typedef enum {
  epos_brushed_dc = 1,
  epos_brushless_ec_sin = 10,
  epos_brushless_ec_block = 11
} epos_motor_type_t;

/** \brief Structure defining an EPOS motor
  */
typedef struct epos_motor_t {
  epos_device_p dev;        //!< The EPOS device of the motor.

  epos_motor_type_t type;   //!< The motor type.
  short max_cont_current;   //!< The motor's continuous current limit in [mA].
  short max_out_current;    //!< The motor's output current limit in [mA].
} epos_motor_t, *epos_motor_p;

/** \brief Initialize EPOS motor
  * \param[in] dev The EPOS device the motor is connected to.
  * \param[in] sensor The EPOS motor to be initialized.
  * \param[in] type The type of the EPOS motor to be initialized.
  * \param[in] max_current The maximum output current of the EPOS motor to
  *   be initialized in [mA]. The continuous current limit will be set
  *   to half of this value.
  * \return The resulting error code.
  */
int epos_motor_init(
  epos_device_p dev,
  epos_motor_p motor,
  epos_motor_type_t type,
  short max_current);

/** \brief Close EPOS motor
  * \param[in] sensor The EPOS motor to be closed.
  * \return The resulting error code.
  */
int epos_motor_close(
  epos_motor_p motor);

/** \brief Retrieve EPOS motor type
  * \param[in] motor The EPOS motor to retrieve the type for.
  * \return The type of the specified EPOS motor.
  */
epos_motor_type_t epos_motor_get_type(
  epos_motor_p motor);

/** \brief Set EPOS motor type
  * \param[in] motor The EPOS motor to set the type for.
  * \param[in] type The type of the specified EPOS motor.
  * \return The resulting device error code.
  */
int epos_motor_set_type(
  epos_motor_p motor,
  epos_motor_type_t type);

/** \brief Retrieve an EPOS motor's continuous current limit
  * \param[in] motor The EPOS motor to retrieve the continuous current
  *   limit for.
  * \return The continuous current limit of the specified EPOS motor in [mA].
  */
short epos_motor_get_max_continuous_current(
  epos_motor_p motor);

/** \brief Set an EPOS motor's continuous current limit
  * \param[in] motor The EPOS motor to set the continuous current limit for.
  * \param[in] current The continuous current limit of the specified EPOS
  *   motor in [mA].
  * \return The resulting device error code.
  */
int epos_motor_set_max_continuous_current(
  epos_motor_p motor,
  short current);

/** \brief Retrieve an EPOS motor's output current limit
  * \param[in] motor The EPOS motor to retrieve the output current
  *   limit for.
  * \return The output current limit of the specified EPOS motor in [mA].
  */
short epos_motor_get_max_output_current(
  epos_motor_p motor);

/** \brief Set an EPOS motor's output current limit
  * \param[in] motor The EPOS motor to set the output current limit for.
  * \param[in] current The output current limit of the specified EPOS
  *   motor in [mA].
  * \return The resulting device error code.
  */
int epos_motor_set_max_output_current(
  epos_motor_p motor,
  short current);

#endif
