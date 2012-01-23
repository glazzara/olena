// Copyright (C) 2009, 2012 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_IO_PLOT_LOAD_HH
# define MLN_IO_PLOT_LOAD_HH

/// \file
///
/// Define a routine which loads in a plot format.

# include <iostream>
# include <fstream>
# include <cstring>
# include <mln/core/image/image1d.hh>
# include <mln/metal/equal.hh>
# include <mln/util/array.hh>
# include <mln/value/int_u8.hh>
# include <mln/value/rgb8.hh>


namespace mln
{

  namespace io
  {

    namespace plot
    {

      /*! \brief Load a Milena array from a plot file.
       *
       * \param[in] arr A reference to the array to load.
       * \param[out] filename The output file.
       *
       * \ingroup ioplot
       */
      template <typename I>
      void load(util::array<I>& arr,
		const std::string& filename);


# ifndef MLN_INCLUDE_ONLY

      template <typename I>
      inline
      void load(util::array<I>& arr, const std::string& filename)
      {
	trace::entering("mln::io::plot::load");

	arr.clear();
	std::ifstream file_in(filename.c_str());

	// int MAX_LENGTH = 100;
	// ISO C++ CAN'T COMPILE WITH VARIABLE LENGTH ARRAY !!
	// THUS MAKE IT CONSTANT
	const int MAX_LENGTH = 100;
	char line[MAX_LENGTH];
	char delims[] = " ";
	char *result = NULL;
	char *tmp_str = NULL;

	while (file_in.getline(line, MAX_LENGTH))
	{
	  if (strlen(line) > 0 && line[0] != '#')
	  {
	    tmp_str = strtok(line, delims);
	    while (tmp_str != NULL)
	    {
	      result = tmp_str;
	      tmp_str = strtok(NULL, delims);
	    }
	    arr.append(atof(result)); // FIXME: type
	  }
	}

	trace::exiting("mln::io::plot::load");
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::io::plot

  } // end of namespace mln::io

} // end of namespace mln


#endif // ! MLN_IO_PLOT_LOAD_HH
