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

#define EPOS_INTERPOLATED_POSITION_EVAL_PARAMETER_FILE        "FILE"
#define EPOS_INTERPOLATED_POSITION_EVAL_PARAMETER_STEP_SIZE   "STEP_SIZE"

#define EPOS_PROFILE_PARSER_OPTION_GROUP                      "epos-profile"
#define EPOS_PROFILE_PARAMETER_OUTPUT                         "output"

config_param_t epos_interpolated_position_eval_default_arguments_params[] = {
  {EPOS_INTERPOLATED_POSITION_EVAL_PARAMETER_FILE,
    config_param_type_string,
    "",
    "",
    "Read interpolated position profile from the specified input file "
    "or '-' for stdin"},
  {EPOS_INTERPOLATED_POSITION_EVAL_PARAMETER_STEP_SIZE,
    config_param_type_float,
    "",
    "(0.0, inf)",
    "The step size used to generate equidistant locations of the profile "
    "function"},
};

const config_default_t epos_interpolated_position_eval_default_arguments = {
  epos_interpolated_position_eval_default_arguments_params,
  sizeof(epos_interpolated_position_eval_default_arguments_params)/
    sizeof(config_param_t),
};

config_param_t epos_profile_default_options_params[] = {
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
    &epos_interpolated_position_eval_default_arguments, 0,
    "Evaluate EPOS interpolated position profile at equidistant locations",
    "The command evaluates an EPOS interpolated position profile at "
    "equidistant locations and prints the corresponding profile function "
    "values to a file or stdout. No communication with an EPOS node is "
    "required to perform the evaluations.");
  config_parser_add_option_group(&parser, EPOS_PROFILE_PARSER_OPTION_GROUP,
    &epos_profile_default_options, "EPOS profile options",
    "These options control the profile trajectory generator.");
  config_parser_parse(&parser, argc, argv, config_parser_exit_error);

  const char* file = config_get_string(&parser.arguments,
    EPOS_INTERPOLATED_POSITION_EVAL_PARAMETER_FILE);
  double step_size = config_get_float(&parser.arguments,
    EPOS_INTERPOLATED_POSITION_EVAL_PARAMETER_STEP_SIZE);
  
  config_parser_option_group_t* epos_profile_option_group =
    config_parser_get_option_group(&parser, EPOS_PROFILE_PARSER_OPTION_GROUP);
  const char* output = config_get_string(
    &epos_profile_option_group->options, EPOS_PROFILE_PARAMETER_OUTPUT);

  file_init_name(&input_file, file);
  if (string_equal(file, "-"))
    file_open_stream(&input_file, stdin, file_mode_read);
  else
    file_open(&input_file, file_mode_read);
  error_exit(&input_file.error);

  char* line = 0;
  epos_interpolated_position_knot_t* knots = 0;
  size_t num_knots = 0;
  
  while (!file_eof(&input_file) &&
      (file_read_line(&input_file, &line, 128) >= 0)) {
    if (string_empty(line) || string_starts_with(line, "#"))
      continue;
    
    double time;
    float position, velocity;
    if (string_scanf(line, "%lg %g %g\n", &time, &position, &velocity) == 3) {
      if (!(num_knots % 64))
        knots = realloc(knots, (num_knots+64)*
          sizeof(epos_interpolated_position_knot_t));
      knots[num_knots].time = time;
      knots[num_knots].position = position;
      knots[num_knots].velocity = velocity;
        
      ++num_knots;
    }
  }
  string_destroy(&line);
  error_exit(&input_file.error);
  file_destroy(&input_file);
  
  epos_interpolated_position_t profile;
  epos_interpolated_position_init(&profile,
    (num_knots > 1) ? &knots[1] : 0,
    (num_knots > 1) ? num_knots-1 : 0);
  if (num_knots) {
    profile.start_knot.time = knots[0].time;
    profile.start_knot.position = knots[0].position;
    profile.start_knot.velocity = knots[0].velocity;
  }
  if (knots)
    free(knots);
  
  file_init_name(&output_file, output);
  if (string_equal(output, "-"))
    file_open_stream(&output_file, stdout, file_mode_write);
  else
    file_open(&output_file, file_mode_write);
  error_exit(&output_file.error);

  if (profile.num_knots) {
    size_t i = 0, j = 0;
    double t = profile.start_knot.time;

    while (t <= profile.knots[profile.num_knots-1].time) {
      epos_profile_value_t values = epos_interpolated_position_eval_linear(
        &profile, t, &i);
      file_printf(&output_file, "%10lg %10d %10g %10g, %10g\n",
        t, i, values.position, values.velocity, values.acceleration);
      error_exit(&output_file.error);
        
      ++j;
      t = step_size*j;
    }
  }
  
  file_destroy(&output_file);
  epos_interpolated_position_destroy(&profile);
  
  return 0;
}
