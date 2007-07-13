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

#ifndef MLN_CORE_PIXEL_HH
# define MLN_CORE_PIXEL_HH

/*! \file mln/core/pixel.hh
 *
 * \brief Definition of the generic pixel class mln::pixel.
 */

# include <mln/core/concept/genpixel.hh>


namespace mln
{


  template <typename I>
  struct pixel_lvalue
  {
    typedef mln_lvalue(I) ret;
  };

  template <typename I>
  struct pixel_lvalue< const I >
  {
    typedef mln_rvalue(I) ret;
  };



  /*! \brief Generic pixel class.
   *
   * The parameter is \c I the type of the image it belongs to.
   */
  template <typename I>
  struct pixel : public Object< pixel<I> >,
		 public GenPixel< pixel<I> >
  {
    typedef mln_psite(I) psite;
    typedef mln_value(I) value;

    pixel(I& ima);
    pixel(I& ima, const psite& p);

    const I& image() const;

    const psite& site() const;
    psite& site();

    mln_rvalue(I) operator*() const;
    typename pixel_lvalue<I>::ret operator*();

    const value* address() const;
    value* address();

  protected:

    I& ima_;
    psite p_;
  };


# ifndef MLN_INCLUDE_ONLY

  template <typename I>
  pixel<I>::pixel(I& image)
    : ima_(image)
  {
  }

  template <typename I>
  pixel<I>::pixel(I& image, const psite& p)
    : ima_(image),
      p_(p)
  {
  }

  template <typename I>
  const I&
  pixel<I>::image() const
  {
    return ima_;
  }

  template <typename I>
  const mln_psite(I)&
  pixel<I>::site() const
  {
    return p_;
  }

  template <typename I>
  mln_psite(I)&
  pixel<I>::site()
  {
    return p_;
  }

  template <typename I>
  mln_rvalue(I)
  pixel<I>::operator*() const
  {
    return ima_(p_);
  }

  template <typename I>
  typename pixel_lvalue<I>::ret
  pixel<I>::operator*()
  {
    return ima_(p_);
  }

  template <typename I>
  const mln_value(I)*
  pixel<I>::address() const
  {
    return & ima_(p_);
  }

  template <typename I>
  mln_value(I)*
  pixel<I>::address()
  {
    return & ima_(p_);
  }

# endif // ! MLN_INCLUDE_ONLY
  
} // end of namespace mln


#endif // ! MLN_CORE_PIXEL_HH
