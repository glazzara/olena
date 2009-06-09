// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
// (LRDE)
//
// This file is part of the Milena Library.  This library is free
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

#ifndef MLN_SET_INTER_HH
# define MLN_SET_INTER_HH

/// \file
///
/// Compute the intersection between a couple of site sets.

# include <algorithm>
# include <iterator>

# include <mln/core/site_set/p_set.hh>
# include <mln/convert/from_to.hh>
# include <mln/util/ord.hh>



namespace mln
{

  namespace set
  {

    /// Intersection between a couple of point sets.
    ///
    /// \relates mln::Site_Set
    ///
    template <typename Sl, typename Sr>
    p_set<mln_site(Sl)>
    inter(const Site_Set<Sl>& lhs, const Site_Set<Sr>& rhs);


# ifndef MLN_INCLUDE_ONLY

    template <typename Sl, typename Sr>
    inline
    p_set<mln_site(Sl)>
    inter(const Site_Set<Sl>& lhs, const Site_Set<Sr>& rhs)
    {
      trace::entering("set::inter");

      typedef mln_site(Sl) P;
      mlc_converts_to(mln_psite(Sr), P)::check(); 
      std::set< P, util::ord<P> > sl, sr, si;
      convert::from_to(lhs, sl);
      convert::from_to(rhs, sr);
      std::set_intersection(sl.begin(), sl.end(),
			    sr.begin(), sr.end(),
			    std::inserter(si, si.begin()),
			    util::ord<P>());
      p_set<P> s;
      convert::from_to(si, s);

      trace::exiting("set::inter");
      return s;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::set

} // end of namespace mln


#endif // ! MLN_SET_INTER_HH
