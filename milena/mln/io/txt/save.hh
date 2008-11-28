// Copyright (C) 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_IO_TXT_SAVE_HH
# define MLN_IO_TXT_SAVE_HH

/// \file   mln/io/txt/save.hh
///
/// \brief Save an image as txt file.

# include <iostream>
# include <fstream>

# include <mln/core/image/image2d.hh>

# include <mln/metal/is.hh>


namespace mln
{

  namespace io
  {

    namespace txt
    {

      /// Save an image as txt file.
      ///
      /// \param[in] ima_ The image to save. Must be an image of char.
      /// \param[in] filename the destination.
      void
      save(const image2d<char>& ima, const std::string& filename);


# ifndef MLN_INCLUDE_ONLY

      inline
      void
      save(const image2d<char>& ima, const std::string& filename)
      {
	trace::entering("mln::io::txt::save");

	mln_precondition(ima.has_data());
	std::ofstream ostr(filename.c_str());
	unsigned col = 0;
	typedef image2d<char> I;
	mln_piter_(I) p(ima.domain());
	for_all(p)
	{
	  ostr << ima(p);
	  if (++col == ima.ncols())
	  {
	    ostr << std::endl;
	    col = 0;
	  }
	}
	ostr.close();

	trace::exiting("mln::io::txt::save");
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::io::txt

  } // end of namespace mln::io

} // end of namespace mln


#endif // ! MLN_IO_TXT_SAVE_HH
