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

#ifndef OLN_CORE_GEN_TORUS_IMAGE_HH
# define OLN_CORE_GEN_TORUS_IMAGE_HH

# include <oln/core/internal/image_base.hh>
# include <oln/core/gen/torus_pset.hh>
# include <oln/core/gen/inplace.hh>



namespace oln
{

  // Fwd decl.
  template <typename I> class torus_image;


  /// Virtual types.
  template <typename I>
  struct vtypes< torus_image<I> >
  {
    typedef I delegatee;
    typedef behavior::identity behavior;

    typedef torus_pset<oln_pset(I)> pset;
    typedef internal::pair<I, pset> data;

    typedef oln_possible_lvalue(I) lvalue;

    typedef torus_image< oln_plain(I) > plain;
    typedef torus_image< pl::rec<I> >   skeleton;
  };


  /// Super type.
  template <typename I>
  struct super_trait_< torus_image<I> >
  {
    typedef torus_image<I> current__;
    typedef internal::single_image_morpher_<current__> ret;
  };


  /// Class for images defined by a point set and a single value.

  template <typename I>
  class torus_image : public internal::single_image_morpher_< torus_image<I> >
  {
    typedef torus_image<I> current;
    typedef internal::single_image_morpher_<current> super;
  public:

    stc_using(point);
    stc_using(rvalue);
    typedef stc_find_type(I, lvalue) lvalue; // FIXME: macro; propagate in similar classes.
    stc_using(value);
    stc_using(data);
    stc_using(pset);
    stc_using(delegatee);

    torus_image();
    torus_image(I& ima);

    const pset& impl_points() const;
    point relocate(const point& p) const;

    bool impl_owns_(const point& p) const;

    rvalue impl_read(const point& p) const;
    lvalue impl_read_write(const point& p);
    void impl_write(const point& p, const value& v);

    delegatee& impl_image();
    const delegatee& impl_image() const;

  }; // end of torus_image<I>



  template <typename I, typename D>
  bool prepare(torus_image<I>& target, with_t, const D& dat);

  template <typename I>
  bool init_(Image<I>* this_,
             const torus_image<I>& data);

  template <typename Ps, typename I>
  bool init_(torus_pset<Ps>* this_,
             const torus_image<I>& data);



  template <typename I>
  torus_image<const I> torus(const Image<I>& ima);

  template <typename I>
  torus_image<I> torus(Mutable_Image<I>& ima);

  template <typename I>
  inplace_< torus_image<I> > torus(inplace_<I> ima);



# ifndef OLN_INCLUDE_ONLY

  template <typename I>
  torus_image<I>::torus_image()
  {
  }

  template <typename I>
  torus_image<I>::torus_image(I& ima)
  {
    precondition(exact(ima).has_data());
    this->data_ = new data(ima, pset(ima.points()));
  }

  template <typename I>
  const typename torus_image<I>::pset&
  torus_image<I>::impl_points() const
  {
    assert(this->has_data());
    return this->data_->second;
  }

  template <typename I>
  typename torus_image<I>::point
  torus_image<I>::relocate(const typename torus_image<I>::point& p) const
  {
    assert(this->has_data());
    return this->data_->second.relocate(p);
  }

  template <typename I>
  bool
  torus_image<I>::impl_owns_(const typename torus_image<I>::point& p) const
  {
    return this->image().owns_(this->relocate(p));
  }

  template <typename I>
  typename torus_image<I>::rvalue
  torus_image<I>::impl_read(const typename torus_image<I>::point& p) const
  {
    assert(this->has_data());
    assert(this->image().has_data());
    return this->image()(this->relocate(p));
  }

  template <typename I>
  typename torus_image<I>::lvalue
  torus_image<I>::impl_read_write(const typename torus_image<I>::point& p)
  {
    assert(this->image().has_data());
    return this->image()(this->relocate(p));
  }

  template <typename I>
  void
  torus_image<I>::impl_write(const typename torus_image<I>::point& p,
			    const typename torus_image<I>::value& v)
  {
    assert(this->image().has_data());
    this->image().write_(this->relocate(p), v);
  }

  template <typename I>
  typename torus_image<I>::delegatee&
  torus_image<I>::impl_image()
  {
    assert(this->has_data());
    return this->data_->first;
  }

  template <typename I>
  const typename torus_image<I>::delegatee&
  torus_image<I>::impl_image() const
  {
    assert(this->has_data());
    return this->data_->first;
  }


  template <typename I>
  torus_image<const I> torus(const Image<I>& ima)
  {
    torus_image<const I> tmp(exact(ima));
    return tmp;
  }

  template <typename I>
  torus_image<I> torus(Mutable_Image<I>& ima)
  {
    torus_image<I> tmp(exact(ima));
    return tmp;
  }

  template <typename I>
  inplace_< torus_image<I> > torus(inplace_<I> ima)
  {
    torus_image<I> tmp_ = torus(ima.unwrap());
    inplace_< torus_image<I> > tmp(tmp_);
    return tmp;
  }


  template <typename I, typename D>
  bool prepare(torus_image<I>& target, with_t, const D& dat)
  {
    precondition(not target.has_data());

    target.data__() = new typename torus_image<I>::data;
    bool ima_ok = prepare(target.data__()->first, with, dat);
    bool pset_ok = init(target.data__()->second, with, dat);
    postcondition(ima_ok);
    postcondition(pset_ok);
    return ima_ok and pset_ok;
  }

  template <typename I>
  bool init_(Image<I>* this_,
             const torus_image<I>& data)
  {
    exact(*this_) = data.image();
    return true;
  }

  template <typename Ps, typename I>
  bool init_(torus_pset<Ps>* this_,
             const torus_image<I>& data)
  {
    exact(*this_) = data.points();
    return true;
  }

# endif // ! OLN_INCLUDE_ONLY

} // end of namespace oln


#endif // ! OLN_CORE_GEN_TORUS_IMAGE_HH
