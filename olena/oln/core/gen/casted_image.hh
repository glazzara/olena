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

#ifndef OLN_CORE_GEN_CASTED_IMAGE_HH
# define OLN_CORE_GEN_CASTED_IMAGE_HH

# include <oln/core/gen/through.hh>
# include <oln/core/gen/pw_value.hh>
# include <oln/core/internal/f_ch_value.hh>


namespace oln
{


# define current casted_image<I, V>
# define super   internal::value_morpher_< current >


  // Fwd decl.
  template <typename I, typename V> class casted_image;


  // Super type.
  template <typename I, typename V>
  struct super_trait_< current >
  {
    typedef super ret;
  };


  // Virtual types.
  template <typename I, typename V>
  struct vtypes< current >
  {
    typedef I delegatee;
    typedef internal::singleton<const I> data;

    typedef V  value;
    typedef V rvalue;

    typedef oln_plain_value(I, V) plain;
    typedef casted_image<pl::rec<I>, V> skeleton;
  };


  /// casted_image<I, V>

  template <typename I, typename V>
  class casted_image : public super
  {
  public:
    stc_using(data);
    stc_using(psite);

    casted_image();
    casted_image(const Image<I>& ima);

    const I& impl_image() const;

    V impl_read(const psite& p) const;
  };


  // value_cast

  template <typename V, typename I>
  casted_image<I, V> value_cast(const Image<I>& ima);


# ifndef OLN_INCLUDE_ONLY

  template <typename I, typename V>
  current::casted_image()
  {
  }

  template <typename I, typename V>
  current::casted_image(const Image<I>& ima)
  {
    precondition(exact(ima).has_data());
    this->data_ = new data(exact(ima));
  }

  template <typename I, typename V>
  const I&
  current::impl_image() const
  {
    assert(this->has_data());
    return this->data_->value;
  }

  template <typename I, typename V>
  V
  current::impl_read(const typename current::psite& p) const
  {
    assert(this->has_data());
    assert(this->image().has_data());
    return this->data_->value(p);
  }

  // value_cast

  template <typename V, typename I>
  current value_cast(const Image<I>& ima)
  {
    current tmp(ima);
    return tmp;
  }

# endif // ! OLN_INCLUDE_ONLY

# undef super
# undef current

} // end of namespace oln


# include <oln/core/gen/value_cast.hh>


#endif // ! OLN_CORE_GEN_CASTED_IMAGE_HH
