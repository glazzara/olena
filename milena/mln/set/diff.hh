// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_SET_DIFF_HH
# define MLN_SET_DIFF_HH

/// \file
///
/// Set theoretic difference (non-symmetrical) of a couple of
/// sets.
///
/// \todo Add a diff(Site_Set& in_place, Function_v2b).

# include <algorithm>
# include <iterator>

# include <mln/core/site_set/p_set.hh>
# include <mln/convert/from_to.hh>
# include <mln/util/ord.hh>



namespace mln
{

  namespace set
  {

    /// Set theoretic difference of \p lhs and \p rhs.
    ///
    /// \relates mln::Site_Set
    ///
    template <typename Sl, typename Sr>
    p_set<mln_site(Sl)>
    diff(const Site_Set<Sl>& lhs, const Site_Set<Sr>& rhs);



# ifndef MLN_INCLUDE_ONLY

    template <typename Sl, typename Sr>
    inline
    p_set<mln_site(Sl)>
    diff(const Site_Set<Sl>& lhs, const Site_Set<Sr>& rhs)
    {
      mln_trace("set::diff");

      typedef mln_site(Sl) P;
      mlc_converts_to(mln_psite(Sr), P)::check(); 
      std::set< P, util::ord<P> > sl, sr, sd;
      convert::from_to(lhs, sl);
      convert::from_to(rhs, sr);
      std::set_difference(sl.begin(), sl.end(),
			  sr.begin(), sr.end(),
			  std::inserter(sd, sd.begin()),
			  util::ord<P>());
      p_set<P> s;
      convert::from_to(sd, s);

      return s;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::set

} // end of namespace mln


#endif // ! MLN_SET_DIFF_HH
