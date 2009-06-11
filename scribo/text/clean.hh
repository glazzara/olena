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

#ifndef SCRIBO_TEXT_CLEAN_HH
# define SCRIBO_TEXT_CLEAN_HH

/// \file scribo/text/clean.hh
///
/// Improve the quality of a text area.

# include <mln/core/concept/weighted_window.hh>
# include <mln/core/routine/extend.hh>

# include <mln/morpho/skeleton_constrained.hh>
# include <mln/morpho/dilation.hh>

# include <mln/topo/skeleton/is_simple_point.hh>
# include <mln/topo/skeleton/crest.hh>

# include <mln/logical/not.hh>

# include <mln/world/binary_2d/enlarge.hh>

# include <mln/debug/filename.hh>
# include <mln/io/pbm/save.hh>

#include <mln/labeling/wrap.hh>
#include <mln/win/octagon2d.hh>

#include <mln/arith/revert.hh>

#include <mln/transform/distance_front.hh>


#include <mln/linear/gaussian.hh>
#include <mln/value/int_u8.hh>



namespace scribo
{

  namespace text
  {

    using namespace mln;


    /// Improve quality of an image with text.
    ///
    /// \param[in] input_ A binary image.
    /// \param[in] dmap_win_ A weighted window.
    ///
    /// \return An image. The text have better quality.
    //
    template <typename I, typename W>
    mln_concrete(I)
    clean(const Image<I>& input_, const Weighted_Window<W>& dmap_win_);


# ifndef MLN_INCLUDE_ONLY


//    static int plop = 0;

    template <typename I, typename W>
    mln_concrete(I)
    clean(const Image<I>& input_, const Weighted_Window<W>& dmap_win_)
    {
      trace::entering("scribo::text::clean");

      const I& input = exact(input_);
      const W& dmap_win = exact(dmap_win_);
      mlc_bool(mln_site_(I)::dim == 2)::check();
      mlc_equal(mln_value(I),bool)::check();
      mln_precondition(input.is_valid());
      mln_precondition(dmap_win.is_valid());

      I input_large = world::binary_2d::enlarge(input, 2);

//     image2d<bool> blur = linear::gaussian(input_large, 2);
//     image2d<value::int_u8> blur = linear::gaussian(level::convert(value::int_u8(), input_large), 2);
//     image2d<bool> blur = level::transform(linear::gaussian(level::convert(value::int_u8(), input_large), 2), fun::v2b::threshold<value::int_u8>(100));

      mln_ch_value(I,unsigned)
        dmap = transform::distance_front(logical::not_(input_large), c8(),
                                         dmap_win,
                                         mln_max(unsigned));
//      io::pgm::save(labeling::wrap(dmap), mln::debug::filename("dmap.pgm"));

//      I skeleton = topo::skeleton::crest(input_large, dmap, c8());
      I constraint = topo::skeleton::crest(input_large, dmap, c8());
      mln_postcondition(constraint.is_valid());

//      io::pgm::save(labeling::wrap(constraint), mln::debug::filename("constraint.pgm"));

      I skeleton =
        morpho::skeleton_constrained(input_large, c8(),
                                     topo::skeleton::is_simple_point<I,neighb2d>,
                                     extend(constraint, false), arith::revert(dmap));

      win::octagon2d disk(7);
      I output = morpho::dilation(skeleton, disk);

//      if (plop > 20 && plop < 50)
      {
//        io::pbm::save(input, mln::debug::filename("input.pbm"));
//        io::pbm::save(input_large, mln::debug::filename("input_large_4x.pbm"));
//        io::pbm::save(skeleton, mln::debug::filename("skeleton.pbm"));
//        io::pbm::save(output, mln::debug::filename("dil_skel.pbm"));
      }

//      ++plop;
      trace::exiting("scribo::text::clean");
      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::text

} // end of namespace scribo

#endif // ! SCRIBO_TEXT_CLEAN_HH
