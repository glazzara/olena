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
  bool operator <= (const Point_Set<L>& lhs, const Point_Set<R>& rhs);


  // FIXME: Guards; without impl!
  template <typename L, typename R> bool operator <  (const Point_Set<L>&, const Point_Set<R>&);
  template <typename L, typename R> bool operator >  (const Point_Set<L>&, const Point_Set<R>&);
  template <typename L, typename R> bool operator >= (const Point_Set<L>&, const Point_Set<R>&);
  // end of FIXME



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

    /*
    // Version for Boxes.

      FIXME: Activate.
    template <typename L, typename R>
    bool op_eq_(const Box<L>& lhs, const Box<R>& rhs)
    {
      return lhs.pmin() == rhs.pmin() and lhs.pmax() == rhs.pmax();
    }
    */


    //  Point_Set L  <=  Point_Set R
    // ------------------------------

    // Generic version.

    template <typename L, typename R>
    bool op_leq_(const Point_Set<L>& lhs, const Point_Set<R>& rhs)
    {
      // quick test:
      if (lhs.npoints() > rhs.npoints())
	return false;
      // final test:
      oln_piter(R) pr(rhs);
      pr.start();
      oln_piter(L) pl(lhs);
      for_all(pl)
	{
	  while (pr.is_valid() and pr.to_point() != pl.to_point())
	    pr.next();
	  if (not pr.is_valid())
	    return false;
	}
      return true;
    }

    /*
    // Version for Boxes.

      FIXME: Activate.
    template <typename L, typename R>
    bool op_leq_(const Box<L>& lhs, const Box<R>& rhs)
    {
      for (unsigned i = 0; i < L::n; ++i)
	if (lhs.pmin()[i] < rhs.pmin()[i] or lhs.pmax()[i] > rhs.pmax()[i])
	  return false;
      return true;
    }
    */


  } // end of namespace oln::impl


  // Facades.

  template <typename L, typename R>
  bool operator == (const Point_Set<L>& lhs, const Point_Set<R>& rhs)
  {
    mlc::assert_equal_< oln_grid(L), oln_grid(R) >::check(); // FIXME: Add err msg.
    return impl::op_eq_(exact(lhs), exact(rhs));
  }

  template <typename L, typename R>
  bool operator <= (const Point_Set<L>& lhs, const Point_Set<R>& rhs)
  {
    mlc::assert_equal_< oln_grid(L), oln_grid(R) >::check(); // FIXME: Add err msg.
    return impl::op_leq_(exact(lhs), exact(rhs));
  }
 
# endif // ! OLN_INCLUDE_ONLY

} // end of namespace oln


#endif // ! OLN_CORE_GEN_PSET_COMPARE_HH
