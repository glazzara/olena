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

#ifndef MLN_FUN_X2X_TRANSLATION_HH
# define MLN_FUN_X2X_TRANSLATION_HH

/*! \file mln/fun/x2x/translation.hh
 *
 * \brief Define a translation function.
 */

# include <mln/core/concept/function.hh>
# include <mln/fun/internal/x2x_linear_impl.hh>
# include <mln/metal/vec.hh>
# include <mln/core/h_mat.hh>
# include <mln/fun/i2v/all.hh>

namespace mln
{

  namespace fun
  {

    namespace x2x
    {

      /*! \brief Represent a translation function.
       *
       */
      template <unsigned n, typename C>
      struct translation

	: internal::x2x_linear_impl_< metal::vec<n,C>, translation<n,C> >
	,
	  public Bijection_x2x< translation<n,C> >
      {
	typedef fun::internal::x2x_linear_impl_< metal::vec<n,C>, translation<n,C> > super_;

	/// Type of the inverse function.
	typedef translation<n,C> invert;
	/// Return the inverse function.
	invert inv() const;

	/// Constructor without argument.
	translation();
	/// Constructor with the translation vector.
	translation(const metal::vec<n,C>& t);

	using super_::operator();
	/// Perform the translation of the given vector
	metal::vec<n,C> operator()(const metal::vec<n,C>& v) const;

	/// Set a net translation vector.
	void set_t(const metal::vec<n,C>& t);

      protected:
	void update();

	metal::vec<n,C> t_;
      };


# ifndef MLN_INCLUDE_ONLY

      template <unsigned n, typename C>
      inline
      translation<n,C>::translation()
      {
      }

      template <unsigned n, typename C>
      inline
      translation<n,C>::translation(const metal::vec<n,C>& t)
	:t_(t)
      {
	this->m_ = h_mat<n,C>::Id;
	this->update();
      }

      template <unsigned n, typename C>
      inline
      metal::vec<n,C>
      translation<n,C>::operator()(const metal::vec<n,C>& v) const
      {
	return v + t_;
      }

      template <unsigned n, typename C>
      inline
      translation<n,C>
      translation<n,C>::inv() const
      {
	typename translation::invert res(-t_);

	return res;
      }

      template <unsigned n, typename C>
      inline
      void
      translation<n,C>::set_t(const metal::vec<n,C>& t)
      {
	this->t_ = t;
	this->update();
      }

      template <unsigned n, typename C>
      inline
      void
      translation<n,C>::update()
      {
	for (unsigned i = 0; i < n; ++i)
	  this->m_(i,n) = this->t_[i];
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::fun::x2x

  } // end of namespace mln::fun

} // end of namespace mln


#endif // ! MLN_FUN_X2X_TRANSLATION_HH
