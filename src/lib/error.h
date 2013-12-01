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

#ifndef EPOS_ERROR_H
#define EPOS_ERROR_H

#include "device.h"

/** \file error.h
  * \brief EPOS error functions
  */

/** \name Object Indexes
  * \brief Predefined EPOS error object indexes
  */
//@{
#define EPOS_ERROR_INDEX_HISTORY                0x1003
#define EPOS_ERROR_SUBINDEX_HISTORY_LENGTH      0x00
#define EPOS_ERROR_SUBINDEX_HISTORY_ENTRIES     0x01
//@}

/** \brief Structure defining an EPOS communication error
  */
typedef struct epos_error_comm_t {
  int code;             //!< The code of the EPOS communication error.
  const char* message;  //!< A descriptive message of the communication error.
} epos_error_comm_t;

/** \brief Structure defining an EPOS device error
  */
typedef struct epos_error_device_t {
  short code;           //!< The code of the EPOS device error.
  unsigned char reg;    //!< The register value of the EPOS device error.
  const char* message;  //!< A descriptive message of the EPOS device error.
} epos_error_device_t;

/** \brief Predefined EPOS communication errors
  */
extern epos_error_comm_t epos_errors_comm[];

/** \brief Predefined EPOS device errors
  */
extern epos_error_device_t epos_errors_device[];

/** \brief Return an EPOS communication error message
  * \param[in] code The communication error code for which a description
  *   will be returned.
  * \return The communication error description corresponding to the
  *   specified error code.
  */
const char* epos_error_comm(
  int code);

/** \brief Return an EPOS device error message
  * \param[in] code The device error code for which a description
  *   will be returned.
  * \return The device error description corresponding to the
  *   specified error code.
  */
const char* epos_error_device(
  short code);

/** \brief Retrieve length of the EPOS device error history
  * \param[in] dev The EPOS device to retrieve the error history length for.
  * \return The length of the error history of the specified EPOS device.
  *   On error to retrieve the full device error history, the returned 
  *   length will correspond to the history entries successfully retrieved
  *   and the error code will be set in dev->error.
  */
unsigned char epos_error_get_history_length(
  epos_device_t* dev);

/** \brief Retrieve EPOS device error history
  * \param[in] dev The EPOS device to retrieve the error history for.
  * \param[out] history The error history for the specified EPOS device.
  * \return The length of the error history of the specified EPOS device.
  *   On error, the return value will be zero and the error code set in
  *   dev->error.
  */
unsigned char epos_error_get_history(
  epos_device_t* dev,
  epos_error_device_t history[]);

/** \brief Clear EPOS device error history
  * \param[in] dev The EPOS device to clear the error history for.
  * \return The resulting device error code.
  */
int epos_error_clear_history(
  epos_device_t* dev);

#endif
