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

/*!
 * \file mln/io/pnm/load.hh
 *
 * \brief Define a function which loads an image of kind PNM 8/16bits with
 * given path.
 */

# include <iostream>
# include <fstream>
# include <string>

# include <mln/core/image/image2d.hh>

# include <mln/value/int_u8.hh>
# include <mln/value/rgb.hh>

# include <mln/io/pnm/load_header.hh>
# include <mln/io/pnm/max_component.hh>
# include <mln/io/pnm/macros.hh>

namespace mln
{

  namespace io
  {

    namespace pnm
    {


# ifndef MLN_INCLUDE_ONLY

      // Read a Milena rgb value (sizeof(int_u8) != 1).
      template <unsigned int n>
      inline
      void read_value(std::ifstream& file, value::rgb<n>& v)
      {
	typedef typename value::int_u<n>::enc E;

	E c;
	file.read((char*)(&c), sizeof(E));
	v.red() = c;
	file.read((char*)(&c), sizeof(E));
	v.green() = c;
	file.read((char*)(&c), sizeof(E));
	v.blue() = c;
      }

      // Read a Milena scalar value (sizeof(int_u8) != 1).
      template <class V>
      inline
      void read_value(std::ifstream& file, value::Scalar<V>& v)
      {
	typedef typename V::enc E;

	E c;
	file.read((char*)(&c), sizeof(E));
	exact(v) = c;
      }

      // Read a builtin scalar value.
      template <typename V>
      inline
      void read_value(std::ifstream& file, V& v)
      {
	V c;
	file.read((char*)(&c), sizeof(V));
	v = c;
      }

      // used when (sizeof(int_u8) != 1)
      template <typename V>
      inline
      void load_raw_2d_uncontiguous(std::ifstream& file, image2d<V>& ima)
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
      inline
      void load_raw_2d_contiguous(std::ifstream& file, I& ima)
      {
	point2d p = point2d(0, ima.domain().pmin().col());
	typedef mln_value(I) V;
	const mln_coord(I)
	  min_row = geom::min_row(ima),
	  max_row = geom::max_row(ima);

	std::size_t len = geom::ncols(ima) * sizeof(V);
	for (p.row() = min_row; p.row() <= max_row; ++p.row())
	  file.read((char*)(&ima(p)), len);
      }

      /// load_ascii.
      template <typename I>
      inline
      void load_ascii(std::ifstream& file, I& ima)
      {
	mln_fwd_piter(I) p(ima.domain());
	for_all(p)
	  file >> ima(p);
      }

      /// load_raw_2d.
      /// for all pnm 8/16 bits formats
      template <typename I>
      inline
      void load_raw_2d(std::ifstream& file, I& ima)
      {
	if (sizeof(value::int_u8) == 1)
	  load_raw_2d_contiguous(file, ima);
	else
	  load_raw_2d_uncontiguous(file, ima);
      }

      /// main function : load pnm format
      template <typename V>
      inline
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

	if (max_component(V()) != maxval)
	{
	  std::cerr << "error: file '" << filename
		    << "' cannot be loaded into this type of image"
		    << std::endl;

	  std::cerr << "input image have " << maxval
		    << " as maximum value while the destination's one is "
		    << max_component(V()) << " (should be the same)."
		    << std::endl;
	  abort();
	}

	image2d<V> ima(nrows, ncols);
	if (type == type_)
	  load_raw_2d(file, ima);
	else
	  if (type == (type_ - 3))
	    pnm::load_ascii(file, ima);
	return ima;

      }

      /// An other way to load pnm files :
      /// the destination is an argument to check if
      /// the type match the file to load.
      template <typename I>
      inline
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

	if (max_component(mln_value(I)()) != maxval)
	{
	  std::cerr << "error: file '" << filename
		    << "' cannot be loaded into this type of image"
		    << std::endl;

	  std::cerr << "input image have " << maxval
		    << " as maximum value while the destination's one is "
		    << max_component(mln_value(I)()) << "."
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

    } // end of namespace mln::io::pnm

  } // end of namespace mln::io

} // end of namespace mln


#endif // ! MLN_IO_PNM_LOAD_HH
