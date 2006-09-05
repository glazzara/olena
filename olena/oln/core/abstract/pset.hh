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

# include <oln/core/abstract/entry.hh> // NEW!


namespace oln
{

  /// Forward declaration.
  namespace abstract { template <typename E> class pset; }



  /// Virtual types associated to oln::bbox_nd<point>.
  template <typename E>
  struct vtypes< abstract::pset<E> >
  {
    typedef mlc::undefined point_type;
    typedef mlc::undefined fwd_piter_type;
    typedef mlc::undefined bkd_piter_type;

    typedef mlc::none      bbox_type;
    typedef mlc::undefined ra_type;
    typedef mlc::undefined fixed_type;
  };


  template <typename E>
  struct ext_vtype< abstract::pset<E>, typedef_::coord_type >
  {
    typedef oln_type_of(E, point) P;
    typedef oln_type_of(P, coord) ret;
  };

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
    class pset : public virtual stc::any__simple<E>,
		 public virtual oln::type
    {
    public:

      // ...

    protected:

      pset()
      {}

      ~pset()
      {
	mlc::assert_defined_< oln_type_of(E, point) >::check();
	mlc::assert_defined_< oln_type_of(E, fwd_piter) >::check();
	mlc::assert_defined_< oln_type_of(E, bkd_piter) >::check();

	mlc::assert_defined_< oln_type_of(E, coord) >::check();
	mlc::assert_defined_< oln_type_of(E, grid) >::check();

// 	mlc::assert_< mlc::eq_< oln_type_of(fwd_piter_t, grid),
//                                 oln_type_of(point_t, grid) > >::check();

// 	mlc::assert_< mlc::eq_< oln_type_of(bkd_piter_t, grid),
//                                 oln_type_of(point_t, grid) > >::check();
      }

    }; // end of class oln::abstract::pset<E>



    template <typename E>
    class fixed_pset : public virtual pset<E>
    {
    public:

      unsigned npoints() const
      {
	return this->exact().impl_npoints();
      }

    protected:
      fixed_pset()
      {}
    };



    template <typename E>
    class ra_pset : public virtual pset<E>
    {
      typedef oln_type_of(E, point) point_t;

    public:

      bool has(const point_t& p) const
      {
	return this->exact().impl_has(p);
      }

    protected:
      ra_pset()
      {}
    };



    template <typename E>
    class bboxed_pset : public virtual pset<E>
    {
      typedef oln_type_of(E, point) point_t;
      typedef oln_type_of(E, bbox)  bbox_t;

      typedef oln_type_of(point_t, coord) coord_t;
      typedef oln_type_of(point_t, dim) dim_t;
      enum { n = mlc_value(dim_t) };
      
    public:

      const bbox_t& bbox() const
      {
	return this->exact().impl_box();
      }

      bool is_valid() const
      {
	return this->exact().impl_is_valid();
      }

      const point_t& pmin() const
      {
	precondition(this->is_valid());
	return pmin_;
      }

      coord_t pmin(unsigned i) const
      {
	precondition(this->is_valid() and i < n);
	return pmin_[i];
      }

      const point_t& pmax() const
      {
	precondition(this->is_valid());
	return pmax_;
      }

      coord_t pmax(unsigned i) const
      {
	precondition(this->is_valid() and i < n);
	return pmax_[i];
      }

      unsigned len(unsigned i) const
      {
	precondition(this->is_valid() and i < n);
	return pmax_[i] - pmin_[i] + 1;
      }

    protected:

      bboxed_pset()
      {}

      point_t pmin_, pmax_;
    };


  } // end of namespace oln::abstract



  typedef hierarchy<abstract::pset, 1> pset_fixed_hierarchy;
  typedef hierarchy<abstract::pset, 2> pset_ra_hierarchy;
  typedef hierarchy<abstract::pset, 3> pset_bboxed_hierarchy;


  template <typename E>
  struct case_ < pset_fixed_hierarchy, E, 1 >
    : where_< mlc::eq_< oln_type_of(E, fixed), mlc::true_ > >
  {
    typedef abstract::fixed_pset<E> ret;
  };


  template <typename E>
  struct case_ < pset_ra_hierarchy, E, 1 >
    : where_< mlc::eq_< oln_type_of(E, ra), mlc::true_ > >
  {
    typedef abstract::ra_pset<E> ret;
  };


  template <typename E>
  struct case_ < pset_bboxed_hierarchy, E, 1 >
    : where_< mlc::neq_< oln_type_of(E, bbox), mlc::none > >
  {
    typedef abstract::bboxed_pset<E> ret;
  };



} // end of namespace oln


#endif // ! OLENA_CORE_ABSTRACT_PSET_HH
