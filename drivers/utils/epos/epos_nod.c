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
#include <stdlib.h>
#include <signal.h>

#include <epos.h>

#define STATUS_TARGET_REACHED 0x0408

int quit = 0;

void epos_signaled(int signal) {
  quit = 1;
}

int main(int argc, char **argv) {
  if ((argc < 4) || (argc > 5)) {
    fprintf(stderr, "Usage: %s DEV POS VEL [ID]\n", argv[0]);
    return -1;
  }

  int id = 1;
  if (argc == 5)
    id = atoi(argv[4]);
  int pos = atoi(argv[2]);
  int vel = atoi(argv[3]);

  can_init(argv[1]);
  epos_fault_reset(id);

  epos_shutdown(id);
  epos_enable_operation(id);
  epos_set_mode_of_operation(id, EPOS_OPERATION_MODE_PROFILE_POSITION);
  epos_set_profile_velocity(id, vel);
  epos_set_target_position(id, pos);
  epos_activate_position(id);

  signal(SIGINT, epos_signaled);
  while (!quit) {
    do {
      epos_get_statusword(id);
    }
    while (!(epos_read.node[id-1].status & STATUS_TARGET_REACHED));
  }

  epos_shutdown(id);
  can_close();

  return 0;
}
