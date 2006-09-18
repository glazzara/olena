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

#ifndef OLENA_CORE_ABSTRACT_BBOX_HH
# define OLENA_CORE_ABSTRACT_BBOX_HH

# include <oln/core/point_set_entry.hh>


namespace oln
{

  // Forward declaration.
  namespace abstract { template <typename E> class bbox; }


  // Super type declaration.
  template <typename E>
  struct set_super_type< abstract::bbox<E> >
  {
    typedef point_set_entry<E> ret;
  };


  /// Virtual types associated to oln::abstract::bbox<E>.
  template <typename E>
  struct vtypes< abstract::bbox<E> >
  {
    typedef mlc::true_ is_random_accessible_type;
    typedef mlc::true_ has_know_size_type;
    typedef mlc::true_ is_connected_type;
    typedef E bbox_type;
  };


  namespace abstract
  {

    /// Abstract bbox (bounding box) class.
    template <typename E>
    class bbox : public point_set_entry<E>
    {
      typedef oln_type_of(E, point) point_t;

    public:

      void print(std::ostream& ostr) const
      {
	this->exact().impl_print(ostr);
      }
      
      friend
      std::ostream& operator<<(std::ostream& ostr, const abstract::bbox<E>& bb)
      {
	bb.print(ostr);
	return ostr;
      }

      const E& impl_bbox() const
      {
	return this->exact();
      }
      
    protected:

      bbox()
      {
      }

      ~bbox()
      {
	mlc::assert_defined_< point_t >::check();

// 	typedef oln_type_of(E, fwd_piter) fwd_piter_t;
// 	typedef oln_type_of(E, bkd_piter) bkd_piter_t;

// 	mlc::assert_defined_< fwd_piter_t >::check();
// 	mlc::assert_defined_< bkd_piter_t >::check();

// 	mlc::assert_< mlc::eq_< oln_type_of(fwd_piter_t, grid),
//                                 oln_type_of(point_t, grid) > >::check();

// 	mlc::assert_< mlc::eq_< oln_type_of(bkd_piter_t, grid),
//                                 oln_type_of(point_t, grid) > >::check();
      }

    }; // end of class oln::abstract::bbox<E>


  } // end of namespace oln::abstract


} // end of namespace oln


#endif // ! OLENA_CORE_ABSTRACT_BBOX_HH
