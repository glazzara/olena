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

#ifndef OLN_CORE_2D_IMAGE2D_HH
# define OLN_CORE_2D_IMAGE2D_HH

# include <oln/core/internal/image_base.hh>
# include <oln/core/internal/utils.hh>
# include <oln/core/2d/array2d.hh>
# include <oln/core/2d/box2d.hh>


namespace oln
{

  // Fwd decl.
  template <typename T> class image2d;


  /// Virtual types.
  template <typename T>
  struct vtypes< image2d<T> >
  {
    typedef point2d point;

    typedef int      coord;
    typedef unsigned index;

    typedef       T   value;
    typedef const T& rvalue;
    typedef       T& lvalue;

    typedef box2d pset;
    typedef internal::pair< array2d_<value, coord>*, box2d > data;

    typedef image2d<T>         plain;
    typedef image2d<pl::value> skeleton;
  };


  /// Super type.
  template <typename T>
  struct super_trait_< image2d<T> >
  {
    typedef image2d<T> current;
    typedef internal::plain_primitive_image_<current> ret;
  };


  /// General 2D image class.

  template <typename T>
  class image2d : public internal::plain_primitive_image_< image2d<T> >
  {
    typedef image2d<T> current;
    typedef internal::plain_primitive_image_<current> super;
    typedef array2d_<T, int> array_t;
  public:
    stc_using(data);

    image2d();
    image2d(const box2d& b);
    image2d(unsigned nrows, unsigned ncols);

    bool impl_owns_(const point2d& p) const;

    bool impl_has_at(int row, int col) const;

    const T& impl_read(const point2d& p) const;
    const T& impl_index_read(unsigned i) const;
    const T& impl_at(int row, int col) const;

    T& impl_read_write(const point2d& p);
    T& impl_index_read_write(unsigned i);
    T& impl_at(int row, int col);

    std::size_t impl_npoints() const;

    const box2d& impl_points() const;
  };

  template <typename T, typename D>
  bool prepare(image2d<T>& target, with_t, const D& dat);


# ifndef OLN_INCLUDE_ONLY

  template <typename T>
  image2d<T>::image2d()
  {
  }

  template <typename T>
  image2d<T>::image2d(const box2d& b)
  {
    this->data_ = new data(new array_t(b.pmin().row(), b.pmin().col(),
				       b.pmax().row(), b.pmax().col()),
			   b);
  }

  template <typename T>
  image2d<T>::image2d(unsigned nrows, unsigned ncols)
  {
    precondition(nrows != 0 and ncols != 0);
    this->data_ = new data(new array_t(0, 0,
				       nrows - 1, ncols - 1),
			   box2d(point2d(0, 0),
				 point2d(nrows - 1, ncols - 1)));
  }

  template <typename T>
  bool image2d<T>::impl_owns_(const point2d& p) const
  {
    assert(this->has_data());
    return this->data_->first.has(p.row(), p.col());
  }

  template <typename T>
  bool image2d<T>::impl_has_at(int row, int col) const
  {
    assert(this->has_data());
    return this->data_->first.has(row, col);
  }

  template <typename T>
  const T& image2d<T>::impl_read(const point2d& p) const
  {
    assert(this->has_data());
    return this->data_->first(p.row(), p.col());
  }

  template <typename T>
  const T& image2d<T>::impl_index_read(unsigned i) const
  {
    assert(this->has_data());
    assert(i < this->npoints());
    return this->data_->first[i];
  }

  template <typename T>
  const T& image2d<T>::impl_at(int row, int col) const
  {
    assert(this->has_data());
    return this->data_->first(row, col);
  }

  template <typename T>
  T& image2d<T>::impl_read_write(const point2d& p)
  {
    assert(this->has_data());
    return this->data_->first(p.row(), p.col());
  }

  template <typename T>
  T& image2d<T>::impl_index_read_write(unsigned i)
  {
    assert(this->has_data());
    assert(i < this->npoints());
    return this->data_->first[i];
  }

  template <typename T>
  T& image2d<T>::impl_at(int row, int col)
  {
    assert(this->has_data());
    return this->data_->first(row, col);
  }

  template <typename T>
  std::size_t image2d<T>::impl_npoints() const
  {
    // faster than the default code given by primitive_image_
    assert(this->has_data());
    return this->data_->first.ncells();
  }

  template <typename T>
  const box2d& image2d<T>::impl_points() const
  {
    assert(this->has_data());
    return this->data_->second;
  }

  template <typename T, typename D>
  bool prepare(image2d<T>& target, with_t, const D& dat)
  {
    precondition(not target.has_data());
    box2d b;
    bool box_ok = init(b, with, dat);
    postcondition(box_ok);
    array2d_<T,int> ptr = new array2d_<T,int>(b.pmin().row(),
					      b.pmin().col(),
					      b.pmax().row(),
					      b.pmax().col());
    target.data__() = new typename image2d<T>::data(ptr, b);
    return box_ok;
  }

# endif // ! OLN_INCLUDE_ONLY


} // end of namespace oln


#endif // ! OLN_CORE_2D_IMAGE2D_HH
