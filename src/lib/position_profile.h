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

#ifndef EPOS_POSITION_PROFILE_H
#define EPOS_POSITION_PROFILE_H

#include "profile.h"

/** \file position_profile.h
  * \brief EPOS position profile mode functions
  */

/** \name Object Indexes
  * \brief Predefined EPOS position profile control object indexes
  */
//@{
#define EPOS_POSITION_PROFILE_INDEX_TARGET            0x607A
#define EPOS_POSITION_PROFILE_INDEX_VELOCITY          0x6081
//@}

/** \name Control Words
  * \brief Predefined EPOS position profile control words
  */
//@{
#define EPOS_POSITION_PROFILE_CONTROL_SET_ABSOLUTE    0x003F
#define EPOS_POSITION_PROFILE_CONTROL_SET_RELATIVE    0x007F
//@}

/** \brief Structure defining an EPOS position profile control operation
  */
typedef struct epos_position_profile_t {
  float target_value;            //!< The target position in [rad].
  float velocity;                //!< The profile velocity in [rad/s].
  float acceleration;            //!< The profile acceleration in [rad/s^2].
  float deceleration;            //!< The profile acceleration in [rad/s^2].

  epos_profile_type_t type;      //!< The position profile type.
  int relative;                  //!< The profile position is relative.

  float start_value;             //!< The start position of the profile in [s].
  double start_time;             //!< The start time of the profile in [s].
} epos_position_profile_t, *epos_position_profile_p;

/** \brief Initialize EPOS position profile control operation
  * \param[in] profile The EPOS position profile control operation to be
  *   initialized.
  * \param[in] target_value The target position in [rad].
  * \param[in] velocity The profile velocity in [rad/s].
  * \param[in] acceleration The profile acceleration in [rad/s^2].
  * \param[in] deceleration The profile deceleration in [rad/s^2].
  * \param[in] type The position profile type.
  * \param[in] relative If zero, the target position is absolute.
  *   Otherwise, it specifies a position relative to the starting
  *   position of the profile.
  */
void epos_position_profile_init(
  epos_position_profile_t* profile,
  float target_value,
  float velocity,
  float acceleration,
  float deceleration,
  epos_profile_type_t type,
  int relative);

/** \brief Start EPOS position profile control operation
  * \param[in] node The EPOS node to start the position profile control
  *   operation for.
  * \param[in] profile The EPOS position profile control operation to be
  *   started.
  * \return The resulting device error code.
  */
int epos_position_profile_start(
  epos_node_t* node,
  epos_position_profile_t* profile);

/** \brief Stop EPOS position profile control operation
  * \param[in] node The EPOS node to stop the position profile control
  *   operation for.
  * \return The resulting device error code.
  */
int epos_position_profile_stop(
  epos_node_t* node);

/** \brief Evaluate the absolute values of an EPOS position profile
  * \param[in] profile The EPOS position profile control operation to
  *   evaluate the absolute values for.
  * \param[in] time The absolute time to evaluate the absolute profile
  *   values at in [s].
  * \return The evaluated absolute profile values.
  * 
  * This function is intended to facilitate the computational generation of
  * motion trajectories.
  */
epos_profile_value_t epos_position_profile_eval(
  const epos_position_profile_t* profile,
  double time);

/** \brief Set the position profile target position of an EPOS device
  * \param[in] dev The EPOS device to set the target position for.
  * \param[in] position The target position for the specified EPOS
  *   device in [pu].
  * \return The resulting device error code.
  */
int epos_position_profile_set_target(
  epos_device_t* dev,
  int position);

/** \brief Set the position profile velocity of an EPOS device
  * \param[in] dev The EPOS device to set the profile velocity for.
  * \param[in] velocity The profile velocity for the specified EPOS
  *   device in [vu].
  * \return The resulting device error code.
  */
int epos_position_profile_set_velocity(
  epos_device_t* dev,
  unsigned int velocity);

#endif
