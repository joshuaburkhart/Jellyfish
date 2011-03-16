/*  This file is part of Jellyfish.

    Jellyfish is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Jellyfish is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Jellyfish.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <config.h>
#include <iostream>
#include <string>
#include <string.h>
#include <jellyfish/misc.hpp>

const char *argp_program_version = PACKAGE_STRING;
const char *argp_program_bug_address = 
  "<gus@umd.edu> <carlk@umiacs.umd.edu>";

typedef int (main_func_t)(int argc, char *argv[]);

main_func_t count_main;
main_func_t stats_main;
main_func_t merge_main;
main_func_t histo_main;
main_func_t query_main;
main_func_t cite_main;
main_func_t sos;
main_func_t version;

struct cmd_func {
  std::string  cmd;
  main_func_t *func;
};
cmd_func cmd_list[] = {
  {"count",             &count_main},
  {"stats",             &stats_main},
  {"merge",             &merge_main},
  {"histo",             &histo_main},
  {"query",             &query_main},
  {"cite",              &cite_main},

  /* help in all its form. Must be first non-command */
  {"help",              &sos},
  {"-h",                &sos},
  {"-help",             &sos},
  {"--help",            &sos},
  {"-?",                &sos},
  {"--version",         &version},
  {"-V",                &version},
  {"",                  0}
};

void __sos(std::ostream *os)
{
  *os << "Usage: jellyfish <cmd> [options] arg..."  << std::endl <<
    "Where <cmd> is one of: ";
  bool comma = false;
  for(cmd_func *ccmd = cmd_list; ccmd->func != sos; ccmd++) {
    *os << (comma ? ", " : "") << ccmd->cmd;
    comma = true;
  }
  *os << "." << std::endl;
  *os << "Options:" << std::endl <<
    "  --version        Display version" << std::endl <<
    "  --help           Display this message" << std::endl;
}

int sos(int argc, char *argv[])
{
  __sos(&std::cout);
  return 0;
}

int version(int argc, char *argv[])
{
  std::cout << argp_program_version << std::endl;
  return 0;
}

int main(int argc, char *argv[])
{
  std::string error;

  if(argc < 2) {
    error = "Too few arguments";
  } else {
    for(cmd_func *ccmd = cmd_list; ccmd->func != 0; ccmd++) {
      if(!ccmd->cmd.compare(argv[1])) {
        std::string name(argv[0]);
        name += " ";
        name += argv[1];
        argv[1] = strdup(name.c_str());
        return ccmd->func(argc - 1, argv + 1);
      }
    }
    error = "Unknown command '";
    error += argv[1];
    error += "'\n";
  }

  std::cerr << error << std::endl;
  __sos(&std::cerr);
  return 1;
}
