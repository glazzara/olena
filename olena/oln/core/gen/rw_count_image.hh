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

#ifndef OLN_CORE_GEN_RW_COUNT_IMAGE_HH
# define OLN_CORE_GEN_RW_COUNT_IMAGE_HH

# include <oln/core/internal/image_base.hh>
# include <oln/core/internal/value_proxy.hh>
# include <oln/core/gen/inplace.hh>


namespace oln
{

# define current rw_count_image<I>


  // Fwd decl.
  template <typename I> class rw_count_image;


  /// Virtual types.
  template <typename I>
  struct vtypes< current >
  {
    typedef I delegatee;
    typedef behavior::identity behavior;

    typedef typename mlc::if_< stc_is_found_type(I, lvalue),
			       internal::value_proxy_< current >,
			       stc::not_delegated >::ret lvalue;

    typedef internal::triplet<I, unsigned, unsigned> data;

    typedef rw_count_image< oln_plain(I) > plain;
    typedef rw_count_image< pl::rec<I> >   skeleton;
  };


  /// Super type.
  template <typename I>
  struct super_trait_< current >
  {
    typedef internal::single_image_morpher_< current > ret;
  };


  /// Class to count the number of read and write in images.

  template <typename I>
  class rw_count_image : public internal::single_image_morpher_< current >
  {
    typedef internal::single_image_morpher_< current > super;
  public:

    stc_using(point);
    stc_using(rvalue);
    stc_using(lvalue);
    stc_using(value);
    stc_using(data);
    stc_using(delegatee);

    rw_count_image();
    rw_count_image(Mutable_Image<I>& ima);

    rvalue impl_read(const point& p) const;
    lvalue impl_read_write(const point& p);
    void impl_write(const point& p, const value& v);

    delegatee& impl_image();
    const delegatee& impl_image() const;

    unsigned nreads() const;
    unsigned nwrites() const;

  }; // end of rw_count_image<I>


  template <typename I, typename D>
  bool prepare(rw_count_image<I>& target, with_t, const D& dat);


  template <typename I>
  current rw_count(Mutable_Image<I>& ima);

  template <typename I>
  inplace_< current > rw_count(inplace_<I> ima);



# ifndef OLN_INCLUDE_ONLY

  template <typename I>
  current::rw_count_image()
  {
  }

  template <typename I>
  current::rw_count_image(Mutable_Image<I>& ima)
  {
    precondition(exact(ima).has_data());
    unsigned nreads = 0, nwrites = 0;
    this->data_ = new data(exact(ima), nreads, nwrites);
  }

  template <typename I>
  typename current::rvalue
  current::impl_read(const typename current::point& p) const
  {
    assert(this->has_data());
    assert(this->image().has_data());
    const_cast<unsigned&>(this->data_->second) += 1;
    return this->data_->first(p);
  }

  template <typename I>
  typename current::lvalue
  current::impl_read_write(const typename current::point& p)
  {
    assert(this->has_data());
    assert(this->image().has_data());
    lvalue tmp(*this, p);
    return tmp;
  }

  template <typename I>
  void
  current::impl_write(const typename current::point& p,
		      const typename current::value& v)
  {
    assert(this->has_data());
    assert(this->image().has_data());
    this->data_->third += 1;
    this->image().write_(p, v);
  }

  template <typename I>
  typename current::delegatee&
  current::impl_image()
  {
    assert(this->has_data());
    return this->data_->first;
  }

  template <typename I>
  const typename current::delegatee&
  current::impl_image() const
  {
    assert(this->has_data());
    return this->data_->first;
  }

  template <typename I>
  unsigned
  current::nreads() const
  {
    assert(this->has_data());
    return this->data_->second;
  }

  template <typename I>
  unsigned
  current::nwrites() const
  {
    assert(this->has_data());
    return this->data_->third;
  }

  template <typename I>
  current rw_count(Mutable_Image<I>& ima)
  {
    current tmp(ima);
    return tmp;
  }

  template <typename I>
  inplace_< current > rw_count(inplace_<I> ima)
  {
    current tmp_ = rw_count(ima.unwrap());
    inplace_< current > tmp(tmp_);
    return tmp;
  }

  template <typename I, typename D>
  bool prepare(rw_count_image<I>& target, with_t, const D& dat)
  {
    precondition(not target.has_data());
    target.data__() = new typename rw_count_image<I>::data;
    bool ima_ok = prepare(target.data__()->first, with, dat);
    target.data__()->second = 0;
    target.data__()->third = 0;
    postcondition(ima_ok);
    return ima_ok;
  }

# endif // ! OLN_INCLUDE_ONLY

# undef current

} // end of namespace oln


#endif // ! OLN_CORE_GEN_RW_COUNT_IMAGE_HH
