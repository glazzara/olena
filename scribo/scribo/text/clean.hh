// Copyright (C) 2009, 2010, 2011 EPITA Research and Development
// Laboratory (LRDE)
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

/// \file
///
/// Improve the quality of a text area.

# include <mln/core/concept/image.hh>
# include <mln/core/concept/weighted_window.hh>
# include <mln/core/routine/extend.hh>

# include <mln/morpho/skeleton_constrained.hh>
# include <mln/morpho/dilation.hh>

# include <mln/topo/skeleton/is_simple_point.hh>
# include <mln/topo/skeleton/crest.hh>

# include <mln/logical/not.hh>

//# include <mln/world/binary_2d/enlarge.hh>

# include <mln/debug/filename.hh>
# include <mln/io/pbm/save.hh>

#include <mln/labeling/wrap.hh>
#include <mln/win/octagon2d.hh>

#include <mln/arith/revert.hh>

#include <mln/transform/distance_front.hh>


#include <mln/linear/gaussian.hh>
#include <mln/value/int_u8.hh>

#include <mln/data/convert.hh>
#include <mln/value/rgb8.hh>
#include <mln/io/pgm/all.hh>

#include <scribo/core/line_info.hh>
#include <scribo/upsampling/bs2x.hh>
#include <scribo/upsampling/eagle.hh>
#include <scribo/subsampling/bilinear.hh>

#include <scribo/text/clean_inplace.hh>

namespace scribo
{

  namespace text
  {

    using namespace mln;


    /// Improve quality of an image with text.
    ///
    /// \param[in] line   Line info providing statistics about the text
    ///                   in the corresponding image \p input.
    /// \param[in] input_ A binary image. Object are set to 'false'
    ///                   and backgroud to 'true'.
    ///
    /// \return An image. The text have better quality.
    //
    template <typename L, typename I>
    mln_concrete(I)
    clean(const line_info<L>& line, const Image<I>& input_);


# ifndef MLN_INCLUDE_ONLY

    template <typename L, typename I>
    mln_concrete(I)
    clean(const line_info<L>& line, const Image<I>& input_)
    {
      trace::entering("scribo::text::clean");

      const I& input = exact(input_);
      mlc_bool(mln_site_(I)::dim == 2)::check();
      mlc_equal(mln_value(I),bool)::check();
      mln_precondition(input.is_valid());
      mln_precondition(line.is_valid());

      mln_concrete(I) output = duplicate(input);

      clean_inplace(line, output);

      trace::exiting("scribo::text::clean");
      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::text

} // end of namespace scribo

#endif // ! SCRIBO_TEXT_CLEAN_HH
