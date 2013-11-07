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

#include <stdio.h>

#include "error.h"

epos_error_comm_t epos_errors_comm[] = {
  {0x00000000, "RS232 communication was successful"},
  {0x05030000, "Toggle bit not alternated"},
  {0x05040000, "SDO protocol timed out"},
  {0x05040001, "Client/server command specifier not valid or unknown"},
  {0x05040005, "Out of memory"},
  {0x06010000, "Unsupported access to an object"},
  {0x06010001, "Read command to a write only object"},
  {0x06010002, "Write command to a read only object"},
  {0x06020000, "Last read or write command had a wrong object (sub)index"},
  {0x06040041, "Object is not mappable to the PDO"},
  {0x06040042, "Number and length of the objects exceeds PDO length"},
  {0x06040043, "General parameter incompatibility"},
  {0x06040047, "General internal incompatibility in device"},
  {0x06060000, "Access failed due to an hardware error"},
  {0x06070010, "Data type mismatch, length or service parameter mismatch"},
  {0x06070012, "Data type mismatch, length of service parameter too high"},
  {0x06070013, "Data type mismatch, length of service parameter too low"},
  {0x06090011, "Last read or write command had a wrong object subindex"},
  {0x06090030, "Value range of parameter exceeded"},
  {0x06090031, "Value of parameter written too high"},
  {0x06090032, "Value of parameter written too low"},
  {0x06090036, "Maximum value is less than minimum value"},
  {0x08000000, "General error"},
  {0x08000020, "Data cannot be transferred or stored"},
  {0x08000021, "Data cannot be transferred or stored because of local control"},
  {0x08000022, "Data cannot be transferred or stored because of device state"},
  {0x0F00FFC0, "Device is in wrong NMT state"},
  {0x0F00FFBF, "RS232 command is illegal (does not exist)"},
  {0x0F00FFBE, "Password is wrong"},
  {0x0F00FFBC, "Device is not in service mode"},
  {0x0F00FFB9, "Wrong CAN id"},
};

epos_error_device_t epos_errors_device[] = {
  {0x0000, 0x00, "No error"},
  {0x1000, 0x01, "Generic error"},
  {0x2310, 0x02, "Over current error"},
  {0x3210, 0x04, "Over voltage error"},
  {0x3220, 0x04, "Under voltage"},
  {0x4210, 0x08, "Over temperature"},
  {0x5113, 0x04, "Supply voltage (+5V) too low"},
  {0x6100, 0x20, "Internal software error"},
  {0x6320, 0x20, "Software parameter error"},
  {0x7320, 0x20, "Sensor position error"},
  {0x8110, 0x10, "CAN overrun error (objects lost)"},
  {0x8111, 0x10, "CAN overrun error"},
  {0x8120, 0x10, "CAN passive mode error"},
  {0x8130, 0x10, "CAN life guard error"},
  {0x8150, 0x10, "CAN transmit COD-ID collision"},
  {0x81FD, 0x10, "CAN bus off"},
  {0x81FE, 0x10, "CAN Rx queue overrun"},
  {0x81FF, 0x10, "CAN Tx queue overrun"},
  {0x8210, 0x10, "CAN PDO length error"},
  {0x8611, 0x20, "Following error"},
  {0xFF01, 0x80, "Hall sensor error"},
  {0xFF02, 0x80, "Index processing error"},
  {0xFF03, 0x80, "Encoder resolution error"},
  {0xFF04, 0x80, "Hall sensor not found error"},
  {0xFF06, 0x80, "Negative limit error"},
  {0xFF07, 0x80, "Positive limit error"},
  {0xFF08, 0x80, "Hall angle detection error"},
  {0xFF09, 0x80, "Software position limit error"},
  {0xFF0A, 0x80, "Position sensor breach"},
  {0xFF0B, 0x20, "System overloaded"},
};

char* epos_error_comm_undefined_message = "Undefined communication error";
char* epos_error_device_undefined_message = "Undefined device error";

const char* epos_error_comm(int code) {
  int i;

  for (i = 0; i < sizeof(epos_errors_comm)/sizeof(epos_error_comm_t); ++i)
      if (epos_errors_comm[i].code == code)
    return epos_errors_comm[i].message;

  return epos_error_comm_undefined_message;
}

const char* epos_error_device(short code) {
  int i;

  for (i = 0; i < sizeof(epos_errors_device)/ sizeof(epos_error_device_t); ++i)
    if (epos_errors_device[i].code == code)
    return epos_errors_device[i].message;

  return epos_error_device_undefined_message;
}

unsigned char epos_error_get_history_length(epos_device_p dev) {
  unsigned char length;
  epos_device_read(dev, EPOS_ERROR_INDEX_HISTORY,
    EPOS_ERROR_SUBINDEX_HISTORY_LENGTH, &length, 1);

  return length;
}

unsigned char epos_error_get_history(epos_device_p dev,
  epos_error_device_t history[]) {
  int i, num = 0;
  unsigned char length = epos_error_get_history_length(dev);

  for (i = 0; i < length; ++i) {
    int code;
    if (!epos_device_read(dev, EPOS_ERROR_INDEX_HISTORY,
      EPOS_ERROR_SUBINDEX_HISTORY_ENTRIES+i, (unsigned char*)&code,
      sizeof(int))) {
      history[num].code = code;
      history[num].reg = 0;
      history[num].message = epos_error_device(history[num].code);

      ++num;
    }
  }

  return num;
}

int epos_error_clear_history(epos_device_p dev) {
  unsigned char length = 0;
  return epos_device_write(dev, EPOS_ERROR_INDEX_HISTORY,
    EPOS_ERROR_SUBINDEX_HISTORY_LENGTH, &length, 1);
}
