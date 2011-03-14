// Copyright (C) 2011 EPITA Research and Development Laboratory (LRDE)
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

#ifndef SCRIBO_IO_IMG_INTERNAL_DRAW_EDGES_HH
# define SCRIBO_IO_IMG_INTERNAL_DRAW_EDGES_HH

/// \file
///
/// \brief Draw object edges.

# include <fstream>
# include <mln/core/image/image2d.hh>
# include <mln/core/image/dmorph/image_if.hh>
# include <mln/pw/all.hh>
# include <mln/data/fill.hh>
# include <mln/data/convert.hh>
# include <mln/literal/colors.hh>
# include <mln/value/rgb8.hh>
# include <mln/morpho/elementary/gradient_external.hh>

# include <scribo/core/component_info.hh>
# include <scribo/core/def/lbl_type.hh>


namespace scribo
{

  namespace io
  {

    namespace img
    {

      namespace internal
      {

	using namespace mln;

        /*! \brief Draw component edges.
	 */
	void
	draw_edges(const component_info& info,
		   image2d<value::rgb8>& output, const value::rgb8& value,
		   const image2d<scribo::def::lbl_type>& edges);


# ifndef MLN_INCLUDE_ONLY


	inline
	void
	draw_edges(const component_info& info,
		   image2d<value::rgb8>& output, const value::rgb8& value,
		   const image2d<scribo::def::lbl_type>& edges)
	{
	  box2d b = info.bbox();
	  b.enlarge(1);
	  data::fill(((output | b).rw()
		      | (pw::value(edges)
			 == pw::cst((scribo::def::lbl_type)info.id().to_equiv()))).rw(),
		     value);
	}


# endif // ! MLN_INCLUDE_ONLY

      } // end of namespace scribo::io::img::internal

    } // end of namespace scribo::io::img

  } // end of namespace scribo::io

} // end of namespace scribo

#endif // ! SCRIBO_IO_IMG_INTERNAL_DRAW_EDGES_HH
