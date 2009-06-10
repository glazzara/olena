// Copyright (C) 2006, 2007, 2009  EPITA Research and Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef MLN_VALUE_BUILTIN_PROMOTIONS_HH
# define MLN_VALUE_BUILTIN_PROMOTIONS_HH

/*! \file
 *
 * \brief Definitions of promotions between built-in types.
 */

# include <mln/trait/promote.hh>
# include <mln/metal/ret.hh>


# define mln_internal_set_promotion_(Builtin1, Builtin2, Result)	\
									\
  template<>								\
  struct set_precise_binary_< promote, Builtin1, Builtin2 >		\
  {									\
    typedef Result ret;							\
  };									\
									\
  struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_u_m_n				\


# define mln_internal_set_promotion_auto_(From, To)	\
							\
  mln_internal_set_promotion_(From, To, To);		\
  mln_internal_set_promotion_(To, From, To);		\
  struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_u_m_n


# define mln_internal_set_promotion_twice_(From, To)	\
							\
  mln_internal_set_promotion_(From, From, To);		\
  struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_u_m_n


# define mln_internal_set_promotion_bi_(From1, From2, To)	\
								\
  mln_internal_set_promotion_(From1, From2, To);		\
  mln_internal_set_promotion_(From2, From1, To);		\
  struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_u_m_n



namespace mln
{

  // Fwd decl.
  namespace value { template <typename B> struct Built_In; }


  namespace trait
  {

    // Builtin binary traits.


    mln_internal_set_promotion_bi_(unsigned char,  signed char,  int);
    mln_internal_set_promotion_bi_(unsigned short, signed char,  int);
    mln_internal_set_promotion_bi_(unsigned int,   signed char,  int);
    mln_internal_set_promotion_bi_(unsigned long,  signed char,  long);

    mln_internal_set_promotion_bi_(unsigned short, signed short, int);
    mln_internal_set_promotion_bi_(unsigned int,   signed short, int);
    mln_internal_set_promotion_bi_(unsigned long,  signed short, long);

    mln_internal_set_promotion_bi_(unsigned long,  signed int,   long);


    mln_internal_set_promotion_twice_(unsigned char,  unsigned);
    mln_internal_set_promotion_twice_(  signed char,  int);
    mln_internal_set_promotion_twice_(unsigned short, unsigned);
    mln_internal_set_promotion_twice_(  signed short, int);
    mln_internal_set_promotion_twice_(unsigned int,   unsigned);
    mln_internal_set_promotion_twice_(  signed int,   int);
    mln_internal_set_promotion_twice_(unsigned long,  unsigned long);
    mln_internal_set_promotion_twice_(  signed long,  long);

    mln_internal_set_promotion_twice_(   float,       float);
    mln_internal_set_promotion_twice_(  double,       double);


    mln_internal_set_promotion_auto_(unsigned char,  unsigned short);
    mln_internal_set_promotion_auto_(unsigned char,    signed short);
    mln_internal_set_promotion_auto_(  signed char,    signed short);

    mln_internal_set_promotion_auto_(unsigned char,  unsigned int);
    mln_internal_set_promotion_auto_(unsigned char,    signed int);
    mln_internal_set_promotion_auto_(  signed char,    signed int);
    mln_internal_set_promotion_auto_(unsigned short, unsigned int);
    mln_internal_set_promotion_auto_(unsigned short,   signed int);
    mln_internal_set_promotion_auto_(  signed short,   signed int);
    mln_internal_set_promotion_auto_(unsigned int,     signed int);

    mln_internal_set_promotion_auto_(unsigned char,  unsigned long);
    mln_internal_set_promotion_auto_(unsigned char,    signed long);
    mln_internal_set_promotion_auto_(  signed char,    signed long);
    mln_internal_set_promotion_auto_(unsigned short, unsigned long);
    mln_internal_set_promotion_auto_(unsigned short,   signed long);
    mln_internal_set_promotion_auto_(  signed short,   signed long);
    mln_internal_set_promotion_auto_(unsigned int,   unsigned long);
    mln_internal_set_promotion_auto_(unsigned int,     signed long);
    mln_internal_set_promotion_auto_(  signed int,     signed long);

    mln_internal_set_promotion_auto_(unsigned char,  float);
    mln_internal_set_promotion_auto_(  signed char,  float);
    mln_internal_set_promotion_auto_(unsigned short, float);
    mln_internal_set_promotion_auto_(  signed short, float);
    mln_internal_set_promotion_auto_(unsigned int,   float);
    mln_internal_set_promotion_auto_(  signed int,   float);
    mln_internal_set_promotion_auto_(unsigned long,  float);
    mln_internal_set_promotion_auto_(  signed long,  float);

    mln_internal_set_promotion_auto_(unsigned char,  double);
    mln_internal_set_promotion_auto_(  signed char,  double);
    mln_internal_set_promotion_auto_(unsigned short, double);
    mln_internal_set_promotion_auto_(  signed short, double);
    mln_internal_set_promotion_auto_(unsigned int,   double);
    mln_internal_set_promotion_auto_(  signed int,   double);
    mln_internal_set_promotion_auto_(unsigned long,  double);
    mln_internal_set_promotion_auto_(  signed long,  double);

    mln_internal_set_promotion_auto_(   float,       double);

  } // end of namespace mln::trait

} // end of namespace mln


#endif // ! MLN_VALUE_BUILTIN_PROMOTIONS_HH
