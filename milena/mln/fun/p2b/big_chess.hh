// Copyright (C) 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_FUN_P2B_BIG_CHESS_HH
# define MLN_FUN_P2B_BIG_CHESS_HH

/*! \file mln/fun/p2b/big_chess.hh
 *
 * \brief FIXME.
 */

# include <mln/core/concept/function.hh>
# include <mln/core/alias/point2d.hh>


namespace mln
{

  namespace fun
  {

    namespace p2b
    {

      // FIXME: Doc!
      template <typename B>
      struct big_chess : public Function_v2b< big_chess<B> >
      {
        typedef bool result;
        typedef mln_psite(B) P;

        big_chess(const B& box, const unsigned div);

	bool operator()(const P& p) const;

        const B& box;
        const unsigned div;
      };


# ifndef MLN_INCLUDE_ONLY

      template < typename B>
      big_chess<B>::big_chess(const B& box, const unsigned div)
        : box(box), div(div)
      {
      }

      template < typename B>
      inline
      bool
      big_chess<B>::operator()(const P& p) const
      {
        mln_coord(P) ret = 0;

        for (short i = 0; i < P::dim; i++)
          ret += p[i] / ((box.pmax()[i] - 1) / div);

        return ret % 2;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::fun::p2b

  } // end of namespace mln::fun

} // end of namespace mln


#endif // ! MLN_FUN_P2B_BIG_CHESS_HH
