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

#ifndef OLN_CORE_INTERNAL_MACRO_EXTENDED_BY_HH
# define OLN_CORE_INTERNAL_MACRO_EXTENDED_BY_HH


# include <oln/core/gen/inplace.hh>


# define oln_decl_op_image_extended_by_const(Concept)		\
								\
  template <typename I, typename E>				\
  op_<const I, extended_by, const E>				\
  operator + (const Image<I>& ima, const Concept<E>& ext);	\
								\
  template <typename I, typename E>				\
  op_<I, extended_by, const E>					\
  operator + (Mutable_Image<I>& ima, const Concept<E>& ext);	\
								\
  template <typename I, typename E>				\
  inplace_< op_<I, extended_by, const E> >			\
  operator + (inplace_<I> ima, const Concept<E>& ext);		\
								\
  struct e_n_d___w_i_t_h___s_e_m_i_c_o_l_u_m_n



# define oln_decl_op_image_extended_by_mutable(Concept)	\
							\
  template <typename I, typename E>			\
  op_<const I, extended_by, E>				\
  operator + (const Image<I>& ima, Concept<E>& ext);	\
							\
  template <typename I, typename E>			\
  op_<I, extended_by, E>				\
  operator + (Mutable_Image<I>& ima, Concept<E>& ext);	\
							\
  template <typename I, typename E>			\
  inplace_< op_<I, extended_by, E> >			\
  operator + (inplace_<I> ima, Concept<E>& ext);	\
							\
  struct e_n_d___w_i_t_h___s_e_m_i_c_o_l_u_m_n



# define oln_def_op_image_extended_by_const(Concept)			\
									\
  template <typename I, typename E>					\
  op_<const I, extended_by, const E>					\
  operator + (const Image<I>& ima, const Concept<E>& ext)		\
  {									\
    op_<const I, extended_by, const E> tmp(exact(ima), exact(ext));	\
    return tmp;								\
  }									\
									\
  template <typename I, typename E>					\
  op_<I, extended_by, const E>						\
  operator + (Mutable_Image<I>& ima, const Concept<E>& ext)		\
  {									\
    op_<I, extended_by, const E> tmp(exact(ima), exact(ext));		\
    return tmp;								\
  }									\
									\
  template <typename I, typename E>					\
  inplace_< op_<I, extended_by, const E> >				\
  operator + (inplace_<I> ima, const Concept<E>& ext)			\
  {									\
    return (ima.unwrap() + ext).inplace();				\
  }									\
									\
  struct e_n_d___w_i_t_h___s_e_m_i_c_o_l_u_m_n



# define oln_def_op_image_extended_by_mutable(Concept)		\
								\
  template <typename I, typename E>				\
  op_<const I, extended_by, E>					\
  operator + (const Image<I>& ima, Concept<E>& ext)		\
  {								\
    op_<const I, extended_by, E> tmp(exact(ima), exact(ext));	\
    return tmp;							\
  }								\
								\
  template <typename I, typename E>				\
  op_<I, extended_by, E>					\
  operator + (Mutable_Image<I>& ima, Concept<E>& ext)		\
  {								\
    op_<I, extended_by, E> tmp(exact(ima), exact(ext));		\
    return tmp;							\
  }								\
								\
  template <typename I, typename E>				\
  inplace_< op_<I, extended_by, E> >				\
  operator + (inplace_<I> ima, Concept<E>& ext)			\
  {								\
    return (ima.unwrap() + ext).inplace();			\
  }								\
								\
  struct e_n_d___w_i_t_h___s_e_m_i_c_o_l_u_m_n


#endif // ! OLN_CORE_INTERNAL_MACRO_EXTENDED_BY_HH
