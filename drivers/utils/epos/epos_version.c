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

#include <epos.h>

int main(int argc, char **argv) {
  if ((argc < 2) || (argc > 3)) {
    fprintf(stderr, "Usage: %s DEV [ID]\n", argv[0]);
    return -1;
  }

  int id = 1;
  if (argc == 3)
    id = atoi(argv[2]);

  can_init(argv[1]);
  epos_get_hardware_version(id);
  epos_get_software_version(id);

  printf("hardware version: 0x%04X\n", epos_read.node[id-1].hw_version);
  printf("software version: 0x%04X\n", epos_read.node[id-1].sw_version);
  can_close();

  return 0;
}
