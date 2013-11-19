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
#include "velocity.h"

int quit = 0;

void epos_signaled(int signal) {
  quit = 1;
}

int main(int argc, char **argv) {
  config_parser_t parser;
  epos_node_t node;
  epos_velocity_t vel;

  config_parser_init_default(&parser,
    "Start EPOS controller in velocity mode",
    "Establish the communication with a connected EPOS device and attempt to "
    "start the controller in velocity mode. The controller will be stopped "
    "if SIGINT is received. The communication interface depends on the "
    "momentarily selected alternative of the underlying CANopen library.");  
  config_param_p velocity_param = config_set_param_value_range(
    &parser.arguments,
    "VELOCITY",
    config_param_type_float,
    "",
    "(-inf, inf)",
    "The demanded angular velocity in [deg/s]");
  epos_init_config_parse(&node, &parser, 0, argc, argv,
    config_parser_exit_error);
  
  signal(SIGINT, epos_signaled);

  if (epos_open(&node))
    return -1;
  
  float target_value = deg_to_rad(config_param_get_float(velocity_param));
  epos_velocity_init(&vel, target_value);
  if (!epos_velocity_start(&node, &vel)) {
    while (!quit) {
      fprintf(stdout, "\rAngular velocity: %8.2f deg/s",
        rad_to_deg(epos_get_velocity(&node)));
      fflush(stdout);
    }
    fprintf(stdout, "\n");
    epos_velocity_stop(&node);
  }
  epos_close(&node);

  epos_destroy(&node);
  return 0;
}
