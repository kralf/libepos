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

#include "position_profile.h"

#define EPOS_POSITION_PROFILE_EVAL_PARAMETER_FILE         "FILE"
#define EPOS_POSITION_PROFILE_EVAL_PARAMETER_STEP_SIZE    "STEP_SIZE"

#define EPOS_PROFILE_PARSER_OPTION_GROUP                  "epos-profile"
#define EPOS_PROFILE_PARAMETER_TYPE                       "type"
#define EPOS_PROFILE_PARAMETER_OUTPUT                     "output"

config_param_t epos_position_profile_eval_default_arguments_params[] = {
  {EPOS_POSITION_PROFILE_EVAL_PARAMETER_FILE,
    config_param_type_string,
    "",
    "",
    "Read position profiles from the specified input file or '-' for stdin"},
  {EPOS_POSITION_PROFILE_EVAL_PARAMETER_STEP_SIZE,
    config_param_type_float,
    "",
    "(0.0, inf)",
    "The step size used to generate equidistant locations of the profile "
    "functions"},
};

const config_default_t epos_position_profile_eval_default_arguments = {
  epos_position_profile_eval_default_arguments_params,
  sizeof(epos_position_profile_eval_default_arguments_params)/
    sizeof(config_param_t),
};

config_param_t epos_profile_default_options_params[] = {
  {EPOS_PROFILE_PARAMETER_TYPE,
    config_param_type_enum,
    "linear",
    "linear|sinusoidal",
    "The type of motion profile, which may represent either 'linear' "
    "or 'sinusoidal' velocity ramps"},
  {EPOS_PROFILE_PARAMETER_OUTPUT,
    config_param_type_string,
    "-",
    "",
    "Write profile function values to the specified output file or '-' "
    "for stdout"},
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
  file_t input_file, output_file;

  config_parser_init_default(&parser,
    &epos_position_profile_eval_default_arguments, 0,
    "Evaluate EPOS position profiles at equidistant locations",
    "The command evaluates a sequence of EPOS position profiles at "
    "equidistant locations and prints the corresponding profile function "
    "values to a file or stdout. No communication with an EPOS node is "
    "required to perform the evaluations.");
  config_parser_add_option_group(&parser, EPOS_PROFILE_PARSER_OPTION_GROUP,
    &epos_profile_default_options, "EPOS profile options",
    "These options control the profile trajectory generator.");
  config_parser_parse(&parser, argc, argv, config_parser_exit_error);

  const char* file = config_get_string(&parser.arguments,
    EPOS_POSITION_PROFILE_EVAL_PARAMETER_FILE);
  double step_size = config_get_float(&parser.arguments,
    EPOS_POSITION_PROFILE_EVAL_PARAMETER_STEP_SIZE);
  
  config_parser_option_group_t* epos_profile_option_group =
    config_parser_get_option_group(&parser, EPOS_PROFILE_PARSER_OPTION_GROUP);
  epos_profile_type_t profile_type = config_get_enum(
    &epos_profile_option_group->options, EPOS_PROFILE_PARAMETER_TYPE);
  const char* output = config_get_string(
    &epos_profile_option_group->options, EPOS_PROFILE_PARAMETER_OUTPUT);

  file_init_name(&input_file, file);
  if (string_equal(file, "-"))
    file_open_stream(&input_file, stdin, file_mode_read);
  else
    file_open(&input_file, file_mode_read);
  error_exit(&input_file.error);

  char* line = 0;
  epos_position_profile_t* profiles = 0;
  size_t num_profiles = 0;
  
  while (!file_eof(&input_file) &&
      (file_read_line(&input_file, &line, 128) >= 0)) {
    if (string_empty(line) || string_starts_with(line, "#"))
      continue;
    
    double target_value, velocity, acceleration, deceleration;
    if (string_scanf(line, "%lg %lg %lg %lg\n", &target_value, &velocity,
          &acceleration, &deceleration) == 4) {
      if (!(num_profiles % 64))
        profiles = realloc(profiles, (num_profiles+64)*
          sizeof(epos_position_profile_t));
      epos_position_profile_init(&profiles[num_profiles], target_value,
        velocity, acceleration, deceleration, profile_type, 0);
      
      ++num_profiles;
    }
  }
  string_destroy(&line);
  error_exit(&input_file.error);
  file_destroy(&input_file);
  
  file_init_name(&output_file, output);
  if (string_equal(output, "-"))
    file_open_stream(&output_file, stdout, file_mode_write);
  else
    file_open(&output_file, file_mode_write);
  error_exit(&output_file.error);
  
  size_t i = 0, j = 0;
  double t = 0.0;
  epos_profile_value_t values = {0.0, 0.0, 0.0};
  
  for (i = 0; i < num_profiles; ++i) {
    profiles[i].start_value = values.position;
    profiles[i].start_time = t;
    
    while (values.position != profiles[i].target_value) {
      values = epos_position_profile_eval(&profiles[i], t);
      file_printf(&output_file, "%10lg %10d %10g %10g, %10g\n",
        t, i, values.position, values.velocity, values.acceleration);
      error_exit(&output_file.error);
        
      ++j;
      t = step_size*j;
    };
  }
  
  file_destroy(&output_file);
  if (profiles)
    free(profiles);
  
  return 0;
}
