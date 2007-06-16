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

#ifndef OLN_CORE_3D_IMAGE3D_HH
# define OLN_CORE_3D_IMAGE3D_HH

# include <oln/core/3d/box3d.hh>
# include <oln/core/internal/image_base.hh>
# include <oln/core/internal/utils.hh>
# include <oln/core/3d/array3d.hh>
# include <oln/core/3d/fast_iterator_3d.hh>


namespace oln
{

  // Fwd decl.
  template <typename T> class image3d;


  // Virtual types.
  template <typename T>
  struct vtypes< image3d<T> >
  {
    typedef point3d point;

    typedef int      coord;
    typedef unsigned index;

    typedef       T   value;
    typedef const T& rvalue;
    typedef       T& lvalue;

    typedef box3d pset;
    typedef internal::pair< array3d_<value, coord>*, box3d > data;

    typedef image3d<T>         plain;
    typedef image3d<pl::value> skeleton;

    typedef fast_iterator_3d<value> fiter;
  };


  // Super type.
  template <typename T>
  struct super_trait_< image3d<T> >
  {
    typedef image3d<T> current;
    typedef internal::plain_primitive_image_<current> ret;
  };


  /// General 3D image class.

  template <typename T>
  class image3d : public internal::plain_primitive_image_< image3d<T> >
  {
    typedef image3d<T> current;
    typedef internal::plain_primitive_image_<current> super;
    typedef array3d_<T, int> array_t;
  public:
    //FIXME (fast image concept??)
    typedef typename vtypes< image3d<T> >::fiter fiter;
    typedef typename vtypes< image3d<T> >::index index;
    stc_using(point);
    stc_using(data);

    image3d();
    image3d(const box3d& b);
    image3d(unsigned nslis, unsigned nrows, unsigned ncols);

    point point_at_offset(index offset) const;
    index offset_from_point(point p) const;

    bool impl_owns_(const point3d& p) const;

    bool impl_has_at(int sli, int row, int col) const;

    const T& impl_read(const point3d& p) const;
    const T& impl_index_read(unsigned i) const;
    const T& impl_at(int sli, int row, int col) const;

    T& impl_read_write(const point3d& p);
    T& impl_index_read_write(unsigned i);
    T& impl_at(int sli, int row, int col);

    void impl_write(const point3d& p, const T& v);

    std::size_t impl_npoints() const;

    const box3d& impl_points() const;
  };

  template <typename T, typename D>
  bool prepare(image3d<T>& target, with_t, const D& dat);

  template <typename T>
  bool init_(box3d* this_, const image3d<T>& data);


# ifndef OLN_INCLUDE_ONLY

  template <typename T>
  image3d<T>::image3d()
  {
  }

  template <typename T>
  image3d<T>::image3d(const box3d& b)
  {
    this->data_ = new data(new array_t(b.pmin().sli(),
				       b.pmin().row(),
				       b.pmin().col(),
				       b.pmax().sli(),
				       b.pmax().row(),
				       b.pmax().col()),
			   b);
  }

  template <typename T>
  image3d<T>::image3d(unsigned nslis, unsigned nrows, unsigned ncols)
  {
    precondition(nslis != 0 and nrows != 0 and ncols != 0);
    this->data_ = new data(new array_t(0, 0, 0,
				       nslis - 1,
				       nrows - 1,
				       ncols - 1),
			   box3d(point3d(0, 0, 0),
				 point3d(nslis - 1,
					 nrows - 1,
					 ncols - 1)));
  }

  // Fast Image implementation
  template <typename T>
  typename image3d<T>::point
  image3d<T>::point_at_offset(typename image3d<T>::index offset) const
  {

    return point3d(offset / ((this->data_->second.pmax().col() + 1) *
			     (this->data_->second.pmax().row() + 1)),
		   (offset / (this->data_->second.pmax().col() + 1)) %
		   (this->data_->second.pmax().row() + 1),
		   offset % (this->data_->second.pmax().col() + 1));
  }

  template <typename T>
  typename image3d<T>::index
  image3d<T>::offset_from_point(typename image3d<T>::point p) const
  {
    return p.sli() * (this->data_->second.pmax().col() + 1) *
      (this->data_->second.pmax().col() + 1) +
      p.row() * (this->data_->second.pmax().col() + 1) +
      p.col();
  }

  template <typename T>
  bool image3d<T>::impl_owns_(const point3d& p) const
  {
    assert(this->has_data());
    return this->data_->first.has(p.sli(), p.row(), p.col());
  }

  template <typename T>
  bool image3d<T>::impl_has_at(int sli, int row, int col) const
  {
    assert(this->has_data());
    return this->data_->first.has(sli, row, col);
  }

  template <typename T>
  const T& image3d<T>::impl_read(const point3d& p) const
  {
    assert(this->has_data());
    return this->data_->first(p.sli(), p.row(), p.col());
  }

  template <typename T>
  const T& image3d<T>::impl_index_read(unsigned i) const
  {
    assert(this->has_data());
    assert(i < this->npoints());
    return this->data_->first[i];
  }

  template <typename T>
  const T& image3d<T>::impl_at(int sli, int row, int col) const
  {
    assert(this->has_data());
    return this->data_->first(sli, row, col);
  }

  template <typename T>
  T& image3d<T>::impl_read_write(const point3d& p)
  {
    assert(this->has_data());
    return this->data_->first(p.sli(), p.row(), p.col());
  }

  template <typename T>
  T& image3d<T>::impl_index_read_write(unsigned i)
  {
    assert(this->has_data());
    assert(i < this->npoints());
    return this->data_->first[i];
  }

  template <typename T>
  T& image3d<T>::impl_at(int sli, int row, int col)
  {
    assert(this->has_data());
    return this->data_->first(sli, row, col);
  }

  template <typename T>
  void image3d<T>::impl_write(const point3d& p, const T& v)
  {
    assert(this->has_data());
    this->data_->first(p.sli(), p.row(), p.col()) = v;
  }

  template <typename T>
  std::size_t image3d<T>::impl_npoints() const
  {
    // faster than the default code given by primitive_image_
    assert(this->has_data());
    return this->data_->first.ncells();
  }

  template <typename T>
  const box3d& image3d<T>::impl_points() const
  {
    assert(this->has_data());
    return this->data_->second;
  }

  template <typename T, typename D>
  bool prepare(image3d<T>& target, with_t, const D& dat)
  {
    precondition(not target.has_data());
    box3d b;
    bool box_ok = init(b, with, dat);
    postcondition(box_ok);
    array3d_<T,int>* ptr = new array3d_<T,int>(b.pmin().sli(),
					       b.pmin().row(),
					       b.pmin().col(),
					       b.pmax().sli(),
					       b.pmax().row(),
					       b.pmax().col());
    target.data__() = new typename image3d<T>::data(ptr, b);
    return box_ok;
  }

  template <typename T>
  bool init_(box3d* this_, const image3d<T>& data)
  {
    *this_ = data.bbox();
    return true;
  }

# endif // ! OLN_INCLUDE_ONLY

} // end of namespace oln


#endif // ! OLN_CORE_3D_IMAGE3D_HH
