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

#ifndef MLN_IO_ICDAR_SAVE_HH
# define MLN_IO_ICDAR_SAVE_HH

/// \file mln/io/icdar/save.hh
///
/// Save an image as a raw buffer of integers.
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
      void save(const Image<I>& ima_, const std::string& filename);


# ifndef MLN_INCLUDE_ONLY

      template <typename I>
      void save(const Image<I>& ima_, const std::string& filename)
      {
	trace::entering("io::icdar::save");

	const I& ima = exact(ima_);
	mln_precondition(ima.is_valid());

	FILE *f = fopen(filename.c_str(), "wb");

	mln_piter(I) p(ima.domain());
	for_all(p)
	{
	  /// Probably too violent...
	  unsigned int value = static_cast<unsigned int>(ima(p));
	  int ret = fwrite(&value, sizeof (int), 1, f);
	  (void)(ret);
	}

	fclose(f);

	trace::exiting("io::icdar::save");
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::io::icdar

  } // end of namespace mln::io

} // end of namespace mln

#endif // ! MLN_IO_ICDAR_SAVE_HH
