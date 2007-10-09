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

#ifndef MLN_IO_PNM_LOAD_HH
# define MLN_IO_PNM_LOAD_HH

/*! \file mln/io/internal/pnm/load.hh
 *
 * \brief load a PNM 8/16bits image
 */

# include <iostream>
# include <fstream>
# include <string>

# include <mln/core/image2d.hh>

# include <mln/value/int_u8.hh>
# include <mln/value/rgb.hh>

# include <mln/io/internal/pnm/load_header.hh>
# include <mln/io/internal/pnm/macros.hh>

namespace mln
{

  namespace io
  {

    namespace internal
    {

      namespace pnm
      {

# ifndef MLN_INCLUDE_ONLY
	//read a rgb value (sizeof(int_u8) != 1)
	template <unsigned int n>
	void read_value(std::ifstream& file,
			value::rgb<n>& v)
	{
	  typedef typename value::rgb<n>::enc::enc E;

	  E c;
	  file.read((char*)(&c), sizeof(E));
	  v.red() = c;
	  file.read((char*)(&c), sizeof(E));
	  v.green() = c;
	  file.read((char*)(&c), sizeof(E));
	  v.blue() = c;
	}

	//read a scalar value (sizeof(int_u8) != 1)
	template <class V>
	void read_value(std::ifstream& file,
			V& v)
	{
	  typedef typename V::enc E;

	  E c;
	  file.read((char*)(&c), sizeof(E));
	  v = c;
	}

	// used when (sizeof(int_u8) != 1)
	template <typename V>
	void load_raw_2d_uncontiguous(std::ifstream& file,
				      image2d<V>& ima)
	{
	  const int
	    min_row = geom::min_row(ima),
	    max_row = geom::max_row(ima),
	    min_col = geom::min_col(ima),
	    max_col = geom::max_col(ima);

	  point2d p;
	  for (p.row()  = min_row; p.row() <= max_row; ++p.row())
	    for (p.col()  = min_col; p.col() <= max_col; ++p.col())
	      read_value(file, ima(p));
	}

	// used in g++ > 2.95
	template <typename I>
	void load_raw_2d_contiguous(std::ifstream& file, I& ima)
	{
	  point2d p = make::point2d(0, ima.domain().pmin().col());
	  typedef mln_value(I) V;
	  const mln_coord(I)
	    min_row = geom::min_row(ima),
	    max_row = geom::max_row(ima);

	  size_t len = geom::ncols(ima) * sizeof(V);
	  for (p.row() = min_row; p.row() <= max_row; ++p.row())
	    file.read((char*)(&ima(p)), len);
	}

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
	/// for all pnm 8/16 bits formats
	template <typename I>
	void load_raw_2d(std::ifstream& file, I& ima)
	{
 	  if (sizeof(value::int_u8) == 1)
	    load_raw_2d_contiguous(file, ima);
	  else
	    load_raw_2d_uncontiguous(file, ima);
	}

	/// main function : load pnm format
	template <typename V>
	image2d<V> load(char type_, const std::string& filename)
	{
	  std::ifstream file(filename.c_str());
	  if (! file)
	  {
	    std::cerr << "error: file '" << filename
		      << "' not found!";
	    abort();
	  }
	  char type = 0;
	  int nrows, ncols;
	  unsigned int maxval;
	  read_header(type_ - 3, type_, file, type,
		      nrows, ncols, maxval);

	  image2d<V> ima(nrows, ncols);
	  if (type == type_)
	    load_raw_2d(file, ima);
	  else
	    if (type == (type_ - 3))
	      pnm::load_ascii(file, ima);
	  return ima;

	}

	/// a new function to load pnm files :
	/// the destination is an argument to check if
	/// the type match the file to load.
	template <typename I>
	void load(char type_,
		  Image<I>& ima_,
		  const std::string& filename)
	{
	  std::ifstream file(filename.c_str());
	  if (! file)
	  {
	    std::cerr << "error: file '" << filename
		      << "' not found!";
	    abort();
	  }

	  I& ima = exact(ima_);

	  char type = 0;
	  int nrows, ncols;
	  unsigned int maxval;
	  read_header(type_ - 3, type_, file, type,
		      nrows, ncols, maxval);

	  if (value::props< mln_value(I) >::max() != maxval)
	  {
	    std::cerr << "max ref : " << value::props< mln_value(I) >::max()
		      << "max image : " << maxval
		      << std::endl;

	    std::cerr << "error: file '" << filename
		      << "' cannot be loaded into this type of image"
		      << std::endl;
	    abort();
	  }

	  ima.init_(make::box2d(nrows, ncols));
	  if (type == type_)
	    load_raw_2d(file, ima);
	  else
	    if (type == (type_ - 3))
	      pnm::load_ascii(file, ima);

	}

# endif // ! MLN_INCLUDE_ONLY

      } // end of namespace mln::io::internal::pnm

    } // end of namespace mln::io::internal

  } // end of namespace mln::io

} // end of namespace mln


#endif // ! MLN_IO_PNM_LOAD_HH
