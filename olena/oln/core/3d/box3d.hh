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

#ifndef OLN_CORE_3D_BOX3D_HH
# define OLN_CORE_3D_BOX3D_HH

# include <oln/core/3d/point3d.hh>
# include <oln/core/concept/point_set.hh> // for internal::tag::i_box_t
# include <oln/core/internal/box.hh>


namespace oln
{

  // Fwd decl.
  class box3d;

  // Super type
  template <>
  struct super_trait_< box3d >
  {
    typedef internal::box_<box3d> ret;
  };

  // Virtual types
  template <>
  struct vtypes<box3d>
  {
    typedef point3d point;
    typedef box3d box;
  };

  // Class box3d
  class box3d : public internal::box_< box3d >
  {
    typedef internal::box_< box3d > super;
  public:
    stc_using_(from_to_t);

    box3d();
    box3d(const from_to_t& dat);
    box3d(const point3d& pmin, const point3d& pmax);
    template <typename D>
    box3d(const internal::initializer_<D>& data);
  };

  namespace internal
  {

    template <typename U>
    bool init__(tag::i_box_t, box3d& b,
		const triplet< nrows_t<U>, ncols_t<U>, nslis_t<U> >& data);

    template <typename U>
    bool init__(tag::i_box_t, box3d& b,
		const quartet< from_t<point3d>,
		               nrows_t<U>, ncols_t<U>, nslis_t<U> >& data);

  } // end of namespace oln::internal


# ifndef OLN_INCLUDE_ONLY

  box3d::box3d()
  {
  }

  box3d::box3d(const box3d::from_to_t& dat) :
    super(dat)
  {
  }

  box3d::box3d(const point3d& pmin, const point3d& pmax) :
    super(pmin, pmax)
  {
  }

  template <typename D>
  box3d::box3d(const internal::initializer_<D>& data)
  {
    bool box_ok = internal::init__(internal::tag::i_box_t(), *this, data.value());
    postcondition(box_ok);
  }


  namespace internal
  {

    template <typename U>
    bool init__(tag::i_box_t, box3d& b,
		const triplet< nrows_t<U>, ncols_t<U>, nslis_t<U> >& data)
    {
      unsigned nrows = data.first.value;
      unsigned ncols = data.second.value;
      unsigned nslis = data.third.value;
      precondition(nrows != 0 and ncols != 0 and nslis != 0);
      b.pmin().row() = 0;
      b.pmin().col() = 0;
      b.pmin().sli() = 0;
      b.pmax().row() = nrows - 1;
      b.pmax().col() = ncols - 1;
      b.pmax().sli() = nslis - 1;
      return true;
    }

    template <typename U>
    bool init__(tag::i_box_t, box3d& b,
		const quartet< from_t<point3d>,
		               nrows_t<U>, ncols_t<U>, nslis_t<U> >& data)
    {
      unsigned nrows = data.second.value;
      unsigned ncols = data.third.value;
      unsigned nslis = data.fourth.value;
      precondition(nrows != 0 and ncols != 0 and nslis != 0);
      b.pmin() = data.first.value;
      b.pmax().row() = b.pmin().row() + nrows - 1;
      b.pmax().col() = b.pmin().col() + ncols - 1;
      b.pmax().sli() = b.pmin().sli() + nslis - 1;
      return true;
    }

  } // end of namespace oln::internal

# endif // ! OLN_INCLUDE_ONLY


} // end of namespace oln


#endif // ! OLN_CORE_3D_BOX3D_HH
