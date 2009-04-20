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
  if (argc < 2) {
    fprintf(stderr, "Usage: %s BITRATE [PARAMETERS]\n", argv[0]);
    return -1;
  }

  epos_node_t node;

  if (epos_init_arg(&node, argc, argv))
    return -1;
  printf("current EPOS bitrate: %d kbit/s\n", node.dev.can_bitrate);
  if (argc > 2) {
    int error = epos_device_set_can_bitrate(&node.dev, atoi(argv[1]));
    if (error) {
      printf("error setting new bitrate: %s\n", epos_device_errors[error]);
      return -1;
    }
    error = epos_device_store_parameters(&node.dev);
    if (error) {
      printf("error storing parameters: %s\n", epos_device_errors[error]);
      return -1;
    }

    printf("new EPOS bitrate: %d kbit/s\n", atoi(argv[1]));
  }
  epos_close(&node);

  return 0;
}
