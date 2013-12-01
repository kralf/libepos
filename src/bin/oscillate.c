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

#include "epos.h"
#include "position_profile.h"
#include "macros.h"

#define EPOS_OSCILLATE_PARAMETER_POSITION             "POSITION"
#define EPOS_OSCILLATE_PARAMETER_VELOCITY             "VELOCITY"
#define EPOS_OSCILLATE_PARAMETER_ACCELERATION         "ACCELERATION"
#define EPOS_OSCILLATE_PARAMETER_DECELERATION         "DECELERATION"

#define EPOS_PROFILE_PARSER_OPTION_GROUP              "epos-profile"
#define EPOS_PROFILE_PARAMETER_TYPE                   "type"
#define EPOS_PROFILE_PARAMETER_RELATIVE               "relative"

config_param_t epos_oscillate_default_arguments_params[] = {
  {EPOS_OSCILLATE_PARAMETER_POSITION,
    config_param_type_float,
    "",
    "(-inf, inf)",
    "The demanded angular position in [deg], whose sign will be inverted "
    "after each completion of the motion profile"},
  {EPOS_OSCILLATE_PARAMETER_VELOCITY,
    config_param_type_float,
    "",
    "(-inf, inf)",
    "The demanded maximum angular velocity in [deg/s]"},
  {EPOS_OSCILLATE_PARAMETER_ACCELERATION,
    config_param_type_float,
    "",
    "[0.0, inf)",
    "The demanded maximum angular acceleration in [deg/s^2]"},
  {EPOS_OSCILLATE_PARAMETER_DECELERATION,
    config_param_type_float,
    "",
    "[0.0, inf)",
    "The demanded maximum angular deceleration in [deg/s^2]"},
};

const config_default_t epos_oscillate_default_arguments = {
  epos_oscillate_default_arguments_params,
  sizeof(epos_oscillate_default_arguments_params)/
    sizeof(config_param_t),
};

config_param_t epos_profile_default_options_params[] = {
  {EPOS_PROFILE_PARAMETER_TYPE,
    config_param_type_enum,
    "linear",
    "linear|sinusoidal",
    "The type of motion profile, which may represent either 'linear' "
    "or 'sinusoidal' velocity ramps"},
  {EPOS_PROFILE_PARAMETER_RELATIVE,
    config_param_type_bool,
    "false",
    "false|true",
    "The demanded angular position is relative to the current angular "
    "position"},
};

const config_default_t epos_profile_default_options = {
  epos_profile_default_options_params,
  sizeof(epos_profile_default_options_params)/sizeof(config_param_t),
};

int quit = 0;

void epos_signaled(int signal) {
  quit = 1;
}

int main(int argc, char **argv) {
  config_parser_t parser;
  epos_node_t node;
  epos_position_profile_t profile;

  config_parser_init_default(&parser,
    &epos_oscillate_default_arguments, 0,
    "Oscillate EPOS controller in profile position mode",
    "Establish the communication with a connected EPOS device, attempt to "
    "start the controller in profile position mode, and repetitively execute "
    "a motion profile whilst inverting the sign of the demanded position. "
    "The controller will be stopped if SIGINT is received. The communication "
    "interface depends on the momentarily selected alternative of the "
    "underlying CANopen library.");  
  config_parser_add_option_group(&parser, EPOS_PROFILE_PARSER_OPTION_GROUP,
    &epos_profile_default_options, "EPOS profile options",
    "These options control the profile trajectory generator.");
  epos_node_init_config_parse(&node, &parser, 0, argc, argv,
    config_parser_exit_error);

  float target_value = deg_to_rad(config_get_float(&parser.arguments,
    EPOS_OSCILLATE_PARAMETER_POSITION));
  float velocity = deg_to_rad(config_get_float(&parser.arguments,
    EPOS_OSCILLATE_PARAMETER_ACCELERATION));
  float acceleration = deg_to_rad(config_get_float(&parser.arguments,
    EPOS_OSCILLATE_PARAMETER_ACCELERATION));
  float deceleration = deg_to_rad(config_get_float(&parser.arguments,
    EPOS_OSCILLATE_PARAMETER_DECELERATION));
  
  config_parser_option_group_t* epos_profile_option_group =
    config_parser_get_option_group(&parser, EPOS_PROFILE_PARSER_OPTION_GROUP);
  epos_profile_type_t profile_type = config_get_enum(
    &epos_profile_option_group->options, EPOS_PROFILE_PARAMETER_TYPE);
  config_param_bool_t profile_relative = config_get_bool(
    &epos_profile_option_group->options, EPOS_PROFILE_PARAMETER_RELATIVE);

  signal(SIGINT, epos_signaled);

  epos_node_connect(&node);
  error_exit(&node.error);
  
  epos_position_profile_init(&profile, target_value, velocity, acceleration,
    deceleration, profile_type, profile_relative);
  
  while (!quit) {
    epos_position_profile_start(&node, &profile);
    error_exit(&node.dev.error);
  
    while (!quit) {
      float actual_value = epos_node_get_position(&node);
      error_exit(&node.error);
      velocity = epos_node_get_velocity(&node);
      error_exit(&node.error);
      
      fprintf(stdout, "\rAngular position: %8.2f deg\n",
        rad_to_deg(actual_value));
      fprintf(stdout, "\rAngular velocity: %8.2f deg/s",
        rad_to_deg(velocity));
      fprintf(stdout, "%c[1A\r", 0x1B);
      
      if (epos_profile_wait(&node, 0.1) != EPOS_DEVICE_ERROR_WAIT_TIMEOUT) {
        error_exit(&node.dev.error);
        break;
      }
    }
    
    profile.target_value = -profile.target_value;
  }
  fprintf(stdout, "%c[1B\n", 0x1B);
  
  epos_position_profile_stop(&node);
  error_exit(&node.dev.error);

  epos_node_disconnect(&node);
  error_exit(&node.error);

  epos_node_destroy(&node);
  config_parser_destroy(&parser);
  
  return 0;
}
