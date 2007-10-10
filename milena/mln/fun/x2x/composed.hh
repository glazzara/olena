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

#ifndef MLN_FUN_X2X_COMPOSED_HH
# define MLN_FUN_X2X_COMPOSED_HH

/*! \file mln/fun/x2x/composed.hh
 *
 * \brief FIXME.
 */

# include <mln/core/concept/function.hh>
# include <mln/fun/internal/x2x_impl.hh>
# include <mln/metal/vec.hh>
# include <mln/core/h_mat.hh>


namespace mln
{

  namespace fun
  {

    namespace x2x
    {

      // Fwd decl.
      template <typename F, typename G>
      struct composed;

      namespace internal
      {

	template <typename F, typename G, typename E>
	struct helper_;

	template <typename F, typename G, typename E>
	struct helper_<Function_x2x<F>, Function_x2x<G>, E>
	  : Function_x2x<E>
	{
	};

	template <typename F, typename G, typename E>
	struct helper_<Bijection_x2x<F>, Bijection_x2x<G>, E >
	  : Bijection_x2x<E>
	{
	    typedef composed<G::invert,F::invert> invert;

	    invert inv() const;
	};
      }

      // FIXME: Doc!

      template <typename F, typename G>
      struct composed
	: internal::x2x_impl_<F::result, composed<F,G> >,
	  public internal::helper_< F, G, composed<F,G> >,
	  private typename metal::bool<(F::dim == G::dim)>::check_t,
	  private typename metal::is<F::argument, G::result>::check_t
      {

	typedef internal::x2x_impl_<F::result, composed<F,G> > super_

	composed();
	composed(const F& f, const G& g);

	using super_:operator();
	metal::vec<super_::dim,C> operator()(const metal::vec<super_::dim,C>& v) const;

	void set_first(const F& f);
	void set_second(const G& g);

      protected:

	F f_;
	G g_;
      };


# ifndef MLN_INCLUDE_ONLY

      template <typename F, typename G>
      composed<F,G>::composed()
      {
	m_ = h_mat<n,C>::Id;
      }

      template <typename F, typename G>
      composed<F,G>::composed(const F& f, const G& g)
	:f_(f),
	 g_(g)
      {
	m_ = f_.mat() * g_.mat();
      }

      template <typename F, typename G>
      metal::vec<super_::dim,C>
      composed<F,G>::operator()(const metal::vec<super_::dim,C>& v) const
      {
	return m_(v);
      }

      template <typename F, typename G>
      composed<F,G>::invert
      composed<F,G>::inv() const
      {
	typename composed::invert res(tr2_.inv(), tr1_.inv());

	return res;
      }

      template <typename F, typename G>
      void 
      composed<F,G>::set_first(const F& f)
      {
	f_ = f;
	m_ = f_.mat() * g_.mat();	
      }

      template <typename F, typename G>
      void 
      composed<F,G>::set_second(const G& g)
      {
	g_ = g;
	m_ = f_.mat() * g_.mat();
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::fun::x2x

  } // end of namespace mln::fun

} // end of namespace mln


#endif // ! MLN_FUN_X2X_COMPOSED_HH
