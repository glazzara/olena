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

#ifndef OLENA_CORE_ABSTRACT_POINT_ND_HH
# define OLENA_CORE_ABSTRACT_POINT_ND_HH

# include <xtd/vec.hh>
# include <xtd/optraits.hh>
# include <oln/core/traits_id.hh>

# include <oln/core/abstract/point.hh>
# include <oln/core/abstract/dpoint_nd.hh>


namespace oln
{


  /// Function "point_ : (n, coord) -> point type".
  template <unsigned n, typename C>
  struct point_
  {
    typedef mlc::undefined ret;
  };



  // Forward declaration.
  namespace abstract { template <typename E> class point_nd; }



  // Super type declaration.
  template <typename E>
  struct set_super_type< abstract::point_nd<E> >
  {
    typedef abstract::point<E> ret;
  };


  namespace abstract
  {

    template <typename E>
    class point_nd : public abstract::point<E>
    {
      typedef point_nd<E> self_t;
      typedef oln_type_of(E, dim) dim;
      typedef oln_type_of(E, coord) coord_t;
      typedef oln_type_of(E, dpoint) dpoint_t;

    public:

      enum { n = mlc_value(dim) };
      
      coord_t operator[](unsigned i) const
      {
	assert(i < n);
	return v_[i];
      }
      
      coord_t& operator[](unsigned i)
      {
	assert(i < n);
	return v_[i];
      }

      bool impl_equal(const self_t& rhs) const
      {
	return v_ == rhs.vec();
      }

      bool impl_less(const self_t& rhs) const
      {
	return xtd::lexi(v_, rhs.vec());
      }

      E& impl_plus_equal(const dpoint_t& rhs)
      {
	v_ += rhs.vec();
	return this->exact();
      }

      E impl_plus(const dpoint_t& rhs) const
      {
	E tmp(v_ + rhs.vec());
	return tmp;
      }

      E& impl_minus_equal(const dpoint_t& rhs)
      {
	v_ += rhs.vec();
	return this->exact();
      }

      E impl_minus(const dpoint_t& rhs) const
      {
	E tmp(v_ - rhs.vec());
	return tmp;
      }

      dpoint_t impl_minus(const self_t& rhs) const
      {
	dpoint_t tmp(v_ - rhs.vec());
	return tmp;
      }

      const xtd::vec<n,coord_t>& vec() const
      {
	return v_;
      }

    protected:

      /// Ctor.
      point_nd()
      {}

      /// Ctor.
      point_nd(const xtd::vec<n,coord_t>& v) :
	v_(v)
      {}

      typedef xtd::vec<n,coord_t> vec_t;
      vec_t v_;
    };

  } // end of namespace oln::abstract



  /// abstract::point_nd + abstract::dpoint_nd
  template <typename P, typename D>
  struct case_ < xtd::op_plus, mlc::pair_<P,D>,
		 oln::id::op_plus_pointnd_dpointnd >
    : where_< mlc::and_< mlc_is_a(P, abstract::point_nd),
                         mlc_is_a(D, abstract::dpoint_nd) > >
  {
    typedef oln_type_of(P, coord) P_coord;
    typedef oln_type_of(D, coord) D_coord;
    typedef xtd_op_plus_trait(P_coord, D_coord) coord;
    typedef oln_type_of(P, dim) dim;
    typedef typename point_<mlc_value(dim), coord>::ret ret;
  };


  /// abstract::point_nd - abstract::dpoint_nd
  template <typename P, typename D>
  struct case_ < xtd::op_minus, mlc::pair_<P,D>,
		 oln::id::op_minus_pointnd_dpointnd >
    : where_< mlc::and_< mlc_is_a(P, abstract::point_nd),
                         mlc_is_a(D, abstract::dpoint_nd) > >
  {
    typedef oln_type_of(P, coord) P_coord;
    typedef oln_type_of(D, coord) D_coord;
    typedef xtd_op_minus_trait(P_coord, D_coord) coord;
    typedef oln_type_of(P, dim) dim;
    typedef typename point_<mlc_value(dim), coord>::ret ret;
  };


  /// abstract::point_nd - abstract::point_nd
  template <typename P1, typename P2>
  struct case_ < xtd::op_minus, mlc::pair_<P1,P2>,
		 oln::id::op_minus_pointnd_pointnd >
    : where_< mlc::and_< mlc_is_a(P1, abstract::point_nd),
                         mlc_is_a(P2, abstract::point_nd) > >
  {
    typedef oln_type_of(P1, coord) P1_coord;
    typedef oln_type_of(P2, coord) P2_coord;
    typedef xtd_op_minus_trait(P1_coord, P2_coord) coord;
    typedef oln_type_of(P1, dim) dim;
    typedef typename dpoint_<mlc_value(dim), coord>::ret ret;
  };


} // end of namespace oln


#endif // ! OLENA_CORE_ABSTRACT_POINT_ND_HH
