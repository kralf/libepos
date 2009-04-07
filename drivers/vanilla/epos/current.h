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

#ifndef EPOS_CURRENT_H
#define EPOS_CURRENT_H

#include "epos.h"

/** \brief Predefined EPOS current control constants
  */
#define EPOS_CURRENT_INDEX_SETTING_VALUE           0x2030
#define EPOS_CURRENT_INDEX_ACTUAL_VALUE            0x6078
#define EPOS_CURRENT_INDEX_AVERAGE_VALUE           0x2027

/** \brief Structure defining an EPOS current control operation
  */
typedef struct epos_current_t {
  float target_value;          //!< The target current in [A].
} epos_current_t, *epos_current_p;

/** \brief Initialize EPOS current control operation
  * \param[in] velocity The EPOS current control operation to be initialized.
  * \param[in] target_value The target current in [A].
  */
void epos_current_init(
  epos_current_p current,
  float target_value);

/** \brief Start EPOS current control operation
  * \param[in] node The EPOS node to start the current control operation for.
  * \param[in] current The EPOS current control operation to be started.
  * \return The resulting device error code.
  */
int epos_current_start(
  epos_node_p node,
  epos_current_p current);

/** \brief Stop EPOS current control operation
  * \param[in] node The EPOS node to stop the current control operation for.
  * \return The resulting device error code.
  */
int epos_current_stop(
  epos_node_p node);

/** \brief Retrieve the actual current of an EPOS device
  * \param[in] dev The EPOS device to retrieve the actual current for.
  * \return The actual current of the specified EPOS device in [mA].
  */
short epos_current_get_actual(
  epos_device_p dev);

/** \brief Retrieve the average current of an EPOS device
  * \param[in] dev The EPOS device to retrieve the average current for.
  * \return The average current of the specified EPOS device in [mA].
  */
short epos_current_get_average(
  epos_device_p dev);

/** \brief Set the demanded current of an EPOS device
  * \param[in] dev The EPOS device to set the demanded current for.
  * \param[in] curr The demanded current for the specified EPOS device in [mA].
  * \return The resulting device error code.
  */
int epos_current_set_demand(
  epos_device_p dev,
  short curr);

#endif
