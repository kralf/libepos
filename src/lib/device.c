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
#include <sys/time.h>

#include "device.h"
#include "error.h"

const char* epos_device_errors[] = {
  "Success",
  "Error opening EPOS device",
  "Error closing EPOS device",
  "Invalid EPOS object size",
  "Error sending to EPOS device",
  "Error receiving from EPOS device",
  "EPOS communication error (abort error)",
  "EPOS internal device error",
  "Error reading from EPOS device",
  "Error writing to EPOS device",
  "Invalid CAN bit rate",
  "Invalid RS232 baud rate",
  "Wait operation timed out",
};

short epos_device_hardware_versions[] = {
  0x6010,
  0x6210,
  0x6410,
  0x6610,
  0x6220,
  0x6320,
  0x6420,
  0x6120,
};

short epos_device_hardware_generations[] = {
  1,
  1,
  1,
  1,
  2,
  2,
  2,
  2,
};

const char* epos_device_names[] = {
  "EPOS 24/1",
  "EPOS 24/5",
  "EPOS 70/10",
  "MCD EPOS 60 W",
  "EPOS2 24/5",
  "EPOS2 50/5",
  "EPOS2 70/10",
  "EPOS2 Module 36/2",
  "Unknown",
};

int epos_device_can_bit_rates[] = {
  1000,
   800,
   500,
   250,
   125,
    50,
    20,
};

int epos2_device_can_bit_rates[] = {
  1000,
   800,
   500,
   250,
   125,
     0,
    50,
    20,
    10,
  EPOS_DEVICE_CAN_BIT_RATE_AUTO,
};

int epos_device_rs232_baud_rates[] = {
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

  dev->hardware_generation = 0;
  dev->type = epos_device_unknown;
  
  dev->num_read = 0;
  dev->num_written = 0;
}

void epos_device_destroy(epos_device_p dev) {
  dev->can_dev = 0;
  dev->node_id = CAN_NODE_ID_BROADCAST;
}

int epos_device_open(epos_device_p dev) {
  if (!can_open(dev->can_dev)) {
    if (dev->reset && epos_device_reset(dev))
      return EPOS_DEVICE_ERROR_OPEN;

    dev->node_id = epos_device_get_id(dev);

    dev->can_bit_rate = epos_device_get_can_bit_rate(dev);
    dev->rs232_baud_rate = epos_device_get_rs232_baud_rate(dev);

    dev->hardware_version = epos_device_get_hardware_version(dev);
    dev->software_version = epos_device_get_software_version(dev);

    if (epos_device_shutdown(dev))
      return EPOS_DEVICE_ERROR_OPEN;
    else
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
    if ((message->id >= CAN_COB_ID_SDO_EMERGENCY) &&
        (message->id <= CAN_COB_ID_SDO_EMERGENCY+CAN_NODE_ID_MAX)) {
      short code;

      memcpy(&code, &message->content[0], sizeof(code));
      if (code != EPOS_DEVICE_ERROR_NONE) {
        fprintf(stderr, "Node %d device error: [0x%hX] %s\n",
          message->id-CAN_COB_ID_SDO_EMERGENCY, code, epos_error_device(code));

        return EPOS_DEVICE_ERROR_INTERNAL;
      }
      else
        return EPOS_DEVICE_ERROR_NONE;
    }
    else if (message->content[0] == CAN_CMD_SDO_ABORT) {
      int code;

      memcpy(&code, &message->content[4], sizeof(code));
      fprintf(stderr, "Node %d communication error: [0x%X] %s\n",
        message->id-CAN_COB_ID_SDO_RECEIVE, code, epos_error_comm(code));

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

  message.id = CAN_COB_ID_SDO_SEND+dev->node_id;
  message.content[0] = CAN_CMD_SDO_READ_SEND;
  message.content[1] = index;
  message.content[2] = index >> 8;
  message.content[3] = subindex;
  message.length = 8;

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

  message.id = CAN_COB_ID_SDO_SEND+dev->node_id;
  switch (num) {
    case 1 : message.content[0] = CAN_CMD_SDO_WRITE_SEND_1_BYTE;
             break;
    case 2 : message.content[0] = CAN_CMD_SDO_WRITE_SEND_2_BYTE;
             break;
    case 4 : message.content[0] = CAN_CMD_SDO_WRITE_SEND_4_BYTE;
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
  message.length = 8;

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
    EPOS_DEVICE_SUBINDEX_STORE, (unsigned char*)"evas", 4);
}

int epos_device_restore_parameters(epos_device_p dev) {
  return epos_device_write(dev, EPOS_DEVICE_INDEX_RESTORE,
    EPOS_DEVICE_SUBINDEX_RESTORE, (unsigned char*)"daol", 4);
}

int epos_device_get_id(epos_device_p dev) {
  unsigned char id;
  epos_device_read(dev, EPOS_DEVICE_INDEX_ID, 0, &id, 1);

  return id;
}

int epos_device_get_can_bit_rate(epos_device_p dev) {
  short can_bit_rate;
  epos_device_read(dev, EPOS_DEVICE_INDEX_CAN_BIT_RATE, 0,
    (unsigned char*)&can_bit_rate, sizeof(short));

  if (dev->hardware_generation == 1)
    return epos_device_can_bit_rates[can_bit_rate];
  else
    return epos2_device_can_bit_rates[can_bit_rate];
}

int epos_device_set_can_bit_rate(epos_device_p dev, int bit_rate) {
  int* bit_rates = (dev->hardware_generation == 1) ?
    epos_device_can_bit_rates : epos2_device_can_bit_rates;
  size_t num_bit_rates = (dev->hardware_generation == 1) ?
    sizeof(epos_device_can_bit_rates)/sizeof(int) :
    sizeof(epos2_device_can_bit_rates)/sizeof(int);
  short b;
  
  for (b = 0; b < num_bit_rates; ++b)
      if (bit_rate == bit_rates[b]) {
    int result = epos_device_write(dev, EPOS_DEVICE_INDEX_CAN_BIT_RATE, 0,
      (unsigned char*)&b, sizeof(short));
    if (!result)
      dev->can_bit_rate = bit_rate;
    
    return result;
  }
  
  return EPOS_DEVICE_ERROR_INVALID_BIT_RATE;
}

int epos_device_get_rs232_baud_rate(epos_device_p dev) {
  short rs232_baud_rate;
  epos_device_read(dev, EPOS_DEVICE_INDEX_RS232_BAUD_RATE, 0,
    (unsigned char*)&rs232_baud_rate, sizeof(short));

  return epos_device_rs232_baud_rates[rs232_baud_rate];
}

int epos_device_set_rs232_baud_rate(epos_device_p dev, int baud_rate) {
  short b;
  for (b = 0; b < sizeof(epos_device_rs232_baud_rates)/sizeof(int); ++b)
      if (epos_device_rs232_baud_rates[b] == baud_rate) {
    int result = epos_device_write(dev, EPOS_DEVICE_INDEX_RS232_BAUD_RATE, 0,
      (unsigned char*)&b, sizeof(short));
    if (!result)
      dev->rs232_baud_rate = baud_rate;
    
    return result;
  }
  return EPOS_DEVICE_ERROR_INVALID_BAUD_RATE;
}

short epos_device_get_hardware_version(epos_device_p dev) {
  short hardware_version;
  epos_device_type_t t;

  epos_device_read(dev, EPOS_DEVICE_INDEX_VERSION,
    EPOS_DEVICE_SUBINDEX_HARDWARE_VERSION,
    (unsigned char*)&hardware_version, sizeof(short));

  for (t = 0; t < sizeof(epos_device_hardware_versions)/sizeof(short); ++t) {
    if ((hardware_version & EPOS_DEVICE_TYPE_MASK) ==
      epos_device_hardware_versions[t]) {
      dev->type = t;
      dev->hardware_generation = epos_device_hardware_generations[t];
    }
  }
    
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

short epos_device_get_configuration(epos_device_p dev) {
  short configuration;
  epos_device_read(dev, EPOS_DEVICE_INDEX_MISC_CONFIGURATION, 0,
    (unsigned char*)&configuration, sizeof(short));

  return configuration;
}

int epos_device_set_configuration(epos_device_p dev, short configuration) {
  return epos_device_write(dev, EPOS_DEVICE_INDEX_MISC_CONFIGURATION, 0,
    (unsigned char*)&configuration, sizeof(short));
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
