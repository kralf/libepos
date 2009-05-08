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

#ifndef EPOS_CONTROL_H
#define EPOS_CONTROL_H

#include "device.h"

/** \brief Predefined EPOS control constants
  */
#define EPOS_CONTROL_INDEX_MODE              0x6060
#define EPOS_CONTROL_INDEX_MODE_DISPLAY      0x6061

/** \brief EPOS controller types
  */
typedef enum {
  epos_homing = 6,
  epos_profile_vel = 3,
  epos_profile_pos = 1,
  epos_position = -1,
  epos_velocity = -2,
  epos_current = -3,
  epos_diagnostic = -4,
  epos_master_enc = -5,
  epos_step_dir = -6
} epos_control_type_t;

/** \brief Structure defining an EPOS controller
  */
typedef struct epos_control_t {
  epos_device_p dev;          //!< The EPOS device of the controller.

  epos_control_type_t type;   //!< The controller type.
} epos_control_t, *epos_control_p;

/** \brief Initialize EPOS controller
  * \param[in] control The EPOS controller to be initialized.
  * \param[in] dev The controller's EPOS device.
  * \param[in] type The type of the EPOS controller to be initialized.
  */
void epos_control_init(
  epos_control_p control,
  epos_device_p dev,
  epos_control_type_t type);

/** \brief Destroy EPOS controller
  * \param[in] sensor The EPOS controller to be destroyed.
  */
void epos_control_destroy(
  epos_control_p control);

/** \brief Retrieve EPOS controller type
  * \param[in] control The EPOS controller to retrieve the type for.
  * \return The type of the specified EPOS controller.
  */
epos_control_type_t epos_control_get_type(
  epos_control_p control);

/** \brief Set EPOS controller type
  * \param[in] control The EPOS controller to set the type for.
  * \param[in] type The type of the specified EPOS controller.
  * \return The resulting device error code.
  */
int epos_control_set_type(
  epos_control_p control,
  epos_control_type_t type);

/** \brief Start EPOS controller
  * \param[in] control The EPOS controller to be started.
  * \return The resulting device error code.
  */
int epos_control_start(
  epos_control_p control);

/** \brief Stop EPOS controller
  * \param[in] control The EPOS controller to be stopped.
  * \return The resulting device error code.
  */
int epos_control_stop(
  epos_control_p control);

#endif
