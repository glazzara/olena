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
# include <mln/metal/vec.hh>
# include <mln/metal/mat.hh>


namespace mln
{

  namespace fun
  {

    namespace x2x
    {

	// Fwd decl.
      template <typename L, typename M>
      struct composed;

      namespace internal
      {

	template <unsigned n, typename L, unsigned m, typename M, typename E>
	struct helper_;

	template <unsigned n, typename L, typename M, typename E>
	struct helper_<n, Function_x2x<L>, n, Function_x2x<M> > : Function_x2x<E>
	{
	    enum {dim = n};
	};

	template <unsigned n, typename L, typename M, typename E>
	struct helper_<n, bijective_tr<L>, n, bijective_tr<M> > : bijective_tr<E>
	{
	    enum {dim = n};
	    typedef composed<M::invert,L::invert> invert;

	    invert inv() const;
	};
      }

      // FIXME: Doc!

      template <typename L, typename M>
      struct composed : public internal::helper_< L::dim, L, M::dim, M, composed<L,M> >
      {

	typedef internal::helper_< L::dim, L, M::dim, M, composed<L,M> > Super

	enum {dim = Super::dim};

	typedef metal::vec<n,C> result;

	composed();
	composed(const L& tr_l, const M& tr_m);

	result operator()(const metal::vec<n,C>& v) const;

	void set_first(const L& tr_l);
	void set_second(const M& tr_m);

      protected:

	L tr1_;
	M tr2_;
	metal::mat<n + 1,n + 1,C> m_;
      };


# ifndef MLN_INCLUDE_ONLY

      template <typename L, typename M>
      composed<L,M>::composed()
      {
	t_ = make::vec<n,C>(0);
	m_ = metal::mat<n+1,n+1,C>::Id;
      }

      template <typename L, typename M>
      composed<L,M>::composed(const L& tr_l, const M& tr_m)
	:tr1_(tr_l),
	 tr2_(tr_m)
      {
	m_ = metal::mat<n+1,n+1,C>::Id;
	m_ = tr1_ * tr2_;
      }

      template <typename L, typename M>
      composed<L,M>::result
      composed<L,M>::operator()(const metal::vec<n,C>& v) const
      {
	return m_(v);
      }

      template <typename L, typename M>
      composed<L,M>::invert
      composed<L,M>::inv() const
      {
	typename composed::invert res(tr2_.inv(), tr1_.inv());

	return res;
      }

      template <typename L, typename M>
      void 
      composed<L,M>::set_first(const L& tr_l)
      {
	tr1_ = tr_l;
      }

      template <typename L, typename M>
      void 
      composed<L,M>::set_second(const M& tr_m)
      {
	tr2_ = tr_m;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::fun::x2x

  } // end of namespace mln::fun

} // end of namespace mln


#endif // ! MLN_FUN_X2X_COMPOSED_HH
