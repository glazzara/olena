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

# include <oln/core/concept/generalised_point.hh>
# include <oln/core/concept/grid.hh>
# include <oln/core/concept/operators.hh>




namespace oln
{
  /// Instant value.
  oln_decl_instant_value(point);


  /// Concept-class "Point".

  template <typename Exact>
  struct Point : public Generalised_Point<Exact>
  {
    stc_using_from(Generalised_Point, dpoint);

    /// Operator ==.
    bool op_equal_(const Exact& rhs) const;

    /// Operator <.
    bool op_less_(const Exact& rhs) const;

    /// Operator +=.
    Exact& op_plus_equal_(const dpoint& rhs);

    /// Operator -=.
    Exact& op_minus_equal_(const dpoint& rhs);

    /// Operator -.
    dpoint op_minus_(const Exact& rhs) const;

  protected:
    Point();

  }; // end of oln::Point<Exact>



  /// Operator -.

  template <typename P>
  struct set_trait_< Point, P, minus_id, Point, P >
  {
    typedef typename P::dpoint ret;
  };

  template <typename P>
  typename P::dpoint
  operator - (const Point<P>& lhs, const Point<P>& rhs);



  /// \{
  /// Invalid operators.

  template <typename P>
  void operator-(const Point<P>& rhs) /* error */ ;

  template <typename P1, typename P2>
  void operator+(const Point<P1>& lhs, const Point<P2>& rhs) /* error */ ;

  /// \}



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


  template <typename Exact>
  bool Point<Exact>::op_equal_(const Exact& rhs) const
  {
    return exact(this)->impl_op_equal_(rhs);
  }

  template <typename Exact>
  bool Point<Exact>::op_less_(const Exact& rhs) const
  {
    return exact(this)->impl_op_less_(rhs);
  }

  template <typename Exact>
  Exact& Point<Exact>::op_plus_equal_(const typename Point<Exact>::dpoint& rhs)
  {
    return exact(this)->impl_op_plus_equal_(rhs);
  }

  template <typename Exact>
  Exact& Point<Exact>::op_minus_equal_(const typename Point<Exact>::dpoint& rhs)
  {
    return exact(this)->impl_op_minus_equal_(rhs);
  }

  template <typename Exact>
  typename Point<Exact>::dpoint Point<Exact>::op_minus_(const Exact& rhs) const
  {
    return exact(this)->impl_op_minus_(rhs);
  }

  template <typename Exact>
  Point<Exact>::Point()
  {
    // mlc::assert_defined_< oln_vtype(Exact, grid)   >::check();
    // mlc::assert_defined_< oln_vtype(Exact, dPoint) >::check();
    // mlc::assert_defined_< oln_vtype(Exact, coord)  >::check();
    // mlc::assert_defined_< oln_vtype(Exact, dim)    >::check();
  }

  template <typename P>
  typename P::dpoint
  operator-(const Point<P>& lhs, const Point<P>& rhs)
  {
    return lhs.op_minus_(exact(rhs));
  }

# endif // ! OLN_INCLUDE_ONLY


} // end of namespace oln


#endif // ! OLN_CORE_CONCEPT_POINT_HH
