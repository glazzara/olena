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

#ifndef MLN_CORE_SAFE_IMAGE_HH
# define MLN_CORE_SAFE_IMAGE_HH

# include <mln/core/internal/image_adaptor.hh>


namespace mln
{


  template <typename I>
  struct safe_image : public internal::image_adaptor_< I, safe_image<I> >
  {
    typedef internal::image_adaptor_< I, safe_image<I> > super;

    safe_image(Image<I>& ima);

    mln_rvalue(I) operator()(const mln_psite(I)& p) const;
    mln_lvalue(I) operator()(const mln_psite(I)& p);

    template <typename U>
    struct change_value
    {
      typedef safe_image<mln_ch_value(I, U)> ret;
    };
  };



  template <typename I>
  safe_image<I> safe(Image<I>& ima);



# ifndef MLN_INCLUDE_ONLY


  template <typename I>
  safe_image<I>::safe_image(Image<I>& ima)
    : super(ima)
  {
  }

  template <typename I>
  mln_rvalue(I)
  safe_image<I>::operator()(const mln_psite(I)& p) const
  {
    static mln_value(I) tmp;
    if (! this->owns_(p))
      return tmp;
    return this->adaptee_(p);
  }

  template <typename I>
  mln_lvalue(I)
  safe_image<I>::operator()(const mln_psite(I)& p)
  {
    static mln_value(I) tmp;
    if (! this->owns_(p))
      return tmp;
    return this->adaptee_(p);
  }

  template <typename I>
  safe_image<I> safe(Image<I>& ima)
  {
    safe_image<I> tmp(ima);
    return tmp;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_SAFE_IMAGE_HH
