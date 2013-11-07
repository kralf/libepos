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

#ifndef EPOS_INPUT_H
#define EPOS_INPUT_H

#include "device.h"

/** \file input.h
  * \brief EPOS input functions
  */

/** \name Object Indexes
  * \brief Predefined EPOS input object indexes
  */
//@{
#define EPOS_INPUT_INDEX_CONFIG             0x2070
#define EPOS_INPUT_INDEX_FUNCS              0x2071
#define EPOS_INPUT_SUBINDEX_STATE           0x01
#define EPOS_INPUT_SUBINDEX_MASK            0x02
#define EPOS_INPUT_SUBINDEX_POLARITY        0x03
#define EPOS_INPUT_SUBINDEX_EXECUTE         0x04
//@}

/** \name Input Functionalities
  * \brief Predefined EPOS input functionalities
  */
//@{
#define EPOS_INPUT_FUNC_RESERVED            7            
#define EPOS_INPUT_FUNC_DUMMY               65535
//@}

/** \name Error Codes
  * \brief Predefined EPOS input error codes
  */
//@{
#define EPOS_INPUT_ERROR_NONE               0
#define EPOS_INPUT_ERROR_SETUP              1
//@}

/** \brief Predefined EPOS input error descriptions
  */
extern const char* epos_input_errors[];

/** \brief Predefined EPOS input channel masks
  */
extern short epos_input_channel_masks[];

/** \brief EPOS input functionalities
  */
typedef enum {
  epos_input_neg_limit,
  epos_input_pos_limit,
  epos_input_home_switch,
  epos_input_pos_marker,
  epos_input_dev_enable
} epos_input_func_type_t;

/** \brief EPOS input polarities
  */
typedef enum {
  epos_input_high,
  epos_input_low
} epos_input_polarity_t;

/** \brief Structure defining an EPOS input functionalities
  */
typedef struct epos_input_func_t {
  int channel;                     //!< The input functionality's channel.
  epos_input_polarity_t polarity;  //!< The input functionality's polarity.
  int execute;                     //!< The input functionality's execution.

  int enabled;                     //!< The input functionality's enabled flag.
} epos_input_func_t, *epos_input_func_p;

/** \brief Structure defining the EPOS input module
  */
typedef struct epos_input_t {
  epos_device_p dev;                   //!< The EPOS device of the input module.

  short channel_mask;                  //!< The EPOS input channel mask.

  epos_input_func_type_t channels[10]; //!< The EPOS input channels.
  short polarity;                      //!< The EPOS input polarity.
  short execute;                       //!< The EPOS input execution.
  short enabled;                       //!< The EPOS input enabled mask.

  epos_input_func_t funcs[5]  ;        //!< The EPOS input functionalities.
} epos_input_t, *epos_input_p;

/** \brief Initialize EPOS input module
  * \param[in] input The EPOS input module to be initialized.
  * \param[in] dev The EPOS device of the input module.
  */
void epos_input_init(
  epos_input_p input,
  epos_device_p dev);

/** \brief Initialize EPOS input functionality
  * \param[in] func The input functionality to be initialized.
  * \param[in] channel The input functionality's channel.
  * \param[in] polarity The input functionality's polarity.
  * \param[in] execute The input functionality's execution flag.
  * \param[in] enabled The input functionality's enabled flag.
  */
void epos_input_init_func(
  epos_input_func_p func,
  int channel,
  epos_input_polarity_t polarity,
  int execute,
  int enabled);

/** \brief Destroy EPOS input module
  * \param[in] input The EPOS input module to be destroyed.
  */
void epos_input_destroy(
  epos_input_p input);

/** \brief Set EPOS input parameters
  * \param[in] input The EPOS input module to set the parameters for.
  * \return The resulting error code.
  */
int epos_input_setup(
  epos_input_p input);

/** \brief Retrieve EPOS input functionality parameters
  * \param[in] input The EPOS input module to retrieve the functionality 
  *   parameters for.
  * \param[in] type The input functionality type to retrieve the 
  *   parameters for.
  * \param[out] func The retrieved input functionality parameters.
  */
void epos_input_get_func(
  epos_input_p input,
  epos_input_func_type_t type,
  epos_input_func_p func);

/** \brief Set EPOS input functionality parameters
  * \param[in] input The EPOS input module to set the functionality 
  *   parameters for.
  * \param[in] type The input functionality type to set the parameters for.
  * \param[in] func The input functionality parameters to be set.
  * \return The resulting device error code.
  */
int epos_input_set_func(
  epos_input_p input,
  epos_input_func_type_t type,
  epos_input_func_p func);

/** \brief Retrieve EPOS input functionality state
  * \param[in] input The EPOS input module to retrieve the functionality 
  *   state for.
  * \param[in] type The input functionality type to retrieve the state for.
  * \return func The retrieved input functionality state.
  */
int epos_input_get_func_state(
  epos_input_p input,
  epos_input_func_type_t type);

/** \brief Retrieve EPOS input functionality channel
  * \param[in] input The EPOS input module to retrieve the functionality 
  *   channel for.
  * \param[in] type The input functionality type to retrieve the channel for.
  * \return The input channel of the specified functionality.
  */
int epos_input_get_func_channel(
  epos_input_p input,
  epos_input_func_type_t type);

/** \brief Set EPOS input functionality channel
  * \param[in] input The EPOS input module to set the functionality 
  *   channel for.
  * \param[in] type The input functionality type to set the channel for.
  * \param[in] channel The functionality channel to be set.
  * \return The resulting device error code.
  */
int epos_input_set_func_channel(
  epos_input_p input,
  epos_input_func_type_t type,
  int channel);

/** \brief Retrieve EPOS input functionality polarity
  * \param[in] input The EPOS input module to retrieve the functionality 
  *   polarity for.
  * \param[in] type The input functionality type to retrieve the polarity for.
  * \return The input polarity of the specified functionality.
  */
epos_input_polarity_t epos_input_get_func_polarity(
  epos_input_p input,
  epos_input_func_type_t type);

/** \brief Set EPOS input functionality polarity
  * \param[in] input The EPOS input module to set the functionality 
  *   polarity for.
  * \param[in] type The input functionality type to set the polarity for.
  * \param[in] polarity The functionality polarity to be set.
  * \return The resulting device error code.
  */
int epos_input_set_func_polarity(
  epos_input_p input,
  epos_input_func_type_t type,
  epos_input_polarity_t polarity);

/** \brief Retrieve EPOS input functionality execution flag
  * \param[in] input The EPOS input module to retrieve the functionality 
  *   execution flag for.
  * \param[in] type The input functionality type to retrieve the execution 
  *   flag for.
  * \return The execution flag of the specified functionality.
  */
int epos_input_get_func_execute(
  epos_input_p input,
  epos_input_func_type_t type);

/** \brief Set EPOS input functionality execution flag
  * \param[in] input The EPOS input module to set the functionality execution 
  *   flag for.
  * \param[in] type The input functionality type to set the execution flag for.
  * \param[in] execute The functionality execution flag to be set.
  * \return The resulting device error code.
  */
int epos_input_set_func_execute(
  epos_input_p input,
  epos_input_func_type_t type,
  int execute);

/** \brief Retrieve EPOS input functionality enabled flag
  * \param[in] input The EPOS input module to retrieve the functionality 
  *   enabled flag for.
  * \param[in] type The input functionality type to retrieve the enabled 
  *   flag for.
  * \return The enabled flag of the specified functionality.
  */
int epos_input_get_func_enabled(
  epos_input_p input,
  epos_input_func_type_t type);

/** \brief Set EPOS input functionality enabled flag
  * \param[in] input The EPOS input module to set the functionality enabled 
  *   flag for.
  * \param[in] type The input functionality type to set the enabled flag for.
  * \param[in] enabled The functionality enabled flag to be set.
  * \return The resulting device error code.
  */
int epos_input_set_func_enabled(
  epos_input_p input,
  epos_input_func_type_t type,
  int enabled);

#endif
