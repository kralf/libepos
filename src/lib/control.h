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

/** \file control.h
  * \brief EPOS generic control functions
  */

/** \name Constants
  * \brief Predefined EPOS control constants
  */
//@{
#define EPOS_CONTROL_START_SLEEP             0.01
//@}

/** \name Object Indexes
  * \brief Predefined EPOS control object indexes
  */
//@{
#define EPOS_CONTROL_INDEX_MODE              0x6060
#define EPOS_CONTROL_INDEX_MODE_DISPLAY      0x6061
//@}

/** \brief EPOS controller modes
  */
typedef enum {
  epos_control_homing,
  epos_control_profile_vel,
  epos_control_profile_pos,
  epos_control_position,
  epos_control_velocity,
  epos_control_current,
  epos_control_diagnostic,
  epos_control_master_enc,
  epos_control_step_dir
} epos_control_mode_t;

/** \brief Structure defining an EPOS controller
  */
typedef struct epos_control_t {
  epos_device_p dev;          //!< The EPOS device of the controller.

  epos_control_mode_t mode;   //!< The controller mode.
} epos_control_t, *epos_control_p;

/** \brief Initialize EPOS controller
  * \param[in] control The EPOS controller to be initialized.
  * \param[in] dev The controller's EPOS device.
  * \param[in] mode The mode of the EPOS controller to be initialized.
  */
void epos_control_init(
  epos_control_p control,
  epos_device_p dev,
  epos_control_mode_t mode);

/** \brief Destroy EPOS controller
  * \param[in] control The EPOS controller to be destroyed.
  */
void epos_control_destroy(
  epos_control_p control);

/** \brief Retrieve EPOS controller mode
  * \param[in] control The EPOS controller to retrieve the mode for.
  * \return The mode of the specified EPOS controller.
  */
epos_control_mode_t epos_control_get_mode(
  epos_control_p control);

/** \brief Set EPOS controller mode
  * \param[in] control The EPOS controller to set the mode for.
  * \param[in] mode The mode of the specified EPOS controller.
  * \return The resulting device error code.
  */
int epos_control_set_mode(
  epos_control_p control,
  epos_control_mode_t mode);

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
