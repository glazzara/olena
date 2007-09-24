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

/*! \file mln/io/internal/pnm/save.hh
 *
 * \brief function to save PNM images,
 *
 */

# include <iostream>
# include <fstream>

# include <mln/core/concept/image.hh>
# include <mln/core/point2d.hh>

# include <mln/value/rgb.hh>
# include <mln/value/rgb8.hh>
# include <mln/value/int_u8.hh>

# include <mln/metal/templated_by.hh>

# include <mln/io/internal/pnm/save_header.hh>
# include <mln/io/internal/pnm/macros.hh>

# include <mln/geom/size2d.hh>


namespace mln
{

  namespace io
  {

    namespace internal
    {

      namespace pnm
      {

# ifndef MLN_INCLUDE_ONLY

	// write a rgb value into for uncontiguous datas
	template <unsigned int n>
	void write_value(std::ofstream& file,
			 const value::rgb<n>& c)
	{
	  typedef typename value::rgb<n>::enc::enc E;

	  E v = c.red().to_enc();
	  file.write((char*)&v, sizeof(E));
	  v = c.green().to_enc();
	  file.write((char*)&v, sizeof(E));
	  v = c.blue().to_enc();
	  file.write((char*)&v, sizeof(E));
	}

	// write a scalar value into for uncontiguous datas
	template <typename V>
	void write_value(std::ofstream& file,
			 V& v)
	{
	  typedef typename V::enc E;

	  E c = v.to_enc();
	  file.write((char*)(&c), sizeof(E));
	}

	// save data for (sizeof(int_u8) != 1) and non fast images
	template <typename I>
	void save_data_uncontiguous(std::ofstream& file,
				    const Image< I >& ima_)
	{
	  const I& ima = exact(ima_);

	  const int
	    min_row = geom::min_row(ima),
	    max_row = geom::max_row(ima),
	    min_col = geom::min_col(ima),
	    max_col = geom::max_col(ima);

	  point2d p;
	  for (p.row() = min_row; p.row() <= max_row; ++p.row())
	    for (p.col() = min_col; p.col() <= max_col; ++p.col())
	      write_value(file, ima(p));
	}

	// save data when (sizeof(int_u8) == 1) with fast images
	// (faster)
	template <typename I>
	void save_data_contiguous(std::ofstream& file,
				  const Fast_Image<I>& ima_)
	{
	  const I& ima = exact(ima_);
	  const int
	    min_row = geom::min_row(ima),
	    max_row = geom::max_row(ima);
	  point2d p;
	  p.col() = geom::min_col(ima);
	  size_t len = geom::ncols(ima) * sizeof(mln_value(I));
	  for (p.row() = min_row; p.row() <= max_row; ++p.row())
	    file.write((char*)(& ima(p)), len);
	}


	// caller for fast images
	template <typename I>
	void save_data(std::ofstream& file,
		       const Fast_Image<I>& ima)
	{
	  if (sizeof(value::int_u8) == 1)
	    save_data_contiguous(file, ima);
	  else
	    save_data_uncontiguous(file, ima);
	}

	// caller for non fast images
	template <typename I>
	void save_data(std::ofstream& file,
		       const Image<I>& ima)
	{
	  save_data_uncontiguous(file, ima);
	}

	// main function : save header and data
	template <typename I>
	void save(const int type, const Image<I>& ima_, const std::string& filename)
	{
	  const I& ima = exact(ima_);
	  std::ofstream file(filename.c_str());
	  io::internal::pnm::save_header(type, ima, filename, file);

	  save_data(file, ima);
	}

# endif // ! MLN_INCLUDE_ONLY

      } // end of namespace mln::io::internal::pnm

    } // end of namespace mln::internal

  } // end of namespace mln::io

} // end of namespace mln


#endif // ! MLN_IO_PNM_SAVE_HH
