// Copyright (C) 2008 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_FUN_P2P_TRANSLATION_HH
# define MLN_FUN_P2P_TRANSLATION_HH

/// \file mln/fun/p2p/translation.hh
///
/// FIXME: doc

# include <mln/core/concept/function.hh>


namespace mln
{

  namespace fun
  {

    namespace p2p
    {

      template <typename P>
      struct translation_t : public Function_p2p< translation_t<P> >
      {
        typedef P result;

        translation_t(const mln_delta(P)& dp);

        P operator()(const P& p) const;
        P inverse(const P& p) const;

      protected:
        mln_delta(P) dp_;
      };


      template <typename D>
      translation_t<mln_site(D)>
      translation(const Gdpoint<D>& dp);



# ifndef MLN_INCLUDE_ONLY

      template <typename P>
      inline
      translation_t<P>::translation_t(const mln_delta(P)& dp)
	: dp_(dp)
      {
      }

      template <typename P>
      inline
      P
      translation_t<P>::operator()(const P& p) const
      {
        return p + dp_;
      }

      template <typename P>
      inline
      P
      translation_t<P>::inverse(const P& p) const
      {
        return p - dp_;
      }


      template <typename D>
      inline
      translation_t<mln_site(D)>
      translation(const Gdpoint<D>& dp)
      {
	translation_t<mln_site(D)> tmp(exact(dp));
	return tmp;
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::fun::p2p

  } // end of namespace mln::fun

} // end of namespace mln


#endif // ! MLN_FUN_P2P_TRANSLATION_HH


