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
#include "string/string.h"
#include "file/file.h"

#include "interpolated_position.h"

#define EPOS_SPLINE_TO_INT_POSITION_EVAL_PARAMETER_FILE       "FILE"

#define EPOS_PROFILE_PARSER_OPTION_GROUP                      "epos-profile"
#define EPOS_PROFILE_PARAMETER_OUTPUT                         "output"

config_param_t epos_spline_to_int_position_default_arguments_params[] = {
  {EPOS_SPLINE_TO_INT_POSITION_EVAL_PARAMETER_FILE,
    config_param_type_string,
    "",
    "",
    "Read spline from the specified input file or '-' for stdin"},
};

const config_default_t epos_spline_to_int_position_default_arguments = {
  epos_spline_to_int_position_default_arguments_params,
  sizeof(epos_spline_to_int_position_default_arguments_params)/
    sizeof(config_param_t),
};

config_param_t epos_profile_default_options_params[] = {
  {EPOS_PROFILE_PARAMETER_OUTPUT,
    config_param_type_string,
    "-",
    "",
    "Write interpolated position profile to the specified output file "
    "or '-' for stdout"},
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
  spline_t spline;
  file_t output_file;

  config_parser_init_default(&parser,
    &epos_spline_to_int_position_default_arguments, 0,
    "Convert cubic spline to EPOS interpolated position profile",
    "The command converts a cubic spline provided from a file or stdin to "
    "an EPOS interpolated position profile and prints the corresponding "
    "profile to a file or stdout. No communication with an EPOS node is "
    "required to perform the conversion.");
  config_parser_add_option_group(&parser, EPOS_PROFILE_PARSER_OPTION_GROUP,
    &epos_profile_default_options, "EPOS profile options",
    "These options control the profile trajectory generator.");
  config_parser_parse(&parser, argc, argv, config_parser_exit_error);

  const char* file = config_get_string(&parser.arguments,
    EPOS_SPLINE_TO_INT_POSITION_EVAL_PARAMETER_FILE);
  
  config_parser_option_group_t* epos_profile_option_group =
    config_parser_get_option_group(&parser, EPOS_PROFILE_PARSER_OPTION_GROUP);
  const char* output = config_get_string(
    &epos_profile_option_group->options, EPOS_PROFILE_PARAMETER_OUTPUT);

  spline_init(&spline);
  
  spline_read(file, &spline);
  error_exit(&spline.error);
  
  epos_interpolated_position_t profile;
  epos_interpolated_position_init_spline(&profile, &spline);
  
  file_init_name(&output_file, output);
  if (string_equal(output, "-"))
    file_open_stream(&output_file, stdout, file_mode_write);
  else
    file_open(&output_file, file_mode_write);
  error_exit(&output_file.error);

  if (profile.num_knots) {
    size_t i;
    
    for (i = 0; i <= profile.num_knots; ++i) {
      const epos_interpolated_position_knot_t* knot = i ?
        &profile.knots[i-1] : &profile.start_knot;
      
      file_printf(&output_file, "%10lg %10g %10g\n", knot->time,
        knot->position, knot->velocity);
      error_exit(&output_file.error);
    }
  }
  
  file_destroy(&output_file);
  spline_destroy(&spline);
  epos_interpolated_position_destroy(&profile);
  
  return 0;
}
