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

#define EPOS_BAUD_RATE_PARAMETER_BAUD_RATE      "BAUD_RATE"
#define EPOS_BAUD_RATE_PARAMETER_STORE          "store"

config_param_t epos_baud_rate_default_arguments_params[] = {
  {EPOS_BAUD_RATE_PARAMETER_BAUD_RATE,
    config_param_type_int,
    "0",
    "[0, 115200]",
    "The new RS232 baud rate of the EPOS device in [baud] or zero for "
    "querying the current baud rate"},
};

const config_default_t epos_baud_rate_default_arguments = {
  epos_baud_rate_default_arguments_params,
  sizeof(epos_baud_rate_default_arguments_params)/sizeof(config_param_t),
};

config_param_t epos_baud_rate_default_options_params[] = {
  {EPOS_BAUD_RATE_PARAMETER_STORE,
    config_param_type_bool,
    "false",
    "false|true",
    "Persistently store the new RS232 baud rate on the EPOS device"},
};

const config_default_t epos_baud_rate_default_options = {
  epos_baud_rate_default_options_params,
  sizeof(epos_baud_rate_default_options_params)/sizeof(config_param_t),
};

int main(int argc, char **argv) {
  config_parser_t parser;
  epos_node_t node;

  config_parser_init_default(&parser, &epos_baud_rate_default_arguments,
    &epos_baud_rate_default_options,
    "Query or set the RS232 baud rate of an EPOS device",
    "Establish the communication with a connected EPOS device and attempt to "
    "query or set its RS232 baud rate. The communication interface depends "
    "on the momentarily selected alternative of the underlying CANopen "
    "library.");
  epos_node_init_config_parse(&node, &parser, 0, argc, argv,
    config_parser_exit_error);

  int baud_rate = config_get_int(&parser.arguments,
    EPOS_BAUD_RATE_PARAMETER_BAUD_RATE);
  config_param_bool_t store = config_get_bool(&parser.options,
    EPOS_BAUD_RATE_PARAMETER_STORE);
  
  epos_node_connect(&node);
  error_exit(&node.error);
  
  fprintf(stdout, "Current RS232 baud rate: %d baud\n",
    node.dev.rs232_baud_rate);
  
  if (baud_rate) {
    epos_device_set_rs232_baud_rate(&node.dev, baud_rate);
    error_exit(&node.dev.error);

    fprintf(stdout, "New RS232 baud rate: %d baud\n",
      node.dev.rs232_baud_rate);
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
