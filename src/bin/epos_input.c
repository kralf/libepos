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
  epos_node_t node;
  epos_init_arg(&node, argc, argv, 0);

  if (epos_open(&node))
    return -1;
  int i;
  printf("%5s  %5s  %5s  %5s  %5s  %5s\n", "type", "chan", "pol", "exec", 
    "mask", "state");
  for (i = 0; i < sizeof(node.input.funcs)/sizeof(epos_input_func_t); ++i)
    printf("%5d  %5d  %5d  %5d  %5d  %5d\n", i, node.input.funcs[i].channel,
    node.input.funcs[i].polarity, node.input.funcs[i].execute,
    node.input.funcs[i].enabled, epos_input_get_func_state(&node.input, i));
  epos_close(&node);

  epos_destroy(&node);
  return 0;
}
