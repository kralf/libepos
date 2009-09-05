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

#ifndef EPOS_DEVICE_H
#define EPOS_DEVICE_H

#include <libcan/can.h>

#include "global.h"

/** \file device.h
  * \brief EPOS device functions
  */

/** Predefined EPOS device constants
  */
#define EPOS_DEVICE_WAIT_FOREVER                -1.0

#define EPOS_DEVICE_INVALID_ID                  0x0000
#define EPOS_DEVICE_MAX_ID                      0x007F
#define EPOS_DEVICE_SEND_ID                     0x0600
#define EPOS_DEVICE_RECEIVE_ID                  0x0580
#define EPOS_DEVICE_EMERGENCY_ID                0x0080

#define EPOS_DEVICE_WRITE_SEND_1_BYTE           0x2F
#define EPOS_DEVICE_WRITE_SEND_2_BYTE           0x2B
#define EPOS_DEVICE_WRITE_SEND_4_BYTE           0x23
#define EPOS_DEVICE_WRITE_SEND_UNDEFINED        0x22
#define EPOS_DEVICE_WRITE_RECEIVE               0x60

#define EPOS_DEVICE_READ_RECEIVE_1_BYTE         0x4F
#define EPOS_DEVICE_READ_RECEIVE_2_BYTE         0x4B
#define EPOS_DEVICE_READ_RECEIVE_4_BYTE         0x43
#define EPOS_DEVICE_READ_RECEIVE_UNDEFINED      0x42
#define EPOS_DEVICE_READ_SEND                   0x40

#define EPOS_DEVICE_ABORT                       0xC0

#define EPOS_DEVICE_INDEX_ERROR_REGISTER        0x1001
#define EPOS_DEVICE_INDEX_STORE                 0x1010
#define EPOS_DEVICE_SUBINDEX_STORE              0x01
#define EPOS_DEVICE_INDEX_RESTORE               0x1011
#define EPOS_DEVICE_SUBINDEX_RESTORE            0x01
#define EPOS_DEVICE_INDEX_ID                    0x2000
#define EPOS_DEVICE_INDEX_CAN_BITRATE           0x2001
#define EPOS_DEVICE_INDEX_RS232_BAUDRATE        0x2002
#define EPOS_DEVICE_INDEX_VERSION               0x2003
#define EPOS_DEVICE_SUBINDEX_SOFTWARE_VERSION   0x01
#define EPOS_DEVICE_SUBINDEX_HARDWARE_VERSION   0x02
#define EPOS_DEVICE_INDEX_CONTROL               0x6040
#define EPOS_DEVICE_INDEX_STATUS                0x6041

#define EPOS_DEVICE_CONTROL_SWITCH_ON           0x0001
#define EPOS_DEVICE_CONTROL_ENABLE_VOLTAGE      0x0003
#define EPOS_DEVICE_CONTROL_SHUTDOWN            0x0006
#define EPOS_DEVICE_CONTROL_QUICK_STOP          0x0007
#define EPOS_DEVICE_CONTROL_ENABLE_OPERATION    0x000F
#define EPOS_DEVICE_CONTROL_FAULT_RESET         0x00FF

/** Predefined EPOS device error codes
  */
#define EPOS_DEVICE_ERROR_NONE                  0
#define EPOS_DEVICE_ERROR_OPEN                  1
#define EPOS_DEVICE_ERROR_CLOSE                 2
#define EPOS_DEVICE_ERROR_INVALID_SIZE          3
#define EPOS_DEVICE_ERROR_SEND                  4
#define EPOS_DEVICE_ERROR_RECEIVE               5
#define EPOS_DEVICE_ERROR_ABORT                 6
#define EPOS_DEVICE_ERROR_INTERNAL              7
#define EPOS_DEVICE_ERROR_READ                  8
#define EPOS_DEVICE_ERROR_WRITE                 9
#define EPOS_DEVICE_ERROR_INVALID_BITRATE       10
#define EPOS_DEVICE_ERROR_INVALID_BAURATE       11
#define EPOS_DEVICE_ERROR_WAIT_TIMEOUT          12

/** \brief Predefined EPOS device error descriptions
  */
extern const char* epos_device_errors[];

/** \brief Structure defining an EPOS device
  */
typedef struct epos_device_t {
  can_device_p can_dev;       //!< The CAN device of the EPOS device.
  int node_id;                //!< The node identifier of the EPOS device.

  int reset;                  //!< Reset the EPOS device after opening.

  int can_bitrate;            //!< The CAN bitrate in [kbit/s].
  int rs232_baudrate;         //!< The RS-232 baudrate in [Baud].

  short hardware_version;     //!< The hardware version of the EPOS device.
  short software_version;     //!< The software version of the EPOS device.

  ssize_t num_read;           //!< The number of message read from the EPOS.
  ssize_t num_written;        //!< The number of message written to the EPOS.
} epos_device_t, *epos_device_p;

/** \brief Initialize EPOS device
  * \param[in] dev The EPOS device to be initialized.
  * \param[in] can_dev The CAN device of the EPOS device.
  * \param[in] node_id The identifier of the EPOS node to be initialized.
  * \param[in] reset Reset the EPOS device after opening.
  */
void epos_device_init(
  epos_device_p dev,
  can_device_p can_dev,
  int node_id,
  int reset);

/** \brief Destroy EPOS device
  * \param[in] dev The EPOS device to be destroyed.
  */
void epos_device_destroy(
  epos_device_p dev);

/** \brief Open EPOS device communication
  * \note This method also reads basic setup parameters from the EPOS node.
  * \param[in] dev The initialized EPOS device to be opened.
  * \return The resulting error code.
  */
int epos_device_open(
  epos_device_p dev);

/** \brief Close EPOS device communication
  * \param[in] dev The opened EPOS device to be closed.
  * \return The resulting error code.
  */
int epos_device_close(
  epos_device_p dev);

/** \brief Send EPOS CAN protocol message to a device
  * \param[in] dev The EPOS device the CAN message will be sent to.
  * \param[in] message The CAN message to be sent.
  * \return The resulting error code.
  */
int epos_device_send_message(
  epos_device_p dev,
  can_message_p message);

/** \brief Receive EPOS CAN protocol message from a device
  * \param[in] dev The EPOS device the CAN message shall be received from.
  * \param[out] message The received CAN message.
  * \return The resulting error code.
  */
int epos_device_receive_message(
  epos_device_p dev,
  can_message_p message);

/** \brief Read an EPOS device data object
  * \param[in] dev The EPOS device the data object will be read from.
  * \param[in] index The index of the EPOS data object.
  * \param[in] subindex The subindex of the EPOS data object.
  * \param[out] data The array the read EPOS data object shall be stored to.
  * \param[in] num The size of the EPOS data object to be read.
  * \return The resulting error code.
  */
int epos_device_read(
  epos_device_p dev,
  short index,
  unsigned char subindex,
  unsigned char* data,
  ssize_t num);

/** \brief Write an EPOS device data object
  * \param[in] dev The EPOS device the data object will be written to.
  * \param[in] index The index of the EPOS data object.
  * \param[in] subindex The subindex of the EPOS data object.
  * \param[in] data The array representing the EPOS data object to be written.
  * \param[in] num The size of the EPOS data object to be written.
  * \return The resulting error code.
  */
int epos_device_write(
  epos_device_p dev,
  short index,
  unsigned char subindex,
  unsigned char* data,
  ssize_t num);

/** \brief Store persistant parameters on an EPOS device
  * \param[in] dev The EPOS device to store the parameters on.
  * \return The resulting error code.
  */
int epos_device_store_parameters(
  epos_device_p dev);

/** \brief Restore default parameters on an EPOS device
  * \param[in] dev The EPOS device to restore the parameters on.
  * \return The resulting error code.
  */
int epos_device_restore_parameters(
  epos_device_p dev);

/** \brief Retrieve EPOS device node identifier
  * \param[in] dev The EPOS device to retrieve the node identifier for.
  * \return The node identifier of the specified EPOS device in.
  */
int epos_device_get_id(
  epos_device_p dev);

/** \brief Retrieve CAN bitrate of an EPOS device
  * \param[in] dev The EPOS device to retrieve the CAN bitrate for.
  * \return The CAN bitrate of the specified EPOS device in [kbit/s].
  */
int epos_device_get_can_bitrate(
  epos_device_p dev);

/** \brief Set CAN bitrate of an EPOS device
  * \param[in] dev The EPOS device to set the CAN bitrate for.
  * \param[in] bitrate The new CAN bitrate of the specified EPOS
  *   device in [kbit/s].
  * \return The resulting error code.
  */
int epos_device_set_can_bitrate(
  epos_device_p dev,
  int bitrate);

/** \brief Retrieve RS232 baudrate of an EPOS device
  * \param[in] dev The EPOS device to retrieve the RS232 baudrate for.
  * \return The RS-232 baudrate of the specified EPOS device in [Baud].
  */
int epos_device_get_rs232_baudrate(
  epos_device_p dev);

/** \brief Set RS232 baudrate of an EPOS device
  * \param[in] dev The EPOS device to set the RS232 baudrate for.
  * \param[in] baudrate The new RS-232 baudrate of the specified EPOS
  *   device in [Baud].
  * \return The resulting error code.
  */
int epos_device_set_rs232_baudrate(
  epos_device_p dev,
  int baudrate);

/** \brief Retrieve hardware version of an EPOS device
  * \param[in] dev The EPOS device to retrieve the hardware version for.
  * \return The hardware version of the specified EPOS device.
  */
short epos_device_get_hardware_version(
  epos_device_p dev);

/** \brief Retrieve software version of an EPOS device
  * \param[in] dev The EPOS device to retrieve the software version for.
  * \return The software version of the specified EPOS device.
  */
short epos_device_get_software_version(
  epos_device_p dev);

/** \brief Retrieve status information of an EPOS device
  * \param[in] dev The EPOS device to retrieve the status information for.
  * \return The status word of the specified EPOS device.
  */
short epos_device_get_status(
  epos_device_p dev);

/** \brief Wait for an EPOS device status
  * \param[in] dev The EPOS device to wait for.
  * \param[in] status The status word mask to be used.
  * \param[in] timeout The timeout of the wait operation in [s].
  *   A negative value will be interpreted as an eternal wait.
  * \return The resulting error code.
  */
int epos_device_wait_status(
  epos_device_p dev,
  short status,
  double timeout);

/** \brief Retrieve control information of an EPOS device
  * \param[in] dev The EPOS device to retrieve the control information for.
  * \return The control word of the specified EPOS device.
  */
short epos_device_get_control(
  epos_device_p dev);

/** \brief Set control information of an EPOS device
  * \param[in] dev The EPOS device to set the control information for.
  * \param[in] control The new control word of the specified EPOS device.
  * \return The resulting error code.
  */
int epos_device_set_control(
  epos_device_p dev,
  short control);

/** \brief Retrieve EPOS device error register
  * \param[in] dev The EPOS device to retrieve the error register for.
  * \return The error register value of the specified EPOS device.
  */
unsigned char epos_device_get_error(
  epos_device_p dev);

/** \brief Shutdown EPOS device
  * \param[in] dev The EPOS device to be shut down.
  * \return The resulting error code.
  */
int epos_device_shutdown(
  epos_device_p dev);

/** \brief Reset EPOS device in fault state
  * \param[in] dev The EPOS device to be reset.
  * \return The resulting error code.
  */
int epos_device_reset(
  epos_device_p dev);

#endif
