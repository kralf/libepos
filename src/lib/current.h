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

/** \file current.h
  * \brief EPOS current mode functions
  */

/** \name Object Indexes
  * \brief Predefined EPOS current control object indexes
  */
//@{
#define EPOS_CURRENT_INDEX_SETTING_VALUE           0x2030
#define EPOS_CURRENT_INDEX_ACTUAL_VALUE            0x6078
#define EPOS_CURRENT_INDEX_AVERAGE_VALUE           0x2027
#define EPOS_CURRENT_INDEX_CONTROL_PARAMETERS      0x60F6
#define EPOS_CURRENT_SUBINDEX_P_GAIN               0x01
#define EPOS_CURRENT_SUBINDEX_I_GAIN               0x02
//@}

/** \brief Structure defining an EPOS current control configuration
  */
typedef struct epos_current_config_t {
  short p_gain;         //!< The current controller's P-gain.
  short i_gain;         //!< The current controller's I-gain.
} epos_current_config_t;

/** \brief Structure defining an EPOS current control operation
  */
typedef struct epos_current_t {
  float target_value;          //!< The target current in [A].
} epos_current_t;

/** \brief Initialize EPOS current control operation
  * \param[in] current The EPOS current control operation to be initialized.
  * \param[in] target_value The target current in [A].
  */
void epos_current_init(
  epos_current_t* current,
  float target_value);

/** \brief Setup EPOS current control
  * \param[in] node The EPOS node to setup current control for.
  * \param[in] config The configuration to be used for setting up the
  *   EPOS current controller.
  * \return The resulting device error code.
  */
int epos_current_setup(
  epos_node_t* node,
  const epos_current_config_t* config);

/** \brief Start EPOS current control operation
  * \param[in] node The EPOS node to start the current control operation for.
  * \param[in] current The EPOS current control operation to be started.
  * \return The resulting device error code.
  */
int epos_current_start(
  epos_node_t* node,
  const epos_current_t* current);

/** \brief Stop EPOS current control operation
  * \param[in] node The EPOS node to stop the current control operation for.
  * \return The resulting device error code.
  */
int epos_current_stop(
  epos_node_t* node);

/** \brief Retrieve the actual current of an EPOS device
  * \param[in] dev The EPOS device to retrieve the actual current for.
  * \return The actual current of the specified EPOS device in [mA].
  *   On error, the return value will be zero and the error code set in
  *   dev->error.
  */
short epos_current_get_actual(
  epos_device_t* dev);

/** \brief Retrieve the average current of an EPOS device
  * \param[in] dev The EPOS device to retrieve the average current for.
  * \return The average current of the specified EPOS device in [mA].
  *   On error, the return value will be zero and the error code set in
  *   dev->error.
  */
short epos_current_get_average(
  epos_device_t* dev);

/** \brief Set the demanded current of an EPOS device
  * \param[in] dev The EPOS device to set the demanded current for.
  * \param[in] current The demanded current for the specified EPOS
  *   device in [mA].
  * \return The resulting device error code.
  */
int epos_current_set_demand(
  epos_device_t* dev,
  short current);

/** \brief Retrieve the demanded current of an EPOS device
  * \param[in] dev The EPOS device to retrieve the demanded current for.
  * \return The demanded current of the specified EPOS device in [mA].
  *   On error, the return value will be zero and the error code set in
  *   dev->error.
  */
short epos_current_get_demand(
  epos_device_t* dev);

/** \brief Set the current control P-gain of an EPOS device
  * \param[in] dev The EPOS device to set the current control P-gain for.
  * \param[in] p_gain The current control P-gain for the specified EPOS
  *   device.
  * \return The resulting device error code.
  */
int epos_current_set_p_gain(
  epos_device_t* dev,
  short p_gain);

/** \brief Set the current control I-gain of an EPOS device
  * \param[in] dev The EPOS device to set the current control I-gain for.
  * \param[in] i_gain The current control I-gain for the specified EPOS
  *   device.
  * \return The resulting device error code.
  */
int epos_current_set_i_gain(
  epos_device_t* dev,
  short i_gain);

#endif
