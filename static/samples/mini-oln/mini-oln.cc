// Copyright (C) 2007 EPITA Research and Development Laboratory.
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

#include <vector>

#include <mlc/case.hh>
#include <stc/any.hh>
#include <stc/scoop.hh>


// Helper macros.
#define oln_type_of_(FromType, Alias)					\
  oln::find_vtype<FromType, oln::typedef_:: Alias##_type>::ret

#define oln_type_of(FromType, Alias)                                    \
  typename oln_type_of_(FromType, Alias)

// Add equipment
stc_scoop_equipment_for_namespace(oln);
mlc_case_equipment_for_namespace(oln);


// Point
template <typename E>
struct Point : public stc::any<E>
{
};

struct point2d : public Point<point2d>
{
  int row;
  int col;
};

struct point3d : public Point<point2d>
{
  int row;
  int col;
  int sli;
};

// Iterator
template <typename E>
struct Iterator;

namespace oln
{
  mlc_decl_typedef(point_type);

  template <typename E>
  struct vtypes<Iterator<E> >
  {
    typedef mlc::undefined point_type;
  };
};

template <typename E>
struct Iterator : public stc::any<E>
{
  typedef oln_type_of(Iterator, point) point_t;

  void start()
  {
    this->exact().start();
  }

  void next()
  {
    this->exact().next();
  }

  bool is_valid() const
  {
    this->exact().is_valid();
  };

  // auto
  operator point_t()
  {
    return this->exact().operator point_t();
  }
};

// Image
template <typename E>
struct Image;

namespace oln
{
  mlc_decl_typedef(iter_type);
  mlc_decl_typedef(value_type);

  template <typename E>
  struct vtypes<Image<E> >
  {
    typedef mlc::undefined point_type;
    typedef mlc::undefined iter_type;
    typedef mlc::undefined value_type;
  };
};

template <typename E>
struct Image : public stc::any<E>
{
  typedef oln_type_of(Image, point) point_t;
  typedef oln_type_of(Image, iter)  iter_t;
  typedef oln_type_of(Image, value) value_t;

  value_t& operator ()(point_t& p)
  {
    return this->exact().operator ()(p);
  }

  bool has(const point_t& p) const
  {
    this->exact().has(p);
  }

};

namespace oln
{
  struct Image2d;

  template<>
  struct set_super_type<Image2d>
  {
    typedef Image<Image2d> ret;
  };
};

struct Image2d : public Image<Image2d>
{
  typedef oln_type_of_(Image2d, point) point_t;
  typedef oln_type_of_(Image2d, iter)  iter_t;
  typedef oln_type_of_(Image2d, value) value_t;

  value_t& operator ()(point_t& p)
  {
    return this->exact().operator ()(p);
  }

  int nrows_get() const
  {
    this->exact().nrows_get();
  }

  int ncols_get() const
  {
    this->exact().ncols_get();
  }
};

namespace oln
{
  struct Image3d;

  template<>
  struct set_super_type<Image3d>
  {
    typedef Image<Image3d> ret;
  };
};

struct Image3d : public Image<Image3d>
{
  typedef oln_type_of_(Image3d, point) point_t;
  typedef oln_type_of_(Image3d, iter)  iter_t;
  typedef oln_type_of_(Image3d, value) value_t;

  value_t& operator ()(point_t& p)
  {
    return this->exact().operator ()(p);
  }

  int nrows_get() const
  {
    this->exact().nrows_get();
  }

  int ncols_get() const
  {
    this->exact().ncols_get();
  }

  int nslis_get() const
  {
    this->exact().nslis_get();
  }
};

// Image with neighborhood
struct image_with_nbh;

namespace oln
{
  mlc_decl_typedef(niter_type);
  mlc_decl_typedef(nbh_type);

  template<>
  struct set_super_type<image_with_nbh>
  {
    typedef Image<image_with_nbh> ret;
  };

  template <>
  struct vtypes<image_with_nbh>
  {
    typedef mlc::undefined niter_type;
    typedef mlc::undefined nbh_type;
  };
};

struct image_with_nbh : public Image<image_with_nbh>
{
  typedef oln_type_of_(image_with_nbh, niter) niter_t;
  typedef oln_type_of_(image_with_nbh, nbh) nbh_t;

  nbh_t nbh() const
  {
    //FIXME
  }
};

// Switch
struct switch_image_base;

namespace oln
{
  namespace error
  {
    struct ERROR_image_base;
  };

  template <typename I>
  struct case_<switch_image_base, I, 1> : public mlc::where_ < mlc::eq_ <I, point2d> >
  {
    typedef Image2d ret;
  };

  template <typename I>
  struct case_<switch_image_base, I, 2> : public mlc::where_ < mlc::eq_ <I, point3d> >
  {
    typedef Image3d ret;
  };

  template <typename I>
  struct default_case_<switch_image_base, I>
  {
    typedef mlc::undefined ret;
  };
};




// Image base
template <typename E>
struct image_base;

namespace oln
{
  template<typename E>
  struct set_super_type<image_base<E> >
  {
    typedef typename oln::switch_<switch_image_base, E>::ret ret;
  };

  template <typename E>
  struct vtypes<image_base<E> >
  {
    typedef mlc::undefined point_type;
    typedef mlc::undefined iter_type;
    typedef mlc::undefined value_type;
  };
};

template <typename E>
struct image_base : public oln::switch_<switch_image_base, E>::ret
{
  typedef oln_type_of(image_base, point) point_t;
  typedef oln_type_of(image_base, iter)  iter_t;
  typedef oln_type_of(image_base, value) value_t;

  value_t& operator ()(point_t& p)
  {
    return this->exact().operator ()(p);
  }

  bool has(const point_t& p) const
  {
    this->exact().has(p);
  }
};

// Iterator 2d
struct iterator2d;

template <typename T>
struct image2d;

namespace oln
{
  template<>
  struct set_super_type<iterator2d>
  {
    typedef Iterator<iterator2d> ret;
  };

  template <>
  struct vtypes<iterator2d>
  {
    typedef point2d point_type;
  };
};

struct iterator2d : public Iterator<iterator2d>
{
  typedef oln_type_of_(iterator2d, point) point_t;

  template <typename T>
  iterator2d(image2d<T>& ima) :
    nrows (ima.nrows_get()),
    ncols (ima.ncols_get())
  {
  }

  void start()
  {
    p.row = 0;
    p.col = 0;
  }

  void next()
  {
    p.col = p.col + 1;

    if (p.col == ncols)
      {
        p.col = 0;
        p.row = p.row + 1;
      }
  }

  bool is_valid() const
  {
    return p.row < nrows;
  }

  operator point_t()
  {
    return p;
  }

protected:
  int nrows;
  int ncols;
  point_t p;
};

// image2d
template <typename T>
struct image2d;

namespace oln
{
  template<typename T>
  struct set_super_type<image2d<T> >
  {
    typedef image_base<image2d<T> > ret;
  };

  template <typename T>
  struct vtypes<image2d<T> >
  {
    typedef point2d point_t;
    typedef iterator2d iter_t;
    typedef T value_t;
  };
};

template <typename T>
struct image2d : public image_base<image2d<T> >
{
  typedef oln_type_of(image2d, point) point_t;
  typedef oln_type_of(image2d, iter) iter_t;
  typedef oln_type_of(image2d, value) value_t;

  image2d(int nrows, int ncols) :
    data (0),
    nrows (nrows),
    ncols (ncols)
  {
    data = new std::vector<value_t>;
  }

  value_t& operator()(const point_t& p)
  {
    return this->data[p.row * nrows + p.col];
  }

  int nrows_get() const
  {
    return nrows;
  }

  int ncols_get() const
  {
    return ncols;
  }

  bool has(point_t& p)
  {
    assert(!!data);
    return p.row >= 0 && p.row < nrows && p.col >= 0 && p.col < ncols;
  }

protected:
  std::vector<value_t>* data;
  int nrows;
  int ncols;
};

// image_morpher
template <typename E>
struct image_morpher;

namespace oln
{
  template<typename E>
  struct set_super_type<image_morpher<E> >
  {
    typedef image_base<image_morpher<E> > ret;
  };

  template <typename E>
  struct vtypes<image_morpher<E> >
  {
    typedef mlc::undefined delegatee_t;
  };
};

template <typename E>
struct image_morpher : public image_base<image_morpher<E> >
{
  typedef oln_type_of(image_morpher, delegatee) delegatee_t;

  image_morpher(delegatee_t& ref_ima) :
    ref_ima (ref_ima)
  {
  }

  delegatee_t& ref ()
  {
    return ref_ima;
  }
protected:
  delegatee_t& ref_ima;
};

//image_plus_neighborhood
template <typename I, typename N>
struct plus;

namespace oln
{
  template <typename I, typename N>
  struct set_super_type<image_morpher<plus<I, N> > >
  {
    typedef image_morpher<plus <I, N> > ret;
  };

  template <typename I, typename N>
  struct vtypes<plus<I, N> >
  {
    typedef I delegatee_t;
  };
};

template <typename I, typename N>
struct plus : public image_morpher<plus<I, N> >
{
  plus(I& ima, N& nbh) :
    nbh (nbh)
  {
  }

protected:
  N& nbh;
};

template <typename I, typename N>
plus<I, N> operator +(I& lhs, N& rhs)
{
  plus<I, N> res(lhs, rhs);
  return res;
}



int main()
{
  typedef plus<image2d<int>, image_with_nbh> my_ima;

  point2d p2d;
  point3d p3d;

  image2d<int> ima(10, 10);
  image_with_nbh ima_nb;

  iterator2d it2d(ima);

  //my_ima p = ima + ima_nb;
}

