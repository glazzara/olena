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

# include <oln/core/internal/encoded_image.hh>



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
    typedef internal::encoded_image_<current> ret;
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

    typedef typename vtypes< internal::encoded_image_< sparse_image<P, T> > >::pset pset__;

    typedef std::pair< pset__, std::vector <std::vector<value> > > data;
  };

  // sparse_image class
  template < typename P, typename T>
  class sparse_image : public internal::encoded_image_< sparse_image<P, T> >
  {
    typedef sparse_image<P, T> current;
    typedef internal::encoded_image_< current > super;

  public:
    stc_using(pset);
    stc_using(point);
    stc_using(value);
    stc_using(rvalue);
    stc_using(lvalue);
    stc_using(psite);
    stc_using(data);

    sparse_image();

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
  void
  sparse_image<P, T>::insert(const point& p, unsigned len,
			     const std::vector<typename sparse_image<P, T>::value>& val)
  {
    assert(this->has_data());
     this->data_->first.insert(p, len);
     this->data_->second.push_back(val);
  }

  template <typename P, typename T>
  typename sparse_image<P, T>::rvalue
  sparse_image<P, T>::impl_read(const sparse_image<P, T>::psite& ps) const
  {
    assert(this->has_data() && ps.pset_pos_ < this->data_->second.size() &&
	   ps.index_ < this->data_->second[ps.pset_pos_].size());

    return this->data_->second[ps.pset_pos_][ps.index_];
  }

  template <typename P, typename T>
  typename sparse_image<P, T>::lvalue
  sparse_image<P, T>::impl_read_write(const sparse_image<P, T>::psite& ps)
  {
    assert(this->has_data() && ps.pset_pos_ < this->data_->second.size() &&
	   ps.index_ < this->data_->second[ps.pset_pos_].size());

    return this->data_->second[ps.pset_pos_][ps.index_];
  }

# endif // ! OLN_INCLUDE_ONLY


} // end of namespace oln

#endif // ! OLN_CORE_GEN_SPARSE_IMAGE_HH

