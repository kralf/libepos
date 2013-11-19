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
#include <timer/timer.h>

#include "epos.h"
#include "position_profile.h"

int quit = 0;

void epos_signaled(int signal) {
  quit = 1;
}

int main(int argc, char **argv) {
  config_parser_t parser;
  epos_node_t node;
  epos_position_profile_t profile;

  config_parser_init_default(&parser,
    "Start EPOS controller in profile position mode and evaluate the profile",
    "Establish the communication with a connected EPOS device, attempt to "
    "start the controller in profile position mode, and concurrently evaluate "
    "trajectory points from the profile parameters and the time elapsed. "
    "The controller will be stopped if SIGINT is received or the motion "
    "profile is completed. The communication interface depends on the "
    "momentarily selected alternative of the underlying CANopen library.");  
  config_param_p position_param = config_set_param_value_range(
    &parser.arguments,
    "POSITION",
    config_param_type_float,
    "",
    "(-inf, inf)",
    "The demanded angular position in [deg]");
  config_param_p velocity_param = config_set_param_value_range(
    &parser.arguments,
    "VELOCITY",
    config_param_type_float,
    "",
    "(-inf, inf)",
    "The demanded maximum angular velocity in [deg/s]");
  config_param_p acceleration_param = config_set_param_value_range(
    &parser.arguments,
    "ACCELERATION",
    config_param_type_float,
    "",
    "[0.0, inf)",
    "The demanded maximum angular acceleration in [deg/s^2]");
  config_param_p deceleration_param = config_set_param_value_range(
    &parser.arguments,
    "DECELERATION",
    config_param_type_float,
    "",
    "[0.0, inf)",
    "The demanded maximum angular deceleration in [deg/s^2]");
  config_parser_option_group_p profile_option_group =
    config_parser_add_option_group(&parser, "profile", 0, "Profile options",
    "These options control the profile trajectory generator.");
  config_param_p relative_param = config_set_param_value_range(
    &profile_option_group->options,
    "relative",
    config_param_type_bool,
    "false",
    "false|true",
    "The demanded angular position is relative to the current angular "
    "position");
  config_param_p type_param = config_set_param_value_range(
    &profile_option_group->options,
    "type",
    config_param_type_enum,
    "linear",
    "linear|sinusoidal",
    "The type of motion profile, which may represent either 'linear' "
    "or 'sinusoidal' velocity ramps");
  epos_init_config_parse(&node, &parser, 0, argc, argv,
    config_parser_exit_error);

  signal(SIGINT, epos_signaled);

  if (epos_open(&node))
    return -1;
  
  float pos = deg_to_rad(config_param_get_float(position_param));
  float vel = deg_to_rad(config_param_get_float(velocity_param));
  float acc = deg_to_rad(config_param_get_float(acceleration_param));
  float dec = deg_to_rad(config_param_get_float(deceleration_param));
  config_param_bool_t rel = config_param_get_bool(relative_param);
  epos_profile_type_t type = config_param_get_enum(type_param);  
  epos_position_profile_init(&profile, pos, vel, acc, dec, type);  
  profile.relative = rel;
  
  if (!epos_position_profile_start(&node, &profile)) {
    while (!quit) {
      double time;
      
      timer_start(&time);
      float pos_a = rad_to_deg(epos_get_position(&node));
      timer_correct(&time);
      float pos_e = rad_to_deg(epos_position_profile_eval(&profile, time));
      
      fprintf(stdout, "\rAngular position (act): %8.2f deg\n", pos_a);
      fprintf(stdout, "\rAngular position (est): %8.2f deg", pos_e);
      fprintf(stdout, "%c[1A\r", 0x1B);
      
      if (!epos_profile_wait(&node, 0.1))
        break;
    }
    fprintf(stdout, "%c[1B\n", 0x1B);
    epos_position_profile_stop(&node);
  }
  epos_close(&node);

  epos_destroy(&node);
  return 0;
}
