// Copyright (C) 2001, 2002, 2003, 2004, 2006 EPITA Research and
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

#ifndef OLENA_CORE_ABSTRACT_POINT_HH
# define OLENA_CORE_ABSTRACT_POINT_HH

# include <oln/core/typedefs.hh>
# include <oln/core/traits_id.hh>
# include <oln/core/abstract/dpoint.hh>




namespace oln
{


  namespace ERROR
  {
    struct OPERANDS_ARE_NOT_COMPATIBLE;

  } // end of namespace oln::ERROR



  // Forward declaration.
  namespace abstract { template <typename E> class point; }


  /// Virtual types associated to abstract::point<E>.
  template <typename E>
  struct vtypes< abstract::point<E> >
  {
    typedef mlc::undefined grid_type;
    typedef mlc::undefined dpoint_type;
    typedef mlc::undefined coord_type;
    typedef mlc::undefined dim_type;
  };


  namespace abstract
  {

    /// Abstract point class.
    template <typename E>
    class point : public stc::any__simple<E>,
		  public oln::type
    {
      typedef E exact_t;
      typedef oln_type_of(E, dpoint) dpoint_t;

    public:


      /// \{
      /// Operator ==.

      bool operator==(const exact_t& rhs) const
      {
	return this->exact().impl_equal(rhs);
      }

      template <typename P>
      bool operator==(const abstract::point<P>& rhs) const
      {
	mlc::assert_equal_< oln_type_of(E, grid),
                            oln_type_of(P, grid),
	                    oln::ERROR::OPERANDS_ARE_NOT_COMPATIBLE >::check();
	return this->exact().impl_equal(rhs.exact());
      }

      /// \}
      

      /// \{
      /// Operator !=.

      bool operator!=(const exact_t& rhs) const
      {
	return not (*this == rhs);
      }

      template <typename P>
      bool operator!=(const abstract::point<P>& rhs) const
      {
	mlc::assert_equal_< oln_type_of(E, grid),
                            oln_type_of(P, grid),
	                    oln::ERROR::OPERANDS_ARE_NOT_COMPATIBLE >::check();
	return not (*this == rhs);
      }

      /// \}


      /// \{
      /// Operator <.

      bool operator<(const exact_t& rhs) const
      {
	return this->exact().impl_less(rhs);
      }

      template <typename P>
      bool operator<(const abstract::point<P>& rhs) const
      {
	mlc::assert_equal_< oln_type_of(E, grid),
                            oln_type_of(P, grid),
	                    oln::ERROR::OPERANDS_ARE_NOT_COMPATIBLE >::check();
	return this->exact().impl_less(rhs.exact());
      }

      /// \}


      /// \{
      /// Operator >.

      bool operator>(const exact_t& rhs) const
      {
	return rhs < *this;
      }

      template <typename P>
      bool operator>(const abstract::point<P>& rhs) const
      {
	mlc::assert_equal_< oln_type_of(E, grid),
                            oln_type_of(P, grid),
	                    oln::ERROR::OPERANDS_ARE_NOT_COMPATIBLE >::check();
	return rhs < *this;
      }

      /// \}


      /// \{
      /// Operator >=.

      bool operator>=(const exact_t& rhs) const
      {
	return not (*this < rhs);
      }

      template <typename P>
      bool operator>=(const abstract::point<P>& rhs) const
      {
	mlc::assert_equal_< oln_type_of(E, grid),
                            oln_type_of(P, grid),
	                    oln::ERROR::OPERANDS_ARE_NOT_COMPATIBLE >::check();
	return not (*this < rhs);
      }

      /// \}


      /// \{
      /// Operator <=.

      bool operator<=(const exact_t& rhs) const
      {
	return not (rhs < *this);
      }

      template <typename P>
      bool operator<=(const abstract::point<P>& rhs) const
      {
	mlc::assert_equal_< oln_type_of(E, grid),
                            oln_type_of(P, grid),
	                    oln::ERROR::OPERANDS_ARE_NOT_COMPATIBLE >::check();
	return not (rhs < *this);
      }

      /// \}


      /// \{
      /// Operator +=.

      exact_t& operator+=(const dpoint_t& rhs)
      {
	return this->exact().impl_plus_equal(rhs);
      }

      template <typename D>
      exact_t& operator+=(const abstract::dpoint<D>& rhs)
      {
	mlc::assert_equal_< oln_type_of(E, grid),
                            oln_type_of(D, grid),
	                    oln::ERROR::OPERANDS_ARE_NOT_COMPATIBLE >::check();
	return this->exact().impl_plus_equal(rhs.exact());
      }

      /// \}


      /// \{
      /// Operator +.

      exact_t operator+(const dpoint_t& rhs) const
      {
	return this->exact().impl_plus(rhs);
      }

      template <typename D>
      xtd_op_plus_trait(E,D) operator+(const abstract::dpoint<D>& rhs)
      {
	mlc::assert_equal_< oln_type_of(E, grid),
	                    oln_type_of(D, grid),
	                    oln::ERROR::OPERANDS_ARE_NOT_COMPATIBLE >::check();
	return this->exact().impl_plus(rhs.exact());
      }

      /// \}


      /// \{
      /// Operator -=.

      exact_t& operator-=(const dpoint_t& rhs)
      {
	return this->exact().impl_minus_equal(rhs);
      }

      template <typename D>
      exact_t& operator-=(const abstract::dpoint<D>& rhs) const
      {
	mlc::assert_equal_< oln_type_of(E, grid),
                            oln_type_of(D, grid),
	                    oln::ERROR::OPERANDS_ARE_NOT_COMPATIBLE >::check();
	return this->exact().impl_minus_equal(rhs.exact());
      }

      /// \}


      /// \{
      /// Operator -.

      exact_t operator-(const dpoint_t& rhs) const
      {
	return this->exact().impl_minus(rhs);
      }

      template <typename D>
      xtd_op_minus_trait(E, D) operator-(const abstract::dpoint<D>& rhs) const
      {
	mlc::assert_equal_< oln_type_of(E, grid),
                            oln_type_of(D, grid),
	                    oln::ERROR::OPERANDS_ARE_NOT_COMPATIBLE >::check();
	return this->exact().impl_minus(rhs.exact());
      }

      /// \}


      /// \{
      /// Operator -.

      dpoint_t operator-(const exact_t& rhs) const
      {
	return this->exact().impl_minus(rhs);
      }

      template <typename P>
      xtd_op_minus_trait(E, P) operator-(const abstract::point<P>& rhs) const
      {
	mlc::assert_equal_< oln_type_of(E, grid),
                            oln_type_of(P, grid),
	                    oln::ERROR::OPERANDS_ARE_NOT_COMPATIBLE >::check();
	return this->exact().impl_minus(rhs.exact());
      }

      /// \}


    protected:

      point()
      {}

      ~point()
      {
	mlc::assert_defined_< oln_type_of(E, grid)   >::check();
	mlc::assert_defined_< oln_type_of(E, dpoint) >::check();
	mlc::assert_defined_< oln_type_of(E, coord)  >::check();
	mlc::assert_defined_< oln_type_of(E, dim)    >::check();
      }

    }; // end of class oln::abstract::point<E>


  } // end of namespace oln::abstract


} // end of namespace oln


#endif // ! OLENA_CORE_ABSTRACT_POINT_HH
