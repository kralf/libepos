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

#ifndef EPOS_HOME_H
#define EPOS_HOME_H

#include "profile.h"

/** \brief Predefined EPOS homing control constants
  */
#define EPOS_HOME_INDEX_METHOD                      0x6098
#define EPOS_HOME_INDEX_VELOCITIES                  0x6099
#define EPOS_HOME_SUBINDEX_SWITCH_SEARCH_VELOCITY   0x01
#define EPOS_HOME_SUBINDEX_ZERO_SEARCH_VELOCITY     0x02
#define EPOS_HOME_INDEX_ACCELERATION                0x609A
#define EPOS_HOME_INDEX_OFFSET                      0x607C
#define EPOS_HOME_INDEX_CURRENT_THRESHOLD           0x2080
#define EPOS_HOME_INDEX_POSITION                    0x2081

#define EPOS_HOME_CONTROL_START                     0x001F
#define EPOS_HOME_CONTROL_HALT                      0x01FF

#define EPOS_HOME_STATUS_REACHED                    0x1000

/** \brief EPOS homing methods
  */
typedef enum {
  epos_home_neg_switch_index = 1,
  epos_home_pos_switch_index = 2,
  epos_home_neg_switch = 17,
  epos_home_pos_switch = 18,
  epos_home_pos_current_index = -1,
  epos_home_neg_current_index = -2,
  epos_home_pos_current = -3,
  epos_home_neg_current = -4
} epos_home_method_t;

/** \brief Structure defining an EPOS homing operation
  */
typedef struct epos_home_t {
  epos_home_method_t method;   //!< The EPOS homing method.
  epos_profile_type_t type;    //!< The homing profile type.

  float current;               //!< The homing current threshold in [A].
  float switch_vel;            //!< The switch search velocity in [rad/s].
  float zero_vel;              //!< The zero search velocity in [rad/s].
  float acceleration;          //!< The homing acceleration in [rad/s^2].

  float offset;                //!< The home offset from the switch in [rad].
  float position;              //!< The home position in [rad].
} epos_home_t, *epos_home_p;

/** \brief Initialize EPOS homing operation
  * \param[in] home The EPOS homing operation to be initialized.
  * \param[in] method The homing method to be used.
  * \param[in] current The homing current threshold to be used in [A].
  * \param[in] velocity The homing velocity to be used in [rad/s].
  * \param[in] acceleration The homing acceleration to be used in [rad/s^2].
  * \param[in] position The home position to be set in [rad].
  */
void epos_home_init(
  epos_home_p home,
  epos_home_method_t method,
  float current,
  float velocity,
  float acceleration,
  float position);

/** \brief Start EPOS homing operation
  * \param[in] node The EPOS node to start the homing operation for.
  * \param[in] home The EPOS homing operation to be started.
  * \return The resulting device error code.
  */
int epos_home_start(
  epos_node_p node,
  epos_home_p home);

/** \brief Stop EPOS homing operation
  * \param[in] node The EPOS node to stop the homing operation for.
  * \return The resulting device error code.
  */
int epos_home_stop(
  epos_node_p node);

/** \brief Wait for completion of an EPOS homing operation
  * \param[in] node The EPOS node to complete the homing operation.
  * \param[in] timeout The timeout of the wait operation in [s].
  * \return The resulting device error code.
  */
int epos_home_wait(
  epos_node_p node,
  double timeout);

/** \brief Set EPOS homing method
  * \param[in] dev The EPOS device to set the homing method for.
  * \param[in] method The homing method to be set.
  * \return The resulting device error code.
  */
int epos_home_set_method(
  epos_device_p dev,
  epos_home_method_t method);

/** \brief Set EPOS homing current threshold
  * \param[in] dev The EPOS device to set the homing current threshold for.
  * \param[in] current The current threshold to be set in [mA].
  * \return The resulting device error code.
  */
int epos_home_set_current_threshold(
  epos_device_p dev,
  short current);

/** \brief Set EPOS homing switch search velocity
  * \param[in] dev The EPOS device to set the homing switch search
  *   velocity for.
  * \param[in] velocity The homing switch search velocity to be set in [vu].
  * \return The resulting device error code.
  */
int epos_home_set_switch_search_velocity(
  epos_device_p dev,
  unsigned int velocity);

/** \brief Set EPOS homing zero search velocity
  * \param[in] dev The EPOS device to set the homing zero search velocity for.
  * \param[in] velocity The homing zero search velocity to be set in [vu].
  * \return The resulting device error code.
  */
int epos_home_set_zero_search_velocity(
  epos_device_p dev,
  unsigned int velocity);

/** \brief Set EPOS homing acceleration
  * \param[in] dev The EPOS device to set the homing acceleration for.
  * \param[in] acceleration The homing acceleration to be set in [au].
  * \return The resulting device error code.
  */
int epos_home_set_acceleration(
  epos_device_p dev,
  unsigned int acceleration);

/** \brief Set EPOS homing offset
  * \param[in] dev The EPOS device to set the homing offset for.
  * \param[in] offset The homing offset to be set in [pu].
  * \return The resulting device error code.
  */
int epos_home_set_offset(
  epos_device_p dev,
  int offset);

/** \brief Set EPOS home position
  * \param[in] dev The EPOS device to set the home position for.
  * \param[in] position The absolute home position to be set in [pu].
  * \return The resulting device error code.
  */
int epos_home_set_position(
  epos_device_p dev,
  int position);

#endif
