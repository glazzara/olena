// Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007 EPITA
// Research and Development Laboratory
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

#ifndef MLN_IO_PBM_LOAD_HH
# define MLN_IO_PBM_LOAD_HH

/*!
 * \file   mln/io/pbm/load.hh
 *
 * \brief Define a function which loads an image of kind pbm with
 * given path.
 *
 */


# include <iostream>
# include <fstream>
# include <string>

# include <mln/core/image2d.hh>
# include <mln/io/internal/pnm/load_header.hh>

namespace mln
{

  namespace io
  {

    namespace pbm
    {

      namespace internal
      {

	/// load_ascii.
	template <typename I>
	void load_ascii(std::ifstream& file, I& ima)
	{
	  mln_fwd_piter(I) p(ima.domain());
	  for_all(p)
	    {
	      unsigned value;
	      file >> value;
	      ima(p) = value;
	      // FIXME: Test alt code below.
	      // file >> ima(p);
	    }
	}


	/// load_raw_2d.
	template <typename I>
	void load_raw_2d(std::ifstream& file, I& ima)
	{
	  point2d p = make::point2d(0, ima.domain().pmin().col());
	  typedef mln_value(I) V;
	  const mln_coord(I)
	    min_row = geom::min_row(ima),
	    max_row = geom::max_row(ima),
	    min_col = geom::min_col(ima),
	    max_col = geom::max_col(ima);

	  unsigned char c;
	  int i = 0;
	  for (p.row() = min_row; p.row() <= max_row; ++p.row())
	    for (p.col() = min_col; p.col() <= max_col; ++p.col())
	    {
	      if (i && (i % 8 == 0))
		file.read((char*)(&c), 1);
	      ima(p) = c & 128;
	      c = c * 2;
	      ++i;
	    }
	}


    } // end of namespace mln::io::internal


    image2d<bool> load(const std::string& filename)
    {
      std::ifstream file(filename.c_str());
      if (! file)
      {
	std::cerr << "error: file '" << filename
		  << "' not found!";
	abort();
      }
      char type;
      int nrows, ncols;
      io::internal::pnm::read_header('1', '4', file, type, nrows, ncols);

      image2d<bool> ima(nrows, ncols);
      if (type == '4')
	internal::load_raw_2d(file, ima);
      else
	if (type == '1')
	  internal::load_ascii(file, ima);
      return ima;
    }


  } // end of namespace mln::io::pbm

} // end of namespace mln::io

} // end of namespace mln


#endif // ! MLN_IO_PBM_LOAD_HH
