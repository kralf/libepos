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

/** \brief EPOS controller operating modes
  */
typedef enum {
  epos_control_homing,        //!< Homing operating mode.
  epos_control_profile_vel,   //!< Profile velocity operating mode.
  epos_control_profile_pos,   //!< Profile position operating mode.
  epos_control_position,      //!< Position operating mode.
  epos_control_velocity,      //!< Velocity operating mode.
  epos_control_current,       //!< Current operating mode.
  epos_control_diagnostic,    //!< Diagnostic operating mode.
  epos_control_master_enc,    //!< Master encoder operating mode.
  epos_control_step_dir       //!< Step/direction operating mode.
} epos_control_mode_t;

/** \brief Structure defining an EPOS controller
  */
typedef struct epos_control_t {
  epos_device_t* dev;         //!< The EPOS device of the controller.

  epos_control_mode_t mode;   //!< The controller's operating mode.
} epos_control_t;

/** \brief Initialize EPOS controller
  * \param[in] control The EPOS controller to be initialized.
  * \param[in] dev The controller's EPOS device.
  * \param[in] mode The operating mode of the EPOS controller to be
  *   initialized.
  */
void epos_control_init(
  epos_control_t* control,
  epos_device_t* dev,
  epos_control_mode_t mode);

/** \brief Destroy EPOS controller
  * \param[in] control The EPOS controller to be destroyed.
  */
void epos_control_destroy(
  epos_control_t* control);

/** \brief Retrieve EPOS controller mode
  * \param[in] control The EPOS controller to retrieve the mode for.
  * \return The mode of the specified EPOS controller. On error, the return
  *   value will be -1 and the error code set in control->dev->error.
  */
epos_control_mode_t epos_control_get_mode(
  epos_control_t* control);

/** \brief Set EPOS controller operating mode
  * \param[in] control The EPOS controller to set the operating mode for.
  * \param[in] mode The operating mode of the specified EPOS controller.
  * \return The resulting device error code.
  */
int epos_control_set_mode(
  epos_control_t* control,
  epos_control_mode_t mode);

/** \brief Start EPOS controller
  * \param[in] control The EPOS controller to be started.
  * \return The resulting device error code.
  */
int epos_control_start(
  epos_control_t* control);

/** \brief Stop EPOS controller
  * \param[in] control The EPOS controller to be stopped.
  * \return The resulting device error code.
  */
int epos_control_stop(
  epos_control_t* control);

#endif
