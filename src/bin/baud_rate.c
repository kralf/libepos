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

int main(int argc, char **argv) {
  config_parser_t parser;
  epos_node_t node;

  config_parser_init_default(&parser,
    "Query or set the RS232 baud rate of an EPOS device",
    "Establish the communication with a connected EPOS device and attempt to "
    "query or set its RS232 baud rate. The communication interface depends "
    "on the momentarily selected alternative of the underlying CANopen "
    "library.");
  config_param_p baud_rate_param = config_set_param_value_range(
    &parser.arguments,
    "BAUD_RATE",
    config_param_type_int,
    "0",
    "[0, 115200]",
    "The new RS232 baud rate of the EPOS device in [baud] or zero for "
    "querying the current baud rate");
  config_param_p store_param = config_set_param_value_range(
    &parser.options,
    "store",
    config_param_type_bool,
    "false",
    "false|true",
    "Persistently store the new RS232 baud rate on the EPOS device");
  epos_init_config_parse(&node, &parser, 0, argc, argv,
    config_parser_exit_both);

  if (epos_open(&node))
    return -1;
  
  fprintf(stdout, "Current RS232 baud rate: %d baud\n",
    node.dev.rs232_baud_rate);
  
  int baud_rate = config_param_get_int(baud_rate_param);
  if (baud_rate) {
    int error = epos_device_set_rs232_baud_rate(&node.dev, baud_rate);
    if (error) {
      fprintf(stderr, "Error setting new RS232 baud rate: %s\n",
        epos_device_errors[error]);
      return -1;
    }
    fprintf(stdout, "New RS232 baud rate: %d baud\n",
      node.dev.rs232_baud_rate);
  }
  
  config_param_bool_t store = config_param_get_bool(store_param);
  if (store) {
    int error = epos_device_store_parameters(&node.dev);
    if (error) {
      fprintf(stderr, "Error storing parameters: %s\n",
        epos_device_errors[error]);
      return -1;
    }
  }
  epos_close(&node);

  epos_destroy(&node);
  return 0;
}
