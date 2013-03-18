// Copyright (C) 2009, 2013 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_WORLD_INTER_PIXEL_DIM2_MAKE_EDGE_IMAGE_HH
# define MLN_WORLD_INTER_PIXEL_DIM2_MAKE_EDGE_IMAGE_HH

/// \file
///
/// Construct a valued image of edges.

# include <mln/core/image/image2d.hh>
# include <mln/core/image/dmorph/extension_ima.hh>
# include <mln/core/image/dmorph/image_if.hh>

# include <mln/core/routine/extend.hh>

# include <mln/world/inter_pixel/neighb2d.hh>
# include <mln/world/inter_pixel/dim2/is_edge.hh>
# include <mln/data/fill.hh>

namespace mln
{

  namespace world
  {

    namespace inter_pixel
    {

      namespace dim2
      {

	template <typename I, typename F>
	mln_concrete(I)
	make_edge_image(const Image<I>& input_, const Function_vv2v<F>& f_)
	{
	  mln_trace("world::inter_pixel::dim2::make_edge_image");

	  const I& input = exact(input_);
	  const F& f = exact(f_);
	  mln_precondition(input.is_valid());

	  typedef image_if<const image2d<value::int_u<8u> >,
			   world::inter_pixel::dim2::is_edge> edges_t;
	  edges_t edges =  input | is_edge();
	  typedef extension_ima<const edges_t,const I> edges_ext_t;
	  edges_ext_t edges_ext = extend(edges, input);

	  mln_piter(edges_ext_t) p(edges_ext.domain());
	  dbl_neighb2d nbh = e2v();
	  mln_niter(dbl_neighb2d) n(nbh, p);
	  mln_value(I) vs[2];

	  mln_concrete(I) output;
	  initialize(output, input);
	  data::fill(output, literal::zero);

	  for_all(p)
	  {
	    unsigned i = 0;
	    for_all(n)
	    {
	      mln_assertion(i < 2);
	      vs[i++] = input(n);
	    }
	    output(p) = f(vs[0], vs[1]);
	  }

	  return output;
	}

      } // end of namespace mln::world::inter_pixel::dim2

    } // end of namespace mln::world::inter_pixel

  } // end of namespace mln::world

} // end of namespace mln

#endif // ! MLN_WORLD_INTER_PIXEL_DIM2_MAKE_EDGE_IMAGE_HH
