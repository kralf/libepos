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

#include <tulibs/config/config.h>

#include "device.h"
#include "sensor.h"
#include "motor.h"
#include "gear.h"
#include "input.h"
#include "control.h"

/** \file epos.h
  * \brief EPOS convenience functions
  */

/** \brief Predefined EPOS argument prefix
  */
#define EPOS_ARG_PREFIX                       "epos-"

/** \name Parameters
  * \brief Predefined EPOS parameters
  */
//@{
#define EPOS_PARAMETER_DEVICE_NODE_ID         "dev-node-id"
#define EPOS_PARAMETER_DEVICE_RESET           "dev-reset"
#define EPOS_PARAMETER_SENSOR_TYPE            "enc-type"
#define EPOS_PARAMETER_SENSOR_POLARITY        "enc-polarity"
#define EPOS_PARAMETER_SENSOR_PULSES          "enc-pulses"
#define EPOS_PARAMETER_SENSOR_SUPERVISION     "enc-supervision"
#define EPOS_PARAMETER_MOTOR_TYPE             "motor-type"
#define EPOS_PARAMETER_MOTOR_CURRENT          "motor-current"
#define EPOS_PARAMETER_GEAR_TRANSMISSION      "gear-trans"
#define EPOS_PARAMETER_CONTROL_MODE           "control-mode"

#define EPOS_PARAMETER_HOME_METHOD            "home-method"
#define EPOS_PARAMETER_HOME_TYPE              "home-type"
#define EPOS_PARAMETER_HOME_CURRENT           "home-current"
#define EPOS_PARAMETER_HOME_VELOCITY          "home-vel"
#define EPOS_PARAMETER_HOME_ACCELERATION      "home-accel"
#define EPOS_PARAMETER_HOME_OFFSET            "home-offset"
#define EPOS_PARAMETER_HOME_POSITION          "home-pos"
//@}

/** \name Error Codes
  * \brief Predefined EPOS error codes
  */
//@{
#define EPOS_ERROR_NONE                       0
#define EPOS_ERROR_CONFIG                     1
#define EPOS_ERROR_OPEN                       2
#define EPOS_ERROR_CLOSE                      3
#define EPOS_ERROR_HOME                       4
//@}

/** \brief Predefined EPOS error descriptions
  */
extern const char* epos_errors[];

/** \brief Predefined EPOS default configuration
  */
extern config_t epos_default_config;

/** \brief Structure defining an EPOS node
  */
typedef struct epos_node_t {
  epos_device_t dev;              //!< The EPOS device.
  epos_sensor_t sensor;           //!< The EPOS position sensor.
  epos_motor_t motor;             //!< The EPOS motor.
  epos_gear_t gear;               //!< The EPOS gear assembly.
  epos_input_t input;             //!< The EPOS input module.
  epos_control_t control;         //!< The EPOS controller.

  config_t config;                //!< The EPOS configuration parameters.
} epos_node_t, *epos_node_p;

/** \brief Initialize EPOS node
  * \note The node will be initialized using default configuration parameters.
  * \param[in] node The EPOS node to be initialized.
  * \param[in] can_dev The CAN communication device of the EPOS node. If
  *   null, a device will be created from default parameters.
  */
void epos_init(
  epos_node_p node,
  can_device_p can_dev);

/** \brief Initialize EPOS node from configuration
  * \param[in] node The EPOS node to be initialized.
  * \param[in] can_dev The CAN communication device of the EPOS node. If
  *   null, a device will be created from default parameters.
  * \param[in] config The EPOS configuration parameters.
  * \return The resulting error code.
  */
int epos_init_config(
  epos_node_p node,
  can_device_p can_dev,
  config_p config);

/** \brief Initialize EPOS node by parsing command line arguments
  * \param[in] node The EPOS node to be initialized.
  * \param[in] parser The initialized configuration parser which will
  *   be used to parse the command line arguments into the EPOS node
  *   configuration.
  * \param[in] prefix An optional argument prefix for the EPOS node
  *   configuration parameters. If null, the default prefix is chosen.
  * \param[in] argc The number of supplied command line arguments.
  * \param[in] argv The list of supplied command line arguments.
  * \param[in] exit The exit policy of the parser in case of an error
  *   or help request.
  * \return The resulting error code.
  */
int epos_init_config_parse(
  epos_node_p node,
  config_parser_p parser,
  const char* prefix,
  int argc,
  char **argv,
  config_parser_exit_t exit);

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
  * \param[in] node The opened EPOS node to retrieve the angular position for.
  * \return The angular position of the specified EPOS node in [rad].
  */
float epos_get_position(
  epos_node_p node);

/** \brief Retrieve the angular velocity of an EPOS node
  * \param[in] node The opened EPOS node to retrieve the angular velocity for.
  * \return The angular velocity of the specified EPOS node in [rad/s].
  */
float epos_get_velocity(
  epos_node_p node);

/** \brief Retrieve the angular acceleration of an EPOS node
  * \param[in] node The opened EPOS node to retrieve the angular
  *   acceleration for.
  * \return The angular acceleration of the specified EPOS node in [rad/s^2].
  */
float epos_get_acceleration(
  epos_node_p node);

/** \brief Retrieve the current of an EPOS node
  * \param[in] node The opened EPOS node to retrieve the current for.
  * \return The current of the specified EPOS node in [A].
  */
float epos_get_current(
  epos_node_p node);

/** \brief Home an EPOS node from configuration settings
  * \param[in] node The opened EPOS node to be homed.
  * \param[in] timeout The timeout of the wait operation in [s].
  * \return The resulting error code.
  */
int epos_home(
  epos_node_p node,
  double timeout);

#endif
