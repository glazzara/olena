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

#ifndef MLN_DISPLAY_DISPLAY_REGION_HH
# define MLN_DISPLAY_DISPLAY_REGION_HH

/// \file
///
/// FIXME

#include <mln/core/concept/image.hh>
#include <mln/core/image/dmorph/image_if.hh>

#include <mln/value/rgb8.hh>

#include <mln/convert/from_to.hh>
#include <mln/data/fill.hh>
#include <mln/data/paste.hh>
#include <mln/pw/all.hh>


namespace mln
{

  namespace display
  {

    /// \brief FIXME
    ///
    template<typename I, typename L>
    inline
    mln_ch_value(I, value::rgb8)
    display_region(const Image<I>& ima, const Image<L>& wst, const value::rgb8 color);


#ifndef MLN_INCLUDE_ONLY

    template<typename I, typename L>
    inline
    mln_ch_value(I, value::rgb8)
    display_region(const Image<I>& ima_, const Image<L>& wst_, const value::rgb8 color)
    {
      trace::entering("display::display_region");
      // TODO: We should check that wst.bbox() == input.bbox()

      const I& ima = exact(ima_);
      const L& wst = exact(wst_);

      mln_ch_value(I, value::rgb8) output;
      initialize(output, ima);

      mln_piter(I) p(ima.domain());
      for_all(p)
	convert::from_to(ima(p), output(p));
      data::fill((output | pw::value(wst) == pw::cst(0)).rw(), color);

      trace::exiting("display::display_region");
      return output;
    }

# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace mln::display

} // end of namespace mln

#endif // ! MLN_DISPLAY_DISPLAY_REGION_HH
