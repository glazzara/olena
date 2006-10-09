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

#ifndef OLN_CORE_INTERNAL_POINT_ND_HH
# define OLN_CORE_INTERNAL_POINT_ND_HH

# include <xtd/vec.hh>
# include <xtd/optraits.hh>
# include <oln/core/traits_id.hh>

# include <oln/core/abstract/point.hh>
# include <oln/core/internal/dpoint_nd.hh>


namespace oln
{


  /// Function \f$point : (n, coord) \rightarrow point type\f$.

  template <unsigned n, typename C>
  struct point_
  {
    typedef mlc::undefined ret;
  };



  // Forward declaration.
  namespace internal { template <typename E> class point_nd; }



  // Super type declaration.
  template <typename E>
  struct set_super_type< internal::point_nd<E> >
  {
    typedef abstract::point<E> ret;
  };


  namespace internal
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
      typedef xtd::vec<n,coord_t> vec_t;
      
      coord_t operator[](unsigned i) const;
      coord_t& operator[](unsigned i);

      bool impl_equal(const self_t& rhs) const;

      bool impl_less(const self_t& rhs) const;

      E& impl_plus_equal(const dpoint_t& rhs);

      E impl_plus(const dpoint_t& rhs) const;

      E& impl_minus_equal(const dpoint_t& rhs);

      E impl_minus(const dpoint_t& rhs) const;

      dpoint_t impl_minus(const self_t& rhs) const;

      const vec_t& vec() const;

    protected:

      /// Ctor.
      point_nd();

      /// Ctor.
      point_nd(const vec_t& v);

      /// Dtor.
      ~point_nd()
      {
      }

      vec_t v_;
    };


# ifndef OLN_INCLUDE_ONLY

    template <typename E>
    typename point_nd<E>::coord_t
    point_nd<E>::operator[](unsigned i) const
    {
      assert(i < n);
      return v_[i];
    }
    
    template <typename E>
    typename point_nd<E>::coord_t&
    point_nd<E>::operator[](unsigned i)
    {
      assert(i < n);
      return v_[i];
    }

    template <typename E>
    bool point_nd<E>::impl_equal(const self_t& rhs) const
    {
      return v_ == rhs.vec();
    }

    template <typename E>
    bool point_nd<E>::impl_less(const self_t& rhs) const
    {
      return xtd::lexi_less(v_, rhs.vec());
    }

    template <typename E>
    E& point_nd<E>::impl_plus_equal(const dpoint_t& rhs)
    {
      v_ += rhs.vec();
      return this->exact();
    }

    template <typename E>
    E point_nd<E>::impl_plus(const dpoint_t& rhs) const
    {
      E tmp(v_ + rhs.vec());
      return tmp;
    }

    template <typename E>
    E& point_nd<E>::impl_minus_equal(const typename point_nd<E>::dpoint_t& rhs)
    {
      v_ += rhs.vec();
      return this->exact();
    }

    template <typename E>
    E point_nd<E>::impl_minus(const typename point_nd<E>::dpoint_t& rhs) const
    {
      E tmp(v_ - rhs.vec());
      return tmp;
    }

    template <typename E>
    typename point_nd<E>::dpoint_t
    point_nd<E>::impl_minus(const self_t& rhs) const
    {
      typename point_nd<E>::dpoint_t tmp(v_ - rhs.vec());
      return tmp;
    }

    template <typename E>
    const typename point_nd<E>::vec_t&
    point_nd<E>::vec() const
    {
      return v_;
    }

    template <typename E>
    point_nd<E>::point_nd()
    {}

    template <typename E>
    point_nd<E>::point_nd(const typename point_nd<E>::vec_t& v) :
      v_(v)
    {}

//     template <typename E>
//     point_nd<E>::~point_nd()
//     {}

# endif



  } // end of namespace oln::internal



  template <typename E>
  std::ostream& operator<<(std::ostream& ostr, const internal::point_nd<E>& p);

# ifndef OLN_INCLUDE_ONLY

  template <typename E>
  std::ostream& operator<<(std::ostream& ostr, const internal::point_nd<E>& p)
  {
    ostr << p.vec();
    return ostr;
  }

# endif



  /// internal::point_nd + internal::dpoint_nd
  template <typename P, typename D>
  struct case_ < xtd::op_plus, mlc::pair_<P,D>,
		 oln::id::op_plus_pointnd_dpointnd >
  : where_< mlc::and_< mlc_is_a(P, internal::point_nd),
		       mlc_is_a(D, internal::dpoint_nd) > >
  {
    typedef oln_type_of(P, coord) P_coord;
    typedef oln_type_of(D, coord) D_coord;
    typedef xtd_op_plus_trait(P_coord, D_coord) coord;
    typedef oln_type_of(P, dim) dim;
    typedef typename point_<mlc_value(dim), coord>::ret ret;
  };


  /// internal::point_nd - internal::dpoint_nd
  template <typename P, typename D>
  struct case_ < xtd::op_minus, mlc::pair_<P,D>,
		 oln::id::op_minus_pointnd_dpointnd >
  : where_< mlc::and_< mlc_is_a(P, internal::point_nd),
		       mlc_is_a(D, internal::dpoint_nd) > >
  {
    typedef oln_type_of(P, coord) P_coord;
    typedef oln_type_of(D, coord) D_coord;
    typedef xtd_op_minus_trait(P_coord, D_coord) coord;
    typedef oln_type_of(P, dim) dim;
    typedef typename point_<mlc_value(dim), coord>::ret ret;
  };


  /// internal::point_nd - internal::point_nd
  template <typename P1, typename P2>
  struct case_ < xtd::op_minus, mlc::pair_<P1,P2>,
		 oln::id::op_minus_pointnd_pointnd >
  : where_< mlc::and_< mlc_is_a(P1, internal::point_nd),
		       mlc_is_a(P2, internal::point_nd) > >
  {
    typedef oln_type_of(P1, coord) P1_coord;
    typedef oln_type_of(P2, coord) P2_coord;
    typedef xtd_op_minus_trait(P1_coord, P2_coord) coord;
    typedef oln_type_of(P1, dim) dim;
    typedef typename dpoint_<mlc_value(dim), coord>::ret ret;
  };


} // end of namespace oln


#endif // ! OLN_CORE_INTERNAL_POINT_ND_HH
