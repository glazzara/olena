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

#ifndef MLN_CONVERT_TO_STD_SET_HH
# define MLN_CONVERT_TO_STD_SET_HH

/*! \file mln/convert/to_std_set.hh
 *
 * \brief Conversions to std::set.
 */

# include <set>
# include <algorithm>
# include <iterator>

# include <mln/core/window.hh>


namespace mln
{

  namespace convert
  {

    /// Convert a window \p win into a std::set of delta-points.
    template <typename W>
    std::set<mln_dpoint(W)> to_std_set(const Window<W>& win);

    /// Convert a point set \p pset into a std::set of points.
    template <typename W>
    std::set<mln_point(W)> to_std_set(const Site_Set<W>& setp);

# ifndef MLN_INCLUDE_ONLY

    template <typename W>
    inline
    std::set<mln_dpoint(W)> to_std_set(const Window<W>& win)
    {
      typedef mln_dpoint(W) D;
      typedef mln_point(D)  P;
      std::set<D> s;
      mln_qiter(W) q(exact(win), P::origin);
      for_all(q)
	s.insert(q - P::origin);
      return s;
    }

    template <typename W>
    inline
    std::set<mln_point(W)> to_std_set(const Site_Set<W>& setp)
    {
      typedef mln_point(W) P;
      std::set<P> s;
      mln_piter(W) p(exact(setp));
      for_all(p)
	s.insert(p);
      return s;
    }
# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::convert

} // end of namespace mln


#endif // ! MLN_CONVERT_TO_STD_SET_HH
