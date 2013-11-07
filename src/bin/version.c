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
    "Print version information about an EPOS device",
    "Establish the communication with a connected EPOS device and attempt "
    "to retrieve its hardware and software version. The communication "
    "interface depends on the momentarily selected alternative of the "
    "underlying CANopen library.");
  epos_init_config_parse(&node, &parser, 0, argc, argv,
    config_parser_exit_both);

  if (epos_open(&node))
    return -1;
  
  printf("Hardware version: 0x%04X (%s)\n", node.dev.hardware_version,
    epos_device_names[node.dev.type]);
  printf("Software version: 0x%04X\n", node.dev.software_version);
  epos_close(&node);

  epos_destroy(&node);
  return 0;
}
