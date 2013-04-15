// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef SCRIBO_TABLE_INTERNAL_CONNECT_LINES_HH
# define SCRIBO_TABLE_INTERNAL_CONNECT_LINES_HH

/// \file
///
/// Connect vertical lines with aligned rows.

# include <mln/core/image/image1d.hh>
# include <mln/core/alias/neighb1d.hh>

# include <mln/data/fill.hh>

# include <mln/transform/influence_zone_geodesic_saturated.hh>

# include <mln/morpho/elementary/dilation.hh>
# include <mln/util/array.hh>
# include <mln/util/couple.hh>

# include <mln/opt/at.hh>

# include <scribo/core/macros.hh>
# include <scribo/core/central_sites.hh>

namespace scribo
{

  namespace table
  {

    namespace internal
    {

      using namespace mln;

      /// Connect vertical and horizontal lines if they are close to each other.
      ///
      ///  ------                 ------
      ///		  --->          |
      ///        |                      |
      ///        |                      |
      ///
      ///
      /// FIXME: doc arguments.
      template <typename P>
      void
      connect_lines(const mln::util::array<int>& aligned_lines,
		    mln::util::array< box<P> >& boxes,
		    unsigned dim,
		    unsigned dim_size,
		    unsigned max_distance);


# ifndef MLN_INCLUDE_ONLY


      template <typename P>
      void
      connect_lines(const mln::util::array<int>& aligned_lines,
		    mln::util::array< box<P> >& boxes,
		    unsigned dim,
		    unsigned dim_size,
		    unsigned max_distance)
      {
	mln_trace("scribo::table::internal::connect_lines");

	image1d<int> l(dim_size);
	data::fill(l, -1);

	for_all_elements(i, aligned_lines)
	  opt::at(l, aligned_lines[i]) = aligned_lines[i];

	l = transform::influence_zone_geodesic_saturated(l, c2(),
							 max_distance, -1);

	for_all_comps(i, boxes)
	{
	  mln::util::couple<P,P> cp = central_sites(boxes[i], dim);
	  if (opt::at(l, cp.first()[dim]) != -1
		&& opt::at(l, cp.first()[dim]) < boxes[i].pmin()[dim])
	    boxes[i].pmin()[dim] = opt::at(l, cp.first()[dim]);
	  if (opt::at(l, cp.second()[dim]) != -1
		&& opt::at(l, cp.second()[dim]) > boxes[i].pmax()[dim])
	    boxes[i].pmax()[dim] = opt::at(l, cp.second()[dim]);
	}

      }


# endif // ! MLN_INCLUDE_ONLY


    } // end of namespace scribo::table::internal

  } // end of namespace scribo::table

} // end of namespace scribo


#endif // ! SCRIBO_TABLE_INTERNAL_CONNECT_LINES_HH
