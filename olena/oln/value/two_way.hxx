// Copyright (C) 2006 EPITA Research and Development Laboratory
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

#ifndef OLN_VALUE_TWO_WAY_HXX
# define OLN_VALUE_TWO_WAY_HXX

# include <oln/value/two_way.hh>


# ifndef OLN_INCLUDE_ONLY

namespace oln
{

  namespace value
  {

    // Ctor.
    template <typename I, typename F>
    two_way<I,F>::two_way(I& ima,
			  F fun,
			  const oln_psite(I)& p)
      : ima_(&ima),
	fun_(fun),
	p_(p)
    {
    }

    // Ctor.
    template <typename I, typename F>
    two_way<I,F>::two_way(I& ima,
			  const oln_psite(I)& p)
      : ima_(&ima),
	fun_(),
	p_(p)
    {
    }

    // Read.
    template <typename I, typename F>
    template <typename V>
    two_way<I,F>::operator V() const
    {
      return read_(fun_);
    }

    // Explicit read.
    template <typename I, typename F>
    typename F::result_type
    two_way<I,F>::value() const
    {
      return read_(fun_);
    }
    
    // Write.

    template <typename I, typename F>
    template <typename V>
    two_way<I,F>&
    two_way<I,F>::operator=(const V& value)
    {
      write_(fun_, value);
      return *this;
    }

    // fun_v2w2v

    template <typename I, typename F>
    template <typename E, typename V>
    void
    two_way<I,F>::write_(const oln::abstract::fun_v2w2v<E>& f, const V& value)
    {
      (*ima_)(p_) = f.exact().reverse(value);
    }

    template <typename I, typename F>
    template <typename E>
    typename E::result_type
    two_way<I,F>::read_(const oln::abstract::fun_v2w2v<E>& f) const
    {
      return f.exact().direct((*ima_)(p_));
    }

    // fun_rw

    template <typename I, typename F>
    template <typename E, typename V>
    void
    two_way<I,F>::write_(const oln::abstract::fun_rw<E>& f, const V& value)
    {
      f.exact().write(*ima_, p_, value);
    }

    template <typename I, typename F>
    template <typename E>
    typename E::result_type
    two_way<I,F>::read_(const oln::abstract::fun_rw<E>& f) const
    {
      return f.exact().read(*ima_, p_);
    }


    // Op <<.
    template <typename I, typename F>
    std::ostream& operator<<(std::ostream& ostr,
			     const two_way<I,F>& proxy)
    {
      return ostr << proxy.value();
    }

  } // end of namespace oln::value


  namespace abstract
  {

    template <typename F>
    template <typename I>
    oln::morpher::two_way<I, F>
    fun_v2w2v<F>::operator()(oln::abstract::mutable_image<I>& input) const
    {
      morpher::two_way<I, F> tmp(input.exact(), this->exact());
      return tmp;
    }

    template <typename F>
    template <typename I>
    oln::morpher::two_way_rw<I, F>
    fun_rw<F>::operator()(oln::abstract::mutable_image<I>& input) const
    {
      morpher::two_way_rw<I, F> tmp(input, *this);
      return tmp;
    }

  } // end of namespace oln::abstract


} // end of namespace oln


# endif // ! OLN_INCLUDE_ONLY


#endif // ! OLN_VALUE_TWO_WAY_HXX
