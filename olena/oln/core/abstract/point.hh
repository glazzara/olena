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

#ifndef OLN_CORE_ABSTRACT_POINT_HH
# define OLN_CORE_ABSTRACT_POINT_HH

# include <oln/core/typedefs.hh>
# include <oln/core/traits_id.hh>
# include <oln/core/abstract/dpoint.hh>

# include <xtd/vec.hh>



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
    typedef mlc::undefined vec_type;
  };

  template <typename E>
  struct single_vtype< abstract::point<E>, typedef_::vec_type >
  {
    typedef oln_type_of(E, coord) coord_t;
    typedef oln_type_of(E, dim) dim_t;
    typedef xtd::vec<mlc_value(dim_t), coord_t> ret;
  };


  namespace abstract
  {

    /// Abstract point class.
    template <typename E>
    class point : public stc::any__simple<E>,
		  public oln::type
    {
      typedef oln_type_of(E, dpoint) dpoint_t;
      typedef oln_type_of(E, vec)    vec_t;

    public:


      /// \{
      /// Operator ==.

      bool operator==(const E& rhs) const;

      template <typename P>
      bool operator==(const abstract::point<P>& rhs) const;

      /// \}
      

      /// \{
      /// Operator !=.

      bool operator!=(const E& rhs) const;

      template <typename P>
      bool operator!=(const abstract::point<P>& rhs) const;

      /// \}


      /// \{
      /// Operator <.

      bool operator<(const E& rhs) const;

      template <typename P>
      bool operator<(const abstract::point<P>& rhs) const;

      /// \}


      /// \{
      /// Operator >.

      bool operator>(const E& rhs) const;

      template <typename P>
      bool operator>(const abstract::point<P>& rhs) const;

      /// \}


      /// \{
      /// Operator >=.

      bool operator>=(const E& rhs) const;

      template <typename P>
      bool operator>=(const abstract::point<P>& rhs) const;

      /// \}


      /// \{
      /// Operator <=.

      bool operator<=(const E& rhs) const;

      template <typename P>
      bool operator<=(const abstract::point<P>& rhs) const;

      /// \}


      /// \{
      /// Operator +=.

      E& operator+=(const dpoint_t& rhs);

      template <typename D>
      E& operator+=(const abstract::dpoint<D>& rhs);

      /// \}


      /// \{
      /// Operator +.

      E operator+(const dpoint_t& rhs) const;

      template <typename D>
      xtd_op_plus_trait(E,D) operator+(const abstract::dpoint<D>& rhs) const;

      /// \}


      /// \{
      /// Operator -=.

      E& operator-=(const dpoint_t& rhs);

      template <typename D>
      E& operator-=(const abstract::dpoint<D>& rhs) const;

      /// \}


      /// \{
      /// Operator -.

      E operator-(const dpoint_t& rhs) const;

      template <typename D>
      xtd_op_minus_trait(E, D) operator-(const abstract::dpoint<D>& rhs) const;

      /// \}


      /// \{
      /// Operator -.

      dpoint_t operator-(const E& rhs) const;

      template <typename P>
      xtd_op_minus_trait(E, P) operator-(const abstract::point<P>& rhs) const;

      /// \}


      const vec_t& vec() const;
      vec_t& vec();


      ~point()
      {
	// FIXME: See code below.
      }

    protected:

      point();

    }; // end of class oln::abstract::point<E>


# ifndef OLN_INCLUDE_ONLY

    template <typename E>
    bool point<E>::operator==(const E& rhs) const
    {
      return this->exact().impl_equal(rhs);
    }

    template <typename E>
    template <typename P>
    bool point<E>::operator==(const abstract::point<P>& rhs) const
    {
      mlc::assert_equal_< oln_type_of(E, grid),
	oln_type_of(P, grid),
	oln::ERROR::OPERANDS_ARE_NOT_COMPATIBLE >::check();
      return this->exact().impl_equal(rhs.exact());
    }

    template <typename E>
    bool point<E>::operator!=(const E& rhs) const
    {
      return not (*this == rhs);
    }

    template <typename E>
    template <typename P>
    bool point<E>::operator!=(const abstract::point<P>& rhs) const
    {
      mlc::assert_equal_< oln_type_of(E, grid),
	oln_type_of(P, grid),
	oln::ERROR::OPERANDS_ARE_NOT_COMPATIBLE >::check();
      return not (*this == rhs);
    }

    template <typename E>
    bool point<E>::operator<(const E& rhs) const
    {
      return this->exact().impl_less(rhs);
    }

    template <typename E>
    template <typename P>
    bool point<E>::operator<(const abstract::point<P>& rhs) const
    {
      mlc::assert_equal_< oln_type_of(E, grid),
	oln_type_of(P, grid),
	oln::ERROR::OPERANDS_ARE_NOT_COMPATIBLE >::check();
      return this->exact().impl_less(rhs.exact());
    }

    template <typename E>
    bool point<E>::operator>(const E& rhs) const
    {
      return rhs < *this;
    }

    template <typename E>
    template <typename P>
    bool point<E>::operator>(const abstract::point<P>& rhs) const
    {
      mlc::assert_equal_< oln_type_of(E, grid),
	oln_type_of(P, grid),
	oln::ERROR::OPERANDS_ARE_NOT_COMPATIBLE >::check();
      return rhs < *this;
    }

    template <typename E>
    bool point<E>::operator>=(const E& rhs) const
    {
      return not (*this < rhs);
    }

    template <typename E>
    template <typename P>
    bool point<E>::operator>=(const abstract::point<P>& rhs) const
    {
      mlc::assert_equal_< oln_type_of(E, grid),
	oln_type_of(P, grid),
	oln::ERROR::OPERANDS_ARE_NOT_COMPATIBLE >::check();
      return not (*this < rhs);
    }

    template <typename E>
    bool point<E>::operator<=(const E& rhs) const
    {
      return not (rhs < *this);
    }

    template <typename E>
    template <typename P>
    bool point<E>::operator<=(const abstract::point<P>& rhs) const
    {
      mlc::assert_equal_< oln_type_of(E, grid),
	oln_type_of(P, grid),
	oln::ERROR::OPERANDS_ARE_NOT_COMPATIBLE >::check();
      return not (rhs < *this);
    }

    template <typename E>
    E& point<E>::operator+=(const typename point<E>::dpoint_t& rhs)
    {
      return this->exact().impl_plus_equal(rhs);
    }

    template <typename E>
    template <typename D>
    E& point<E>::operator+=(const abstract::dpoint<D>& rhs)
    {
      mlc::assert_equal_< oln_type_of(E, grid),
	oln_type_of(D, grid),
	oln::ERROR::OPERANDS_ARE_NOT_COMPATIBLE >::check();
      return this->exact().impl_plus_equal(rhs.exact());
    }

    template <typename E>
    E point<E>::operator+(const typename point<E>::dpoint_t& rhs) const
    {
      return this->exact().impl_plus(rhs);
    }

    template <typename E>
    template <typename D>
    xtd_op_plus_trait(E,D) point<E>::operator+(const abstract::dpoint<D>& rhs) const
    {
      mlc::assert_equal_< oln_type_of(E, grid),
	oln_type_of(D, grid),
	oln::ERROR::OPERANDS_ARE_NOT_COMPATIBLE >::check();
      return this->exact().impl_plus(rhs.exact());
    }

    template <typename E>
    E& point<E>::operator-=(const typename point<E>::dpoint_t& rhs)
    {
      return this->exact().impl_minus_equal(rhs);
    }

    template <typename E>
    template <typename D>
    E& point<E>::operator-=(const abstract::dpoint<D>& rhs) const
    {
      mlc::assert_equal_< oln_type_of(E, grid),
	oln_type_of(D, grid),
	oln::ERROR::OPERANDS_ARE_NOT_COMPATIBLE >::check();
      return this->exact().impl_minus_equal(rhs.exact());
    }

    template <typename E>
    E point<E>::operator-(const typename point<E>::dpoint_t& rhs) const
    {
      return this->exact().impl_minus(rhs);
    }

    template <typename E>
    template <typename D>
    xtd_op_minus_trait(E, D) point<E>::operator-(const abstract::dpoint<D>& rhs) const
    {
      mlc::assert_equal_< oln_type_of(E, grid),
	oln_type_of(D, grid),
	oln::ERROR::OPERANDS_ARE_NOT_COMPATIBLE >::check();
      return this->exact().impl_minus(rhs.exact());
    }

    template <typename E>
    typename point<E>::dpoint_t point<E>::operator-(const E& rhs) const
    {
      return this->exact().impl_minus(rhs);
    }

    template <typename E>
    template <typename P>
    xtd_op_minus_trait(E, P) point<E>::operator-(const abstract::point<P>& rhs) const
    {
      mlc::assert_equal_< oln_type_of(E, grid),
	oln_type_of(P, grid),
	oln::ERROR::OPERANDS_ARE_NOT_COMPATIBLE >::check();
      return this->exact().impl_minus(rhs.exact());
    }

    template <typename E>
    const typename point<E>::vec_t&
    point<E>::vec() const
    {
      return this->exact().impl_vec();
    }

    template <typename E>
    typename point<E>::vec_t&
    point<E>::vec()
    {
      return this->exact().impl_vec();
    }

    template <typename E>
    point<E>::point()
    {
    }

//     template <typename E>
//     point<E>::~point()
//     {
//       mlc::assert_defined_< oln_type_of(E, grid)   >::check();
//       mlc::assert_defined_< oln_type_of(E, dpoint) >::check();
//       mlc::assert_defined_< oln_type_of(E, coord)  >::check();
//       mlc::assert_defined_< oln_type_of(E, dim)    >::check();
//     }

# endif


  } // end of namespace oln::abstract


} // end of namespace oln


#endif // ! OLN_CORE_ABSTRACT_POINT_HH
