// Copyright (C) 2005 EPITA Research and Development Laboratory
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

#ifndef METALIC_TYPEDEF_HH
# define METALIC_TYPEDEF_HH

# include <mlc/types.hh>
# include <mlc/bool.hh>


// do not use the macro below if you equip a namespace with properties

# define mlc_equip_namespace_with_typedef()				\
namespace internal							\
{									\
									\
  template <typename type, typename typedef_type>			\
  struct get_typedef							\
  {									\
    typedef mlc::internal::not_found ret;				\
  };									\
									\
  template <typename type, typename typedef_type, typename cond>	\
  struct get_typedef_onlyif						\
  {									\
    typedef mlc::internal::not_found ret;				\
  };									\
									\
}									\
struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_o_n




/*! \macro  mlc_decl_typedef (TypedefName)
**
** Macro to equip mlc with a static mechanism for a typedef retrieval.
**
** FIXME
*/

# define mlc_decl_typedef(TypedefName)							\
namespace  internal									\
{											\
											\
  namespace typedef_									\
  {											\
    struct TypedefName;									\
  }											\
											\
  struct helper_get_typedef__##TypedefName						\
  {											\
    typedef char yes;									\
    struct no { char tmp[2]; };								\
											\
    template <typename type, typename alias = typename type::TypedefName>		\
    struct run_on;									\
											\
    template <typename type>								\
    static yes selector(run_on<type>*);							\
											\
    template <typename type>								\
    static no selector(...);								\
											\
    template <typename type, bool found>						\
    struct result;									\
											\
    template <typename type>								\
    struct result <type, true>								\
    {											\
      typedef typename type::TypedefName ret;						\
    };											\
											\
    template <typename type>								\
    struct result <type, false>								\
    {											\
      typedef mlc::internal::not_found ret;						\
    };											\
  };											\
											\
  template <typename type>								\
  struct get_typedef__##TypedefName							\
  {											\
    typedef  helper_get_typedef__##TypedefName  helper_type;				\
    static const bool found = ( sizeof(helper_type::selector<type>(0)) == 1 );		\
    typedef typename helper_type::result<type, found>::ret ret;				\
  };											\
											\
  template <typename type>								\
  struct get_typedef <type, typedef_::TypedefName>					\
  {											\
    typedef typename get_typedef__##TypedefName <type> ::ret ret;			\
  };											\
											\
											\
  template <typename type, bool cond>							\
  struct helper_get_typedef_onlyif__##TypedefName;					\
											\
  template <typename type>								\
  struct helper_get_typedef_onlyif__##TypedefName <type, false>				\
  {											\
    typedef mlc::internal::not_ok ret;							\
  };											\
											\
  template <typename type>								\
  struct helper_get_typedef_onlyif__##TypedefName <type, true>				\
  {											\
    typedef typename type::TypedefName ret;						\
  };											\
											\
  template <typename type, typename cond>						\
  struct get_typedef_onlyif__##TypedefName						\
    : public helper_get_typedef_onlyif__##TypedefName <type, mlc_bool(cond)>		\
  {											\
    typedef helper_get_typedef_onlyif__##TypedefName <type, mlc_bool(cond)> super;	\
    using super::ret;									\
  };											\
											\
  template <typename type, typename cond>						\
  struct get_typedef_onlyif <type, typedef_::TypedefName, cond>				\
  {											\
    typedef typename get_typedef_onlyif__##TypedefName <type, cond> ::ret ret;		\
  };											\
											\
											\
}											\
struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_o_n



// FIXME: mlc should be equipped for typedef retrieval
// FIXME: and should provide at least facilities for 'ret'

// namespace mlc
// {
//   mlc_equip_namespace_with_typedef();
//   mlc_decl_typedef(ret);
// } // end of namespace mlc




// sample code to understand why mlc_typedef_onlyif_of is required
// in particular situations:

//     #include <mlc/typedef.hh>

//     mlc_equip_namespace_with_typedef();
//     mlc_decl_typedef(ret);

//     struct toto
//     {
//       typedef float does_not_exist;
//     };

//     template <class T>
//     struct ok
//     {
// 	 typedef double ret;
// 	 typedef typename T::does_not_exist err;
//     };

//     template <class T>
//     struct ko
//     {
//     };

//     template <class T>
//     void foo()
//     {
// 	 typedef mlc_typedef_of(T, ret) type;
//     }

//     template <class T, class B>
//     void bar()
//     {
// 	 typedef mlc_typedef_onlyif_of(T, ret, B) type;
//     }

//     int main()
//     {
// 	 // code that does not compile:
// 	 // foo< ok<int> >();
// 	 // bar< ok<int>, mlc::true_type >();

// 	 // code that works:
// 	 foo< ok<toto> >();
// 	 bar< ok<toto>, mlc::true_type >();
// 	 bar< ok<int>, mlc::false_type >();
//     }


/*! \macro  mlc_typedef_of( FromType, TypedefName )
**
** FIXME
** Sample use: mlc_typedef_of(std::vector<int>, value_type)
*/
# define mlc_typedef_of_(FromType, TypedefName)		\
internal::get_typedef__##TypedefName <FromType>::ret

# define mlc_typedef_of(FromType, TypedefName)	\
typename mlc_typedef_of_(FromType, TypedefName)



# define mlc_typedef_onlyif_of_(FromType, TypedefName, Condition)	\
internal::get_typedef_onlyif__##TypedefName <FromType, Condition>::ret

# define mlc_typedef_onlyif_of(FromType, TypedefName, Condition)	\
typename mlc_typedef_onlyif_of_(FromType, TypedefName, Condition)



#endif // ! METALIC_TYPEDEF_HH
