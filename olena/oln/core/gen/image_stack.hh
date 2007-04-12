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

#ifndef OLN_CORE_GEN_IMAGE_STACK_HH
# define OLN_CORE_GEN_IMAGE_STACK_HH

# include <oln/core/internal/image_base.hh>
# include <oln/core/internal/value_proxy.hh>
# include <oln/core/internal/f_ch_value.hh>

# include <mlc/int.hh>
# include <xtd/vec.hh>


namespace oln
{

# define current image_stack<n_, I>

# define super   internal::multiple_image_morpher_< current >


  // Fwd decl.
  template <unsigned n_, typename I> class image_stack;


  // Super type.
  template <unsigned n_, typename I>
  struct super_trait_< current >
  {
    typedef super ret;
  };


  /// Virtual types.
  template <unsigned n_, typename I>
  struct vtypes< current >
  {
    typedef I delegatee;
    typedef behavior::identity behavior;

    typedef internal::singleton< xtd::vec<n_,I> > data;

    typedef mlc::uint_<n_> n;

    typedef xtd::vec<n_, oln_value(I)> value;
    typedef value rvalue;
    
    typedef typename mlc::if_< stc_is_found_type(I, lvalue),
			       internal::value_proxy_< current >,
			       stc::not_delegated >::ret lvalue;

    typedef oln_plain_value(I, value)    plain;
    typedef image_stack< n_, pl::rec<I> > skeleton;
  };


  /// Class for a stack of n images.

  template <unsigned n_, typename I>
  class image_stack : public super
  {
  public:

    stc_using(psite);
    stc_using(rvalue);
    stc_using(lvalue);
    stc_using(value);
    stc_using(data);
    stc_using(delegatee);

    image_stack();
    image_stack(const xtd::vec<n_,I>& imas);

    // FIXME: Force this type to be read-only?
    rvalue impl_read(const psite& p) const;
    lvalue impl_read_write(const psite& p);
    void impl_write(const psite& p, const value& v);

    delegatee& impl_image(unsigned i);
    const delegatee& impl_image(unsigned i) const;

  }; // end of current



  template <typename I>
  image_stack<2,I> stack(I& ima_0, I& ima_1);

  template <typename I>
  image_stack<3,I> stack(I& ima_0, I& ima_1, I& ima_2);


# ifndef OLN_INCLUDE_ONLY

  template <unsigned n_, typename I>
  current::image_stack()
  {
  }

  template <unsigned n_, typename I>
  current::image_stack(const xtd::vec<n_,I>& imas)
  {
    for (unsigned i = 0; i < n_; ++i)
      precondition(imas[i].has_data());
    this->data_ = new data(imas);
  }

  template <unsigned n_, typename I>
  typename current::rvalue
  current::impl_read(const typename current::psite& p) const
  {
    assert(this->has_data());
    rvalue tmp;
    for (unsigned i = 0; i < n_; ++i)
      tmp[i] = this->data_->value[i](p);
    return tmp;
  }

  template <unsigned n_, typename I>
  typename current::lvalue
  current::impl_read_write(const typename current::psite& p)
  {
    assert(this->has_data());
    lvalue tmp(*this, p);
    return tmp;
  }

  template <unsigned n_, typename I>
  void
  current::impl_write(const typename current::psite& p,
		      const typename current::value& v)
  {
    assert(this->has_data());
    for (unsigned i = 0; i < n_; ++i)
      this->data_->value[i](p) = v[i];
  }

  template <unsigned n_, typename I>
  typename current::delegatee&
  current::impl_image(unsigned i)
  {
    precondition(i < n_);
    assert(this->has_data());
    return this->data_->value[i];
  }

  template <unsigned n_, typename I>
  const typename current::delegatee&
  current::impl_image(unsigned i) const
  {
    precondition(i < n_);
    assert(this->has_data());
    return this->data_->value[i];
  }

  template <typename I>
  image_stack<2,I> stack(I& ima_0, I& ima_1)
  {
    xtd::vec<2,I> v;
    v[0] = ima_0;
    v[1] = ima_1;
    image_stack<2,I> tmp(v);
    return tmp;
  }

  template <typename I>
  image_stack<3,I> stack(I& ima_0, I& ima_1, I& ima_2)
  {
    xtd::vec<3,I> v;
    v[0] = ima_0;
    v[1] = ima_1;
    v[2] = ima_2;
    image_stack<3,I> tmp(v);
    return tmp;
  }


# endif // ! OLN_INCLUDE_ONLY

# undef super
# undef current

} // end of namespace oln


#endif // ! OLN_CORE_GEN_IMAGE_STACK_HH
