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

#ifndef MLN_SET_UNIQUE_HH
# define MLN_SET_UNIQUE_HH

/// \file
///
/// Give the unique set.

# include <algorithm>
# include <iterator>

# include <mln/core/site_set/p_set.hh>
# include <mln/convert/from_to.hh>
# include <mln/util/ord.hh>


namespace mln
{

  namespace set
  {

    /// Give the unique set of \p s.
    ///
    /// \relates mln::Site_Set
    ///
    template <typename S>
    p_set<mln_site(S)>
    unique(const Site_Set<S>& s);


# ifndef MLN_INCLUDE_ONLY

    template <typename S>
    inline
    p_set<mln_site(S)>
    unique(const Site_Set<S>& s)
    {
      mln_trace("set::unique");

      typedef mln_site(S) P;
      std::set< P, util::ord<P> > s_;
      convert::from_to(s, s_);
      p_set<P> su;
      convert::from_to(s_, su);

      return su;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::set

} // end of namespace mln


#endif // ! MLN_SET_UNIQUE_HH
