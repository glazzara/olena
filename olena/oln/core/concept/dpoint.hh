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

#ifndef OLN_CORE_CONCEPT_DPOINT_HH
# define OLN_CORE_CONCEPT_DPOINT_HH

# include <oln/core/concept/point.hh>
# include <xtd/vec.hh>




namespace oln
{


  /// Function "dpoint_ : (n, coord) -> dpoint type".
  template <unsigned n, typename C>
  struct dpoint_
  {
    typedef mlc::undefined ret;
  };




  /// Abstract dpoint class.
  template <typename Exact>
  struct Dpoint : public Any<Exact>
  {
    stc_typename(grid);
    stc_typename(coord);
    stc_typename(dim);
    stc_typename(point);

    enum { n = mlc_value(dim) };

    void assign_(zero_t);

  protected:
    Dpoint();

  }; // end of oln::Dpoint<Exact>



  // Operators on Dpoints.

  template <typename D1, typename D2>
  bool operator == (const Dpoint<D1>& lhs, const Dpoint<D2>& rhs);

  template <typename D1, typename D2>
  bool operator < (const Dpoint<D1>& lhs, const Dpoint<D2>& rhs);

  template <typename D, typename D2>
  D& operator += (Dpoint<D>& lhs, const Dpoint<D2>& rhs);

  template <typename D, typename D2>
  D& operator -= (Dpoint<D>& lhs, const Dpoint<D2>& rhs);

  template <typename D>
  D operator - (const Dpoint<D>& rhs);

  template <typename D, typename D2>
  D& operator %= (Dpoint<D>& lhs, const Dpoint<D2>& rhs);



  // \{
  // Traits.

  template <typename P, typename D>
  struct set_trait_< Point, P, plus_id, Dpoint, D >
  {
    typedef P ret; // FIXME: Wrong! (too restrictive)
  };

  template <typename P, typename D>
  struct set_trait_< Point, P, minus_id, Dpoint, D >
  {
    typedef P ret; // FIXME: Wrong! (too restrictive)
  };

  // \}



  // Operators mixing Generalized_Point and Dpoint.

  template <typename P, typename D>
  oln_plus_trait(P, D)
  operator + (const Generalized_Point<P>& lhs, const Dpoint<D>& rhs);

  template <typename P, typename D>
  oln_minus_trait(P, D)
  operator - (const Generalized_Point<P>& lhs, const Dpoint<D>& rhs);



  // \{
  // Invalid operators.

  template <typename D, typename P>
  void operator - (const Dpoint<D>& lhs, const Generalized_Point<P>& rhs);

  template <typename D, typename P>
  void operator + (const Dpoint<D>& lhs, const Generalized_Point<P>& rhs);

  // \}



# ifndef OLN_INCLUDE_ONLY

  template <typename Exact>
  Dpoint<Exact>::Dpoint()
  {
    // FIXME: Uncomment!
    //       mlc::assert_defined_< oln_vtype(Exact, grid)  >::check();
    //       mlc::assert_defined_< oln_vtype(Exact, point) >::check();
    //       mlc::assert_defined_< oln_vtype(Exact, coord) >::check();
    //       mlc::assert_defined_< oln_vtype(Exact, dim)   >::check();
  }

  template <typename Exact>
  void
  Dpoint<Exact>::assign_(zero_t)
  {
    exact(this)->vec().set_all(0);
  }

  // Operators on Dpoints.

  template <typename D1, typename D2>
  bool operator == (const Dpoint<D1>& lhs, const Dpoint<D2>& rhs)
  {
    return exact(lhs).vec() == exact(rhs).vec();
  }

  template <typename D1, typename D2>
  bool operator < (const Dpoint<D1>& lhs, const Dpoint<D2>& rhs)
  {
    return xtd::lexi_less(exact(lhs).vec(), exact(rhs).vec());
  }

  template <typename D, typename D2>
  D& operator += (Dpoint<D>& lhs, const Dpoint<D2>& rhs)
  {
    exact(lhs).vec() += exact(rhs).vec();
    return exact(lhs);
  }

  template <typename D, typename D2>
  D& operator -= (Dpoint<D>& lhs, const Dpoint<D2>& rhs)
  {
    exact(lhs).vec() -= exact(rhs).vec();
    return exact(lhs);
  }

  template <typename D>
  D operator - (const Dpoint<D>& rhs)
  {
    D tmp;
    tmp.vec() = - exact(rhs).vec();
    return tmp;
  }

  template <typename D, typename D2>
  D& operator %= (Dpoint<D>& lhs, const Dpoint<D2>& rhs)
  {
    // FIXME: Check that D::n == D2::n
    for (unsigned i = 0; i < D::n; ++i)
      {
	exact(lhs)[i] %= exact(rhs)[i];
	if (exact(lhs)[i] < 0)
	  exact(lhs)[i] += exact(rhs)[i];
      }
    return exact(lhs);
  }


  // Operators mixing Generalized_Point and Dpoint.

  template <typename P, typename D>  // from oln/core/concept/point.hh
  P& operator += (Generalized_Point<P>& lhs, const Dpoint<D>& rhs)
  {
    exact(lhs).vec() += exact(rhs).vec();
    return exact(lhs);
  }

  template <typename P, typename D>  // from oln/core/concept/point.hh
  P& operator -= (Generalized_Point<P>& lhs, const Dpoint<D>& rhs)
  {
    exact(lhs).vec() -= exact(rhs).vec();
    return exact(lhs);
  }

  template <typename P, typename D>
  oln_plus_trait(P, D)
  operator + (const Generalized_Point<P>& lhs, const Dpoint<D>& rhs)
  {
    oln_plus_trait(P, D) tmp;
    tmp.vec() = exact(lhs).vec() + exact(rhs).vec();
    return tmp;
  }

  template <typename P, typename D>
  oln_minus_trait(P, D)
  operator - (const Generalized_Point<P>& lhs, const Dpoint<D>& rhs)
  {
    oln_minus_trait(P, D) tmp;
    tmp.vec() = exact(lhs).vec() - exact(rhs).vec();
    return tmp;
  }


  // Invalid operators.

  template <typename D, typename P>
  void operator + (const Dpoint<D>& lhs, const Generalized_Point<P>& rhs)
  {
    mlc::abort_<D,
      typename ERROR::operator_< plus_id >::template _is_invalid_for_types_<D, P>
      >::check();
  }

  template <typename D, typename P>
  void operator - (const Dpoint<D>& lhs, const Generalized_Point<P>& rhs)
  {
    mlc::abort_<D,
      typename ERROR::operator_< minus_id >::template _is_invalid_for_types_<D, P>
      >::check();
  }

# endif // ! OLN_INCLUDE_ONLY

} // end of namespace oln


#endif // ! OLN_CORE_CONCEPT_DPOINT_HH
