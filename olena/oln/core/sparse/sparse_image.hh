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


#ifndef OLN_CORE_GEN_SPARSE_IMAGE_HH
# define OLN_CORE_GEN_SPARSE_IMAGE_HH


# include <vector>
# include <map>

# include <oln/core/internal/image_base.hh>



namespace oln
{
    // Fwd decl.
  template <typename P, typename T>
  struct sparse_image;

  // Super type.
  template <typename P, typename T>
  struct super_trait_< sparse_image<P, T> >
  {
    typedef sparse_image<P, T> current;
    typedef internal::primitive_image_<current> ret;
  };

  // Virtual types
  template <typename P, typename T>
  struct vtypes< sparse_image<P, T> >
  {
    typedef T value;
    typedef const T& rvalue;
    typedef T& lvalue;

    typedef P point;
    typedef typename P::coord coord;

    typedef rle_psite<P> psite;

    typedef rle_pset<point> pset;

    //FIXME: set correct trait
    typedef mlc::none plain;

    typedef std::pair< pset, std::map<point, std::vector<value> > > data;
  };

  /*!
  ** \class sparse_image
  ** \brief represent an image not plain
  **
  */
  template < typename P, typename T>
  class sparse_image : public internal::primitive_image_< sparse_image<P, T> >
  {
    typedef sparse_image<P, T> current;
    typedef internal::primitive_image_< sparse_image<P, T> > super;
  public:
    stc_using(pset);
    stc_using(box);
    stc_using(point);
    stc_using(value);
    stc_using(rvalue);
    stc_using(lvalue);
    stc_using(psite);
    stc_using(data);

    sparse_image();

    const pset& impl_points() const;
    bool impl_owns_(const psite& p) const;
    void insert(const point& p, unsigned len, const std::vector<value>& val);
    rvalue impl_read(const psite& p) const;
    lvalue impl_read_write(const psite& p);

  };


# ifndef OLN_INCLUDE_ONLY

  template <typename P, typename T>
  sparse_image<P, T>::sparse_image()
  {
    this->data_ = new data;
  }

  template <typename P, typename T>
  const typename sparse_image<P, T>::pset&
  sparse_image<P, T>::impl_points() const
  {
    assert(this->has_data());
    return this->data_->first;
  }

  template <typename P, typename T>
  bool
  sparse_image<P, T>::impl_owns_(const typename sparse_image<P, T>::psite& p) const
  {
    assert(this->has_data());
    return this->data_->first.has(p.start_);
  }

  template <typename P, typename T>
  void
  sparse_image<P, T>::insert(const point& p, unsigned len, const std::vector<typename sparse_image<P, T>::value>& val)
  {
    assert(this->has_data());
     this->data_->first.insert(p, len);
     this->data_->second[p] = val;
  }

  template <typename P, typename T>
  typename sparse_image<P, T>::rvalue
  sparse_image<P, T>::impl_read(const sparse_image<P, T>::psite& ps) const
  {
    assert(this->has_data());
    typename std::map<point, std::vector<value> >::const_iterator irun;

    irun.operator= (this->data_->second.find(ps.start_));
    assert(irun != this->data_->second.end() && ps.index_ < this->data_->first.range_len_(ps.start_));
    return irun->second[ps.index_];
  }

  template <typename P, typename T>
  typename sparse_image<P, T>::lvalue
  sparse_image<P, T>::impl_read_write(const sparse_image<P, T>::psite& ps)
  {
    assert(this->has_data());
    typename std::map<point, std::vector<value> >::iterator irun;

    irun.operator= (this->data_->second.find(ps.start_));
    assert(irun != this->data_->second.end() && ps.index_ < this->data_->first.range_len_(ps.start_));
    return irun->second[ps.index_];
  }

# endif // ! OLN_INCLUDE_ONLY


} // end of namespace oln

#endif // ! OLN_CORE_GEN_SPARSE_IMAGE_HH

