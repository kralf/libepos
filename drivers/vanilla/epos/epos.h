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

#ifndef EPOS_H
#define EPOS_H

#include "device.h"
#include "sensor.h"
#include "motor.h"
#include "gear.h"
#include "control.h"

/** \brief Predefined EPOS constants
  */
#define EPOS_CONFIG_ARG_PREFIX              "--epos-"

#define EPOS_PARAMETER_ID                   "node-id"
#define EPOS_PARAMETER_RESET                "node-reset"
#define EPOS_PARAMETER_SENSOR_TYPE          "enc-type"
#define EPOS_PARAMETER_SENSOR_POLARITY      "enc-polarity"
#define EPOS_PARAMETER_SENSOR_PULSES        "enc-pulses"
#define EPOS_PARAMETER_MOTOR_TYPE           "motor-type"
#define EPOS_PARAMETER_MOTOR_CURRENT        "motor-current"
#define EPOS_PARAMETER_GEAR_TRANSMISSION    "gear-trans"
#define EPOS_PARAMETER_CONTROL_TYPE         "control-type"

/** \brief Predefined EPOS error codes
  */
#define EPOS_ERROR_NONE                     0
#define EPOS_ERROR_OPEN                     1
#define EPOS_ERROR_CLOSE                    2
#define EPOS_ERROR_FORMAT                   3

/** \brief Predefined EPOS error descriptions
  */
extern const char* epos_errors[];

/** \brief Predefined EPOS default configuration
  */
extern config_t epos_default_config;

/** \brief Structure defining an EPOS node
  */
typedef struct epos_node_t {
  epos_device_t dev;              //!< The EPOS node device.
  epos_sensor_t sensor;           //!< The EPOS position sensor.
  epos_motor_t motor;             //!< The EPOS motor.
  epos_gear_t gear;               //!< The EPOS gear assembly.
  epos_control_t control;         //!< The EPOS controller.

  config_t config;                //!< The EPOS configuration parameters.
} epos_node_t, *epos_node_p;

/** \brief Initialize EPOS node
  * \param[in] node The EPOS node to be initialized.
  * \param[in] can_dev The CAN communication device of the EPOS node. If
  *   null, a device will be created from default parameters.
  * \param[in] config The optional EPOS configuration parameters. Can be null.
  */
void epos_init(
  epos_node_p node,
  can_device_p can_dev,
  config_p config);

/** \brief Initialize EPOS node from command line arguments
  * \param[in] node The EPOS node to be initialized.
  * \param[in] argc The number of supplied command line arguments.
  * \param[in] argv The list of supplied command line arguments.
  */
void epos_init_arg(
  epos_node_p node,
  int argc,
  char **argv);

/** \brief Destroy EPOS node
  * \note This method automatically destroys unused CAN communication devices.
  * \param[in] node The EPOS node to be destroyed.
  */
void epos_destroy(
  epos_node_p node);

/** \brief Open EPOS node
  * \param[in] node The initialized EPOS node to be opened.
  * \return The resulting error code.
  */
int epos_open(
  epos_node_p node);

/** \brief Close EPOS node
  * \param[in] node The opened EPOS node to be closed.
  * \return The resulting error code.
  */
int epos_close(
  epos_node_p node);

/** \brief Retrieve the angular position of an EPOS node
  * \param[in] node The EPOS node to retrieve the angular position for.
  * \return The angular position of the specified EPOS node in [rad].
  */
float epos_get_position(
  epos_node_p node);

/** \brief Retrieve the angular velocity of an EPOS node
  * \param[in] node The EPOS node to retrieve the angular velocity for.
  * \return The angular velocity of the specified EPOS node in [rad/s].
  */
float epos_get_velocity(
  epos_node_p node);

/** \brief Retrieve the current of an EPOS node
  * \param[in] node The EPOS node to retrieve the current for.
  * \return The current of the specified EPOS node in [A].
  */
float epos_get_current(
  epos_node_p node);

#endif
