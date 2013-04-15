// Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007, 2008, 2009,
// 2010 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_IO_PNM_SAVE_HH
# define MLN_IO_PNM_SAVE_HH

/// \file
///
/// Define a function which saves an image of kind PNM into
/// given path.

# include <iostream>
# include <fstream>

# include <mln/core/concept/image.hh>
# include <mln/core/alias/point2d.hh>
# include <mln/value/concept/scalar.hh>

# include <mln/value/rgb.hh>
# include <mln/value/rgb8.hh>
# include <mln/value/int_u8.hh>

# include <mln/metal/templated_by.hh>
# include <mln/metal/not_equal.hh>

# include <mln/io/pnm/save_header.hh>
# include <mln/io/pnm/macros.hh>

# include <mln/geom/size2d.hh>


namespace mln
{

  namespace io
  {

    namespace pnm
    {

      /*! Save a Milena image as a pnm image.
       *
       * \param[in] type The type of the image to save (can be PPM,
       * PGM, PBM).
       * \param[in] ima_ The image to save.
       * \param[in,out] filename the destination.
       */
      template <typename I>
      void save(char type, const Image<I>& ima_, const std::string& filename);



# ifndef MLN_INCLUDE_ONLY

      namespace impl
      {

	// write a rgb value into for uncontiguous datas
	template <unsigned int n>
	inline
	void write_value(std::ofstream& file,
			 const value::rgb<n>& c)
	{
	  typedef typename value::int_u<n>::enc E;

	  E v = c.red().to_enc();
	  file.write((char*)&v, sizeof(E));
	  v = c.green().to_enc();
	  file.write((char*)&v, sizeof(E));
	  v = c.blue().to_enc();
	  file.write((char*)&v, sizeof(E));
	}

	// write a scalar value into for uncontiguous datas
	template <typename V>
	inline
	void write_value(std::ofstream& file,
			 const V& v)
	{
	  mlc_not_equal(V,bool)::check();
	  file.write((char*)(&v), sizeof(V));
	}

	// write a scalar value into for uncontiguous datas
	template <typename S>
	inline
	void write_value(std::ofstream& file,
			 const value::Scalar<S>& s)
	{
	  typedef typename S::enc E;

	  E c = s.to_enc();
	  file.write((char*)(&c), sizeof(E));
	}

	// save data for (sizeof(int_u8) != 1) and non fastest images
	template <typename I>
	inline
	void save_data_uncontiguous(std::ofstream& file,
				    const I& ima)
	{
	  const def::coord
	    min_row = geom::min_row(ima),
	    max_row = geom::max_row(ima),
	    min_col = geom::min_col(ima),
	    max_col = geom::max_col(ima);

	  point2d p;
	  for (p.row() = min_row; p.row() <= max_row; ++p.row())
	    for (p.col() = min_col; p.col() <= max_col; ++p.col())
	      write_value(file, ima(p));
	}

	// save data when (sizeof(int_u8) == 1) with fastest images
	// (faster)
	template <typename I>
	inline
	void save_data_contiguous(std::ofstream& file,
				  const I& ima_)
	{
	  const I& ima = exact(ima_);
	  const def::coord
	    min_row = geom::min_row(ima),
	    max_row = geom::max_row(ima);
	  point2d p;
	  p.col() = geom::min_col(ima);
	  std::size_t len = geom::ncols(ima) * sizeof(mln_value(I));
	  for (p.row() = min_row; p.row() <= max_row; ++p.row())
	    file.write((char*)(& ima(p)), len);
	}


	// caller for fastest images
	template <typename I>
	inline
	void save_data_(std::ofstream& file,
			trait::image::speed::fastest, const I& ima)
	{
	  // FIXME: GCC 4.1.1 crashes on that line.
	  if (sizeof(value::int_u8) == 1)
	    save_data_contiguous(file, ima);
	  else
	    save_data_uncontiguous(file, ima);
	}

	// caller for non fastest images
	template <typename I>
	inline
	void save_data_(std::ofstream& file,
			trait::image::speed::any, const I& ima)
	{
	  save_data_uncontiguous(file, ima);
	}

      } // end of namespace mln::io::pnm::impl


	// Facades.

      template <typename I>
      inline
      void save(char type, const Image<I>& ima_, const std::string& filename)
      {
	mln_trace("mln::io::pnm::save");
	const I& ima = exact(ima_);
	std::ofstream file(filename.c_str());
	io::pnm::save_header(type, ima, filename, file);

	impl::save_data_(file,
			 mln_trait_image_speed(I)(), ima);
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::io::pnm

  } // end of namespace mln::io

} // end of namespace mln


#endif // ! MLN_IO_PNM_SAVE_HH
