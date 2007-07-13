// Copyright (C) 2007 EPITA Research and Development Laboratory
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

#ifndef MLN_ESTIM_COUNT_HH
# define MLN_ESTIM_COUNT_HH

/*! \file mln/estim/count.hh
 *
 * \brief Several routines to count the number of points.
 */


namespace mln
{

  namespace estim
  {


    /// Compute the number FIXME minimum and maximum points when browsing with
    /// iterator \p p.
    template <typename S>
    std::size_t count(const Point_Set<S>& pset);



# ifndef MLN_INCLUDE_ONLY

    template <typename I>
    std::pair<mln_point(I), mln_point(I)>
    count(const Piter<I>& p_)
    {
      const I& p = exact(p_);

      typedef mln_point(I) P;
      std::pair<P, P> tmp;
      P& pmin = tmp.first;
      P& pmax = tmp.second;

      // init with first point
      p.start();
      pmin = pmax = p;

      // update with remaining points
      for_all_remaining(p)
	for (unsigned i = 0; i < P::dim; ++i)
	  if (p[i] < pmin[i])
	    pmin()[i] = p[i];
	  else if (p[i] > pmax[i])
	    pmax[i] = p[i];

      return tmp;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::estim

} // end of namespace mln


#endif // ! MLN_ESTIM_COUNT_HH
