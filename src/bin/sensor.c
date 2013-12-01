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

int quit = 0;

void epos_signaled(int signal) {
  quit = 1;
}

int main(int argc, char **argv) {
  config_parser_t parser;
  epos_node_t node;

  config_parser_init(&parser,
    "Print sensor position of an EPOS device",
    "Establish the communication with a connected EPOS device and attempt to "
    "retrieve its sensor position until receiving SIGINT. The communication "
    "interface depends on the momentarily selected alternative of the "
    "underlying CANopen library.");
  epos_node_init_config_parse(&node, &parser, 0, argc, argv,
    config_parser_exit_error);
  config_parser_destroy(&parser);

  signal(SIGINT, epos_signaled);

  epos_node_connect(&node);
  error_exit(&node.error);
  
  while (!quit) {
    int pos = epos_sensor_get_position(&node.sensor);
    error_exit(&node.dev.error);
    
    fprintf(stdout, "\rSensor position: %10d steps", pos);
    fflush(stdout);
  }
  fprintf(stdout, "\n");
  
  epos_node_disconnect(&node);
  error_exit(&node.error);

  epos_node_destroy(&node);
  return 0;
}
