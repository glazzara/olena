// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef SCRIBO_DEBUG_USAGE_HH
# define SCRIBO_DEBUG_USAGE_HH

#include <iostream>

namespace scribo
{

  namespace debug
  {

    /// Format a standard usage output.
    ///
    /// \param[in] argv Arguments passed to the program.
    /// \param[in] desc Description of the program.
    /// \param[in] args The expected arguments.
    /// \param[in] args_desc The description of the expected arguments.
    /// \param[in] out_desc The description of the output.
    ///
    /// \return Return 1.
    //
    int usage(char* argv[], const char *desc, const char* args,
	      const char*args_desc[][2], const char *out_desc);


# ifndef MLN_INCLUDE_ONLY

    inline
    int
    usage(char* argv[], const char *desc, const char* args,
	  const char*args_desc[][2], const char *out_desc)
    {
      std::cout << desc << std::endl;
      std::cout << "-----------" << std::endl;
      std::cout << "Usage: " << argv[0] << " " << args << std::endl;

      for (unsigned i = 0; args_desc[i][0] != 0; ++i)
	std::cout << "  " << args_desc[i][0] << ": " << args_desc[i][1]
	  << std::endl;

      std::cout << std::endl << "Output: " << out_desc << std::endl;
      std::cout << "-----------" << std::endl;
      std::cout << "EPITA/LRDE - Scribo 2009" << std::endl;
      return 1;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::debug

} // end of namespace scribo

#endif // ! SCRIBO_DEBUG_USAGE_HH
