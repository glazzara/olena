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

#ifndef OLN_CORE_GEN_PSET_COMPARE_HH
# define OLN_CORE_GEN_PSET_COMPARE_HH

# include <oln/core/concept/point_set.hh>


namespace oln
{

  // Fwd decls.

  template <typename L, typename R>
  bool operator == (const Point_Set<L>& lhs, const Point_Set<R>& rhs);

  template <typename L, typename R>
  bool operator <  (const Point_Set<L>& lhs, const Point_Set<R>& rhs);

  template <typename L, typename R>
  bool operator <= (const Point_Set<L>& lhs, const Point_Set<R>& rhs);

  template <typename L, typename R>
  bool operator >  (const Point_Set<L>& lhs, const Point_Set<R>& rhs);

  template <typename L, typename R>
  bool operator >= (const Point_Set<L>& lhs, const Point_Set<R>& rhs);


# ifndef OLN_INCLUDE_ONLY

  namespace impl
  {

    //  Point_Set L  ==  Point_Set R
    // ------------------------------

    // Generic version.

    template <typename L, typename R>
    bool op_eq_(const Point_Set<L>& lhs, const Point_Set<R>& rhs)
    {
      oln_piter(L) pl(lhs);
      oln_piter(R) pr(rhs);
      for (pl.start(), pr.start();
	   pl.is_valid();
	   pl.next(), pr.next())
	{
	  if (not pr.is_valid()) // while pl is valid
	    return false;
	  if (pl.to_point() != pr.to_point())
	    return false;
	}
      if (pr.is_valid()) // while pl is not valid
	return false;
      return true;
    }

    // Version for Boxes.

    template <typename L, typename R>
    bool op_eq_(const Box<L>& lhs, const Box<R>& rhs)
    {
      return lhs.pmin() == rhs.pmin() and lhs.pmax() == rhs.pmax();
    }


    //  Point_Set L  <  Point_Set R
    // -----------------------------

    // Generic version.

    template <typename L, typename R>
    bool op_less_(const Point_Set<L>& lhs, const Point_Set<R>& rhs)
    {
      if (lhs.npoints() >= rhs.npoints()) // quick test
	return false;
      // we have lhs.npoints() < rhs.npoints()
      // so we shall now test that all points of lhs are IN rhs
      oln_piter(R) p_rhs(rhs);
      p_rhs.start();
      oln_piter(L) p_lhs(lhs);
      for_all(p_lhs)
	{
	  while (p_rhs.is_valid() and p_rhs.to_point() != p_lhs.to_point())
	    p_rhs.next();
	  if (not p_rhs.is_valid())
	    return false;
	}
      return true;
    }



    // Box L <= Box R.

    template <typename L, typename R>
    bool op_leq_(const Box<L>& lhs, const Box<R>& rhs)
    {
      for (unsigned i = 0; i < L::n; ++i)
	{
	  if (lhs.pmin()[i] < rhs.pmin()[i] or lhs.pmax()[i] > rhs.pmax()[i])
	    return false;
	}
      return true;
    }

    // Box L < Box R.

    template <typename L, typename R>
    bool op_less_(const Box<L>& lhs, const Box<R>& rhs)
    {
      return op_leq_(lhs, rhs) and lhs != rhs;
    }

    // Box L >= Box R.

    template <typename L, typename R>
    bool op_geq_(const Box<L>& lhs, const Box<R>& rhs)
    {
      for (unsigned i = 0; i < L::n; ++i)
	{
	  if (lhs.pmin()[i] > rhs.pmin()[i] or lhs.pmax()[i] < rhs.pmax()[i])
	    return false;
	}
      return true;
    }

    // Box L > Box R.

    template <typename L, typename R>
    bool op_greater_(const Box<L>& lhs, const Box<R>& rhs)
    {
      return op_geq_(lhs, rhs) and lhs != rhs;
    }


  } // end of namespace oln::impl


  // Facades.

  template <typename L, typename R>
  bool operator == (const Point_Set<L>& lhs, const Point_Set<R>& rhs)
  {
    assert_same_grid_<L, R>::check();
    return impl::op_eq_(exact(lhs), exact(rhs));
  }

  template <typename L, typename R>
  bool operator <= (const Point_Set<L>& lhs, const Point_Set<R>& rhs)
  {
    assert_same_grid_<L, R>::check();
    return impl::op_leq_(exact(lhs), exact(rhs));
  }

  template <typename L, typename R>
  bool operator < (const Point_Set<L>& lhs, const Point_Set<R>& rhs)
  {
    assert_same_grid_<L, R>::check();
    return impl::op_less_(exact(lhs), exact(rhs));
  }

  template <typename L, typename R>
  bool operator >= (const Point_Set<L>& lhs, const Point_Set<R>& rhs)
  {
    assert_same_grid_<L, R>::check();
    return impl::op_geq_(exact(lhs), exact(rhs));
  }

  template <typename L, typename R>
  bool operator > (const Point_Set<L>& lhs, const Point_Set<R>& rhs)
  {
    assert_same_grid_<L, R>::check();
    return impl::op_greater_(exact(lhs), exact(rhs));
  }
 
# endif // ! OLN_INCLUDE_ONLY

} // end of namespace oln


#endif // ! OLN_CORE_GEN_PSET_COMPARE_HH
