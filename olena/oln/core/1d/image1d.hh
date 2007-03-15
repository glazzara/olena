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

#ifndef OLN_CORE_1D_IMAGE1D_HH
# define OLN_CORE_1D_IMAGE1D_HH

# include <oln/core/internal/image_base.hh>
# include <oln/core/1d/array1d.hh>
# include <oln/core/1d/box1d.hh>


namespace oln
{

  // Fwd decl.
  template <typename T> class image1d;


  /// Virtual types.
  template <typename T>
  struct vtypes< image1d<T> >
  {
    typedef point1d point;

    typedef int      coord;
    typedef unsigned index;

    typedef       T   value;
    typedef const T& rvalue;
    typedef       T& lvalue;

    typedef box1d                  pset;
    typedef array1d_<value, coord> data;

    typedef image1d<T>         plain;
    typedef image1d<pl::value> skeleton;
  };


  /// Super type.
  template <typename T>
  struct super_trait_< image1d<T> >
  {
    typedef image1d<T> current;
    typedef internal::plain_primitive_image_<current> ret;
  };


  /// General 1D image class.

  template <typename T>
  class image1d : public internal::plain_primitive_image_< image1d<T> >
  {
    typedef image1d<T> current;
    typedef internal::plain_primitive_image_<current> super;
  public:
    stc_using(data);

    image1d();
    image1d(const box1d& b);
    image1d(unsigned n);

    bool impl_owns_(const point1d& p) const;

    bool impl_has(const point1d& p) const;

    const T& impl_read(const point1d& p) const;
    const T& impl_index_read(unsigned i) const;

    T& impl_read_write(const point1d& p);
    T& impl_index_read_write(unsigned i);

    std::size_t impl_npoints() const;

    box1d impl_points() const;
  };



# ifndef OLN_INCLUDE_ONLY

  template <typename T>
  image1d<T>::image1d()
  {
  }

  template <typename T>
  image1d<T>::image1d(const box1d& b)
  {
    this->data_ = new data(b.pmin().ind(), b.pmax().ind());
  }

  template <typename T>
  image1d<T>::image1d(unsigned n)
  {
    precondition(n != 0);
    this->data_ = new data(0, n - 1);
  }

  template <typename T>
  bool image1d<T>::impl_owns_(const point1d& p) const
  {
    assert(this->has_data());
    return this->data_->has(p.ind());
  }

  template <typename T>
  bool image1d<T>::impl_has(const point1d& p) const
  {
    assert(this->has_data());
    return this->data_->has(p.ind());
  }

  template <typename T>
  const T& image1d<T>::impl_read(const point1d& p) const
  {
    assert(this->has_data());
    return this->data_->operator()(p.ind());
  }

  template <typename T>
  const T& image1d<T>::impl_index_read(unsigned i) const
  {
    assert(this->has_data());
    assert(i < this->npoints());
    return this->data_->operator[](i);
  }

  template <typename T>
  T& image1d<T>::impl_read_write(const point1d& p)
  {
    assert(this->has_data());
    return this->data_->operator()(p.ind());
  }

  template <typename T>
  T& image1d<T>::impl_index_read_write(unsigned i)
  {
    assert(this->has_data());
    assert(i < this->npoints());
    return this->data_->operator[](i);
  }

  template <typename T>
  std::size_t image1d<T>::impl_npoints() const
  {
    // faster than the default code given by primitive_image_
    assert(this->has_data());
    return this->data_->ncells();
  }

  template <typename T>
  box1d image1d<T>::impl_points() const
  {
    assert(this->has_data());
    box1d b(this->data_->imin(), this->data_->imax());
    return b;
  }

# endif


} // end of namespace oln


#endif // ! OLN_CORE_1D_IMAGE1D_HH
