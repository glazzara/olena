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

#ifndef MLN_IO_PNM_SAVE_HH
# define MLN_IO_PNM_SAVE_HH

# include <iostream>
# include <fstream>

# include <mln/core/concept/image.hh>

# include <mln/value/rgb.hh>
# include <mln/value/int_u8.hh>

# include <mln/io/internal/pnm/save_header.hh>
# include <mln/io/internal/pnm/macros.hh>

# include <mln/metal/instance_of.hh>

# include <mln/geom/size2d.hh>

/*! \file mln/level/transform.hh
 *
 * \brief save pgm,
 *
 */

namespace mln
{

  namespace io
  {

    namespace internal
    {


# ifndef MLN_INCLUDE_ONLY

      namespace pnm
      {

	// for 3dimensionals values (rgb)
	template <unsigned int n>
	void save_data_uncontiguous(std::ofstream& file,
			       const image2d_b< value::rgb<n> >& ima)
	{
	  typedef typename value::rgb<n>::enc V;
	  typedef typename value::rgb<n>::enc::enc E;

	  const int
	    min_row = geom::min_row(ima),
	    max_row = geom::max_row(ima),
	    min_col = geom::min_col(ima),
	    max_col = geom::max_col(ima);

	  point2d p;

	  for (p.row() = min_row; p.row() <= max_row; ++p.row())
	    for (p.col() = min_col; p.col() <= max_col; ++p.col())
	    {
	      const V* buf = ima(p).buffer();

	      E v = buf[0].to_enc();
	      file.write((char*)&v, sizeof(E));
	      v = ima(p).green().to_enc();
	      file.write((char*)&v, sizeof(E));
	      v = ima(p).blue().to_enc();
 	      file.write((char*)&v, sizeof(E));
	    }

	}

	// for scalar value
	template <typename V>
	void save_data_uncontiguous(std::ofstream& file,
			       const image2d_b<V>& ima)
	{
	  typedef typename V::enc E;

	  const int
	    min_row = geom::min_row(ima),
	    max_row = geom::max_row(ima),
	    min_col = geom::min_col(ima),
	    max_col = geom::max_col(ima);

	  point2d p;
	  for (p.row() = min_row; p.row() <= max_row; ++p.row())
	    for (p.col() = min_col; p.col() <= max_col; ++p.col())
	    {
	      E c = ima(p).to_enc();
	      file.write((char*)(&c), sizeof(E));
	    }

	}

	template <typename I>
	void save_data(std::ofstream& file,
		   const I& ima)
	{
	  const int
	    min_row = geom::min_row(ima),
	    max_row = geom::max_row(ima);
	  point2d p;
	  p.col() = geom::min_col(ima);
	  size_t len = geom::ncols(ima) * sizeof(mln_value(I));
	  for (p.row() = min_row; p.row() <= max_row; ++p.row())
	    file.write((char*)(& ima(p)), len);
	}

	template <typename I>
	void save(const int type, const Fast_Image<I>& ima_, const std::string& filename)
	{
	  const I& ima = exact(ima_);
	  std::ofstream file(filename.c_str());
	  io::internal::pnm::save_header(type, mln_max(mln_value(I)::enc),
					 ima, filename, file);

	  if (sizeof(value::int_u8) == 1)
	    save_data(file, ima);
 	  else
	    save_data_uncontiguous(file, ima);
	}

      } // end of namespace mln::io::internal::pnm


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::internal

  } // end of namespace mln::io

} // end of namespace mln


#endif // ! MLN_IO_PNM_SAVE_HH
