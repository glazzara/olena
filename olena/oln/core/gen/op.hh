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

#ifndef OLN_CORE_GEN_OP_HH
# define OLN_CORE_GEN_OP_HH

# include <oln/core/internal/special_op.hh>
# include <oln/core/internal/category_of.hh>
# include <oln/core/concept/function.hh>
# include <oln/core/concept/value.hh>
# include <oln/core/gen/inplace.hh>



// Macro.

# define oln_decl_op_(OpName, Lconcept, OpSym, Rconcept)		\
									\
  template <typename L, typename R>					\
  op_<L, OpName, R>							\
  operator OpSym (Lconcept<L>& lhs, Rconcept<R>& rhs)			\
  {									\
    op_<L, OpName, R> tmp(exact(lhs), exact(rhs));			\
    return tmp;								\
  }									\
									\
  template <typename L, typename R>					\
  op_<L, OpName, const R>						\
  operator OpSym (Lconcept<L>& lhs, const Rconcept<R>& rhs)		\
  {									\
    op_<L, OpName, const R> tmp(exact(lhs), exact(rhs));		\
    return tmp;								\
  }									\
									\
  template <typename L, typename R>					\
  op_<const L, OpName, R>						\
  operator OpSym (const Lconcept<L>& lhs, Rconcept<R>& rhs)		\
  {									\
    op_<const L, OpName, R> tmp(exact(lhs), exact(rhs));		\
    return tmp;								\
  }									\
									\
  template <typename L, typename R>					\
  op_<const L, OpName, const R>						\
  operator OpSym (const Lconcept<L>& lhs, const Rconcept<R>& rhs)	\
  {									\
    op_<const L, OpName, const R> tmp(exact(lhs), exact(rhs));		\
    return tmp;								\
  }									\
									\
  struct e_n_d___w_i_t_h___a___s_e_m_i___c_o_l_u_m_n



# define oln_decl_inplace_image_op(OpName, OpSym, Rconcept)	\
								\
  template <typename L, typename R>				\
  inplace_< op_<L, OpName, R> >					\
  operator OpSym (inplace_<L> lhs, Rconcept<R>& rhs)		\
  {								\
    mlc::assert_< mlc_is_a(L, Mutable_Image) >::check();	\
    op_<L, OpName, R> tmp(lhs.unwrap(), exact(rhs));		\
    return inplace(tmp);					\
  }								\
								\
  struct e_n_d___w_i_t_h___a___s_e_m_i___c_o_l_u_m_n



// FIXME: Split into different files so that the client can explicitly include
// what is really needed...

# define oln_decl_op_extended_by(Lconcept, Rconcept)   oln_decl_op_( extended_by,   Lconcept, +, Rconcept) 
# define oln_decl_op_such_as(Lconcept, Rconcept)       oln_decl_op_( such_as,       Lconcept, |, Rconcept) 
# define oln_decl_op_restricted_to(Lconcept, Rconcept) oln_decl_op_( restricted_to, Lconcept, |, Rconcept) 
# define oln_decl_op_over(Lconcept, Rconcept)          oln_decl_op_( over,          Lconcept, /, Rconcept) 
# define oln_decl_op_applied_on(Lconcept, Rconcept)    oln_decl_op_( applied_on,    Lconcept, <<, Rconcept) 
# define oln_decl_op_through(Lconcept, Rconcept)       oln_decl_op_( through,       Lconcept, >>, Rconcept) 
// ...



namespace oln
{


  /// \{
  /// Operator Names.

  struct extended_by;
  struct such_as;
  struct restricted_to;
  struct over;
  struct applied_on;
  struct through;

  /// \}




  // Fwd decl.
  template <typename L, typename OpName, typename R> class op_;


  /// Virtual types.
  template <typename L, typename OpName, typename R>
  struct vtypes< op_<L, OpName, R> >
  {
  };


  /// Super type.

# define  super \
  internal::special_op_< oln_category_of_(L), L, OpName, oln_category_of_(R), R >

  template <typename L, typename OpName, typename R>
  struct super_trait_< op_<L, OpName, R> >
  {
    typedef super ret;
  };


  /// Class for result of "L op R".

  template <typename L, typename OpName, typename R>
  class op_ : public super
  {
  public:
    op_();
    op_(L& l, R& r);
    template <typename D>
    op_(const internal::initializer_<D>& data);

  }; // end of op_<L, OpName, R>



# ifndef OLN_INCLUDE_ONLY

  template <typename L, typename OpName, typename R>
  op_<L, OpName, R>::op_()
  {
  }

  template <typename L, typename OpName, typename R>
  op_<L, OpName, R>::op_(L& l, R& r)
    : super (l, r)
  {
  }

  template <typename L, typename OpName, typename R>
  template <typename D>
  op_<L, OpName, R>::op_(const internal::initializer_<D>& data)
  {
    bool op_ok = this->super::init__(data);
    postcondition(op_ok);
  }

# endif // ! OLN_INCLUDE_ONLY

# undef super

} // end of namespace oln


#endif // ! OLN_CORE_GEN_OP_HH
