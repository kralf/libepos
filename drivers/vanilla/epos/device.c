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
#include <string.h>
#include <time.h>

#include "device.h"
#include "error.h"

const char* epos_device_errors[] = {
  "success",
  "error opening EPOS device",
  "error closing EPOS device",
  "invalid EPOS object size",
  "error sending to EPOS device",
  "error receiving from EPOS device",
  "EPOS communication error (abort error)",
  "EPOS internal device error",
  "error reading from EPOS device",
  "error writing to EPOS device",
  "invalid CAN bitrate",
  "invalid RS232 baudrate",
  "wait operation timed out",
};

int epos_device_can_bitrates[] = {
  1000,
  800,
  500,
  250,
  125,
  50,
  20,
};

int epos_device_rs232_baudrates[] = {
  9600,
  14400,
  19200,
  38400,
  57600,
  115200,
};

void epos_device_init(epos_device_p dev, can_device_p can_dev, int node_id,
  int reset) {
  dev->can_dev = can_dev;
  dev->node_id = node_id;

  dev->reset = reset;

  dev->num_read = 0;
  dev->num_written = 0;
}

void epos_device_destroy(epos_device_p dev) {
  dev->can_dev = 0;
  dev->node_id = EPOS_DEVICE_INVALID_ID;
}

int epos_device_open(epos_device_p dev) {
  if (!can_open(dev->can_dev)) {
    if (dev->reset && epos_device_reset(dev))
      return EPOS_DEVICE_ERROR_OPEN;

    dev->node_id = epos_device_get_id(dev);

    dev->can_bitrate = epos_device_get_can_bitrate(dev);
    dev->rs232_baudrate = epos_device_get_rs232_baudrate(dev);

    dev->hardware_version = epos_device_get_hardware_version(dev);
    dev->software_version = epos_device_get_software_version(dev);

    return EPOS_DEVICE_ERROR_NONE;
  }
  else {
    fprintf(stderr, "Node %d connection error\n", dev->node_id);
    return EPOS_DEVICE_ERROR_OPEN;
  }
}

int epos_device_close(epos_device_p dev) {
  if (!epos_device_shutdown(dev) && !can_close(dev->can_dev))
    return EPOS_DEVICE_ERROR_NONE;
  else
    return EPOS_DEVICE_ERROR_CLOSE;
}

int epos_device_send_message(epos_device_p dev, can_message_p message) {
  if (!can_send_message(dev->can_dev, message))
    return EPOS_DEVICE_ERROR_NONE;
  else
    return EPOS_DEVICE_ERROR_SEND;
}

int epos_device_receive_message(epos_device_p dev, can_message_p message) {
  if (!can_receive_message(dev->can_dev, message)) {
    if ((message->id >= EPOS_DEVICE_EMERGENCY_ID) &&
      (message->id <= EPOS_DEVICE_EMERGENCY_ID+EPOS_DEVICE_MAX_ID)) {
      short code;

      memcpy(&code, &message->content[0], sizeof(code));
      if (code != EPOS_DEVICE_ERROR_NONE) {
        fprintf(stderr, "Node %d device error: [0x%hX] %s\n",
          message->id-EPOS_DEVICE_EMERGENCY_ID, code,
          epos_error_device(code));

        return EPOS_DEVICE_ERROR_INTERNAL;
      }
      else
        return EPOS_DEVICE_ERROR_NONE;
    }
    else if (message->content[0] == EPOS_DEVICE_ABORT) {
      int code;

      memcpy(&code, &message->content[4], sizeof(code));
      fprintf(stderr, "Node %d communication error: [0x%X] %s\n",
        message->id-EPOS_DEVICE_RECEIVE_ID, code,
        epos_error_comm(code));

      return EPOS_DEVICE_ERROR_ABORT;
    }
    else
      return EPOS_DEVICE_ERROR_NONE;
  }
  else
    return EPOS_DEVICE_ERROR_RECEIVE;
}

int epos_device_read(epos_device_p dev, short index, unsigned char subindex,
  unsigned char* data, ssize_t num) {
  can_message_t message;
  memset(&message, 0, sizeof(can_message_t));

  message.id = EPOS_DEVICE_SEND_ID+dev->node_id;
  message.content[0] = EPOS_DEVICE_READ_SEND;
  message.content[1] = index;
  message.content[2] = index >> 8;
  message.content[3] = subindex;

  if (!epos_device_send_message(dev, &message) &&
    !epos_device_receive_message(dev, &message)) {
    memcpy(data, &message.content[4], num);
    ++dev->num_read;
    return EPOS_DEVICE_ERROR_NONE;
  }
  else
    return EPOS_DEVICE_ERROR_READ;
}

int epos_device_write(epos_device_p dev, short index, unsigned char subindex,
  unsigned char* data, ssize_t num) {
  can_message_t message;

  message.id = EPOS_DEVICE_SEND_ID+dev->node_id;
  switch (num) {
    case 1 : message.content[0] = EPOS_DEVICE_WRITE_SEND_1_BYTE;
             break; 
    case 2 : message.content[0] = EPOS_DEVICE_WRITE_SEND_2_BYTE;
             break; 
    case 4 : message.content[0] = EPOS_DEVICE_WRITE_SEND_4_BYTE;
             break; 
    default: return EPOS_DEVICE_ERROR_INVALID_SIZE;
  }
  message.content[1] = index;
  message.content[2] = index >> 8;
  message.content[3] = subindex;
  message.content[4] = data[0];
  message.content[5] = (num > 1) ? data[1] : 0x00;
  message.content[6] = (num > 2) ? data[2] : 0x00;
  message.content[7] = (num > 2) ? data[3] : 0x00;

  if (!epos_device_send_message(dev, &message) &&
    !epos_device_receive_message(dev, &message)) {
    ++dev->num_written;
    return EPOS_DEVICE_ERROR_NONE;
  }
  else
    return EPOS_DEVICE_ERROR_WRITE;
}

int epos_device_store_parameters(epos_device_p dev) {
  return epos_device_write(dev, EPOS_DEVICE_INDEX_STORE,
    EPOS_DEVICE_SUBINDEX_STORE, "save", 4);
}

int epos_device_restore_parameters(epos_device_p dev) {
  return epos_device_write(dev, EPOS_DEVICE_INDEX_STORE,
    EPOS_DEVICE_SUBINDEX_STORE, "load", 4);
}

int epos_device_get_id(epos_device_p dev) {
  unsigned char id;
  epos_device_read(dev, EPOS_DEVICE_INDEX_ID, 0, &id, 1);

  return id;
}

int epos_device_get_can_bitrate(epos_device_p dev) {
  short can_bitrate;
  epos_device_read(dev, EPOS_DEVICE_INDEX_CAN_BITRATE, 0,
    (unsigned char*)&can_bitrate, sizeof(short));

  return epos_device_can_bitrates[can_bitrate];
}

int epos_device_set_can_bitrate(epos_device_p dev, int bitrate) {
  short b;
  for (b = 0; b < sizeof(epos_device_can_bitrates)/sizeof(int); ++b)
    if (epos_device_can_bitrates[b] == bitrate) {
    return epos_device_write(dev, EPOS_DEVICE_INDEX_CAN_BITRATE, 0,
      (unsigned char*)&b, sizeof(short));
  }
  return EPOS_DEVICE_ERROR_INVALID_BITRATE;
}

int epos_device_get_rs232_baudrate(epos_device_p dev) {
  short rs232_baudrate;
  epos_device_read(dev, EPOS_DEVICE_INDEX_RS232_BAUDRATE, 0,
    (unsigned char*)&rs232_baudrate, sizeof(short));

  return epos_device_rs232_baudrates[rs232_baudrate];
}

int epos_device_set_rs232_baudrate(epos_device_p dev, int baudrate) {
  short b;
  for (b = 0; b < sizeof(epos_device_rs232_baudrates)/sizeof(int); ++b)
    if (epos_device_rs232_baudrates[b] == baudrate) {
    return epos_device_write(dev, EPOS_DEVICE_INDEX_RS232_BAUDRATE, 0,
      (unsigned char*)&b, sizeof(short));
  }
  return EPOS_DEVICE_ERROR_INVALID_BAURATE;
}

short epos_device_get_hardware_version(epos_device_p dev) {
  short hardware_version;
  epos_device_read(dev, EPOS_DEVICE_INDEX_VERSION,
    EPOS_DEVICE_SUBINDEX_HARDWARE_VERSION,
    (unsigned char*)&hardware_version, sizeof(short));

  return hardware_version;
}

short epos_device_get_software_version(epos_device_p dev) {
  short software_version;
  epos_device_read(dev, EPOS_DEVICE_INDEX_VERSION,
    EPOS_DEVICE_SUBINDEX_SOFTWARE_VERSION,
    (unsigned char*)&software_version, sizeof(short));

  return software_version;
}

short epos_device_get_status(epos_device_p dev) {
  short status;
  epos_device_read(dev, EPOS_DEVICE_INDEX_STATUS, 0,
    (unsigned char*)&status, sizeof(short));

  return status;
}

int epos_device_wait_status(epos_device_p dev, short status, double timeout) {
  struct timeval tv;
  gettimeofday(&tv, 0);
  double time = tv.tv_sec+tv.tv_usec/1e6;

  while (!(status & epos_device_get_status(dev))) {
    if (timeout >= 0.0) {
      gettimeofday(&tv, 0);
      if (tv.tv_sec+tv.tv_usec/1e6-time > timeout)
        return EPOS_DEVICE_ERROR_WAIT_TIMEOUT;
    }
  }

  return EPOS_DEVICE_ERROR_NONE;
}

short epos_device_get_control(epos_device_p dev) {
  short control;
  epos_device_read(dev, EPOS_DEVICE_INDEX_CONTROL, 0,
    (unsigned char*)&control, sizeof(short));

  return control;
}

int epos_device_set_control(epos_device_p dev, short control) {
  return epos_device_write(dev, EPOS_DEVICE_INDEX_CONTROL, 0,
    (unsigned char*)&control, sizeof(short));
}

unsigned char epos_device_get_error(epos_device_p dev) {
  unsigned char error;
  epos_device_read(dev, EPOS_DEVICE_INDEX_ERROR_REGISTER, 0, &error, 1);

  return error;
}

int epos_device_shutdown(epos_device_p dev) {
  return epos_device_set_control(dev, EPOS_DEVICE_CONTROL_SHUTDOWN);
}

int epos_device_reset(epos_device_p dev) {
  return epos_device_set_control(dev, EPOS_DEVICE_CONTROL_FAULT_RESET);
}
