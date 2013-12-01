/***************************************************************************
 *   Copyright (C) 2004 by Ralf Kaestner                                   *
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

#include <config/parser.h>

#include "epos.h"


#define EPOS_BIT_RATE_PARAMETER_BIT_RATE       "BIT_RATE"
#define EPOS_BIT_RATE_PARAMETER_STORE          "store"

config_param_t epos_bit_rate_default_arguments_params[] = {
  {EPOS_BIT_RATE_PARAMETER_BIT_RATE,
    config_param_type_int,
    "-1",
    "[-1, 1000]",
    "The new CAN bit rate of the EPOS device in [kbit/s], zero for putting "
    "the device in in automatic bit rate detection mode, or a negative value "
    "for querying the current bit rate"},
};

const config_default_t epos_bit_rate_default_arguments = {
  epos_bit_rate_default_arguments_params,
  sizeof(epos_bit_rate_default_arguments_params)/sizeof(config_param_t),
};

config_param_t epos_bit_rate_default_options_params[] = {
  {EPOS_BIT_RATE_PARAMETER_STORE,
    config_param_type_bool,
    "false",
    "false|true",
    "Persistently store the new CAN bit rate on the EPOS device"},
};

const config_default_t epos_bit_rate_default_options = {
  epos_bit_rate_default_options_params,
  sizeof(epos_bit_rate_default_options_params)/sizeof(config_param_t),
};

int main(int argc, char **argv) {
  config_parser_t parser;
  epos_node_t node;

  config_parser_init_default(&parser, &epos_bit_rate_default_arguments,
    &epos_bit_rate_default_options,
    "Query or set the CAN bit rate of an EPOS device",
    "Establish the communication with a connected EPOS device and attempt to "
    "query or set its CAN bit rate. The communication interface depends "
    "on the momentarily selected alternative of the underlying CANopen "
    "library.");
  epos_node_init_config_parse(&node, &parser, 0, argc, argv,
    config_parser_exit_error);
  
  int bit_rate = config_get_int(&parser.arguments,
    EPOS_BIT_RATE_PARAMETER_BIT_RATE);
  config_param_bool_t store = config_get_bool(&parser.options,
    EPOS_BIT_RATE_PARAMETER_STORE);
  
  epos_node_connect(&node);
  error_exit(&node.error);

  if (node.dev.can_bit_rate != EPOS_DEVICE_CAN_BIT_RATE_AUTO)
    fprintf(stdout, "Current CAN bit rate: %d kbit/s\n",
      node.dev.can_bit_rate);
  else
    fprintf(stdout, "Current CAN bit rate: auto detect\n");
  
  if (bit_rate) {
    epos_device_set_can_bit_rate(&node.dev, bit_rate);
    error_exit(&node.dev.error);

    if (node.dev.can_bit_rate != EPOS_DEVICE_CAN_BIT_RATE_AUTO)
      fprintf(stdout, "New CAN bit rate: %d kbit/s\n",
        node.dev.can_bit_rate);
    else
      fprintf(stdout, "New CAN bit rate: auto detect\n");
  }

  if (store) {
    epos_device_store_parameters(&node.dev);
    error_exit(&node.dev.error);
  }
  
  epos_node_disconnect(&node);
  error_exit(&node.error);

  epos_node_destroy(&node);
  config_parser_destroy(&parser);
  
  return 0;
}
