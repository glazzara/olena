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

#ifndef OLENA_CORE_ABSTRACT_DPOINT_HH
# define OLENA_CORE_ABSTRACT_DPOINT_HH

# include <oln/core/typedefs.hh>




namespace oln
{


  /// Function "dpoint_ : (n, coord) -> dpoint type".
  template <unsigned n, typename C>
  struct dpoint_
  {
    typedef mlc::undefined ret;
  };


  // Forward declaration.
  namespace abstract { template <typename E> class dpoint; }


  /// Virtual types associated to abstract::dpoint<E>.
  template <typename E>
  struct vtypes_< abstract::dpoint<E> >
  {
    typedef mlc::undefined grid_type;
    typedef mlc::undefined point_type;
    typedef mlc::undefined coord_type;
    typedef mlc::undefined dim_type;
  };



  namespace abstract
  {

    /// Abstract dpoint class.
    template <typename E>
    class dpoint : public stc::any__simple<E>,
		   public oln::type
    {
    public:

      bool operator==(const abstract::dpoint<E>& rhs) const
      {
	return this->exact().impl_eq(rhs.exact());
      }

      bool operator!=(const abstract::dpoint<E>& rhs) const { return not (*this == rhs); }

      bool operator<(const abstract::dpoint<E>& rhs) const
      {
	return this->exact().impl_less(rhs.exact());
      }

      bool operator> (const abstract::dpoint<E>& rhs) const { return rhs < *this; }
      bool operator>=(const abstract::dpoint<E>& rhs) const { return not (*this < rhs); }
      bool operator<=(const abstract::dpoint<E>& rhs) const { return not (rhs < *this); }

    protected:
      dpoint()
      {}

      ~dpoint() {
	mlc::assert_defined_< oln_type_of(E, grid)  >::check();
	mlc::assert_defined_< oln_type_of(E, point) >::check();
	mlc::assert_defined_< oln_type_of(E, coord) >::check();
	mlc::assert_defined_< oln_type_of(E, dim)   >::check();
      }
    };


  } // end of namespace oln::abstract

} // end of namespace oln


#endif // ! OLENA_CORE_ABSTRACT_DPOINT_HH
