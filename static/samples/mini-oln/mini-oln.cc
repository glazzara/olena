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

/* \file samples/mini-oln/mini-oln.cc
   \brief A proof of concept of Static using a mini-version of Olena.  */

#include <vector>

#include <mlc/case.hh>
#include <stc/any.hh>
#include <stc/scoop.hh>


/*-------------.
| Equipement.  |
`-------------*/

// Helper macros.
#define oln_type_of_(FromType, Alias)					\
  find_vtype<FromType, oln::typedef_:: Alias##_type>::ret

#define oln_type_of(FromType, Alias)                                    \
  typename oln_type_of_(FromType, Alias)

// Add equipment
stc_scoop_equipment_for_namespace(oln);
mlc_case_equipment_for_namespace(oln);

// Virtual types declaration.
namespace oln
{
  mlc_decl_typedef(point_type);
  mlc_decl_typedef(iter_type);
  mlc_decl_typedef(value_type);

  mlc_decl_typedef(nbh_type);
  mlc_decl_typedef(niter_type);
}


/*---------------.
| Abstractions.  |
`---------------*/

namespace oln
{
  // ------- //
  // Point.  //
  // ------- //

  template <typename E>
  struct Point : public stc::any<E>
  {
  };


  // ---------- //
  // Iterator.  //
  // ---------- //

  template <typename E>
  struct Iterator : public stc::any<E>
  {
    typedef oln_type_of(E, point) point_t;

    void start()
    {
      this->exact().impl_start();
    }

    void next()
    {
      this->exact().impl_next();
    }

    bool is_valid() const
    {
      return this->exact().impl_is_valid();
    }

    // auto
    operator point_t() const
    {
      return this->exact().impl_op_point_type();
    }
  };


  // ------- //
  // Image.  //
  // ------- //

  template <typename E>
  struct Image : public stc::any<E>
  {
    typedef oln_type_of(E, point) point_t;
    typedef oln_type_of(E, value) value_t;

    value_t& operator ()(point_t& p)
    {
      return this->exact().impl_op_parens(p);
    }

    bool has(const point_t& p) const
    {
      return this->exact().impl_has(p);
    }
  };


  // --------- //
  // Image2d.  //
  // --------- //

  template <typename E>
  struct Image2d : public Image<E>
  {
    typedef oln_type_of(E, point) point_t;
    typedef oln_type_of(E, value) value_t;

    // FIXME: delete?
    value_t& operator ()(point_t& p)
    {
      return this->exact().impl_op_parens(p);
    }

    int nrows_get() const
    {
      return this->exact().impl_nrows_get();
    }

    int ncols_get() const
    {
      return this->exact().impl_ncols_get();
    }
  };


  // --------- //
  // Image3d.  //
  // --------- //

  template <typename E>
  struct Image3d : public Image<E>
  {
    typedef oln_type_of(E, point) point_t;
    typedef oln_type_of(E, value) value_t;

    // FIXME: delete?
    value_t& operator ()(point_t& p)
    {
      return this->exact().impl_op_parens(p);
    }

    int nrows_get() const
    {
      return this->exact().impl_nrows_get();
    }

    int ncols_get() const
    {
      return this->exact().impl_ncols_get();
    }

    int nslis_get() const
    {
      return this->exact().impl_nslis_get();
    }
  };


  // ------------------------- //
  // Image_with_neighborhood.  //
  // ------------------------- //

  template <typename E>
  struct Image_with_neighborhood : public Image<E>
  {
    typedef oln_type_of(E, nbh) nbh_t;
    typedef oln_type_of(E, niter) niter_t;

    nbh_t nbh() const
    {
      return this->exact().impl_nbh();
    }
  };

} // End of namespace oln.


/*---------.
| Bridge.  |
`---------*/

namespace oln
{
  // -------- //
  // Switch.  //
  // -------- //

  // Tag.
  struct switch_image_base;

  // Forward declarations.
  struct point2d;
  struct point3d;

  template <typename E>
  struct case_<switch_image_base, E, 1> :
    public mlc::where_ < mlc::eq_ <oln_type_of(E, point), point2d> >
  {
    typedef Image2d<E> ret;
  };

  template <typename E>
  struct case_<switch_image_base, E, 2> :
    public mlc::where_ < mlc::eq_ <oln_type_of(E, point), point3d> >
  {
    typedef Image3d<E> ret;
  };

  template <typename E>
  struct default_case_<switch_image_base, E>
  {
    typedef Image<E> ret;
  };


  // ------------ //
  // Image base.  //
  // ------------ //

  // Forward declaration.
  template <typename E> struct image_base;

  template<typename E>
  struct set_super_type< image_base<E> >
  {
    typedef mlc::none ret;
  };

  template <typename E>
  struct vtypes< image_base<E> >
  {
    typedef stc::abstract point_type;
    typedef stc::abstract iter_type;
    typedef stc::abstract value_type;

    typedef mlc::none niter_type;
    typedef mlc::none nbh_type;
  };

  template <typename E>
  struct image_base : public switch_<switch_image_base, E>::ret
  {
    image_base()
    {
    }
  };

} // End of namespace oln.


/*------------------.
| Implementations.  |
`------------------*/

namespace oln
{
  // -------- //
  // Points.  //
  // -------- //

  struct point2d : public Point<point2d>
  {
    int row;
    int col;
  };

  struct point3d : public Point<point3d>
  {
    int row;
    int col;
    int sli;
  };


  // ------------- //
  // Iterator 2d.  //
  // ------------- //

  // Forward declarations.
  struct iterator2d;
  template <typename T> struct image2d;

  template<>
  struct set_super_type<iterator2d>
  {
    typedef mlc::none ret;
  };

  template <>
  struct vtypes<iterator2d>
  {
    typedef point2d point_type;
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

    void impl_start()
    {
      p.row = 0;
      p.col = 0;
    }

    void impl_next()
    {
      p.col = p.col + 1;

      if (p.col == ncols)
	{
	  p.col = 0;
	  p.row = p.row + 1;
	}
    }

    bool impl_is_valid() const
    {
      return p.row < nrows;
    }

    point_t& impl_op_point_type()
    {
      return p;
    }

  protected:
    int nrows;
    int ncols;
    point_t p;
  };


  // ---------- //
  // Image 2d.  //
  // ---------- //

  // Forward declaration.
  template <typename T> struct image2d;

  template<typename T>
  struct set_super_type< image2d<T> >
  {
    typedef image_base< image2d<T> > ret;
  };

  template <typename T>
  struct vtypes< image2d<T> >
  {
    typedef point2d point_type;
    typedef iterator2d iter_type;
    typedef T value_type;
  };

  template <typename T>
  struct image2d : public image_base< image2d<T> >
  {
    typedef image2d<T> self_t;
    typedef oln_type_of(self_t, point) point_t;
    typedef oln_type_of(self_t, iter) iter_t;
    typedef oln_type_of(self_t, value) value_t;

    image2d(int nrows, int ncols) :
      data (0),
      nrows (nrows),
      ncols (ncols)
    {
      data = new std::vector<value_t>;
    }

    value_t& impl_op_parens(const point_t& p)
    {
      return this->data[p.row * nrows + p.col];
    }

    int impl_nrows_get() const
    {
      return nrows;
    }

    int impl_ncols_get() const
    {
      return ncols;
    }

    bool impl_has(point_t& p) const
    {
      assert(!!data);
      return p.row >= 0 && p.row < nrows && p.col >= 0 && p.col < ncols;
    }

  protected:
    std::vector<value_t>* data;
    int nrows;
    int ncols;
  };


  // ----------------- //
  // Neighborhood 2d.  //
  // ----------------- //

  struct niter2d {};

  struct neighborhood2d
  {
    typedef niter2d niter_type;
  };


  // --------------- //
  // Image morpher.  //
  // --------------- //

  // Forward declaration.
  template <typename E> struct image_morpher;

  template<typename E>
  struct set_super_type< image_morpher<E> >
  {
    typedef image_base<E> ret;
  };

  template <typename E>
  struct vtypes< image_morpher<E> >
  {
    typedef stc::abstract delegatee_type;
  };

  template <typename E>
  struct image_morpher : public image_base<E>
  {
    typedef oln_type_of(E, delegatee) delegatee_t;

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


  // ------------------------- //
  // Image with neighborhood.  //
  // ------------------------- //

  // Forward declaration.
  template <typename I, typename N> struct plus;

  template <typename I, typename N>
  struct set_super_type< plus<I, N> >
  {
    typedef image_morpher< plus <I, N> > ret;
  };

  template <typename I, typename N>
  struct vtypes< plus<I, N> >
  {
    typedef I delegatee_type;
    typedef N nbh_type;
    // For the sake of simplicity, the niter type is obtained directly
    // from the neighborhood type itself (without using a virtual
    // type).
    typedef typename N::niter_type niter_type;
  };

  template <typename I, typename N>
  struct plus : public image_morpher< plus<I, N> >
  {
    typedef plus<I, N> self_t;
    typedef image_morpher<self_t> super_t;

    plus(I& ima, N& nbh) :
      super_t (ima),
      nbh (nbh)
    {
    }

    N impl_nbh() const
    {
      return nbh;
    }

  protected:
    N& nbh;
  };

  /// Helper.
  template <typename I, typename N>
  plus<I, N> operator+(I& lhs, N& rhs)
  {
    plus<I, N> res(lhs, rhs);
    return res;
  }

} // End of namespace oln.


/*--------------.
| Client code.  |
`--------------*/

int main()
{
  using namespace oln;

  point2d p;
  typedef image2d<int> ima_t;
  ima_t ima1(10, 10);

  iterator2d it(ima1);

  typedef neighborhood2d nbh_t;

  // A type of image with neighborhood2d.
  typedef plus<ima_t, nbh_t> ima_wih_nbh_t;
  neighborhood2d nbh;
  ima_wih_nbh_t ima2(ima1, nbh);

  // Another way to build an image with neigborhood.
  ima_wih_nbh_t ima3 = ima1 + nbh;


  // FIXME: Same with 3d.
  point3d q;

  // ...
}
