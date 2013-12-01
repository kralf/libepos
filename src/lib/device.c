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
  "Failed to open EPOS device",
  "Failed to close EPOS device",
  "Invalid EPOS object size",
  "Failed to send to EPOS device",
  "Failed to receive from EPOS device",
  "EPOS communication error (abort)",
  "EPOS internal device error",
  "Failed to read EPOS device data object",
  "Failed to write EPOS device data object",
  "Invalid EPOS CAN bit rate",
  "Invalid EPOS RS232 baud rate",
  "EPOS device timeout",
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
  EPOS_DEVICE_CAN_BIT_RATE_RESERVED,
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

void epos_device_init(epos_device_t* dev, can_device_t* can_dev, int node_id,
    int reset) {
  dev->can_dev = can_dev;
  dev->node_id = node_id;

  dev->reset = reset;

  dev->hardware_generation = 0;
  dev->type = epos_device_unknown;
  
  dev->num_read = 0;
  dev->num_written = 0;
  
  error_init(&dev->error, epos_device_errors);
}

void epos_device_destroy(epos_device_t* dev) {
  dev->can_dev = 0;
  dev->node_id = CAN_NODE_ID_BROADCAST;
  
  error_destroy(&dev->error);
}

int epos_device_open(epos_device_t* dev) {
  error_clear(&dev->error);
  
  if (!can_device_open(dev->can_dev)) {
    if (dev->reset && epos_device_reset(dev))
      return dev->error.code;

    dev->node_id = epos_device_get_id(dev);
    error_return(&dev->error);

    dev->can_bit_rate = epos_device_get_can_bit_rate(dev);
    error_return(&dev->error);
    dev->rs232_baud_rate = epos_device_get_rs232_baud_rate(dev);
    error_return(&dev->error);

    dev->hardware_version = epos_device_get_hardware_version(dev);
    error_return(&dev->error);
    dev->software_version = epos_device_get_software_version(dev);
    error_return(&dev->error);

    epos_device_type_t type;
    for (type = 0; type < sizeof(epos_device_hardware_versions)/
        sizeof(short); ++type)
      if ((dev->hardware_version & EPOS_DEVICE_TYPE_MASK) ==
        epos_device_hardware_versions[type]) {
      dev->type = type;
      dev->hardware_generation = epos_device_hardware_generations[type];
    }
    
    epos_device_shutdown(dev);
  }
  else
    error_blame(&dev->error, &dev->can_dev->error, EPOS_DEVICE_ERROR_OPEN);
  
  return dev->error.code;
}

int epos_device_close(epos_device_t* dev) {
  if (!epos_device_shutdown(dev) && can_device_close(dev->can_dev))
    error_blame(&dev->error, &dev->can_dev->error, EPOS_DEVICE_ERROR_CLOSE);
  
  return dev->error.code;
}

int epos_device_send_message(epos_device_t* dev, const can_message_t*
    message) {
  error_clear(&dev->error);
  
  if (can_device_send_message(dev->can_dev, message))
    error_blame(&dev->error, &dev->can_dev->error, EPOS_DEVICE_ERROR_SEND);

  return dev->error.code;
}

int epos_device_receive_message(epos_device_t* dev, can_message_t* message) {
  error_clear(&dev->error);
  
  if (!can_device_receive_message(dev->can_dev, message)) {
    if ((message->id >= CAN_COB_ID_SDO_EMERGENCY) &&
        (message->id <= CAN_COB_ID_SDO_EMERGENCY+CAN_NODE_ID_MAX)) {
      short code;

      memcpy(&code, &message->content[0], sizeof(code));
      if (code)
        error_setf(&dev->error, EPOS_DEVICE_ERROR_INTERNAL, "[0x%hX] %s",
          message->id-CAN_COB_ID_SDO_EMERGENCY, code, epos_error_device(code));
    }
    else if (message->content[0] == CAN_CMD_SDO_ABORT) {
      int code;

      memcpy(&code, &message->content[4], sizeof(code));
      error_setf(&dev->error, EPOS_DEVICE_ERROR_ABORT, "[0x%hX] %s",
        message->id-CAN_COB_ID_SDO_RECEIVE, code, epos_error_comm(code));
    }
  }
  else
    error_blame(&dev->error, &dev->can_dev->error, EPOS_DEVICE_ERROR_RECEIVE);
  
  return dev->error.code;
}

int epos_device_read(epos_device_t* dev, short index, unsigned char subindex,
    unsigned char* data, size_t num) {
  can_message_t message;
  memset(&message, 0, sizeof(can_message_t));

  error_clear(&dev->error);
  
  message.id = CAN_COB_ID_SDO_SEND+dev->node_id;
  message.content[0] = CAN_CMD_SDO_READ_SEND;
  message.content[1] = index;
  message.content[2] = index >> 8;
  message.content[3] = subindex;
  message.length = 8;

  if (epos_device_send_message(dev, &message) ||
      epos_device_receive_message(dev, &message))
    return -dev->error.code;
  
  memcpy(data, &message.content[4], num);
  ++dev->num_read;

  return num;
}

int epos_device_write(epos_device_t* dev, short index, unsigned char subindex,
    unsigned char* data, size_t num) {
  can_message_t message;
  size_t num_written = 0;

  error_clear(&dev->error);
  
  if (num > 4) {
    message.id = CAN_COB_ID_SDO_SEND+dev->node_id;
    message.content[0] = CAN_CMD_SDO_WRITE_SEND_N_BYTE_INIT;
    message.content[1] = index;
    message.content[2] = index >> 8;
    message.content[3] = subindex;
    message.content[4] = num;
    message.content[5] = num >> 8;
    message.content[6] = num >> 16;
    message.content[7] = num >> 24;
    
    if (epos_device_send_message(dev, &message) ||
        epos_device_receive_message(dev, &message))
      return -dev->error.code;
      
    ++dev->num_written;
  }
  
  while (num_written < num) {
    message.id = CAN_COB_ID_SDO_SEND+dev->node_id;    
    switch (num) {
      case 1 :
        message.content[0] = CAN_CMD_SDO_WRITE_SEND_1_BYTE;
        break;
      case 2 :
        message.content[0] = CAN_CMD_SDO_WRITE_SEND_2_BYTE;
        break;
      case 4 :
        message.content[0] = CAN_CMD_SDO_WRITE_SEND_4_BYTE;
        break;
      default:
        if (num > 4)
          message.content[0] = CAN_CMD_SDO_WRITE_SEND_N_BYTE_SEGMENT;
        else {
          error_setf(&dev->error, EPOS_DEVICE_ERROR_INVALID_SIZE, "%d", num);
          return -dev->error.code;
        }
    }
    message.content[1] = index;
    message.content[2] = index >> 8;
    message.content[3] = subindex;
    message.content[4] = data[num_written];
    message.content[5] = (num-num_written > 1) ? data[num_written+1] : 0x00;
    message.content[6] = (num-num_written > 2) ? data[num_written+2] : 0x00;
    message.content[7] = (num-num_written > 2) ? data[num_written+3] : 0x00;
    message.length = 8;

    if (epos_device_send_message(dev, &message) ||
        epos_device_receive_message(dev, &message))
      return -dev->error.code;
    
    num_written += (num_written+4 > num) ? num : num_written+4;
    ++dev->num_written;
  }

  return num_written;
}

int epos_device_store_parameters(epos_device_t* dev) {
  epos_device_write(dev, EPOS_DEVICE_INDEX_STORE,
    EPOS_DEVICE_SUBINDEX_STORE, (unsigned char*)"evas", 4);
  
  return dev->error.code;
}

int epos_device_restore_parameters(epos_device_t* dev) {
  epos_device_write(dev, EPOS_DEVICE_INDEX_RESTORE,
    EPOS_DEVICE_SUBINDEX_RESTORE, (unsigned char*)"daol", 4);
  
  return dev->error.code;
}

int epos_device_get_id(epos_device_t* dev) {
  unsigned char id = 0;
  epos_device_read(dev, EPOS_DEVICE_INDEX_ID, 0, &id, 1);

  return id;
}

int epos_device_get_can_bit_rate(epos_device_t* dev) {
  short can_bit_rate = 0;
  if (epos_device_read(dev, EPOS_DEVICE_INDEX_CAN_BIT_RATE, 0,
      (unsigned char*)&can_bit_rate, sizeof(short)) < 0)
    return 0;

  if (dev->hardware_generation == 1)
    return epos_device_can_bit_rates[can_bit_rate];
  else
    return epos2_device_can_bit_rates[can_bit_rate];
}

int epos_device_set_can_bit_rate(epos_device_t* dev, int bit_rate) {
  int* bit_rates = (dev->hardware_generation == 1) ?
    epos_device_can_bit_rates : epos2_device_can_bit_rates;
  size_t num_bit_rates = (dev->hardware_generation == 1) ?
    sizeof(epos_device_can_bit_rates)/sizeof(int) :
    sizeof(epos2_device_can_bit_rates)/sizeof(int);
  short b;
  
  error_clear(&dev->error);
  
  for (b = 0; b < num_bit_rates; ++b)
      if (bit_rate == bit_rates[b]) {
    if (epos_device_write(dev, EPOS_DEVICE_INDEX_CAN_BIT_RATE, 0,
        (unsigned char*)&b, sizeof(short)) > 0)
      dev->can_bit_rate = bit_rate;    

    return dev->error.code;
  }
  
  error_setf(&dev->error, EPOS_DEVICE_ERROR_INVALID_BIT_RATE, "%d", bit_rate);
  return dev->error.code;
}

int epos_device_get_rs232_baud_rate(epos_device_t* dev) {
  short rs232_baud_rate = 0;
  if (epos_device_read(dev, EPOS_DEVICE_INDEX_RS232_BAUD_RATE, 0,
      (unsigned char*)&rs232_baud_rate, sizeof(short)) < 0)
    return 0;

  return epos_device_rs232_baud_rates[rs232_baud_rate];
}

int epos_device_set_rs232_baud_rate(epos_device_t* dev, int baud_rate) {  
  short b;
  
  error_clear(&dev->error);

  for (b = 0; b < sizeof(epos_device_rs232_baud_rates)/sizeof(int); ++b)
      if (epos_device_rs232_baud_rates[b] == baud_rate) {
    if (epos_device_write(dev, EPOS_DEVICE_INDEX_RS232_BAUD_RATE, 0,
        (unsigned char*)&b, sizeof(short)) > 0)
      dev->rs232_baud_rate = baud_rate;
    
    return dev->error.code;
  }
  
  error_setf(&dev->error, EPOS_DEVICE_ERROR_INVALID_BAUD_RATE, "%d",
    baud_rate);
  return dev->error.code;
}

short epos_device_get_hardware_version(epos_device_t* dev) {
  short hardware_version = 0;
  epos_device_read(dev, EPOS_DEVICE_INDEX_VERSION,
    EPOS_DEVICE_SUBINDEX_HARDWARE_VERSION,
    (unsigned char*)&hardware_version, sizeof(short));

  return hardware_version;
}

short epos_device_get_software_version(epos_device_t* dev) {
  short software_version = 0;
  epos_device_read(dev, EPOS_DEVICE_INDEX_VERSION,
    EPOS_DEVICE_SUBINDEX_SOFTWARE_VERSION,
    (unsigned char*)&software_version, sizeof(short));

  return software_version;
}

short epos_device_get_status(epos_device_t* dev) {
  short status = 0;
  epos_device_read(dev, EPOS_DEVICE_INDEX_STATUS, 0,
    (unsigned char*)&status, sizeof(short));

  return status;
}

int epos_device_wait_status(epos_device_t* dev, short status, double
    timeout) {
  struct timeval tv;
  gettimeofday(&tv, 0);
  double time = tv.tv_sec+tv.tv_usec/1e6;

  error_clear(&dev->error);
  
  while (!(status & epos_device_get_status(dev)) && !dev->error.code) {
    if (timeout >= 0.0) {
      gettimeofday(&tv, 0);
      
      if (tv.tv_sec+tv.tv_usec/1e6-time > timeout) {
        error_set(&dev->error, EPOS_DEVICE_ERROR_WAIT_TIMEOUT);
        break;
      }
    }
  }

  return dev->error.code;
}

short epos_device_get_control(epos_device_t* dev) {
  short control = 0;
  epos_device_read(dev, EPOS_DEVICE_INDEX_CONTROL, 0,
    (unsigned char*)&control, sizeof(short));

  return control;
}

int epos_device_set_control(epos_device_t* dev, short control) {
  epos_device_write(dev, EPOS_DEVICE_INDEX_CONTROL, 0,
    (unsigned char*)&control, sizeof(short));
  
  return dev->error.code;
}

short epos_device_get_configuration(epos_device_t* dev) {
  short configuration = 0;
  epos_device_read(dev, EPOS_DEVICE_INDEX_MISC_CONFIGURATION, 0,
    (unsigned char*)&configuration, sizeof(short));

  return configuration;
}

int epos_device_set_configuration(epos_device_t* dev, short configuration) {
  epos_device_write(dev, EPOS_DEVICE_INDEX_MISC_CONFIGURATION, 0,
    (unsigned char*)&configuration, sizeof(short));
  
  return dev->error.code;
}

unsigned char epos_device_get_error(epos_device_t* dev) {
  unsigned char error = 0;
  epos_device_read(dev, EPOS_DEVICE_INDEX_ERROR_REGISTER, 0, &error, 1);

  return error;
}

int epos_device_shutdown(epos_device_t* dev) {
  return epos_device_set_control(dev, EPOS_DEVICE_CONTROL_SHUTDOWN);
}

int epos_device_reset(epos_device_t* dev) {
  return epos_device_set_control(dev, EPOS_DEVICE_CONTROL_FAULT_RESET);
}
