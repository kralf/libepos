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
#include <math.h>

#include "epos.h"
#include "current.h"

int quit = 0;

void epos_signaled(int signal) {
  quit = 1;
}

int main(int argc, char **argv) {
  if (argc < 2) {
    fprintf(stderr, "usage: %s CURR [PARAMS]\n", argv[0]);
    return -1;
  }
  float target_value = atof(argv[1]);

  epos_node_t node;
  epos_current_t curr;
  epos_init_arg(&node, argc, argv, 0);

  signal(SIGINT, epos_signaled);

  if (epos_open(&node))
    return -1;
  epos_current_init(&curr, target_value);
  if (!epos_current_start(&node, &curr)) {
    while (!quit) {
      fprintf(stdout, "\rEPOS current: %8.4f A",
        epos_get_current(&node));
      fflush(stdout);
    }
    fprintf(stdout, "\n");
    epos_current_stop(&node);
  }
  epos_close(&node);

  epos_destroy(&node);
  return 0;
}
