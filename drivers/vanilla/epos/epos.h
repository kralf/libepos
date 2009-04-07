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
#define EPOS_ARG_DEVICE                     "dev-"

#define EPOS_PARAMETER_ID                   0
#define EPOS_PARAMETER_RESET                1
#define EPOS_PARAMETER_SENSOR_TYPE          2
#define EPOS_PARAMETER_SENSOR_POLARITY      3
#define EPOS_PARAMETER_SENSOR_PULSES        4
#define EPOS_PARAMETER_MOTOR_TYPE           5
#define EPOS_PARAMETER_MOTOR_CURRENT        6
#define EPOS_PARAMETER_GEAR_TRANSMISSION    7
#define EPOS_PARAMETER_CONTROL_TYPE         8

/** \brief Predefined EPOS error codes
  */
#define EPOS_ERROR_NONE                     0
#define EPOS_ERROR_INIT                     1
#define EPOS_ERROR_CLOSE                    2
#define EPOS_ERROR_FORMAT                   3

/** \brief Predefined EPOS error descriptions
  */
extern const char* epos_errors[];

/** \brief Structure defining an EPOS parameter
  */
typedef struct epos_parameter_t {
  char name[64];                  //!< The EPOS parameter name.
  char value[64];                 //!< The EPOS parameter value.
} epos_parameter_t, *epos_parameter_p;

/** \brief Predefined EPOS parameters
  */
extern epos_parameter_t epos_parameters[];

/** \brief Structure defining an EPOS node
  */
typedef struct epos_node_t {
  epos_device_t dev;          //!< The EPOS node device.
  epos_sensor_t sensor;       //!< The EPOS position sensor.
  epos_motor_t motor;         //!< The EPOS motor.
  epos_gear_t gear;           //!< The EPOS gear assembly.
  epos_control_t control;     //!< The EPOS controller.
} epos_node_t, *epos_node_p;

/** \brief Set EPOS parameters from command line arguments
  * \param[in] argc The number of supplied command line arguments.
  * \param[in] argv The list of supplied command line arguments.
  */
void epos_set_arg(
  int argc,
  char **argv);

/** \brief Initialize EPOS node
  * \param[in] node The EPOS node to be initialized.
  * \param[in] node_id The EPOS node identifier.
  * \return The resulting error code.
  */
int epos_init(
  epos_node_p node,
  int node_id);

/** \brief Initialize EPOS node from command line arguments
  * \param[in] node The EPOS node to be initialized.
  * \param[in] argc The number of supplied command line arguments.
  * \param[in] argv The list of supplied command line arguments.
  * \return The resulting error code.
  */
int epos_init_arg(
  epos_node_p node,
  int argc,
  char **argv);

/** \brief Close EPOS node
  * \param[in] node The initialized EPOS node to be closed.
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
