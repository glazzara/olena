// Copyright (C) 2008, 2009, 2011 EPITA Research and Development
// Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

/// \brief Compute 3D connectivity numbers.

#include "connectivity_numbers_3d.hh"

void
usage(const std::string& program)
{
  std::cerr <<
    "usage: " << program << " <nbhs>" << std::endl <<
    "where <nbhs> is one of these values:\n\n"
    "  `6_26'  :  6-c foreground,  26-c background\n"
    "  `26_6'  : 26-c foreground,  6-c background\n"
    "  `6p_18' : 6+-c foreground, 18-c background\n"
    "  `18_6p' : 18-c foreground, 6+-c background\n" << std::endl;
  std::exit(1);
}

int main(int argc, char* argv[])
{
  if (argc != 2)
    usage(argv[0]);

  std::string nbhs = argv[1];
  std::vector<conn_number_t> conn_numbers;
  if      (nbhs == "6_26")  conn_numbers = connectivity_numbers_3d__6_26();
  else if (nbhs == "26_6")  conn_numbers = connectivity_numbers_3d__26_6();
  else if (nbhs == "6p_18") conn_numbers = connectivity_numbers_3d__6p_18();
  else if (nbhs == "18_6p") conn_numbers = connectivity_numbers_3d__18_6p();
  else
    usage(argv[0]);

  // Display numbers.
  for (size_t i = 0; i < conn_numbers.size(); ++i)
    {
      std::cout << std::setw(2) << conn_numbers[i] << ", ";
      if (! ((i + 1) % 4)) std::cout << " ";
      if (! ((i + 1) % 16)) std::cout << std::endl;
      if (! ((i + 1) % 64)) std::cout <<  std::endl;
    }
}
