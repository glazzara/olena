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

#ifndef OLN_CORE_GEN_INPLACE_HH
# define OLN_CORE_GEN_INPLACE_HH

# include <oln/core/concept/image.hh>


namespace oln
{


  template <typename I>
  class inplace_ : private mlc::assert_< mlc_is_a(I, Mutable_Image) >
  {
    typedef inplace_<I> current;
  public:

    inplace_(I& ima);

    I& unwrap() const;  // explicit
    operator I() const; // implicit

  private:
    I ima_;
  };



  template <typename I>
  inplace_<I>
  inplace(Mutable_Image<I>& ima);


  template <typename I>
  inplace_<I>
  inplace(inplace_<I> ima);



# ifndef OLN_INCLUDE_ONLY

  template <typename I>
  inplace_<I>::inplace_(I& ima)
    : ima_(ima)
  {
  }

  template <typename I>
  I&
  inplace_<I>::unwrap() const
  {
    current* this_ = const_cast<current*>(this);
    return this_->ima_;
  }

  template <typename I>
  inplace_<I>::operator I() const
  {
    return this->unwrap();
  }

  // inplace

  template <typename I>
  inplace_<I>
  inplace(Mutable_Image<I>& ima)
  {
    inplace_<I> tmp(exact(ima));
    return tmp;
  }

  template <typename I>
  inplace_<I>
  inplace(inplace_<I> ima)
  {
    return ima;
  }

# endif // ! OLN_INCLUDE_ONLY

} // end of namespace oln


#endif // ! OLN_CORE_GEN_INPLACE_HH
