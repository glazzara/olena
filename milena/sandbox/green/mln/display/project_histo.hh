// Copyright (C) 2007 EPITA Research and Development Laboratory (LRDE)
// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_DISPLAY_PROJECT_HISTO_HH
# define MLN_DISPLAY_PROJECT_HISTO_HH

# include <mln/core/image/image2d.hh>
# include <mln/core/image/image3d.hh>
# include <mln/core/image/dmorph/unproject_image.hh>
# include <mln/fun/v2v/projection.hh>

# include <mln/accu/image/init.hh>
# include <mln/accu/image/take.hh>
# include <mln/accu/image/to_result.hh>

/// \file
///
/// \brief Allow the visualization of 3d histogram.
/// The 3d histogram is projected in 2d such as the data in that direction
/// are accumulated to the two others.

namespace mln
{

  namespace display
  {

    // Forward declaration.
    template <typename A, unsigned direction, typename V>
    image2d<mln_result(A)>
    project_histo(const image3d<V>& histo);

#ifndef MLN_INCLUDE_ONLY

    /// \brief Allow the visualization of 3d histogram.
    ///
    /// The 3d histogram is projected in 2d such as the data in that direction
    /// are accumulated to the two others.
    ///
    /// Parameter A is the type of accumulator, for instance, accu::math::sum.
    /// Parameter direction is the way of the projection, for instance blue one.
    /// Parameter V is the value we use to accumulate information.
    ///
    /// \prameter[in] the histogram 3d.
    /// \result       the 2d projection of the 3d histogram.

    template <typename A, unsigned direction, typename V>
    image2d<mln_result(A)>
    project_histo(const image3d<V>& histo)
    {
      typedef fun::v2v::projection<point3d,direction> t_projection;

      image2d<A> histo_accu(histo.nrows(), histo.ncols());

      accu::image::init(histo_accu);

      accu::image::take(unproject(histo_accu,
				  histo.domain(),
				  t_projection()).rw(),
			histo);

      return accu::image::to_result(histo_accu);
    }

#endif // ! MLN_INCLUDE_ONLY


  } // end of namespace mln::transform

} // end of namespace mln


#endif // ! MLN_DISPLAY_PROJECT_HISTO_HH
