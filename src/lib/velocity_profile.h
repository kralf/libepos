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

#ifndef EPOS_VELOCITY_PROFILE_H
#define EPOS_VELOCITY_PROFILE_H

#include "profile.h"

/** \file velocity_profile.h
  * \brief EPOS velocity profile mode functions
  */

/** \name Object Indexes
  * \brief Predefined EPOS velocity profile control object indexes
  */
//@{
#define EPOS_VELOCITY_PROFILE_INDEX_TARGET            0x60FF
//@}

/** \name Control Words
  * \brief Predefined EPOS velocity profile control words
  */
//@{
#define EPOS_VELOCITY_PROFILE_CONTROL_SET             0x007F
//@}

/** \brief Structure defining an EPOS velocity profile control operation
  */
typedef struct epos_velocity_profile_t {
  float target_value;            //!< The target velocity in [rad/s].
  float acceleration;            //!< The profile acceleration in [rad/s^2].
  float deceleration;            //!< The profile acceleration in [rad/s^2].

  epos_profile_type_t type;      //!< The velocity profile type.

  float start_value;             //!< The profile's start velocity in [rad].
  double start_time;             //!< The start time of the profile in [s].
} epos_velocity_profile_t;

/** \brief Initialize EPOS velocity profile control operation
  * \param[in] profile The EPOS velocity profile control operation to be
  *   initialized.
  * \param[in] target_value The target velocity in [rad/s].
  * \param[in] acceleration The profile acceleration in [rad/s^2].
  * \param[in] deceleration The profile deceleration in [rad/s^2].
  * \param[in] type The velocity profile type.
  */
void epos_velocity_profile_init(
  epos_velocity_profile_t* profile,
  float target_value,
  float acceleration,
  float deceleration,
  epos_profile_type_t type);

/** \brief Start EPOS velocity profile control operation
  * \param[in] node The EPOS node to start the velocity profile control
  *   operation for.
  * \param[in] profile The EPOS velocity profile control operation to be
  *   started.
  * \return The resulting device error code.
  */
int epos_velocity_profile_start(
  epos_node_t* node,
  epos_velocity_profile_t* profile);

/** \brief Stop EPOS velocity profile control operation
  * \param[in] node The EPOS node to stop the velocity profile control
  *   operation for.
  * \return The resulting device error code.
  */
int epos_velocity_profile_stop(
  epos_node_t* node);

/** \brief Evaluate the values of an EPOS velocity profile
  * \param[in] profile The EPOS velocity profile control operation to
  *   evaluate the absolute values for.
  * \param[in] time The absolute time to evaluate the absolute profile
  *   values at in [s].
  * \return The evaluated absolute profile values.
  * 
  * This function is intended to facilitate the computational generation
  * of motion trajectories.
  */
epos_profile_value_t epos_velocity_profile_eval(
  const epos_velocity_profile_t* profile,
  double time);

/** \brief Set the velocity profile target velocity of an EPOS device
  * \param[in] dev The EPOS device to set the target velocity for.
  * \param[in] velocity The target velocity for the specified EPOS
  *   device in [vu].
  * \return The resulting device error code.
  */
int epos_velocity_profile_set_target(
  epos_device_t* dev,
  int velocity);

#endif
