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

#ifndef OLN_CORE_3D_IMAGE3D_B_HH
# define OLN_CORE_3D_IMAGE3D_B_HH

# include <oln/core/3d/box3d.hh>
# include <oln/core/internal/image_base.hh>
# include <oln/core/internal/utils.hh>
# include <oln/core/3d/array3d.hh>
# include <oln/core/3d/fast_iterator_3d_b.hh>

namespace oln
{


  // Fwd decl.
  template <typename T> class image3d_b;


  // Virtual types.
  template <typename T>
  struct vtypes< image3d_b<T> >
  {
    typedef point3d point;

    typedef int      coord;
    typedef unsigned index;

    typedef       T   value;
    typedef const T& rvalue;
    typedef       T& lvalue;

    typedef box3d pset;
    typedef internal::triplet< array3d_<T, int>*,
			       unsigned,
			       box3d > data;

    typedef mlc::true_ vborder;

    typedef image3d_b<T>         plain;
    typedef image3d_b<pl::value> skeleton;

    typedef fast_iterator_3d_b<value> fiter;
  };


  // Super type.
  template <typename T>
  struct super_trait_< image3d_b<T> >
  {
    typedef image3d_b<T> current;
    typedef internal::plain_primitive_image_<current> ret;
  };


  /// General 3D image class.

  template <typename T>
  class image3d_b : public internal::plain_primitive_image_< image3d_b<T> >
  {
    typedef image3d_b<T> current;
    typedef internal::plain_primitive_image_<current> super;
    typedef array3d_<T, int> array_t;
  public:
    typedef stc_type(current, fiter) fiter;
    stc_using(data);

    image3d_b();
    image3d_b(const box3d& b,
	      unsigned border = 2);
    image3d_b(unsigned nslis, unsigned nrows, unsigned ncols,
	      unsigned border = 2);

    array_t& img_array();
    const array_t& img_array() const;

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

    unsigned impl_border() const;
    std::size_t pad(const dpoint3d& dp) const;
  };

//   template <typename T, typename D>
//   bool init_(image3d_b<T>* this_, const D& dat);

  template <typename T, typename D>
  bool prepare(image3d_b<T>& target, with_t, const D& dat);



# ifndef OLN_INCLUDE_ONLY

  template <typename T>
  image3d_b<T>::image3d_b()
  {
  }

  template <typename T>
  image3d_b<T>::image3d_b(const box3d& b,
			  unsigned border)
  {
    this->data_ = new data(new array_t(b.pmin().sli() - border,
				       b.pmin().row() - border,
				       b.pmin().col() - border,
				       b.pmax().sli() + border,
				       b.pmax().row() + border,
				       b.pmax().col() + border),
			   border,
			   b);
  }

  template <typename T>
  image3d_b<T>::image3d_b(unsigned nslis, unsigned nrows, unsigned ncols,
			  unsigned border)
  {
    precondition(nslis != 0 and nrows != 0 and ncols != 0);
    this->data_ = new data(new array_t(- border,
				       - border,
				       - border,
				       nslis - 1 + border,
				       nrows - 1 + border,
				       ncols - 1 + border),
			   border,
			   box3d(point3d(0, 0, 0),
				 point3d(nslis - 1,
					 nrows - 1,
					 ncols - 1)));
  }

  template <typename T>
  typename image3d_b<T>::array_t&
  image3d_b<T>::img_array()
  {
    return this->data_->first;
  }

  template <typename T>
  const typename image3d_b<T>::array_t&
  image3d_b<T>::img_array() const
  {
    return this->data_->first;
  }

  template <typename T>
  bool image3d_b<T>::impl_owns_(const point3d& p) const
  {
    assert(this->has_data());
    return this->data_->first.has(p.sli(), p.row(), p.col());
  }

  template <typename T>
  bool image3d_b<T>::impl_has_at(int sli, int row, int col) const
  {
    assert(this->has_data());
    return this->data_->first.has(sli, row, col);
  }

  template <typename T>
  const T& image3d_b<T>::impl_read(const point3d& p) const
  {
    assert(this->has_data());
    return this->data_->first(p.sli(), p.row(), p.col());
  }

  template <typename T>
  const T& image3d_b<T>::impl_index_read(unsigned i) const
  {
    assert(this->has_data());
    assert(i < this->npoints());
    return this->data_->first[i];
  }

  template <typename T>
  const T& image3d_b<T>::impl_at(int sli, int row, int col) const
  {
    assert(this->has_data());
    return this->data_->first(sli, row, col);
  }

  template <typename T>
  T& image3d_b<T>::impl_read_write(const point3d& p)
  {
    assert(this->has_data());
    return this->data_->first(p.sli(), p.row(), p.col());
  }

  template <typename T>
  T& image3d_b<T>::impl_index_read_write(unsigned i)
  {
    assert(this->has_data());
    assert(i < this->npoints());
    return this->data_->first[i];
  }

  template <typename T>
  T& image3d_b<T>::impl_at(int sli, int row, int col)
  {
    assert(this->has_data());
    return this->data_->first(sli, row, col);
  }

  template <typename T>
  void image3d_b<T>::impl_write(const point3d& p, const T& v)
  {
    assert(this->has_data());
    this->data_->first(p.sli(), p.row(), p.col()) = v;
  }

  template <typename T>
  const box3d& image3d_b<T>::impl_points() const
  {
    assert(this->has_data());
    return this->data_->third;
  }

  template <typename T>
  unsigned image3d_b<T>::impl_border() const
  {
    assert(this->has_data());
    return this->data_->second;
  }

  template <typename T>
  std::size_t image3d_b<T>::pad(const dpoint3d& dp) const
  {
    assert(this->has_data());
    return
      this->data_->first.i_pad() * dp.sli() +
      this->data_->first.j_pad() * dp.row() +
      dp.col();
  }

//   template <typename T, typename D>
//   bool init_(image3d_b<T>* this_, const D& dat)
//   {
//     precondition(not this_->has_data());
//     box3d b;
//     bool box_ok = init(b, with, dat);
//     postcondition(box_ok);
//     unsigned border = 2; // FIXME: Use init!
//     this_->data__() = new typename image3d_b<T>::data(b.pmin(), b.pmax(), border);
//     return box_ok;
//   }

  template <typename T, typename D>
  bool prepare(image3d_b<T>& target, with_t, const D& dat)
  {
    precondition(not target.has_data());
    box3d b;
    bool box_ok = init(b, with, dat);
    postcondition(box_ok);
    unsigned border = 2; // FIXME: Use init!
    array3d_<T,int>* ptr = new array3d_<T,int>(b.pmin().sli() - border,
					       b.pmin().row() - border,
					       b.pmin().col() - border,
					       b.pmax().sli() + border,
					       b.pmax().row() + border,
					       b.pmax().col() + border);
    target.data__() = new typename image3d_b<T>::data(ptr, border, b);
    return box_ok;
  }

# endif // ! OLN_INCLUDE_ONLY

} // end of namespace oln


#endif // ! OLN_CORE_3D_IMAGE3D_B_HH
