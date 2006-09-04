// Copyright (C) 2001, 2003, 2004, 2005, 2006 EPITA Research and
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

#ifndef OLENA_CORE_ABSTRACT_PSET_HH
# define OLENA_CORE_ABSTRACT_PSET_HH

# include <stc/any.hh>
# include <oln/core/type.hh>
# include <oln/core/typedefs.hh>


namespace oln
{

  /// Forward declaration.
  namespace abstract { template <typename E> class pset; }



  /// Virtual types associated to oln::bbox_nd<point>.
  template <typename E>
  struct vtypes< abstract::pset<E> >
  {
    typedef mlc::undefined point_type;
//     typedef mlc::undefined fwd_piter_type;
//     typedef mlc::undefined bkd_piter_type;
  };


//   template <typename E>
//   struct single_vtype< abstract::pset<E>,    typedef_::coord_type >
//     : type_of_< oln_type_of(E, point), typedef_::coord_type >
//   {};


//   template <typename E>
//   struct ext_vtype< abstract::pset<E>, typedef_::coord_type >
//   {
//   private:
//     typedef oln_type_of(E, point) P;
//   public:
//     typedef oln_type_of(P, coord) ret;
//   };


  template <typename E>
  struct ext_vtype< abstract::pset<E>, typedef_::grid_type >
  {
    typedef oln_type_of(E, point) P;
    typedef oln_type_of(P, grid) ret;
  };


  namespace abstract
  {

    /// Abstract point class.
    template <typename E>
    class pset : public stc::any__simple<E>,
		 public oln::type
    {

    protected:

      pset()
      {}

      ~pset()
      {
	mlc::assert_defined_< oln_type_of(E, point) >::check();
	mlc::assert_defined_< oln_type_of(E, coord) >::check();

	// FIXME: BUG! Trouble with circular dependencies.

// 	mlc::assert_defined_< fwd_piter_t >::check();
// 	mlc::assert_defined_< bkd_piter_t >::check();

// 	mlc::assert_< mlc::eq_< oln_type_of(fwd_piter_t, grid),
//                                 oln_type_of(point_t, grid) > >::check();

// 	mlc::assert_< mlc::eq_< oln_type_of(bkd_piter_t, grid),
//                                 oln_type_of(point_t, grid) > >::check();
      }

    }; // end of class oln::abstract::pset<E>


  } // end of namespace oln::abstract


} // end of namespace oln


#endif // ! OLENA_CORE_ABSTRACT_PSET_HH
