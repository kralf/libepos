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

#ifndef EPOS_POSITION_H
#define EPOS_POSITION_H

#include "epos.h"

/** \brief Predefined EPOS position control constants
  */
#define EPOS_POSITION_INDEX_SETTING_VALUE           0x2062
#define EPOS_POSITION_INDEX_DEMAND_VALUE            0x6062
#define EPOS_POSITION_INDEX_ACTUAL_VALUE            0x6064
#define EPOS_POSITION_INDEX_SOFTWARE_LIMIT          0x607D
#define EPOS_POSITION_SUBINDEX_NEG_LIMIT            0x01
#define EPOS_POSITION_SUBINDEX_POS_LIMIT            0x02

/** \brief Structure defining an EPOS position control operation
  */
typedef struct epos_position_t {
  float angle;            //!< The demanded position in [rad].

  float min_angle;        //!< The minimum position limit in [rad].
  float max_angle;        //!< The maximum position limit in [rad].
} epos_position_t, *epos_position_p;

/** \brief Initialize EPOS position control operation
  * \param[in] position The EPOS position control operation to be initialized.
  * \param[in] angle The demanded position in [rad].
  */
void epos_position_init(
  epos_position_p position,
  float angle);

/** \brief Initialize EPOS position control operation involving limits
  * \param[in] position The EPOS position control operation to be initialized.
  * \param[in] angle The demanded position in [rad].
  * \param[in] min_angle The minimum position angle in [rad].
  * \param[in] max_angle The maximum position angle in [rad].
  */
void epos_position_init_limits(
  epos_position_p position,
  float angle,
  float min_angle,
  float max_angle);

/** \brief Start EPOS position control operation
  * \param[in] node The EPOS node to start the position control operation for.
  * \param[in] position The EPOS position control operation to be started.
  * \return The resulting device error code.
  */
int epos_position_start(
  epos_node_p node,
  epos_position_p position);

/** \brief Stop EPOS position control operation
  * \param[in] node The EPOS node to stop the position control operation for.
  * \return The resulting device error code.
  */
int epos_position_stop(
  epos_node_p node);

/** \brief Set EPOS software position limits
  * \param[in] dev The EPOS device to set the software position limits for.
  * \param[in] min_steps The minimal position limit of the EPOS
  *   device in [steps].
  * \param[in] max_steps The maximal position limit of the EPOS
  *   device in [steps].
  * \return The resulting device error code.
  */
int epos_position_set_limits(
  epos_device_p dev,
  int min_steps,
  int max_steps);

/** \brief Retrieve the actual position of an EPOS device
  * \param[in] dev The EPOS device to retrieve the actual position for.
  * \return The actual position of the specified EPOS device in [steps].
  */
int epos_position_get_actual(
  epos_device_p dev);

/** \brief Set the demanded position of an EPOS device
  * \param[in] dev The EPOS device to set the demanded position for.
  * \param[in] The demanded position for the specified EPOS device in [steps].
  * \return The resulting device error code.
  */
int epos_position_set_demand(
  epos_device_p dev,
  int num_steps);

/** \brief Retrieve the demanded position of an EPOS device
  * \param[in] dev The EPOS device to retrieve the demanded position for.
  * \return The demanded position of the specified EPOS device in [steps].
  */
int epos_position_get_demand(
  epos_device_p dev);

#endif
