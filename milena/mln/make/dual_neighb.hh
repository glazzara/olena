// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_MAKE_DUAL_NEIGHB_HH
# define MLN_MAKE_DUAL_NEIGHB_HH

/// \file
///
/// Routine to create a dual neighborhood.

# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/pw/value.hh>
# include <mln/win/multiple_size.hh>
# include <mln/core/neighb.hh>


namespace mln
{

  namespace make
  {

    template <typename I, typename N>
    neighb< win::multiple_size< 2, mln_window(N), pw::value_<I> > >
    dual_neighb(const Image<I>& ima,
		const Neighborhood<N>& nbh_true,
		const Neighborhood<N>& nbh_false);



# ifndef MLN_INCLUDE_ONLY


    template <typename I, typename N>
    inline
    neighb< win::multiple_size< 2, mln_window(N), pw::value_<I> > >
    dual_neighb(const Image<I>& ima_,
		const Neighborhood<N>& nbh_true_,
		const Neighborhood<N>& nbh_false_)
    {
      mln_trace("make::dual_neighb");

      mlc_is(mln_trait_image_kind(I), trait::image::kind::logic)::check();

      const I& ima = exact(ima_);
      const N& nbh_true = exact(nbh_true_);
      const N& nbh_false = exact(nbh_false_);
      mln_precondition(ima.is_valid());

      typedef win::multiple_size< 2, mln_window(N), pw::value_<I> > W;
      W win(pw::value(ima));
      win.set_window(false, nbh_false.win()); // 0
      win.set_window(true,  nbh_true .win()); // 1

      neighb<W> nbh(win);

      return nbh;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::make

} // end of namespace mln


#endif // ! MLN_MAKE_DUAL_NEIGHB_HH
