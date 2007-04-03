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

#ifndef OLN_CORE_GEN_SAFE_IMAGE_HH
# define OLN_CORE_GEN_SAFE_IMAGE_HH

# include <oln/core/internal/image_base.hh>
# include <oln/core/gen/infty_pset.hh>
# include <oln/core/gen/inplace.hh>


namespace oln
{


  // Fwd decl.
  template <typename I> class safe_image;


  /// Virtual types.
  template <typename I>
  struct vtypes< safe_image<I> >
  {
    typedef I delegatee;
    typedef behavior::identity behavior;

    typedef internal::singleton<I> data;
    typedef infty_pset< oln_grid(I) > pset;

    typedef safe_image< oln_plain(I) > plain;
    typedef safe_image< pl::rec<I> >   skeleton;
  };


  /// Super type.
  template <typename I>
  struct super_trait_< safe_image<I> >
  {
    typedef safe_image<I> current;
    typedef internal::single_image_morpher_<current> ret;
  };


  /// Class for images defined by a point set and a single value.

  template <typename I>
  class safe_image : public internal::single_image_morpher_< safe_image<I> >
  {
    typedef safe_image<I> current;
    typedef internal::single_image_morpher_<current> super;
  public:

    stc_using(point);
    stc_using(rvalue);
    stc_using(lvalue);
    stc_using(value);
    stc_using(data);
    stc_using(pset);
    stc_using(delegatee);

    safe_image();
    safe_image(Mutable_Image<I>& ima);

    bool impl_owns_(const point& p) const;
    const pset& impl_points() const;

    // FIXME: Force this type to be read-only?
    rvalue impl_read(const point& p) const;
    lvalue impl_read_write(const point& p);
    void impl_write(const point& p, const value& v);

    delegatee& impl_image();
    const delegatee& impl_image() const;

  }; // end of safe_image<I>



  template <typename I>
  safe_image<I> safe(Mutable_Image<I>& ima);

  template <typename I>
  inplace_< safe_image<I> > safe(inplace_<I> ima);



# ifndef OLN_INCLUDE_ONLY

  template <typename I>
  safe_image<I>::safe_image()
  {
  }

  template <typename I>
  safe_image<I>::safe_image(Mutable_Image<I>& ima)
  {
    precondition(exact(ima).has_data());
    this->data_ = new data(exact(ima));
  }

  template <typename I>
  bool
  safe_image<I>::impl_owns_(const typename safe_image<I>::point&) const
  {
    assert(this->has_data());
    return true; // always!
  }

  template <typename I>
  const typename safe_image<I>::pset&
  safe_image<I>::impl_points() const
  {
    static infty_pset<oln_grid(I)> infty_;
    return infty_;
  }

  template <typename I>
  typename safe_image<I>::rvalue
  safe_image<I>::impl_read(const typename safe_image<I>::point& p) const
  {
    assert(this->has_data());
    assert(this->image().has_data());
    static oln_value(I) tmp;
    if (this->image().owns_(p))
      return this->image()(p);
    else
      return tmp;
  }

  template <typename I>
  typename safe_image<I>::lvalue
  safe_image<I>::impl_read_write(const typename safe_image<I>::point& p)
  {
    assert(this->has_data());
    assert(this->image().has_data());
    static oln_value(I) tmp;
    if (this->image().owns_(p))
      return this->image()(p);
    else
      return tmp;
  }

  template <typename I>
  void
  safe_image<I>::impl_write(const typename safe_image<I>::point& p,
			    const typename safe_image<I>::value& v)
  {
    assert(this->has_data());
    assert(this->image().has_data());
    static oln_value(I) tmp;
    if (this->image().owns_(p))
      this->image().write_(p, v);
  }

  template <typename I>
  typename safe_image<I>::delegatee&
  safe_image<I>::impl_image()
  {
    assert(this->has_data());
    return this->data_->value;
  }

  template <typename I>
  const typename safe_image<I>::delegatee&
  safe_image<I>::impl_image() const
  {
    assert(this->has_data());
    return this->data_->value;
  }

  template <typename I>
  safe_image<I> safe(Mutable_Image<I>& ima)
  {
    safe_image<I> tmp(ima);
    return tmp;
  }

  template <typename I>
  inplace_< safe_image<I> > safe(inplace_<I> ima)
  {
    safe_image<I> tmp_ = safe(ima.unwrap());
    inplace_< safe_image<I> > tmp(tmp_);
    return tmp;
  }

# endif // ! OLN_INCLUDE_ONLY

} // end of namespace oln


#endif // ! OLN_CORE_GEN_SAFE_IMAGE_HH
