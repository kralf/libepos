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

#include <can.h>

#include <error/error.h>

/** \file device.h
  * \brief EPOS device interface
  * 
  * The EPOS device interface provides the low-level communication with
  * an EPOS node via the CANopen protocol.
  */

/** \name Constants
  * \brief Predefined EPOS device constants
  */
//@{
#define EPOS_DEVICE_WAIT_FOREVER                -1.0
#define EPOS_DEVICE_CAN_BIT_RATE_RESERVED       1
#define EPOS_DEVICE_CAN_BIT_RATE_AUTO           0
//@}

/** \name Object Indexes
  * \brief Predefined EPOS device object indexes
  */
//@{
#define EPOS_DEVICE_INDEX_ERROR_REGISTER        0x1001
#define EPOS_DEVICE_INDEX_STORE                 0x1010
#define EPOS_DEVICE_SUBINDEX_STORE              0x01
#define EPOS_DEVICE_INDEX_RESTORE               0x1011
#define EPOS_DEVICE_SUBINDEX_RESTORE            0x01
#define EPOS_DEVICE_INDEX_ID                    0x2000
#define EPOS_DEVICE_INDEX_CAN_BIT_RATE          0x2001
#define EPOS_DEVICE_INDEX_RS232_BAUD_RATE       0x2002
#define EPOS_DEVICE_INDEX_VERSION               0x2003
#define EPOS_DEVICE_INDEX_MISC_CONFIGURATION    0x2008
#define EPOS_DEVICE_SUBINDEX_SOFTWARE_VERSION   0x01
#define EPOS_DEVICE_SUBINDEX_HARDWARE_VERSION   0x02
#define EPOS_DEVICE_INDEX_CONTROL               0x6040
#define EPOS_DEVICE_INDEX_STATUS                0x6041
//@}

/** \name Control Words
  * \brief Predefined EPOS device control words
  */
//@{
#define EPOS_DEVICE_CONTROL_SWITCH_ON           0x0001
#define EPOS_DEVICE_CONTROL_ENABLE_VOLTAGE      0x0003
#define EPOS_DEVICE_CONTROL_SHUTDOWN            0x0006
#define EPOS_DEVICE_CONTROL_QUICK_STOP          0x0007
#define EPOS_DEVICE_CONTROL_ENABLE_OPERATION    0x000F
#define EPOS_DEVICE_CONTROL_FAULT_RESET         0x00FF
//@}

/** \brief Predefined EPOS device type mask
  */
#define EPOS_DEVICE_TYPE_MASK                   0xFFF0

/** \name Error Codes
  * \brief Predefined EPOS device error codes
  */
//@{
#define EPOS_DEVICE_ERROR_NONE                  0
//!< Success
#define EPOS_DEVICE_ERROR_OPEN                  1
//!< Failed to open EPOS device
#define EPOS_DEVICE_ERROR_CLOSE                 2
//!< Failed to close EPOS device
#define EPOS_DEVICE_ERROR_INVALID_SIZE          3
//!< Invalid EPOS object size
#define EPOS_DEVICE_ERROR_SEND                  4
//!< Failed to send to EPOS device
#define EPOS_DEVICE_ERROR_RECEIVE               5
//!< Failed to receive from EPOS device
#define EPOS_DEVICE_ERROR_ABORT                 6
//!< EPOS communication error (abort)
#define EPOS_DEVICE_ERROR_INTERNAL              7
//!< EPOS internal device error
#define EPOS_DEVICE_ERROR_INVALID_BIT_RATE      8
//!< Invalid EPOS CAN bit rate
#define EPOS_DEVICE_ERROR_INVALID_BAUD_RATE     9
//!< Invalid EPOS RS232 baud rate
#define EPOS_DEVICE_ERROR_WAIT_TIMEOUT          10
//!< EPOS device timeout
//@}

/** \brief Predefined EPOS device error descriptions
  */
extern const char* epos_device_errors[];

/** \brief Predefined EPOS device hardware versions
  */
extern short epos_device_hardware_versions[];

/** \brief Predefined EPOS device hardware generations
  */
extern short epos_device_hardware_generations[];

/** \brief Predefined EPOS device names
  */
extern const char* epos_device_names[];

/** \brief EPOS device types
  */
typedef enum {
  epos_device_epos_24_1,              //!< EPOS 24/1.
  epos_device_epos_24_5,              //!< EPOS 24/5.
  epos_device_epos_70_10,             //!< EPOS 70/10.
  epos_device_mcd_epos_60_w,          //!< MCD EPOS 60W.
  epos_device_epos2_24_5,             //!< EPOS2 24/5.
  epos_device_epos2_50_5,             //!< EPOS2 50/5.
  epos_device_epos2_70_10,            //!< EPOS2 70/10.
  epos_device_epos2_module_36_2,      //!< EPOS2 Module 36/2.
  epos_device_unknown                 //!< Unknown device.
} epos_device_type_t;

/** \brief Structure defining an EPOS device
  */
typedef struct epos_device_t {
  can_device_t* can_dev;      //!< The CAN device of the EPOS device.
  int node_id;                //!< The node identifier of the EPOS device.

  int reset;                  //!< Reset the EPOS device after opening.

  int can_bit_rate;           //!< The CAN bit rate in [kbit/s].
  int rs232_baud_rate;        //!< The RS232 baud rate in [baud].

  short hardware_version;     //!< The hardware version of the EPOS device.
  short software_version;     //!< The software version of the EPOS device.
  
  epos_device_type_t type;    //!< The type of the EPOS device.
  short hardware_generation;  //!< The hardware generation of the EPOS device.

  size_t num_read;            //!< The number of messages read from the EPOS.
  size_t num_written;         //!< The number of messages written to the EPOS.
  
  error_t error;              //!< The most recent EPOS device error.
} epos_device_t;

/** \brief Initialize EPOS device
  * \param[in] dev The EPOS device to be initialized.
  * \param[in] can_dev The CAN device of the EPOS device.
  * \param[in] node_id The node identifier of the EPOS device to be
  *   initialized.
  * \param[in] reset Reset the EPOS device after opening.
  */
void epos_device_init(
  epos_device_t* dev,
  can_device_t* can_dev,
  int node_id,
  int reset);

/** \brief Destroy EPOS device
  * \param[in] dev The EPOS device to be destroyed.
  */
void epos_device_destroy(
  epos_device_t* dev);

/** \brief Open EPOS device communication
  * \param[in] dev The initialized EPOS device to be opened.
  * \return The resulting error code.
  * 
  * This method also reads basic setup parameters from the EPOS node.
  */
int epos_device_open(
  epos_device_t* dev);

/** \brief Close EPOS device communication
  * \param[in] dev The opened EPOS device to be closed.
  * \return The resulting error code.
  */
int epos_device_close(
  epos_device_t* dev);

/** \brief Send EPOS CAN protocol message to a device
  * \param[in] dev The EPOS device the CAN message will be sent to.
  * \param[in] message The CAN message to be sent.
  * \return The resulting error code.
  */
int epos_device_send_message(
  epos_device_t* dev,
  const can_message_t* message);

/** \brief Receive EPOS CAN protocol message from a device
  * \param[in] dev The EPOS device the CAN message shall be received from.
  * \param[out] message The received CAN message.
  * \return The resulting error code.
  */
int epos_device_receive_message(
  epos_device_t* dev,
  can_message_t* message);

/** \brief Read an EPOS device data object
  * \param[in] dev The EPOS device the data object will be read from.
  * \param[in] index The index of the EPOS data object.
  * \param[in] subindex The subindex of the EPOS data object.
  * \param[out] data The array the read EPOS data object shall be stored to.
  * \param[in] num The size of the EPOS data object to be read.
  * \return The number of data object bytes read or the negative error code.
  */
int epos_device_read(
  epos_device_t* dev,
  short index,
  unsigned char subindex,
  unsigned char* data,
  size_t num);

/** \brief Write an EPOS device data object
  * \param[in] dev The EPOS device the data object will be written to.
  * \param[in] index The index of the EPOS data object.
  * \param[in] subindex The subindex of the EPOS data object.
  * \param[in] data The array representing the EPOS data object to be written.
  * \param[in] num The size of the EPOS data object to be written.
  * \return The number of data object bytes written or the negative error code.
  */
int epos_device_write(
  epos_device_t* dev,
  short index,
  unsigned char subindex,
  unsigned char* data,
  size_t num);

/** \brief Store persistent parameters on an EPOS device
  * \param[in] dev The EPOS device to store the parameters on.
  * \return The resulting error code.
  */
int epos_device_store_parameters(
  epos_device_t* dev);

/** \brief Restore default parameters on an EPOS device
  * \param[in] dev The EPOS device to restore the parameters on.
  * \return The resulting error code.
  */
int epos_device_restore_parameters(
  epos_device_t* dev);

/** \brief Retrieve EPOS device node identifier
  * \param[in] dev The EPOS device to retrieve the node identifier for.
  * \return The node identifier of the specified EPOS device. On error, the
  *   return value will be zero and the error code set in dev->error.
  */
int epos_device_get_id(
  epos_device_t* dev);

/** \brief Retrieve CAN bit rate of an EPOS device
  * \param[in] dev The EPOS device to retrieve the CAN bit rate for.
  * \return The CAN bit rate of the specified EPOS device in [kbit/s] or
  *   EPOS_DEVICE_CAN_BIT_RATE_AUTO if the device is in automatic bit rate
  *   detection mode. On error, the return value will be zero and the error
  *   code set in dev->error.
  * 
  * Automatic bit rate detection is supported by EPOS devices of the second
  * hardware generation or newer.
  */
int epos_device_get_can_bit_rate(
  epos_device_t* dev);

/** \brief Set CAN bit rate of an EPOS device
  * \param[in] dev The EPOS device to set the CAN bit rate for.
  * \param[in] bit_rate The new CAN bit rate of the specified EPOS
  *   device in [kbit/s] or EPOS_DEVICE_CAN_BIT_RATE_AUTO to put the
  *   device in automatic bit rate detection mode.
  * \return The resulting error code.
  * 
  * Automatic bit rate detection is supported by EPOS devices of the second
  * hardware generation or newer.
  */
int epos_device_set_can_bit_rate(
  epos_device_t* dev,
  int bit_rate);

/** \brief Retrieve RS232 baud rate of an EPOS device
  * \param[in] dev The EPOS device to retrieve the RS232 baud rate for.
  * \return The RS232 baud rate of the specified EPOS device in [baud].
  *   On error, the return value will be zero and the error code set in
  *   dev->error.
  */
int epos_device_get_rs232_baud_rate(
  epos_device_t* dev);

/** \brief Set RS232 baud rate of an EPOS device
  * \param[in] dev The EPOS device to set the RS232 baud rate for.
  * \param[in] baud_rate The new RS232 baud rate of the specified EPOS
  *   device in [baud].
  * \return The resulting error code.
  */
int epos_device_set_rs232_baud_rate(
  epos_device_t* dev,
  int baud_rate);

/** \brief Retrieve hardware version of an EPOS device
  * \param[in] dev The EPOS device to retrieve the hardware version for.
  * \return The hardware version of the specified EPOS device. On error,
  *   the return value will be zero and the error code set in dev->error.
  */
short epos_device_get_hardware_version(
  epos_device_t* dev);

/** \brief Retrieve software version of an EPOS device
  * \param[in] dev The EPOS device to retrieve the software version for.
  * \return The software version of the specified EPOS device. On error,
  *   the return value will be zero and the error code set in dev->error.
  */
short epos_device_get_software_version(
  epos_device_t* dev);

/** \brief Retrieve status information of an EPOS device
  * \param[in] dev The EPOS device to retrieve the status information for.
  * \return The status word of the specified EPOS device. On error, the 
  *   return value will be zero and the error code set in dev->error.
  */
short epos_device_get_status(
  epos_device_t* dev);

/** \brief Wait for an EPOS device status
  * \param[in] dev The EPOS device to wait for.
  * \param[in] status The status word mask to be used.
  * \param[in] timeout The timeout of the wait operation in [s].
  *   A negative value will be interpreted as an eternal wait.
  * \return The resulting error code.
  */
int epos_device_wait_status(
  epos_device_t* dev,
  short status,
  double timeout);

/** \brief Retrieve control information of an EPOS device
  * \param[in] dev The EPOS device to retrieve the control information for.
  * \return The control word of the specified EPOS device. On error, the
  *   return value will be zero and the error code set in dev->error.
  */
short epos_device_get_control(
  epos_device_t* dev);

/** \brief Set control information of an EPOS device
  * \param[in] dev The EPOS device to set the control information for.
  * \param[in] control The new control word of the specified EPOS device.
  * \return The resulting error code.
  */
int epos_device_set_control(
  epos_device_t* dev,
  short control);

/** \brief Retrieve miscellaneous configuration of an EPOS device
  * \param[in] dev The EPOS device to retrieve the miscellaneous
  *   configuration for.
  * \return The miscellaneous configuration word of the specified EPOS device.
  *   On error, the return value will be zero and the error code set in
  *   dev->error.
  */
short epos_device_get_configuration(
  epos_device_t* dev);

/** \brief Set miscellaneous configuration of an EPOS device
  * \param[in] dev The EPOS device to set the miscellaneous configuration for.
  * \param[in] configuration The new miscellaneous configuration word of the
  *   specified EPOS device.
  * \return The resulting error code.
  */
int epos_device_set_configuration(
  epos_device_t* dev,
  short configuration);

/** \brief Retrieve EPOS device error register
  * \param[in] dev The EPOS device to retrieve the error register for.
  * \return The error register value of the specified EPOS device. On error,
  *   the return value will be zero and the error code set in dev->error.
  */
unsigned char epos_device_get_error(
  epos_device_t* dev);

/** \brief Shutdown EPOS device
  * \param[in] dev The EPOS device to be shut down.
  * \return The resulting error code.
  */
int epos_device_shutdown(
  epos_device_t* dev);

/** \brief Reset EPOS device in fault state
  * \param[in] dev The EPOS device to be reset.
  * \return The resulting error code.
  */
int epos_device_reset(
  epos_device_t* dev);

#endif
