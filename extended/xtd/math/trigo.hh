// Copyright (C) 2002, 2005, 2006  EPITA Research and Development Laboratory
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
// the Free Software Foundation, 59 Temple Place - Suite 330, Boston,
// MA 02111-1307, USA.
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

#ifndef EXTENDED_MATH_TRIGO_HH
# define EXTENDED_MATH_TRIGO_HH

# include <cmath>

# include <mlc/assert.hh>
# include <mlc/logic.hh>
# include <mlc/cmp.hh>

# include <xtd/abstract/plain_fun.hh>
# include <xtd/mfun.hh>





// FIXME: macro doc!

# define xtd_decl_trigo_fun(FunName)									\
													\
  template <typename T> struct FunName##_;								\
													\
  template <typename T>											\
  struct fun_traits_< FunName##_<T> >									\
													\
    : private mlc::assert_< mlc::or_list_< mlc::eq_<T, float>,						\
					   mlc::eq_<T, double>,						\
					   mlc::eq_<T, long double> >,					\
			    xtd::ERROR::TRIGONOMETRY_ONLY_RUNS_ON_float_OR_double_OR_long_double >	\
  {													\
    typedef mlc::dummy arg_type;									\
    typedef mlc::dummy res_type;									\
  };													\
													\
  template <>												\
  struct fun_traits_< FunName##_<float> >								\
  {													\
    typedef float arg_type;										\
    typedef float res_type;										\
  };													\
													\
  template <>												\
  struct fun_traits_< FunName##_<double> >								\
  {													\
    typedef double arg_type;										\
    typedef double res_type;										\
  };													\
													\
  template <>												\
  struct fun_traits_< FunName##_<long double> >								\
  {													\
    typedef long double arg_type;									\
    typedef long double res_type;									\
  };													\
													\
  template <typename T>											\
  struct FunName##_ : public abstract::trigo_< T, FunName##_<T> >					\
  {													\
    typedef FunName##_<T> self;										\
													\
    xtd_res(self) impl_op(const T& arg) const								\
    {													\
      return std::FunName(arg);										\
    }													\
  };													\
													\
  const FunName##_<float> f##FunName;									\
  const FunName##_<double> d##FunName;									\
  const FunName##_<long double> ld##FunName;								\
													\
  typedef m1fun_<FunName##_> FunName##_type;								\
  const FunName##_type FunName;										\
													\
  struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_o_n



namespace xtd
{

  namespace ERROR
  {

    struct TRIGONOMETRY_ONLY_RUNS_ON_float_OR_double_OR_long_double;

  } // end of namespace xtd::ERROR


  namespace abstract
  {

    template <typename T, typename E>
    struct trigo_ :

      private mlc::assert_< mlc::or_list_< mlc::eq_<T, float>,
					   mlc::eq_<T, double>,
					   mlc::eq_<T, long double> >,
			    xtd::ERROR::TRIGONOMETRY_ONLY_RUNS_ON_float_OR_double_OR_long_double >,

      public abstract::plain_nary_fun_< 1, E >
    {
    protected:
      trigo_() {}
    };


  } // end of namespace xtd::abstract


  xtd_decl_trigo_fun(cos);
  xtd_decl_trigo_fun(sin);
  xtd_decl_trigo_fun(tan);

  xtd_decl_trigo_fun(acos);
  xtd_decl_trigo_fun(asin);
  xtd_decl_trigo_fun(atan);

  xtd_decl_trigo_fun(cosh);
  xtd_decl_trigo_fun(sinh);
  xtd_decl_trigo_fun(tanh);


} // end of namespace xtd



#endif // ! EXTENDED_MATH_COS_HH
