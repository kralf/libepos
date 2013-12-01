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
#include <signal.h>

#include <config/parser.h>

#include "epos.h"
#include "position.h"
#include "macros.h"

#define EPOS_POSITION_PARAMETER_POSITION        "POSITION"

config_param_t epos_position_default_arguments_params[] = {
  {EPOS_POSITION_PARAMETER_POSITION,
    config_param_type_float,
    "",
    "(-inf, inf)",
    "The demanded angular position in [deg]"},
};

const config_default_t epos_position_default_arguments = {
  epos_position_default_arguments_params,
  sizeof(epos_position_default_arguments_params)/sizeof(config_param_t),
};

int quit = 0;

void epos_signaled(int signal) {
  quit = 1;
}

int main(int argc, char **argv) {
  config_parser_t parser;
  epos_node_t node;
  epos_position_t position;

  config_parser_init_default(&parser, &epos_position_default_arguments, 0,
    "Start EPOS controller in position mode",
    "Establish the communication with a connected EPOS device and attempt to "
    "start the controller in position mode. The controller will be stopped "
    "if SIGINT is received. The communication interface depends on the "
    "momentarily selected alternative of the underlying CANopen library.");  
  epos_node_init_config_parse(&node, &parser, 0, argc, argv,
    config_parser_exit_error);
  
  float target_value = config_get_float(&parser.arguments,
    EPOS_POSITION_PARAMETER_POSITION);
  
  signal(SIGINT, epos_signaled);

  epos_node_connect(&node);
  error_exit(&node.error);
  
  epos_position_init(&position, target_value);
  epos_position_start(&node, &position);
  error_exit(&node.dev.error);
  
  while (!quit) {
    float actual_value = epos_node_get_position(&node);
    error_exit(&node.error);
    
    fprintf(stdout, "\rAngular position: %8.2f deg",
      rad_to_deg(actual_value));
    fflush(stdout);
  }
  fprintf(stdout, "\n");
  
  epos_position_stop(&node);
  error_exit(&node.dev.error);

  epos_node_disconnect(&node);
  error_exit(&node.error);

  epos_node_destroy(&node);
  config_parser_destroy(&parser);
  
  return 0;
}
