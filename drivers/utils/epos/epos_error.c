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

#include <epos.h>
#include <error.h>

int main(int argc, char **argv) {
  int i;
  epos_node_t node;

  if (epos_init_arg(&node, argc, argv))
    return -1;
  int num_errors = epos_error_get_history_length(&node.dev);
  fprintf(stdout, "EPOS error history has length %d\n", num_errors);

  if (num_errors) {
    epos_error_device_t history[num_errors];
    epos_error_get_history(&node.dev, history);
    for (i = 0; i < num_errors; ++i)
      fprintf(stdout, "%2d: (0x%04X) %s\n", i, history[i].code,
      history[i].message);
  }
  epos_close(&node);

  return 0;
}
