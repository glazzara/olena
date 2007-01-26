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

#ifndef OLN_CORE_INTERNAL_DPOINT_ND_HH
# define OLN_CORE_INTERNAL_DPOINT_ND_HH

# include <mlc/value.hh>
# include <xtd/vec.hh>
# include <stc/exact.hh>
# include <oln/core/abstract/dpoint.hh>
# include <oln/core/traits_id.hh>


namespace oln
{

  // Forward declaration.
  namespace internal { template <typename E> class dpoint_nd; }


  // Super type declaration.
  template <typename E>
  struct set_super_type< internal::dpoint_nd<E> >
  {
    typedef abstract::dpoint<E> ret;
  };


  template <typename E>
  struct vtypes< internal::dpoint_nd<E> >
  {
# ifndef OLENA_USE_NEW_SCOOP2
  private:
    typedef oln_deferred_vtype(E, dim)   dim_t;
    typedef oln_deferred_vtype(E, coord) coord_t;
  public:
    typedef xtd::vec<mlc_value(dim_t), coord_t> vec_type;
# endif // !OLENA_USE_NEW_SCOOP2
  };

# ifdef OLENA_USE_NEW_SCOOP2
  template <typename E>
  struct single_vtype< internal::dpoint_nd<E>, typedef_::vec_type >
  {
  private:
    typedef oln_deferred_vtype(E, dim)   dim_t;
    typedef oln_deferred_vtype(E, coord) coord_t;
  public:
    typedef xtd::vec<mlc_value(dim_t), coord_t> ret;
  };
# endif // OLENA_USE_NEW_SCOOP2

  namespace internal
  {

    template <typename E>
    class dpoint_nd : public abstract::dpoint<E>
    {
// #ifdef USE_ALT_SCOOP2
//       typedef typename vtypes< internal::dpoint_nd<E> >::self_type dummy;
// #endif // USE_ALT_SCOOP2

      typedef E exact_t;
      typedef dpoint_nd<E> self_t;
      typedef oln_dim(E) dim_t;
      typedef oln_coord(E) coord_t;
      typedef xtd::vec< mlc_value(dim_t), coord_t> vec_t;

    public:

      enum { n = mlc_value(dim_t) };

      bool impl_equal(const dpoint_nd<E>& rhs) const;
      
      coord_t operator[](unsigned i) const;
      
      coord_t& operator[](unsigned i);

      void set_all(const coord_t& c);

      const vec_t& vec() const;

      E operator-() const;

      bool impl_less(const dpoint_nd<E>& rhs) const;

    protected:

      /// Ctor.
      dpoint_nd();

      /// Ctor.
      dpoint_nd(const vec_t& v);

      vec_t v_;
    };


# ifndef OLN_INCLUDE_ONLY

    template <typename E>
    bool
    dpoint_nd<E>::impl_equal(const dpoint_nd<E>& rhs) const
    {
      return v_ == rhs.v_;
    }
      
    template <typename E>
    typename dpoint_nd<E>::coord_t
    dpoint_nd<E>::operator[](unsigned i) const
    {
      assert(i < n);
      return v_[i];
    }
      
    template <typename E>
    typename dpoint_nd<E>::coord_t&
    dpoint_nd<E>::operator[](unsigned i)
    {
      assert(i < n);
      return v_[i];
    }

    template <typename E>
    void
    dpoint_nd<E>::set_all(const typename dpoint_nd<E>::coord_t& c)
    {
      v_.set_all(c);
    }

    template <typename E>
    const typename dpoint_nd<E>::vec_t&
    dpoint_nd<E>::vec() const
    {
      return v_;
    }

    template <typename E>
    E
    dpoint_nd<E>::operator-() const
    {
      E tmp(-v_);
      return tmp;
    }

    template <typename E>
    bool
    dpoint_nd<E>::impl_less(const dpoint_nd<E>& rhs) const
    {
      return xtd::lexi_less(v_, rhs.vec());
    }

    template <typename E>
    dpoint_nd<E>::dpoint_nd()
    {}

    template <typename E>
    dpoint_nd<E>::dpoint_nd(const typename dpoint_nd<E>::vec_t& v) :
      v_(v)
    {}


# endif


  } // end of namespace oln::internal



  /// - internal::dpoint_nd
  template <typename D>
  struct case_ < xtd::op_uminus, D,
		 oln::id::op_uminus_dpointnd >
  : where_< mlc_is_a(D, internal::dpoint_nd) >
  {
    typedef stc_to_exact(D) ret;
  };



} // end of namespace oln


#endif // ! OLN_CORE_INTERNAL_DPOINT_ND_HH
