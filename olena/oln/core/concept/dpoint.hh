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
    
    /// Operator ==.
    bool op_equal_(const Exact& rhs) const;

    /// Operator <.
    bool op_less_(const Exact& rhs) const;

    /// Operator +=.
    Exact& op_plus_equal_(const Exact& rhs);

    /// Operator -=.
    Exact& op_minus_equal_(const Exact& rhs);

    /// Operator %=.
    Exact& op_mod_equal_(const Exact& rhs);

    /// Operator -.
    Exact op_unary_minus_() const;

  protected:
    Dpoint();

  private:
    void check__() const;

  }; // end of oln::Dpoint<Exact>


  /// \{
  /// Operator "Point - Dpoint".

  template <typename P, typename D>
  struct set_trait_< Point, P, minus_id, Dpoint, D >
  {
    typedef P ret;
  };

  template <typename P, typename D>
  P operator-(const Point<P>& lhs, const Dpoint<D>& rhs);

  /// \}


  /// \{
  /// Operator "Point + Dpoint".

  template <typename P, typename D>
  struct set_trait_< Point, P, plus_id, Dpoint, D >
  {
    typedef P ret;
  };

  template <typename P, typename D>
  P operator+(const Point<P>& lhs, const Dpoint<D>& rhs);

  /// \}


  /// \{
  /// Invalid operators.

  template <typename D, typename P>
  void operator-(const Dpoint<D>& lhs, const Point<P>& rhs) /* error */ ;

  template <typename D, typename P>
  void operator+(const Dpoint<D>& lhs, const Point<P>& rhs) /* error */ ;

  /// \}


# ifndef OLN_INCLUDE_ONLY


  template <typename Exact>
  bool Dpoint<Exact>::op_equal_(const Exact& rhs) const
  {
    return exact(this)->impl_op_equal_(rhs);
  }

  template <typename Exact>
  bool Dpoint<Exact>::op_less_(const Exact& rhs) const
  {
    return exact(this)->impl_op_less_(rhs);
  }

  template <typename Exact>
  Exact& Dpoint<Exact>::op_plus_equal_(const Exact& rhs)
  {
    return exact(this)->impl_op_plus_equal_(rhs);
  }

  template <typename Exact>
  Exact& Dpoint<Exact>::op_minus_equal_(const Exact& rhs)
  {
    return exact(this)->impl_op_minus_equal_(rhs);
  }

  template <typename Exact>
  Exact& Dpoint<Exact>::op_mod_equal_(const Exact& rhs)
  {
    return exact(this)->impl_op_mod_equal_(rhs);
  }

  template <typename Exact>
  Exact Dpoint<Exact>::op_unary_minus_() const
  {
    return exact(this)->impl_op_unary_minus_();
  }

  template <typename Exact>
  Dpoint<Exact>::Dpoint()
  {
    this->check__();
    // FIXME: Uncomment!
    //       mlc::assert_defined_< oln_vtype(Exact, grid)  >::check();
    //       mlc::assert_defined_< oln_vtype(Exact, point) >::check();
    //       mlc::assert_defined_< oln_vtype(Exact, coord) >::check();
    //       mlc::assert_defined_< oln_vtype(Exact, dim)   >::check();
  }


  template <typename Exact>
  void Dpoint<Exact>::check__() const
  {
    bool (Exact::*impl_op_equal_adr)(const Exact& rhs) const = & Exact::impl_op_equal_;
    impl_op_equal_adr = 0;
    bool (Exact::*impl_op_less_adr)(const Exact& rhs) const = & Exact::impl_op_less_;
    impl_op_less_adr = 0;
    Exact& (Exact::*impl_op_plus_equal_adr)(const Exact& rhs) = & Exact::impl_op_plus_equal_;
    impl_op_plus_equal_adr = 0;
    Exact& (Exact::*impl_op_minus_equal_adr)(const Exact& rhs) = & Exact::impl_op_minus_equal_;
    impl_op_minus_equal_adr = 0;
    Exact& (Exact::*impl_op_mod_equal_adr)(const Exact& rhs) = & Exact::impl_op_mod_equal_;
    impl_op_mod_equal_adr = 0;
    Exact (Exact::*impl_op_unary_minus_adr)() const = & Exact::impl_op_unary_minus_;
    impl_op_unary_minus_adr = 0;
  }


  /// \{
  /// Operators.

  template <typename P, typename D>
  P operator-(const Point<P>& lhs, const Dpoint<D>& rhs)
  {
    P tmp = exact(lhs);
    return tmp -= rhs;
  }

  template <typename P, typename D>
  P operator+(const Point<P>& lhs, const Dpoint<D>& rhs)
  {
    P tmp = exact(lhs);
    return tmp += rhs;
  }

  /// \}

# endif // ! OLN_INCLUDE_ONLY


} // end of namespace oln


#endif // ! OLN_CORE_CONCEPT_DPOINT_HH
