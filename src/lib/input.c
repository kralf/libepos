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

#include "input.h"

short epos_input_channel_masks[] = {
  0x003F,
  0x003F,
  0x00FF,
  0x00CF,
  0x003F,
  0x03FF,
  0x00FF,
  0x00CF,
  0x0000,
};

epos_input_func_type_t epos_input_get_channel_func(epos_input_t* input,
  int channel);
int epos_input_set_channel_func(epos_input_t* input, int channel, 
  epos_input_func_type_t type);

short epos_input_get_polarity(epos_input_t* input);
int epos_input_set_polarity(epos_input_t* input, short polarity);

short epos_input_get_execute(epos_input_t* input);
int epos_input_set_execute(epos_input_t* input, short execute);

short epos_input_get_enabled(epos_input_t* input);
int epos_input_set_enabled(epos_input_t* input, short enabled);

void epos_input_init(epos_input_t* input, epos_device_t* dev) {
  int i;

  input->dev = dev;

  input->channel_mask = 0x0000;
  
  for (i = 0; i < sizeof(input->channels)/sizeof(epos_input_func_type_t); ++i)
    input->channels[i] = EPOS_INPUT_FUNC_DUMMY;

  for (i = 0; i < sizeof(input->funcs)/sizeof(epos_input_func_t); ++i) {
    input->funcs[i].channel = 0;
    input->funcs[i].polarity = epos_input_high;
    input->funcs[i].execute = 0;

    input->funcs[i].enabled = 0;
  }
}

void epos_input_init_func(epos_input_func_t* func, int channel, 
  epos_input_polarity_t polarity, int execute, int enabled) {
  func->channel = channel;
  func->polarity = polarity;
  func->execute = execute;

  func->enabled = enabled;
}

void epos_input_destroy(epos_input_t* input) {
  input->dev = 0;
}

epos_input_func_type_t epos_input_get_channel_func(epos_input_t* input, int 
    channel) {
  short type = 0;
  epos_device_read(input->dev, EPOS_INPUT_INDEX_CONFIG, channel, 
    (unsigned char*)&type, sizeof(short));

  return type;  
}

int epos_input_set_channel_func(epos_input_t* input, int channel, 
    epos_input_func_type_t type) {
  short t = EPOS_INPUT_FUNC_RESERVED;

  if (input->funcs[type].channel) {
    if (epos_device_write(input->dev, EPOS_INPUT_INDEX_CONFIG, 
        input->funcs[type].channel, (unsigned char*)&t, sizeof(short)) > 0) {
      input->channels[input->funcs[type].channel-1] = EPOS_INPUT_FUNC_DUMMY;
      input->funcs[type].channel = 0;
    }
    else
      return input->dev->error.code;
  }

  if (channel) {
    t = type;

    if (epos_device_write(input->dev, EPOS_INPUT_INDEX_CONFIG, 
        channel, (unsigned char*)&t, sizeof(short)) > 0) {
      input->channels[channel-1] = type;
      input->funcs[type].channel = channel;
    }
  }

  return input->dev->error.code;
}

short epos_input_get_polarity(epos_input_t* input) {
  short polarity = 0;
  epos_device_read(input->dev, EPOS_INPUT_INDEX_FUNCS, 
    EPOS_INPUT_SUBINDEX_POLARITY, (unsigned char*)&polarity, sizeof(short));

  return polarity;
}

int epos_input_set_polarity(epos_input_t* input, short polarity) {
  if (epos_device_write(input->dev, EPOS_INPUT_INDEX_FUNCS, 
      EPOS_INPUT_SUBINDEX_POLARITY, (unsigned char*)&polarity,
      sizeof(short)) > 0)
    input->polarity = polarity;

  return input->dev->error.code;
}

short epos_input_get_execute(epos_input_t* input) {
  short execute = 0;
  epos_device_read(input->dev, EPOS_INPUT_INDEX_FUNCS, 
    EPOS_INPUT_SUBINDEX_EXECUTE, (unsigned char*)&execute, sizeof(short));

  return execute;
}

int epos_input_set_execute(epos_input_t* input, short execute) {
  if (epos_device_write(input->dev, EPOS_INPUT_INDEX_FUNCS, 
      EPOS_INPUT_SUBINDEX_EXECUTE, (unsigned char*)&execute,
      sizeof(short)) > 0)
    input->execute = execute;

  return input->dev->error.code;
}

short epos_input_get_enabled(epos_input_t* input) {
  short enabled = 0;
  epos_device_read(input->dev, EPOS_INPUT_INDEX_FUNCS, 
    EPOS_INPUT_SUBINDEX_MASK, (unsigned char*)&enabled, sizeof(short));

  return enabled;
}

int epos_input_set_enabled(epos_input_t* input, short enabled) {
  if (epos_device_write(input->dev, EPOS_INPUT_INDEX_FUNCS, 
      EPOS_INPUT_SUBINDEX_MASK, (unsigned char*)&enabled,
      sizeof(short)) > 0)
    input->enabled = enabled;

  return input->dev->error.code;
}

int epos_input_setup(epos_input_t* input) {
  input->channel_mask = epos_input_channel_masks[input->dev->type];

  int i;
  for (i = 0; i < sizeof(input->channels)/sizeof(epos_input_func_type_t);
      ++i) {
    short c = (0x01 << i);
    if (c & input->channel_mask) {
      input->channels[i] = epos_input_get_channel_func(input, i+1);
      error_return(&input->dev->error);
    }
  }
  input->polarity = epos_input_get_polarity(input);
  error_return(&input->dev->error);  
  input->execute = epos_input_get_execute(input);
  error_return(&input->dev->error);
  input->enabled = epos_input_get_enabled(input);
  error_return(&input->dev->error);

  for (i = 0; i < sizeof(input->funcs)/sizeof(epos_input_func_t); ++i)
    epos_input_get_func(input, i, &input->funcs[i]);

  return input->dev->error.code;
}

void epos_input_get_func(epos_input_t* input, epos_input_func_type_t type,
    epos_input_func_t* func) {
  input->funcs[type].channel = epos_input_get_func_channel(input, type);
  input->funcs[type].polarity = epos_input_get_func_polarity(input, type);
  input->funcs[type].execute = epos_input_get_func_execute(input, type);

  input->funcs[type].enabled = epos_input_get_func_enabled(input, type);
}

int epos_input_set_func(epos_input_t* input, epos_input_func_type_t type,
    epos_input_func_t* func) {
  short execute = input->execute;

  if (!epos_input_set_execute(input, 0) &&
      !epos_input_set_func_channel(input, type, func->channel) &&
      !epos_input_set_func_polarity(input, type, func->polarity) &&
      !epos_input_set_func_enabled(input, type, func->enabled)) {
    short mask = 0x0001 << type;
    return epos_input_set_execute(input, (func->execute) ? execute | mask : 
      execute & ~mask);
  }

  return input->dev->error.code;
}

int epos_input_get_func_state(epos_input_t* input, epos_input_func_type_t 
    type) {
  short mask = 0x0001 << type;
  short state = 0;

  if (epos_device_read(input->dev, EPOS_INPUT_INDEX_FUNCS, 
      EPOS_INPUT_SUBINDEX_STATE, (unsigned char*)&state, sizeof(short)) > 0)
    return ((state & mask) != 0);
  else
    return 0;
}

int epos_input_get_func_channel(epos_input_t* input, epos_input_func_type_t 
    type) {
  int i;
  for (i = 0; i < sizeof(input->channels)/sizeof(epos_input_func_type_t); ++i)
    if (input->channels[i] == type) return i+1;

  return 0;
}

int epos_input_set_func_channel(epos_input_t* input, epos_input_func_type_t 
    type, int channel) {
  return epos_input_set_channel_func(input, channel, type);
}

epos_input_polarity_t epos_input_get_func_polarity(epos_input_t* input, 
    epos_input_func_type_t type) {
  short mask = 0x0001 << type;
  return ((input->polarity & mask) != 0);
}

int epos_input_set_func_polarity(epos_input_t* input, epos_input_func_type_t 
    type, epos_input_polarity_t polarity) {
  short mask = 0x0001 << type;
  return epos_input_set_polarity(input, (polarity) ? input->polarity | mask : 
    input->polarity & ~mask);
}

int epos_input_get_func_execute(epos_input_t* input, epos_input_func_type_t 
    type) {
  short mask = 0x0001 << type;
  return ((input->execute & mask) != 0);
}

int epos_input_set_func_execute(epos_input_t* input, epos_input_func_type_t
    type, int execute) {
  short mask = 0x0001 << type;
  return epos_input_set_execute(input, (execute) ? input->execute | mask : 
    input->execute & ~mask);
}

int epos_input_get_func_enabled(epos_input_t* input, epos_input_func_type_t 
    type) {
  short mask = 0x0001 << type;
  return ((input->enabled & mask) != 0);
}

int epos_input_set_func_enabled(epos_input_t* input, epos_input_func_type_t 
    type, int enabled) {
  short mask = 0x0001 << type;
  return epos_input_set_enabled(input, (enabled) ? input->enabled | mask : 
    input->enabled & ~mask);
}
