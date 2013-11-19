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

  config_parser_init_default(&parser,
    "Print input functionalities and states of an EPOS device",
    "Establish the communication with a connected EPOS device and attempt "
    "to retrieve its input functionalities and states until receiving SIGINT. "
    "The communication interface depends on the momentarily selected "
    "alternative of the underlying CANopen library.");
  epos_init_config_parse(&node, &parser, 0, argc, argv,
    config_parser_exit_error);
  
  signal(SIGINT, epos_signaled);
  
  if (epos_open(&node))
    return -1;

  fprintf(stdout, "%5s  %5s  %5s  %5s  %5s  %5s\n", "Type", "Chan", "Pol",
    "Exec", "Mask", "State");
  int num_inputs = sizeof(node.input.funcs)/sizeof(epos_input_func_t);
  while (!quit) {
    int i;    
    for (i = 0; i < num_inputs; ++i)
      fprintf(stdout, "\r%5d  %5d  %5d  %5d  %5d  %5d%s", i,
        node.input.funcs[i].channel,
        node.input.funcs[i].polarity,
        node.input.funcs[i].execute,
        node.input.funcs[i].enabled,
        epos_input_get_func_state(&node.input, i),
        (i+1 < num_inputs) ? "\n" : "");
    fprintf(stdout, "%c[%dA\r", 0x1B, num_inputs-1);
  }
  fprintf(stdout, "%c[%dB\n", 0x1B, num_inputs-1);
  epos_close(&node);

  epos_destroy(&node);
  return 0;
}
