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

#ifndef MLN_FUN_X2X_TRANSLATION_ALT_HH
# define MLN_FUN_X2X_TRANSLATION_ALT_HH

/*! \file mln/fun/x2x/translation_alt.hh
 *
 * \brief FIXME.
 */

# include <mln/fun/x2x/bijective_tr.hh>
# include <mln/fun/internal/x2x_impl.hh>


namespace mln
{

  namespace fun
  {

    namespace x2x
    {

      // FIXME: Doc!

      template <unsigned n, typename C>
      struct translation_alt
	:

	fun::internal::x2x_impl_< metal::vec<n,C>, translation_alt<n,C> >
	,
	Bijection_x2x< translation_alt<n,C> >

	// FIXME: Activate public bijective_tr< translation_alt<n,C> >
      {
	typedef fun::internal::x2x_base_< metal::vec<n,C>, translation_alt<n,C> > super_;

// 	typedef translation_alt<n,C> invert;
// 	invert inv() const;

	translation_alt();
	translation_alt(const metal::vec<n,C>& t);

	using super_::operator();
	metal::vec<n,C> operator()(const metal::vec<n,C>& v) const;

	void set_t(const metal::vec<n,C>& t);

      protected:
	metal::vec<n,C> t_;
      };


# ifndef MLN_INCLUDE_ONLY

      template <unsigned n, typename C>
      translation_alt<n,C>::translation_alt()
      {
      }

      template <unsigned n, typename C>
      translation_alt<n,C>::translation_alt(const metal::vec<n,C>& t)
	:
	t_(t)
      {
	this->m_ = matrix::Id;
	for (unsigned i = 0; i < n; ++i)
	  this->m_(i,n) = t_[i];
      }

      template <unsigned n, typename C>
      metal::vec<n,C>
      translation_alt<n,C>::operator()(const metal::vec<n,C>& v) const
      {
	return v + t;
      }

      template <unsigned n, typename C>
      void
      translation_alt<n,C>::set_t(const metal::vec<n,C>& t)
      {
	t_ = t;
	for (unsigned i = 0; i < n; ++i)
	  this->m_(i, n) = t_[i];
      }

//       template <unsigned n, typename C>
//       translation_alt<n,C>
//       translation_alt<n,C>::inv() const
//       {
// 	typename translation_alt::invert res(-t_);
// 	return res;
//       }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::fun::x2x

  } // end of namespace mln::fun

} // end of namespace mln


#endif // ! MLN_FUN_X2X_TRANSLATION_ALT_HH
