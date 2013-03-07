// Copyright (C) 2009, 2013 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef SCRIBO_MAKE_INFLUENCE_ZONE_GRAPH_HH
# define SCRIBO_MAKE_INFLUENCE_ZONE_GRAPH_HH

/// \file
///
/// Create a region adjacency graph from the influence zone image
/// of binary image.


# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>

# include <mln/labeling/blobs.hh>

# include <mln/transform/influence_zone_geodesic.hh>

# include <mln/util/graph.hh>

# include <mln/make/influence_zone_adjacency_graph.hh>

# include <mln/pw/all.hh>

namespace scribo
{

  namespace make
  {
    using namespace mln;


    /// Compute a labeled image of input, then compute an influence
    /// zone image and make a graph from it.
    ///
    /// \param[in] input_	a binary image.
    /// \param[in] nbh_	a neighbordhood.
    /// \param[in] label_type The type of this argument is used
    ///			as label type while labeling the image.
    /// \param[in] iz_dmax	Max distance of the influence zone.
    ///
    /// \return a region adjacency graph.
    template <typename I, typename N, typename V>
    mln::util::graph
    influence_zone_graph(const Image<I>& input_,
			 const Neighborhood<N>& nbh_,
			 const V& label_type,
			 unsigned iz_dmax);


# ifndef MLN_INCLUDE_ONLY

    template <typename I, typename N, typename V>
    mln::util::graph
    influence_zone_graph(const Image<I>& input_,
			 const Neighborhood<N>& nbh_,
			 const V& label_type,
			 unsigned iz_dmax)
    {
      trace::entering("scribo::make::influence_zone_graph");

      (void) label_type;
      I& input = exact(input_);
      const N& nbh = exact(nbh_);

      mlc_equal(mln_value(I), bool)::check();
      mln_assertion(input.is_valid());
      mln_assertion(nbh.is_valid());

      V nlabels;
      typedef mln_ch_value(I,V) lbl_t;
      lbl_t lbl = labeling::blobs(input, nbh, nlabels);

      lbl_t iz = transform::influence_zone_geodesic(lbl, nbh, iz_dmax);

      mln::util::graph g
	= mln::make::influence_zone_adjacency_graph(
	  iz | (pw::value(iz) != pw::cst(literal::zero)),
	  nbh, nlabels);

      trace::exiting("scribo::make::influence_zone_graph");
      return g;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::make

} // end of namespace scribo



#endif // ! SCRIBO_MAKE_INFLUENCE_ZONE_GRAPH_HH
