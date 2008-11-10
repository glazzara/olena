// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef MLN_CO_OCCURENCE_HH
# define MLN_CO_OCCURENCE_HH

/*! \file ??
 *
 * \brief FIXME
 */

# include <mln/core/concept/image.hh>
# include <mln/core/dpoint.hh>
# include <mln/core/image/image2d.hh>
# include <mln/level/fill.hh>


namespace mln
{


  template <typename I, typename D>
  //  image2d<unsigned> co_occurence (const Image<I>&, const Dpoint<D>&);
  image2d<unsigned> co_occurence (const Image<I>&, const Gdpoint<D>&);

# ifndef MLN_INCLUDE_ONLY

  template <typename I, typename D>
  image2d<unsigned> co_occurence (const Image<I> &ima_, const Gdpoint<D> &dp_)
  {
    mln::metal::equal<mln_psite(I), mln_psite(D)>::check();
    // FIXME : check thaat Image<I> is quant low

    const I &ima = exact(ima_);
    const D &dp = exact(dp_);
    image2d<unsigned> mco(mln_card(mln_value(I)), mln_card(mln_value(I)), 0);
    level::fill(mco, 0);


    mln_piter(I) p(ima.domain());
    for_all(p)
      if (ima.domain().has(p + dp))
	mco.at(ima(p), ima(p + dp))++;

    return mco;
  }

#endif // MLN_INCLUDE_ONLY

}

#endif // MLN_CO_OCCURENCE_HH
