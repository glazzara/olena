// Copyright (C) 2001, 2002, 2003, 2004, 2006, 2007 EPITA Research and
// Development Laboratory
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

#ifndef OLN_CORE_CONCEPT_POINT_HH
# define OLN_CORE_CONCEPT_POINT_HH

# include <mlc/value.hh>

# include <oln/core/concept/grid.hh>
# include <oln/core/concept/operators.hh>
# include <oln/core/gen/zero.hh>




namespace oln
{

  // Fwd decl.
  template <typename Exact> struct Dpoint;  


  /// Concept-class "Generalized_Point". 

  template <typename Exact>
  struct Generalized_Point : virtual public Any<Exact>
  {
    stc_typename(grid);
    stc_typename(dim);
    stc_typename(coord);
    stc_typename(dpoint);

    enum { n = mlc_value(dim) };

  protected:
    Generalized_Point();
  };


  // Operator -.

  template <typename P1, typename P2>
  oln_minus_trait(P1, P2)
  operator - (const Generalized_Point<P1>& lhs, const Generalized_Point<P2>& rhs);

  // Operator ==.

  template <typename P1, typename P2>
  bool operator == (const Generalized_Point<P1>& lhs, const Generalized_Point<P2>& rhs);

  // Operator <.

  template <typename P1, typename P2>
  bool operator < (const Generalized_Point<P1>& lhs, const Generalized_Point<P2>& rhs);

  // Operator +=.

  template <typename P, typename Dp>  // Defined in oln/core/concept/dpoint.hh
  P& operator += (Generalized_Point<P>& lhs, const Dpoint<Dp>& rhs);

  // Operator -=.

  template <typename P, typename Dp>  // Defined in oln/core/concept/dpoint.hh
  P& operator -= (Generalized_Point<P>& lhs, const Dpoint<Dp>& rhs);



  // \{
  // Invalid operators.

  template <typename P1, typename P2>
  void operator + (const Generalized_Point<P1>& lhs,
		   const Generalized_Point<P2>& rhs);

  template <typename P1, typename P2>
  void operator += (const Generalized_Point<P1>& lhs,
		    const Generalized_Point<P2>& rhs);

  template <typename P>
  void operator - (const Generalized_Point<P>& rhs);

  template <typename P1, typename P2>
  void operator -= (const Generalized_Point<P1>& lhs,
		    const Generalized_Point<P2>& rhs);

  // \}



  /// Concept-class "Point".

  template <typename Exact>
  struct Point : public Generalized_Point<Exact>
  {

    void assign_(zero_t);

  protected:
    Point();

  }; // end of oln::Point<Exact>



  template <typename P>
  struct set_trait_< Point, P, minus_id, Point, P >
  {
    typedef oln_dpoint(P) ret; // FIXME: Wrong! (too restrictive)
  };


  namespace internal
  {
    
    template <typename P>
    const P& to_point(const Point<P>& p);

  } // end of namespace oln::internal



  namespace ERROR
  {

    template < typename Type_1,
	       typename Type_2,
	       typename Point_1 = oln_point(Type_1),
	       typename Point_2 = oln_point(Type_2) >
    struct both_types_should_have_the_same_point_
    {
    };

  } // end of namespace oln::ERROR


  template <typename T1, typename T2>
  struct assert_same_point_
    : public mlc::assert_< mlc::eq_<oln_point(T1), oln_point(T2)>,
			   ERROR::both_types_should_have_the_same_point_<T1, T2> >
  {};



# ifndef OLN_INCLUDE_ONLY

  // Generalized_Point

  template <typename Exact>
  Generalized_Point<Exact>::Generalized_Point()
  {
  }

  template <typename Exact>
  void
  Point<Exact>::assign_(zero_t)
  {
    exact(this)->vec().set_all(0);
  }

  template <typename P>
  oln_minus_trait(P, P)
  operator - (const Generalized_Point<P>& lhs, const Generalized_Point<P>& rhs)
  {
    oln_minus_trait(P, P) tmp;
    tmp.vec() = exact(lhs).vec() - exact(rhs).vec();
      return tmp;
  }

  template <typename P1, typename P2>
  bool
  operator == (const Generalized_Point<P1>& lhs, const Generalized_Point<P2>& rhs)
  {
    return exact(lhs).vec() == exact(rhs).vec();
  }

  template <typename P1, typename P2>
  bool
  operator < (const Generalized_Point<P1>& lhs, const Generalized_Point<P2>& rhs)
  {
    return xtd::lexi_less(exact(lhs).vec(), exact(rhs).vec());
  }


  // Invalid operators.

  template <typename P1, typename P2>
  void operator + (const Generalized_Point<P1>& lhs,
		   const Generalized_Point<P2>& rhs)
  {
    mlc::abort_<P1,
      typename ERROR::operator_< plus_id >::template _is_invalid_for_types_<P1, P2>
      >::check();
  }

  template <typename P1, typename P2>
  void operator += (const Generalized_Point<P1>& lhs,
		    const Generalized_Point<P2>& rhs)
  {
    mlc::abort_<P1,
      typename ERROR::operator_< plus_equal_id >::template _is_invalid_for_types_<P1, P2>
      >::check();
  }

  template <typename P1, typename P2>
  void operator -= (const Generalized_Point<P1>& lhs,
		    const Generalized_Point<P2>& rhs)
  {
    mlc::abort_<P1,
      typename ERROR::operator_< minus_equal_id >::template _is_invalid_for_types_<P1, P2>
      >::check();
  }

  template <typename P>
  void operator - (const Generalized_Point<P>& rhs)
  {
    mlc::abort_<P,
      typename ERROR::operator_< uminus_id >::template _is_invalid_for_<P>
      >::check();
  }


  // Point

  template <typename Exact>
  Point<Exact>::Point()
  {
    // FIXME: Uncomment!
    // mlc::assert_defined_< oln_vtype(Exact, grid)   >::check();
    // mlc::assert_defined_< oln_vtype(Exact, dPoint) >::check();
    // mlc::assert_defined_< oln_vtype(Exact, coord)  >::check();
    // mlc::assert_defined_< oln_vtype(Exact, dim)    >::check();
  }

  namespace internal
  {
    
    template <typename P>
    const P& to_point(const Point<P>& p)
    {
      return exact(p);
    }

  } // end of namespace oln::internal

# endif // ! OLN_INCLUDE_ONLY

} // end of namespace oln



#endif // ! OLN_CORE_CONCEPT_POINT_HH
