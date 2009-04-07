// Copyright (C) 2009 EPITA Research and Development Laboratory
// (LRDE)
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef MLN_IO_ICDAR_LOAD_HH
# define MLN_IO_ICDAR_LOAD_HH

/// \file mln/io/icdar/load.hh
///
/// Load an image saved as a raw buffer of integers.
/// Output file format for ICDAR 2009 contest.
///
/// Reference: http://www.iit.demokritos.gr/~bgat/HandSegmCont2009/Protocol.html

#include <cstdio>
#include <mln/core/concept/image.hh>


namespace mln
{

  namespace io
  {

    namespace icdar
    {

      template <typename I>
      void load(Image<I>& ima_, const std::string& filename,
		int nrows, int ncols);


# ifndef MLN_INCLUDE_ONLY

      template <typename I>
      void load(Image<I>& ima_, const std::string& filename,
		int nrows, int ncols)
      {
	trace::entering("io::icdar::load");

	I& ima = exact(ima_);
	ima = I(nrows, ncols, 0); // Make sure there is no border.

	int size = nrows * ncols;
	unsigned int *buffer; //Pointer to store raw data
	FILE *f;

	buffer = (unsigned int *) calloc (size, sizeof (int));
	f = fopen(filename.c_str(), "rb");
	fread(buffer, size, sizeof (int),f);

	for (int i = 0; i < size; ++i)
	{
	  mln_value(I) v;// = buffer[i];
	  convert::from_to(buffer[i], v);
	  ima(ima.point_at_index(i)) = v;
	}

	fclose(f);

	trace::exiting("io::icdar::load");
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::io::icdar

  } // end of namespace mln::io

} // end of namespace mln

#endif // ! MLN_IO_ICDAR_LOAD_HH
