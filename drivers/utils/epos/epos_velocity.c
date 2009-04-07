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

#include <epos.h>
#include <velocity.h>

int quit = 0;

void epos_signaled(int signal) {
  quit = 1;
}

int main(int argc, char **argv) {
  epos_node_t node;
  epos_velocity_t vel;
  signal(SIGINT, epos_signaled);

  if (argc < 2) {
    fprintf(stderr, "usage: %s VEL [PARAMS]\n", argv[0]);
    return -1;
  }
  float target_value = atof(argv[1]);

  if (epos_init_arg(&node, argc, argv))
    return -1;
  epos_velocity_init(&vel, target_value*M_PI/180.0);
  if (!epos_velocity_start(&node, &vel)) {
    while (!quit) {
      fprintf(stdout, "\rEPOS angular velocity: %8.2f deg/s",
        epos_get_velocity(&node)*180.0/M_PI);
      fflush(stdout);
    }
    fprintf(stdout, "\n");
    epos_velocity_stop(&node);
  }
  epos_close(&node);

  return 0;
}
