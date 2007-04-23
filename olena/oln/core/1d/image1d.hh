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

# include <oln/core/1d/box1d.hh>
# include <oln/core/internal/image_base.hh>
# include <oln/core/internal/utils.hh>
# include <oln/core/1d/array1d.hh>


namespace oln
{

  // Fwd decl.
  template <typename T> class image1d;


  // Virtual types.
  template <typename T>
  struct vtypes< image1d<T> >
  {
    typedef point1d point;

    typedef int      coord;
    typedef unsigned index;

    typedef       T   value;
    typedef const T& rvalue;
    typedef       T& lvalue;

    typedef box1d pset;
    typedef internal::pair< array1d_<value, coord>*, box1d > data;

    typedef image1d<T>         plain;
    typedef image1d<pl::value> skeleton;
  };


  // Super type.
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
    typedef array1d_<T, int> array_t;
  public:
    stc_using(data);

    image1d();
    image1d(const box1d& b);
    image1d(const point1d& pmin, const point1d& pmax);
    image1d(int imin, int imax);
    image1d(unsigned n);

    bool impl_owns_(const point1d& p) const;

    const T& impl_read(const point1d& p) const;
    const T& impl_index_read(unsigned i) const;
    T& impl_read_write(const point1d& p);
    T& impl_index_read_write(unsigned i);
    void impl_write(const point1d& p, const T& v);

    bool impl_has_at(int ind) const;
    const T& impl_at(int ind) const;
    T& impl_at(int ind);

    std::size_t impl_npoints() const;
    const box1d& impl_points() const;
  };

  template <typename T, typename D>
  bool prepare(image1d<T>& target, with_t, const D& dat);

  template <typename T>
  bool init_(box1d* this_, const image1d<T>& data);



# ifndef OLN_INCLUDE_ONLY

  template <typename T>
  image1d<T>::image1d()
  {
  }

  template <typename T>
  image1d<T>::image1d(const box1d& b)
  {
    this->data_ = new data(new array_t(b.pmin().ind(), b.pmax().ind()),
			   b);
  }

  template <typename T>
  image1d<T>::image1d(const point1d& pmin, const point1d& pmax)
  {
    precondition(pmax >= pmin);
    box1d b(pmin, pmax);
    this->data_ = new data(new array_t(pmin.ind(), pmax.ind()),
			   b);
  }

  template <typename T>
  image1d<T>::image1d(int imin, int imax)
  {
    precondition(imax >= imin);
    this->data_ = new data(new array_t(imin, imax),
			   box1d(point1d(imin), point1d(imax)));
  }

  template <typename T>
  image1d<T>::image1d(unsigned n)
  {
    precondition(n != 0);
    this->data_ = new data(new array_t(0, n - 1),
			   box1d(point1d(0), point1d(n - 1)));
  }

  template <typename T>
  bool image1d<T>::impl_owns_(const point1d& p) const
  {
    assert(this->has_data());
    return this->data_->first.has(p.ind());
  }

  template <typename T>
  const T& image1d<T>::impl_read(const point1d& p) const
  {
    assert(this->has_data());
    return this->data_->first[p.ind()];
  }

  template <typename T>
  const T& image1d<T>::impl_index_read(unsigned i) const
  {
    assert(this->has_data());
    assert(i < this->npoints());
    return this->data_->first[i];
  }

  template <typename T>
  T& image1d<T>::impl_read_write(const point1d& p)
  {
    assert(this->has_data());
    return this->data_->first[p.ind()];
  }

  template <typename T>
  T& image1d<T>::impl_index_read_write(unsigned i)
  {
    assert(this->has_data());
    assert(i < this->npoints());
    return this->data_->first[i];
  }

  template <typename T>
  void image1d<T>::impl_write(const point1d& p, const T& v)
  {
    assert(this->has_data());
    this->data_->first[p.ind()] = v;
  }

  template <typename T>
  bool image1d<T>::impl_has_at(int ind) const
  {
    assert(this->has_data());
    return this->data_->first.has(ind);
  }

  template <typename T>
  const T& image1d<T>::impl_at(int ind) const
  {
    assert(this->has_data());
    return this->data_->first[ind];
  }

  template <typename T>
  T& image1d<T>::impl_at(int ind)
  {
    assert(this->has_data());
    return this->data_->first[ind];
  }

  template <typename T>
  std::size_t image1d<T>::impl_npoints() const
  {
    // faster than the default code given by primitive_image_
    assert(this->has_data());
    return this->data_->first.ncells();
  }

  template <typename T>
  const box1d& image1d<T>::impl_points() const
  {
    assert(this->has_data());
    return this->data_->second;
  }

  template <typename T, typename D>
  bool prepare(image1d<T>& target, with_t, const D& dat)
  {
    precondition(not target.has_data());
    box1d b;
    bool box_ok = init(b, with, dat);
    postcondition(box_ok);
    array1d_<T,int>* ptr = new array1d_<T,int>(b.pmin().ind(),
					       b.pmax().ind());
    target.data__() = new typename image1d<T>::data(ptr, b);
    return box_ok;
  }

  template <typename T>
  bool init_(box1d* this_, const image1d<T>& data)
  {
    *this_ = data.bbox();
    return true;
  }

# endif // ! OLN_INCLUDE_ONLY


} // end of namespace oln


#endif // ! OLN_CORE_1D_IMAGE1D_HH
