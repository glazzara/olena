// Copyright (C) 2006 EPITA Research and Development Laboratory
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

#ifndef OLN_VALUE_PROXY_HH
# define OLN_VALUE_PROXY_HH

# include <xtd/optraits.hh>
# include <oln/core/typedefs.hh>
# include <oln/core/type.hh>
# include <oln/core/traits_id.hh>


namespace oln
{


  namespace value
  {

    namespace abstract
    {

      template <typename E>
      struct proxy : public virtual stc::any__simple<E>,
		     public oln::type
      {
      protected:
	proxy();
      };


# ifndef OLN_INCLUDE_ONLY

      template <typename E>
      proxy<E>::proxy()
      {
      }

# endif

    } // end of namespace oln::value::abstract


  } // end of namespace oln::value



  // Traits.
  //---------------------------


  /// unary op on a value proxy
  template <typename name, typename T>
  struct case_ < xtd::op_<name>, T,
		 oln::id::unop_vproxy >
  : where_< mlc_is_a(T, value::abstract::proxy) >
  {
    struct protected_ {
      typedef typename T::value_type V;
      typedef xtd_unop_trait(xtd::op_<name>, V) ret;
    };
  };


  /// binary op involving at least one value proxy
  template <typename name, typename T1, typename T2>
  struct case_ < xtd::op_<name>, mlc::pair_<T1,T2>,
		 oln::id::binop_vproxy >
  : where_< mlc::or_< mlc_is_a(T1, value::abstract::proxy),
		      mlc_is_a(T2, value::abstract::proxy) > >
  {
    template <typename T>
    struct value_type
    {
      typedef mlc_typedef(T, value_type) V;
      typedef typename mlc::if_< mlc_is_found(V), V, T >::ret ret;
    };
    struct protected_ {
      typedef mlc_ret(value_type<T1>) V1;
      typedef mlc_ret(value_type<T2>) V2;
      typedef xtd_binop_trait(xtd::op_<name>, V1, V2) ret;
    };
  };



  // Unary ops.
  //---------------------------


  // Op -.
  template <typename P>
  xtd_op_uminus_trait(P) operator - (const value::abstract::proxy<P>& val);
  // Op not.
  template <typename P>
  xtd_op_lnot_trait(P) operator not (const value::abstract::proxy<P>& val);

  // FIXME: To be continued.



  // Binary ops.
  //---------------------------

  // Op +.
  template <typename P, typename V>
  xtd_op_plus_trait(P, V) operator + (const value::abstract::proxy<P>& lhs, const V& rhs);
  template <typename V, typename P>
  xtd_op_plus_trait(V, P) operator + (const V& lhs, const value::abstract::proxy<P>& rhs);
  // Op -.
  template <typename P, typename V>
  xtd_op_minus_trait(P, V) operator - (const value::abstract::proxy<P>& lhs, const V& rhs);
  template <typename V, typename P>
  xtd_op_minus_trait(V, P) operator - (const V& lhs, const value::abstract::proxy<P>& rhs);
  // Op *.
  template <typename P, typename V>
  xtd_op_mult_trait(P, V) operator * (const value::abstract::proxy<P>& lhs, const V& rhs);
  template <typename V, typename P>
  xtd_op_mult_trait(V, P) operator * (const V& lhs, const value::abstract::proxy<P>& rhs);
  // Op /.
  template <typename P, typename V>
  xtd_op_div_trait(P, V) operator / (const value::abstract::proxy<P>& lhs, const V& rhs);
  template <typename V, typename P>
  xtd_op_div_trait(V, P) operator / (const V& lhs, const value::abstract::proxy<P>& rhs);
  // Op %.
  template <typename P, typename V>
  xtd_op_mod_trait(P, V) operator % (const value::abstract::proxy<P>& lhs, const V& rhs);
  template <typename V, typename P>
  xtd_op_mod_trait(V, P) operator % (const V& lhs, const value::abstract::proxy<P>& rhs);

  // Op ==.
  template <typename P, typename V>
  xtd_op_eq_trait(P, V) operator == (const value::abstract::proxy<P>& lhs, const V& rhs);
  template <typename V, typename P>
  xtd_op_eq_trait(V, P) operator == (const V& lhs, const value::abstract::proxy<P>& rhs);
  // Op !=.
  template <typename P, typename V>
  xtd_op_neq_trait(P, V) operator != (const value::abstract::proxy<P>& lhs, const V& rhs);
  template <typename V, typename P>
  xtd_op_neq_trait(V, P) operator != (const V& lhs, const value::abstract::proxy<P>& rhs);

  // Op <.
  template <typename P, typename V>
  xtd_op_less_trait(P, V) operator < (const value::abstract::proxy<P>& lhs, const V& rhs);
  template <typename V, typename P>
  xtd_op_less_trait(V, P) operator < (const V& lhs, const value::abstract::proxy<P>& rhs);
  // Op <=.
  template <typename P, typename V>
  xtd_op_leq_trait(P, V) operator <= (const value::abstract::proxy<P>& lhs, const V& rhs);
  template <typename V, typename P>
  xtd_op_leq_trait(V, P) operator <= (const V& lhs, const value::abstract::proxy<P>& rhs);
  // Op >=.
  template <typename P, typename V>
  xtd_op_geq_trait(P, V) operator >= (const value::abstract::proxy<P>& lhs, const V& rhs);
  template <typename V, typename P>
  xtd_op_geq_trait(V, P) operator >= (const V& lhs, const value::abstract::proxy<P>& rhs);
  // Op >.
  template <typename P, typename V>
  xtd_op_greater_trait(P, V) operator > (const value::abstract::proxy<P>& lhs, const V& rhs);
  template <typename V, typename P>
  xtd_op_greater_trait(V, P) operator > (const V& lhs, const value::abstract::proxy<P>& rhs);

  // Op and.
  template <typename P, typename V>
  xtd_op_land_trait(P, V) operator and (const value::abstract::proxy<P>& lhs, const V& rhs);
  template <typename V, typename P>
  xtd_op_land_trait(V, P) operator and (const V& lhs, const value::abstract::proxy<P>& rhs);
  // Op or.
  template <typename P, typename V>
  xtd_op_lor_trait(P, V) operator or (const value::abstract::proxy<P>& lhs, const V& rhs);
  template <typename V, typename P>
  xtd_op_lor_trait(V, P) operator or (const V& lhs, const value::abstract::proxy<P>& rhs);
  // Op xor.
  template <typename P, typename V>
  xtd_op_lxor_trait(P, V) operator xor (const value::abstract::proxy<P>& lhs, const V& rhs);
  template <typename V, typename P>
  xtd_op_lxor_trait(V, P) operator xor (const V& lhs, const value::abstract::proxy<P>& rhs);
  
  // FIXME: To be continued.



# ifndef OLN_INCLUDE_ONLY


  // Unary ops.
  //---------------------------


  // Op -.
  template <typename P>
  xtd_op_uminus_trait(P) operator - (const value::abstract::proxy<P>& val)
  {
    return - val.exact().value();
  }

  // Op not.

  template <typename P>
  xtd_op_lnot_trait(P)
  operator not (const value::abstract::proxy<P>& val)
  {
    return not val.exact().value();
  }


  // FIXME: To be continued.



  // Binary ops.
  //---------------------------

  // Op +.

  template <typename P, typename V>
  xtd_op_plus_trait(P, V)
  operator + (const value::abstract::proxy<P>& lhs, const V& rhs)
  {
    return lhs.exact().value() + rhs;
  }

  template <typename V, typename P>
  xtd_op_plus_trait(V, P)
  operator + (const V& lhs, const value::abstract::proxy<P>& rhs)
  {
    return lhs + rhs.exact().value();
  }

  // Op -.

  template <typename P, typename V>
  xtd_op_minus_trait(P, V)
  operator - (const value::abstract::proxy<P>& lhs, const V& rhs)
  {
    return lhs.exact().value() - rhs;
  }

  template <typename V, typename P>
  xtd_op_minus_trait(V, P)
  operator - (const V& lhs, const value::abstract::proxy<P>& rhs)
  {
    return lhs - rhs.exact().value();
  }

  // Op *.

  template <typename P, typename V>
  xtd_op_mult_trait(P, V)
  operator * (const value::abstract::proxy<P>& lhs, const V& rhs)
  {
    return lhs.exact().value() * rhs;
  }

  template <typename V, typename P>
  xtd_op_mult_trait(V, P)
  operator * (const V& lhs, const value::abstract::proxy<P>& rhs)
  {
    return lhs * rhs.exact().value();
  }

  // Op /.

  template <typename P, typename V>
  xtd_op_div_trait(P, V)
  operator / (const value::abstract::proxy<P>& lhs, const V& rhs)
  {
    return lhs.exact().value() / rhs;
  }

  template <typename V, typename P>
  xtd_op_div_trait(V, P)
  operator / (const V& lhs, const value::abstract::proxy<P>& rhs)
  {
    return lhs / rhs.exact().value();
  }

  // Op %.

  template <typename P, typename V>
  xtd_op_mod_trait(P, V)
  operator % (const value::abstract::proxy<P>& lhs, const V& rhs)
  {
    return lhs.exact().value() % rhs;
  }

  template <typename V, typename P>
  xtd_op_mod_trait(V, P)
  operator % (const V& lhs, const value::abstract::proxy<P>& rhs)
  {
    return lhs % rhs.exact().value();
  }


  // Op ==.

  template <typename P, typename V>
  xtd_op_eq_trait(P, V)
  operator == (const value::abstract::proxy<P>& lhs, const V& rhs)
  {
    return lhs.exact().value() == rhs;
  }

  template <typename V, typename P>
  xtd_op_eq_trait(V, P)
  operator == (const V& lhs, const value::abstract::proxy<P>& rhs)
  {
    return lhs == rhs.exact().value();
  }


  // Op !=.

  template <typename P, typename V>
  xtd_op_neq_trait(P, V)
  operator != (const value::abstract::proxy<P>& lhs, const V& rhs)
  {
    return lhs.exact().value() != rhs;
  }

  template <typename V, typename P>
  xtd_op_neq_trait(V, P)
  operator != (const V& lhs, const value::abstract::proxy<P>& rhs)
  {
    return lhs != rhs.exact().value();
  }


  // Op <.

  template <typename P, typename V>
  xtd_op_less_trait(P, V)
  operator < (const value::abstract::proxy<P>& lhs, const V& rhs)
  {
    return lhs.exact().value() < rhs;
  }

  template <typename V, typename P>
  xtd_op_less_trait(V, P)
  operator < (const V& lhs, const value::abstract::proxy<P>& rhs)
  {
    return lhs < rhs.exact().value();
  }

  // Op <=.

  template <typename P, typename V>
  xtd_op_leq_trait(P, V)
  operator <= (const value::abstract::proxy<P>& lhs, const V& rhs)
  {
    return lhs.exact().value() <= rhs;
  }

  template <typename V, typename P>
  xtd_op_leq_trait(V, P)
  operator <= (const V& lhs, const value::abstract::proxy<P>& rhs)
  {
    return lhs <= rhs.exact().value();
  }

  // Op >=.

  template <typename P, typename V>
  xtd_op_geq_trait(P, V)
  operator >= (const value::abstract::proxy<P>& lhs, const V& rhs)
  {
    return lhs.exact().value() >= rhs;
  }

  template <typename V, typename P>
  xtd_op_geq_trait(V, P)
  operator >= (const V& lhs, const value::abstract::proxy<P>& rhs)
  {
    return lhs >= rhs.exact().value();
  }

  // Op >.

  template <typename P, typename V>
  xtd_op_greater_trait(P, V)
  operator > (const value::abstract::proxy<P>& lhs, const V& rhs)
  {
    return lhs.exact().value() > rhs;
  }

  template <typename V, typename P>
  xtd_op_greater_trait(V, P)
  operator > (const V& lhs, const value::abstract::proxy<P>& rhs)
  {
    return lhs > rhs.exact().value();
  }


  // Op and.

  template <typename P, typename V>
  xtd_op_land_trait(P, V)
  operator and (const value::abstract::proxy<P>& lhs, const V& rhs)
  {
    return lhs.exact().value() and rhs;
  }

  template <typename V, typename P>
  xtd_op_land_trait(V, P)
  operator and (const V& lhs, const value::abstract::proxy<P>& rhs)
  {
    return lhs and rhs.exact().value();
  }

  // Op or.

  template <typename P, typename V>
  xtd_op_lor_trait(P, V)
  operator or (const value::abstract::proxy<P>& lhs, const V& rhs)
  {
    return lhs.exact().value() or rhs;
  }

  template <typename V, typename P>
  xtd_op_lor_trait(V, P)
  operator or (const V& lhs, const value::abstract::proxy<P>& rhs)
  {
    return lhs or rhs.exact().value();
  }

  // Op xor.

  template <typename P, typename V>
  xtd_op_lxor_trait(P, V)
  operator xor (const value::abstract::proxy<P>& lhs, const V& rhs)
  {
    return lhs.exact().value() xor rhs;
  }

  template <typename V, typename P>
  xtd_op_lxor_trait(V, P)
  operator xor (const V& lhs, const value::abstract::proxy<P>& rhs)
  {
    return lhs xor rhs.exact().value();
  }
  
  // FIXME: To be continued.

# endif

} // end of namespace oln


#endif // ! OLN_VALUE_PROXY_HH
