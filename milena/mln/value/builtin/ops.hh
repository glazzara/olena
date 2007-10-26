// Copyright (C) 2007 EPITA Research and Development Laboratory
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

#ifndef MLN_VALUE_BUILTIN_OPS_HH
# define MLN_VALUE_BUILTIN_OPS_HH

/*! \file mln/value/builtin/ops.hh
 *
 * \brief Definitions of binary operators when lhs is a built-in and
 * rhs is an mln object.
 */

# include <mln/value/scalar.hh>
# include <mln/trait/op/all.hh>
# include <mln/value/builtin/promotions.hh>


// The call "built-in (op) Object" inverts the couple of arguments; so
// it results in the effective call: "Object (op) built-in."  In the
// definitions of objects, we do not have to handle the possible calls
// "built-in (op) Object".  Furthermore, the built-in value is wrapped
// into a value::scalar_ (which is a value::Scalar); as a consequence,
// the definition of an object should only handle the single case
// "Object (op) Scalar".
// 
// For instance:
//   an expression such as " int * Image<I> "
//   is transformed into   "  I  * scalar_<int> "
//   with the corresponding return type.



// Operators "object OP  built-in" => "object OP  scalar".
//       and "object OP= built-in" => "object OP= scalar".

# define mln_internal_decl_op_obj_bi_(Symb, Name, Builtin)	\
								\
  template <typename O>						\
  mln_trait_op_##Name (O, value::scalar_< Builtin >)		\
  operator Symb (const Object<O>& lhs, const Builtin & rhs);	\
								\
  template <typename O>						\
  O&								\
  operator Symb##= (Object<O>& lhs, const Builtin & rhs);	\
								\
  struct m_a_c_r_o__e_n_d__w_i_t_h__s_e_m_i_c_o_l_u_m_n


# define mln_internal_def_op_obj_bi_(Symb, Name, Builtin)	\
								\
  template <typename O>						\
  mln_trait_op_##Name (O, value::scalar_< Builtin >)		\
  operator Symb (const Object<O>& lhs, const Builtin & rhs)	\
  {								\
    return exact(lhs) Symb value::scalar(rhs);			\
  }								\
								\
  template <typename O>						\
  O&								\
  operator Symb##= (Object<O>& lhs, const Builtin & rhs)	\
  {								\
    return exact(lhs) Symb##= value::scalar(rhs);		\
  }								\
								\
  struct m_a_c_r_o__e_n_d__w_i_t_h__s_e_m_i_c_o_l_u_m_n


// Operator "built-in OP object" => "object OP scalar" iff OP commutes.

# define mln_internal_decl_bi_op_obj_(Symb, Name, Builtin)	\
								\
  template <typename O>						\
  mln_trait_op_##Name (O, value::scalar_< Builtin >)		\
  operator Symb (const Builtin & lhs, const Object<O>& rhs);	\
								\
  struct m_a_c_r_o__e_n_d__w_i_t_h__s_e_m_i_c_o_l_u_m_n

# define mln_internal_def_bi_op_obj_(Symb, Name, Builtin)	\
								\
  template <typename O>						\
  mln_trait_op_##Name (O, value::scalar_< Builtin >)		\
  operator Symb (const Builtin & lhs, const Object<O>& rhs)	\
  {								\
    return exact(rhs) Symb value::scalar(lhs);			\
  }								\
								\
  struct m_a_c_r_o__e_n_d__w_i_t_h__s_e_m_i_c_o_l_u_m_n



// Comparison.

# define mln_internal_decl_op_cmp_(Symb, Name, Builtin)		\
								\
  template <typename O>						\
  mln_trait_op_##Name (O, value::scalar_< Builtin >)		\
  operator Symb (const Object<O>& lhs, const Builtin & rhs);	\
								\
  template <typename O>						\
  mln_trait_op_##Name (value::scalar_< Builtin >, O)		\
  operator Symb (const Builtin & lhs, const Object<O>& rhs);	\
								\
  struct m_a_c_r_o__e_n_d__w_i_t_h__s_e_m_i_c_o_l_u_m_n

# define mln_internal_def_op_cmp_(Symb, Name, Builtin)		\
								\
  template <typename O>						\
  mln_trait_op_##Name (O, value::scalar_< Builtin >)		\
  operator Symb (const Object<O>& lhs, const Builtin & rhs)	\
  {								\
    return exact(lhs) Symb value::scalar(rhs);			\
  }								\
								\
  template <typename O>						\
  mln_trait_op_##Name (value::scalar_< Builtin >, O)		\
  operator Symb (const Builtin & lhs, const Object<O>& rhs)	\
  {								\
    return value::scalar(lhs) Symb exact(rhs);			\
  }								\
								\
  struct m_a_c_r_o__e_n_d__w_i_t_h__s_e_m_i_c_o_l_u_m_n






# define mln_internal_op_obj_builtins_(De, Symb, Name)		\
								\
  mln_internal_##De##_op_obj_bi_(Symb, Name,   signed char);	\
  mln_internal_##De##_op_obj_bi_(Symb, Name, unsigned char);	\
  mln_internal_##De##_op_obj_bi_(Symb, Name,   signed short);	\
  mln_internal_##De##_op_obj_bi_(Symb, Name, unsigned short);	\
  mln_internal_##De##_op_obj_bi_(Symb, Name,   signed int);	\
  mln_internal_##De##_op_obj_bi_(Symb, Name, unsigned int);	\
  mln_internal_##De##_op_obj_bi_(Symb, Name,   signed long);	\
  mln_internal_##De##_op_obj_bi_(Symb, Name, unsigned long);	\
  mln_internal_##De##_op_obj_bi_(Symb, Name, float);		\
  mln_internal_##De##_op_obj_bi_(Symb, Name, double);		\
								\
  struct m_a_c_r_o__e_n_d__w_i_t_h__s_e_m_i_c_o_l_u_m_n


# define mln_internal_builtins_op_obj_(De, Symb, Name)		\
								\
  mln_internal_##De##_bi_op_obj_(Symb, Name,   signed char);	\
  mln_internal_##De##_bi_op_obj_(Symb, Name, unsigned char);	\
  mln_internal_##De##_bi_op_obj_(Symb, Name,   signed short);	\
  mln_internal_##De##_bi_op_obj_(Symb, Name, unsigned short);	\
  mln_internal_##De##_bi_op_obj_(Symb, Name,   signed int);	\
  mln_internal_##De##_bi_op_obj_(Symb, Name, unsigned int);	\
  mln_internal_##De##_bi_op_obj_(Symb, Name,   signed long);	\
  mln_internal_##De##_bi_op_obj_(Symb, Name, unsigned long);	\
  mln_internal_##De##_bi_op_obj_(Symb, Name, float);		\
  mln_internal_##De##_bi_op_obj_(Symb, Name, double);		\
								\
  struct m_a_c_r_o__e_n_d__w_i_t_h__s_e_m_i_c_o_l_u_m_n




// Operator "Builtin minus Object" is a special case.


# define mln_internal_decl_bi_minus_obj_(Builtin)		\
								\
  template <typename O>						\
  mln_trait_op_minus(Builtin, O)				\
  operator - (const Builtin & lhs, const Object<O>& rhs);	\
								\
  struct m_a_c_r_o__e_n_d__w_i_t_h__s_e_m_i_c_o_l_u_m_n


# define mln_internal_def_bi_minus_obj_(Builtin)		\
								\
  template <typename O>						\
  mln_trait_op_minus(Builtin, O)				\
  operator - (const Builtin & lhs, const Object<O>& rhs)	\
  {								\
    return (- exact(rhs)) + value::scalar(lhs);			\
  }								\
								\
  struct m_a_c_r_o__e_n_d__w_i_t_h__s_e_m_i_c_o_l_u_m_n


# define mln_internal_builtins_minus_obj_(De)		\
							\
  mln_internal_##De##_bi_minus_obj_(  signed char);	\
  mln_internal_##De##_bi_minus_obj_(unsigned char);	\
  mln_internal_##De##_bi_minus_obj_(  signed short);	\
  mln_internal_##De##_bi_minus_obj_(unsigned short);	\
  mln_internal_##De##_bi_minus_obj_(  signed int);	\
  mln_internal_##De##_bi_minus_obj_(unsigned int);	\
  mln_internal_##De##_bi_minus_obj_(  signed long);	\
  mln_internal_##De##_bi_minus_obj_(unsigned long);	\
  mln_internal_##De##_bi_minus_obj_(float);		\
  mln_internal_##De##_bi_minus_obj_(double);		\
							\
  struct m_a_c_r_o__e_n_d__w_i_t_h__s_e_m_i_c_o_l_u_m_n




// Operator "Builtin 'div or mod' Object" is a special case.


# define mln_internal_decl_bi_dvmd_obj_(Symb, Name, Builtin)	\
								\
  template <typename O>						\
  mln_trait_op_##Name (value::scalar_< Builtin >, O)		\
  operator Symb (const Builtin & lhs, const Object<O>& rhs);	\
								\
  struct m_a_c_r_o__e_n_d__w_i_t_h__s_e_m_i_c_o_l_u_m_n


# define mln_internal_def_bi_dvmd_obj_(Symb, Name, Builtin)	\
								\
  template <typename O>						\
  mln_trait_op_##Name (value::scalar_< Builtin >, O)		\
  operator Symb (const Builtin & lhs, const Object<O>& rhs)	\
  {								\
    return value::scalar(lhs) / exact(rhs);			\
  }								\
								\
  struct m_a_c_r_o__e_n_d__w_i_t_h__s_e_m_i_c_o_l_u_m_n


# define mln_internal_builtins_dvmd_obj_(De, Symb, Name)	\
								\
  mln_internal_##De##_bi_dvmd_obj_(Symb, Name,   signed char);	\
  mln_internal_##De##_bi_dvmd_obj_(Symb, Name, unsigned char);	\
  mln_internal_##De##_bi_dvmd_obj_(Symb, Name,   signed short);	\
  mln_internal_##De##_bi_dvmd_obj_(Symb, Name, unsigned short);	\
  mln_internal_##De##_bi_dvmd_obj_(Symb, Name,   signed int);	\
  mln_internal_##De##_bi_dvmd_obj_(Symb, Name, unsigned int);	\
  mln_internal_##De##_bi_dvmd_obj_(Symb, Name,   signed long);	\
  mln_internal_##De##_bi_dvmd_obj_(Symb, Name, unsigned long);	\
  mln_internal_##De##_bi_dvmd_obj_(Symb, Name, float);		\
  mln_internal_##De##_bi_dvmd_obj_(Symb, Name, double);		\
								\
  struct m_a_c_r_o__e_n_d__w_i_t_h__s_e_m_i_c_o_l_u_m_n


# define mln_internal_op_builtins_cmp_(De, Symb, Name)		\
								\
  mln_internal_##De##_op_cmp_(Symb, Name,   signed char);	\
  mln_internal_##De##_op_cmp_(Symb, Name, unsigned char);	\
  mln_internal_##De##_op_cmp_(Symb, Name,   signed short);	\
  mln_internal_##De##_op_cmp_(Symb, Name, unsigned short);	\
  mln_internal_##De##_op_cmp_(Symb, Name,   signed int);	\
  mln_internal_##De##_op_cmp_(Symb, Name, unsigned int);	\
  mln_internal_##De##_op_cmp_(Symb, Name,   signed long);	\
  mln_internal_##De##_op_cmp_(Symb, Name, unsigned long);	\
  mln_internal_##De##_op_cmp_(Symb, Name, float);		\
  mln_internal_##De##_op_cmp_(Symb, Name, double);		\
								\
  struct m_a_c_r_o__e_n_d__w_i_t_h__s_e_m_i_c_o_l_u_m_n

// FIXME: What about pointers, arrays, bool, etc.

// FIXME: Mod is not defined for float and double...






# define mln_internal_set_builtin_trait_is_promotion_(Name)				\
											\
  template <typename Bl, typename Br>							\
  struct set_binary_< Name,  mln::value::Built_In, Bl,  mln::value::Built_In, Br >	\
  {											\
    typedef mln_trait_promote(Bl, Br) ret;						\
  };											\
											\
  struct m_a_c_r_o__e_n_d__w_i_t_h__s_e_m_i_c_o_l_u_m_n


# define mln_internal_set_builtin_trait_is_bool_(Name)					\
											\
  template <typename Bl, typename Br>							\
  struct set_binary_< Name,  mln::value::Built_In, Bl,  mln::value::Built_In, Br >	\
  {											\
    typedef bool ret;							       		\
  };											\
											\
  struct m_a_c_r_o__e_n_d__w_i_t_h__s_e_m_i_c_o_l_u_m_n



namespace mln
{

  namespace trait
  {

    // A couple of builtins => promotion...

    mln_internal_set_builtin_trait_is_promotion_(op::plus);
    mln_internal_set_builtin_trait_is_promotion_(op::minus);
    mln_internal_set_builtin_trait_is_promotion_(op::times);
    mln_internal_set_builtin_trait_is_promotion_(op::div);
    mln_internal_set_builtin_trait_is_promotion_(op::mod);

    // ...or for comparisons => bool.

    mln_internal_set_builtin_trait_is_bool_(op::eq);
    mln_internal_set_builtin_trait_is_bool_(op::neq);
    // FIXME: ...

    // FIXME: What about +=, etc.



    template< template <class> class Name,
	      typename B >
    struct set_unary_< Name,
		       mln::value::Built_In, B >
    {
      typedef B ret; // FIXME: Wrong because some types are unsigned!
    };


    // Operators "Object OP Built_In" => "Object OP scalar_"

    template <typename O, typename B>
    struct set_binary_< op::plus,  mln::Object, O,  mln::value::Built_In, B >
    {
      typedef mln_trait_op_plus(O, mln::value::scalar_<B>) ret;
    };

    template <typename O, typename B>
    struct set_binary_< op::minus,  mln::Object, O,  mln::value::Built_In, B >
    {
      typedef mln_trait_op_minus(O, mln::value::scalar_<B>) ret;
    };

    template <typename O, typename B>
    struct set_binary_< op::times,  mln::Object, O,  mln::value::Built_In, B >
    {
      typedef mln_trait_op_times(O, mln::value::scalar_<B>) ret;
    };

    template <typename O, typename B>
    struct set_binary_< op::div,  mln::Object, O,  mln::value::Built_In, B >
    {
      typedef mln_trait_op_div(O, mln::value::scalar_<B>) ret;
    };

    template <typename O, typename B>
    struct set_binary_< op::mod,  mln::Object, O,  mln::value::Built_In, B >
    {
      typedef mln_trait_op_mod(O, mln::value::scalar_<B>) ret;
    };


    // 'Op+' is commutative so "o + b" => "o + scalar(b)".

    template <typename B, typename O>
    struct set_binary_< op::plus,  mln::value::Built_In, B,  mln::Object, O >
    {
      typedef mln_trait_op_plus(O, mln::value::scalar_<B>) ret;
    };

    // Likewise for 'Op*'.

    template <typename B, typename O>
    struct set_binary_< op::times,  mln::value::Built_In, B,  mln::Object, O >
    {
      typedef mln_trait_op_times(O, mln::value::scalar_<B>) ret;
    };


    // 'Op-' is tricky for "b - o" => "(-o) + scalar(b)".

    template <typename B, typename O>
    struct set_binary_< op::minus,  mln::value::Built_In, B,  mln::Object, O >
    {
      typedef mln_trait_op_uminus(O) minus_O;
      typedef mln_trait_op_plus(minus_O, mln::value::scalar_<B>) ret;
    };


    // 'Op/' for "b / o" => "scalar(b) / o".

    template <typename B, typename O>
    struct set_binary_< op::div,  mln::value::Built_In, B,  mln::Object, O >
    {
      typedef mln_trait_op_div(mln::value::scalar_<B>, O) ret;
    };

    // Likewise for 'Op%'.

    template <typename B, typename O>
    struct set_binary_< op::mod,  mln::value::Built_In, B,  mln::Object, O >
    {
      typedef mln_trait_op_mod(mln::value::scalar_<B>, O) ret;
    };

  } // end of namespace mln::trait


  mln_internal_op_obj_builtins_(decl, +, plus);
  mln_internal_op_obj_builtins_(decl, -, minus);
  mln_internal_op_obj_builtins_(decl, *, times);
  mln_internal_op_obj_builtins_(decl, /, div);
  mln_internal_op_obj_builtins_(decl, %, mod);

  // Op+ and op* respectively commute:
  mln_internal_builtins_op_obj_(decl, +, plus);
  mln_internal_builtins_op_obj_(decl, *, times);

  // Op "builtin - object" is special:
  mln_internal_builtins_minus_obj_(decl);


  // Ops "bi / obj" and "bi % obj"
  mln_internal_builtins_dvmd_obj_(decl, /, div);
  mln_internal_builtins_dvmd_obj_(decl, %, mod);


  // Ops "bi CMP obj" and "bi CMP obj"
  mln_internal_op_builtins_cmp_(decl, ==, eq);
  mln_internal_op_builtins_cmp_(decl, !=, neq);
  // FIXME: ...


# ifndef MLN_INCLUDE_ONLY

  mln_internal_op_obj_builtins_(def, +, plus);
  mln_internal_op_obj_builtins_(def, -, minus);
  mln_internal_op_obj_builtins_(def, *, times);
  mln_internal_op_obj_builtins_(def, /, div);
  mln_internal_op_obj_builtins_(def, %, mod);

  // Op+ and op* respectively commute:
  mln_internal_builtins_op_obj_(def, +, plus);
  mln_internal_builtins_op_obj_(def, *, times);

  // Op "builtin - object" is special:
  mln_internal_builtins_minus_obj_(def);

  // Ops "bi / obj" and "bi % obj"
  mln_internal_builtins_dvmd_obj_(def, /, div);
  mln_internal_builtins_dvmd_obj_(def, %, mod);

  // Ops "bi CMP obj" and "bi CMP obj"
  mln_internal_op_builtins_cmp_(def, ==, eq);
  mln_internal_op_builtins_cmp_(def, !=, neq);

  // FIXME: Add less, etc.


# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_VALUE_BUILTIN_OPS_HH
