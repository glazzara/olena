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

#ifndef OLN_CORE_2D_IMAGE2D_B_HH
# define OLN_CORE_2D_IMAGE2D_B_HH

# include <oln/core/internal/image_base.hh>
# include <oln/core/2d/array2d.hh>
# include <oln/core/2d/box2d.hh>


namespace oln
{


  // FIXME: Move it!

  namespace internal
  {


    template <typename P, typename T>
    struct f_point_value_to_array_;

    template <typename T>
    struct f_point_value_to_array_< point2d, T >
    {
      typedef array2d_<T, int> ret;
    };


    template <typename P, typename T>
    struct array_b_
    {
      typedef typename f_point_value_to_array_<P, T>::ret array_t;
      typedef typename P::coord coord;

      array_b_(const P& pmin, const P& pmax, unsigned border)
	: array(pmin.row() - border, pmin.col() - border,
		pmax.row() + border, pmax.col() + border),
	  border(border),
	  box(pmin, pmax)
      {
      }

      array_t  array;
      unsigned border;
      box_<P>  box;
    };


  } // end of namespace oln::internal


  // end of FIXME



  // Fwd decl.
  template <typename T> class image2d_b;


  /// Virtual types.
  template <typename T>
  struct vtypes< image2d_b<T> >
  {
    typedef point2d point;

    typedef int      coord;
    typedef unsigned index;

    typedef       T   value;
    typedef const T& rvalue;
    typedef       T& lvalue;

    typedef box2d pset;
    typedef internal::array_b_<point2d, T> data;

    typedef image2d_b<T>         plain;
    typedef image2d_b<pl::value> skeleton;

    // FIXME: wrong qiter!!!
  };


  /// Super type.
  template <typename T>
  struct super_trait_< image2d_b<T> >
  {
    typedef image2d_b<T> current;
    typedef internal::plain_primitive_image_<current> ret;
  };


  /// General 2D image class.

  template <typename T>
  class image2d_b : public internal::plain_primitive_image_< image2d_b<T> >
  {
    typedef image2d_b<T> current;
    typedef internal::plain_primitive_image_<current> super;
  public:
    stc_using(data);

    image2d_b();
    image2d_b(const box2d& b, unsigned border = 2);
    image2d_b(unsigned nrows, unsigned ncols, unsigned border = 2);

    bool impl_owns_(const point2d& p) const;

    bool impl_has(const point2d& p) const;
    bool impl_has_at(int row, int col) const;

    const T& impl_read(const point2d& p) const;
    const T& impl_index_read(unsigned i) const;
    const T& impl_at(int row, int col) const;

    T& impl_read_write(const point2d& p);
    T& impl_index_read_write(unsigned i);
    T& impl_at(int row, int col);

    std::size_t impl_npoints() const;

    box2d impl_points() const;

    unsigned border() const;
  };

//   template <typename T, typename D>
//   bool init_(image2d_b<T>* this_, const D& dat);


# ifndef OLN_INCLUDE_ONLY

  template <typename T>
  image2d_b<T>::image2d_b()
  {
  }

  template <typename T>
  image2d_b<T>::image2d_b(const box2d& b, unsigned border)
  {
    this->data_ = new data(b.pmin(), b.pmax(), border);
  }

  template <typename T>
  image2d_b<T>::image2d_b(unsigned nrows, unsigned ncols, unsigned border)
  {
    precondition(nrows != 0 and ncols != 0);
    this->data_ = new data(point2d(0, 0),
			   point2d(nrows - 1, ncols - 1),
			   border);
  }

  template <typename T>
  bool image2d_b<T>::impl_owns_(const point2d& p) const
  {
    assert(this->has_data());
    return this->data_->array.has(p.row(), p.col());
  }

  template <typename T>
  bool image2d_b<T>::impl_has(const point2d& p) const
  {
    assert(this->has_data());
    return this->data_->box.has(p);
  }

  template <typename T>
  bool image2d_b<T>::impl_has_at(int row, int col) const
  {
    assert(this->has_data());
    return this->data_->box.has(point2d(row, col));
  }

  template <typename T>
  const T& image2d_b<T>::impl_read(const point2d& p) const
  {
    assert(this->has_data());
    return this->data_->array(p.row(), p.col());
  }

  template <typename T>
  const T& image2d_b<T>::impl_index_read(unsigned i) const
  {
    assert(this->has_data());
    assert(i < this->npoints());
    return this->data_->array[i];
  }

  template <typename T>
  const T& image2d_b<T>::impl_at(int row, int col) const
  {
    assert(this->has_data());
    return this->data_->array(row, col);
  }

  template <typename T>
  T& image2d_b<T>::impl_read_write(const point2d& p)
  {
    assert(this->has_data());
    return this->data_->array(p.row(), p.col());
  }

  template <typename T>
  T& image2d_b<T>::impl_index_read_write(unsigned i)
  {
    assert(this->has_data());
    assert(i < this->npoints());
    return this->data_->array[i];
  }

  template <typename T>
  T& image2d_b<T>::impl_at(int row, int col)
  {
    assert(this->has_data());
    return this->data_->array(row, col);
  }

  template <typename T>
  box2d image2d_b<T>::impl_points() const
  {
    assert(this->has_data());
    return this->data_->box;
  }

  template <typename T>
  unsigned image2d_b<T>::border() const
  {
    assert(this->has_data());
    return this->data_->border;
  }

//   template <typename T, typename D>
//   bool init_(image2d_b<T>* this_, const D& dat)
//   {
//     precondition(not this_->has_data());
//     box2d b;
//     bool box_ok = init(b, with, dat);
//     postcondition(box_ok);
//     unsigned border = 2; // FIXME: Use init!
//     this_->data__() = new typename image2d_b<T>::data(b.pmin(), b.pmax(), border);
//     return box_ok;
//   }

  template <typename T, typename D>
  bool prepare(image2d_b<T>& target, with_t, const D& dat)
  {
    precondition(not target.has_data());
    box2d b;
    bool box_ok = init(b, with, dat);
    postcondition(box_ok);
    unsigned border = 2; // FIXME: Use init!
    target.data__() = new typename image2d_b<T>::data(b.pmin(), b.pmax(), border);
    return box_ok;
  }

# endif // ! OLN_INCLUDE_ONLY


} // end of namespace oln


#endif // ! OLN_CORE_2D_IMAGE2D_B_HH
