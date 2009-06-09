// Copyright (C) 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_FUN_P2P_MIRROR_HH
# define MLN_FUN_P2P_MIRROR_HH

/// \file
///
/// FIXME: doc

# include <mln/core/concept/function.hh>

namespace mln
{

  namespace fun
  {

    namespace p2p
    {

      template <typename B>
      struct mirror : public Function_v2v< mirror<B> >
      {
        typedef mln_psite(B) result;

        mirror(const B& box);

        mln_psite(B)
        operator()(const mln_psite(B)&) const;

        const B& box_;
      };

# ifndef MLN_INCLUDE_ONLY

      template <typename B>
      inline
      mirror<B>::mirror(const B& box)
        : box_(box)
      {
      }

      template <typename B>
      inline
      mln_psite(B)
      mirror<B>::operator()(const mln_psite(B)& p) const
      {
        mln_precondition(box_.has(p));

        mln_psite(B) r(p);
        r[0] = box_.max()[0] - p[0];

        return r;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::fun::p2p

  } // end of namespace mln::fun

} // end of namespace mln

#endif // ! MLN_FUN_P2P_MIRROR_HH

