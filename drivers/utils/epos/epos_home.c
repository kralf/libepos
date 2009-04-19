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
#include <home.h>

int quit = 0;

void epos_signaled(int signal) {
  quit = 1;
}

int main(int argc, char **argv) {
  epos_node_t node;
  epos_home_t home;
  signal(SIGINT, epos_signaled);

  if (argc < 6) {
    fprintf(stderr, "usage: %s METHOD CURR VEL ACC POS [PARAMS]\n", argv[0]);
    return -1;
  }
  epos_home_method_t method = atoi(argv[1]);
  float curr = atof(argv[2]);
  float vel = atof(argv[3])*M_PI/180.0;
  float acc = atof(argv[4])*M_PI/180.0;
  float pos = atof(argv[5])*M_PI/180.0;

  if (epos_init_arg(&node, argc, argv))
    return -1;
  epos_home_init(&home, method, curr, vel, acc, pos);
  if (!epos_home_start(&node, &home)) {
    while (!quit && epos_home_wait(&node, 0.1));
    epos_home_stop(&node);
  }
  epos_close(&node);

  return 0;
}
