// Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007 EPITA
// Research and Development Laboratory
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

#ifndef OLN_CORE_RLE_RLE_IMAGE_HH_
# define OLN_CORE_RLE_RLE_IMAGE_HH_

# include <map>
# include <utility>


# include <oln/core/internal/image_base.hh>
# include <oln/core/rle/rle_psite.hh>
# include <oln/core/rle/rle_pset.hh>



namespace oln
{
  // Fwd decl.
  template <typename P, typename T>
  struct rle_image;

  // Super type.
  template <typename P, typename T>
  struct super_trait_< rle_image<P, T> >
  {
    typedef rle_image<P, T> current;
    typedef internal::primitive_image_<current> ret;
  };

  // Virtual types
  template <typename P, typename T>
  struct vtypes< rle_image<P, T> >
  {
    typedef T value;
    typedef const T& rvalue;
    typedef T& lvalue;

    typedef P point;
    typedef typename P::coord coord;

    typedef rle_psite<P, T> psite;

    typedef rle_pset<point> pset;
//     typedef typename pset::box box;

    typedef std::map<std::pair<point, unsigned>, value> data;
  };


  // Rle image class
  template < typename P, typename T>
  class rle_image : public internal::primitive_image_< rle_image<P, T> >
  {
    typedef rle_image<P, T> current;
    typedef internal::primitive_image_< rle_image<P, T> > super;
  public:
    stc_using(pset);
    stc_using(box);
    stc_using(point);
    stc_using(value);
    stc_using(rvalue);
    stc_using(lvalue);
    stc_using(psite);
    stc_using(data);

    rle_image();

    pset impl_points() const;
    box impl_bbox() const;
    bool impl_has(const point& p) const;
    bool impl_owns_(const psite& p) const;
    void insert(const point& p, unsigned len, value val);
    rvalue impl_read(const psite& p) const;
    lvalue impl_read_write(const psite& p);

    //Internal
    const data& get_data_() const;

  protected:
    pset pset_;
  };

# ifndef OLN_INCLUDE_ONLY

  template <typename P, typename T>
  rle_image<P, T>::rle_image()
  {
    this->data_ = new typename rle_image<P, T>::data;
  }

  template <typename P, typename T>
  typename rle_image<P, T>::pset
  rle_image<P, T>::impl_points() const
  {
    return pset_;
  }


  template <typename P, typename T>
  typename rle_image<P, T>::box
  rle_image<P, T>::impl_bbox() const
  {
    return pset_.bbox();
  }

  template <typename P, typename T>
  bool
  rle_image<P, T>::impl_has(const typename rle_image<P, T>::point& p) const
  {
    return this->pset_.has(p.point_());
  }

  template <typename P, typename T>
  bool
  rle_image<P, T>::impl_owns_(const typename rle_image<P, T>::psite& p) const
  {
    //FIXME
    return this->pset_.has(p.point_());
  }

  template <typename P, typename T>
  void
  rle_image<P, T>::insert(const typename rle_image<P, T>::point& p, unsigned len, rle_image<P, T>::value val)
  {
    pset_.insert(p, len);
    ((this->data_))->operator[](std::make_pair(p, len)) = val;
  }

  template <typename P, typename T>
  typename rle_image<P, T>::rvalue
  rle_image<P, T>::impl_read(const rle_image<P, T>::psite& p) const
  {
    precondition(p.iterator_() != this->data_->end());
    return p.iterator_()->second;
  }

  template <typename P, typename T>
  typename rle_image<P, T>::lvalue
  rle_image<P, T>::impl_read_write(const rle_image<P, T>::psite& p)
  {
    precondition(p.iterator_() != this->data_->end());
    return p.iterator_()->second;
  }

  template <typename P, typename T>
  const typename rle_image<P, T>::data&
  rle_image<P, T>::get_data_() const
  {
    //FIXME: another way to do that?
    return *(this->data_.ptr_);
  }


# endif // !OLN_INCLUDE_ONLY

} // end of namespace oln

#endif /* !OLN_CORE_RLE_RLE_IMAGE_HH_ */
