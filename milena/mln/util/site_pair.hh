// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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
// License.
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef MLN_UTIL_SITE_PAIR_HH
# define MLN_UTIL_SITE_PAIR_HH

# include <mln/util/ordpair.hh>

/// \file mln/util/site_pair.hh
/// \brief Definition of a site pair type.

namespace mln
{
  /* FIXME: The class mln::internal::image_base requires its site
     types to have a `coord' typedef, but util::ordpair_ has none.
     Hence this small workaround.  Remove as soon as image_base is
     refurbished.  */
  template <typename P>
  struct site_pair : public mln::Object< site_pair<P> >
  {
    site_pair();
    site_pair(const P& first, const P& second);
    typedef mln_coord(P) coord;
    util::ordpair_<P> pair_;
  };

  /* FIXME: The proxy mechanism requires these operators; why?  It
     seems its static assertions do not check the right guy.  IMHO
     (Roland's), it should look for line_graph_psite's operators,
     not site_pair's.  */
  template <typename P>
  bool operator==(const site_pair<P>& lhs, const site_pair<P>& rhs);

  template <typename P>
  bool operator< (const site_pair<P>& lhs, const site_pair<P>& rhs);



# ifndef MLN_INCLUDE_ONLY

  template <typename P>
  site_pair<P>::site_pair()
    : pair_(P(), P())
  {
  }

  template <typename P>
  site_pair<P>::site_pair(const P& first, const P& second)
    : pair_(first, second)
  {
  }

  template <typename P>
  inline
  bool
  operator==(const site_pair<P>& lhs, const site_pair<P>& rhs)
  {
    abort();
    return lhs.pair_ == rhs.pair_;
  }

  template <typename P>
  inline
  bool
  operator< (const site_pair<P>& lhs, const site_pair<P>& rhs)
  {
    abort();
    return lhs.pair_ < rhs.pair_;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of mln


#endif // ! MLN_UTIL_SITE_PAIR_HH
