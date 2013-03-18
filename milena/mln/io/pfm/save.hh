// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_IO_PFM_SAVE_HH
# define MLN_IO_PFM_SAVE_HH

/// \file
///
/// Define a function which saves an image of kind pfm into
/// given path.

# include <iostream>
# include <fstream>

# include <mln/core/alias/point2d.hh>
# include <mln/geom/size2d.hh>
# include <mln/metal/equal.hh>
# include <mln/metal/bexpr.hh>


namespace mln
{

  namespace io
  {

    namespace pfm
    {

      /// Save a Milena image as a pfm image.
      ///
      /// \param[in] ima The image to save.
      /// \param[in,out] filename the destination.
      ///
      template <typename I>
      void save(const Image<I>& ima, const std::string& filename);

# ifndef MLN_INCLUDE_ONLY

      namespace impl
      {

	template <typename I>
	inline
	void save_header_(const I& ima, std::ofstream& file)
	{
	  file <<  geom::nrows(ima) << ' ' << geom::nrows(ima) << std::endl
	       << "float" << std::endl;
	}

	template <typename I>
	inline
	void save_(const Image<I>& ima_, const std::string& filename)
	{
	  const I& ima = exact(ima_);

	  std::ofstream file(filename.c_str());
	  save_header_(ima, file);

	  unsigned int ncols = geom::ncols(ima);

	  const mln_deduce(I, site, coord)
	    min_row = geom::min_row(ima),
	    max_row = geom::max_row(ima);

	  //image2d<float>& ima_ = const_cast< image2d<float>& >(ima);
	  point2d p(point2d(0, 0));
	  for (p.row() = min_row; p.row() <= max_row; ++p.row())
	    file.write((const char*)(&(ima(p))),
		       sizeof(float) * ncols);
	  file.close();

	}

      } // end of  namespace mln::io::impl

      template <typename I>
      inline
      void save(const Image<I>& ima, const std::string& filename)
      {
	mln_trace("mln::io::pfm::save");
	mln::metal::equal<mln_value(I), float >::check();
	impl::save_(exact(ima), filename);
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::pfm

  } // end of namespace mln::io

} // end of namespace mln


#endif // ! MLN_IO_PFM_SAVE_HH
