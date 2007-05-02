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

#ifndef OLN_ARITH_MINUS_HH
# define OLN_ARITH_MINUS_HH

# include <oln/core/concept/image.hh>
# include <oln/core/gen/traits.hh>
# include <oln/core/gen/pw_value.hh>
# include <oln/core/gen/value_cast.hh>
# include <oln/level/fill.hh>
# include <oln/core/internal/f_ch_value.hh>


namespace oln
{

  // Trait.

  template <typename I, typename J>
  struct set_trait_< Image, I, minus_id, Image, J >
  {
    typedef oln_minus_trait(oln_value(I), oln_value(J)) V;
    typedef oln_plain_value(I, V) ret;
  };


  // Fwd decls.

  template <typename I, typename J>
  oln_minus_trait(I, J)
  operator - (const Image<I>& lhs, const Image<J>& rhs);

  namespace arith
  {

    template <typename V, typename I, typename J>
    oln_plain_value(I, V)
    minus(const Image<I>& lhs, const Image<J>& rhs);

  } // end of namespace oln::arith


# ifndef OLN_INCLUDE_ONLY

  template <typename I, typename J>
  oln_minus_trait(I, J)
  operator - (const Image<I>& lhs, const Image<J>& rhs)
  {
    precondition(lhs.points() == rhs.points());
    oln_minus_trait(I, J) output;
    prepare(output, with, lhs);
    oln_piter(I) p(lhs.points());
    for_all(p)
      output(p) = lhs(p) - rhs(p);
    return output;
  }

  namespace arith
  {

    template <typename V, typename I, typename J>
    oln_plain_value(I, V)
    minus(const Image<I>& lhs, const Image<J>& rhs)
    {
      precondition(lhs.points() == rhs.points());
      oln_plain_value(I, V) output;
      prepare(output, with, lhs);
      level::fill(inplace(output),
 		  value_cast<V>(pw_value(lhs) - pw_value(rhs)));
      return output;
    }

  } // end of namespace oln::arith

# endif // ! OLN_INCLUDE_ONLY

} // end of namespace oln

#endif // ! OLN_ARITH_MINUS_HH
