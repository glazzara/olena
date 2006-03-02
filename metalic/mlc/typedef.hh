// Copyright (C) 2005, 2006 EPITA Research and Development Laboratory
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

# include <mlc/flags.hh>
# include <mlc/bool.hh>
# include <mlc/pair.hh>
# include <mlc/cmp.hh>


/*! \macro mlc_decl_typedef(TypedefName)
**
** This macro is for declaring the use of a typedef embedded in
** classes.  After having declaring a typedef, accessing the typedef
** is performed through the mlc_typedef macro.  The main difference
** between the classic access, "Type::TypedefName", and its
** equivalent, "mlc_typedef(Type, TypedefName)", is that the latter
** also works when the access is ill-formed.  Put differently,
** mlc_typedef also works when Type does not have TypedefName in its
** interface; the result is then the special type mlc::not_found.
**
**
** Sample use: Let us declare the "value_type" typedef name in the
** global namespace.
**
**   mlc_decl_typedef(value_type);
**
** The following couple of typedef access:
**
**      typename std::vector<int>::value_type
**   mlc_typedef(std::vector<int>, value_type)
**
** are exactly equivalent and give "int" as result.  However, with
** "type" being "std::pair<int, int>", the access:
**
**   typename type::value_type
**
** is invalid and does not compile (because there is no value_type
** defined in the interface of std::pair), wheras the replacement
** access:
**
**   mlc_typedef(type, value_type)
**
** does compile and gives mlc::not_found.
**
**
** When the declaration mlc_decl_typedef is located in a particular
** namespace, mlc_typedef can be used within this namespace and its
** sub-namespaces.  However, it is highly recommended to define a
** particular access macro if you want to call mlc_typedef from
** another namespace (the global one for instance).
**
**   namespace my {
**   
**     struct foo {
**       typedef float value_type;
**     };
**   
**     mlc_decl_typedef(value_type);
**   
**     namespace sub {
**   
**       template <class T>
**       void bar()
**       {
**         mlc_typedef(foo, value_type) tmp; // ok
**       }
**   
**     } // end of namespace my::sub
**   
**   } // end of namespace my
**   
**   int main()
**   {
**     {
**       mlc_typedef_(my::foo, value_type) tmp; // KO
**     }
**     {
**       using my::typedef_;
**       mlc_typedef_(my::foo, value_type) tmp; // ok
**     }
**   }
**
** better:
**
**  #define my_typedef(Type, TypedefName)  mlc_typedef_in(my, Type, TypedefName)
**  #define my_typedef_(Type, TypedefName) mlc_typedef_in_(my, Type, TypedefName)
**
** and then:
**
**   int main()
**   {
**     my_typedef_(my::foo, value_type) tmp; // ok
**   }
**
**
** Design notes:
** The declaration of a typedef name leads to the creation of the type
** "typedef_::TypedefName" located in the namespace where the
** declaration has been performed.  This type can be used as a flag to
** designate a particular class typedef, whatever the classes it can
** be applied to.
**
** \see mlc_typedef(Type, TypedefName)
*/

# define mlc_decl_typedef(TypedefName)				\
								\
namespace typedef_ {						\
								\
  namespace internal {						\
								\
    struct TypedefName						\
    {								\
      typedef char yes;						\
      struct no { char tmp[2]; };				\
								\
      template <class T,					\
		typename alias = typename T::TypedefName>	\
      struct run_on;						\
								\
      template <class T>					\
      static yes selector(run_on<T>*);				\
								\
      template <class T>					\
      static no selector(...);					\
								\
      template <class T, bool found_>				\
      struct result;						\
								\
      template <class T>					\
      struct result <T, true> {					\
	typedef typename T::TypedefName ret;			\
      };							\
								\
      template <class T>					\
      struct result <T, false> {				\
	typedef mlc::not_found ret;				\
      };							\
								\
      template <class T, bool found_>				\
      struct result2;						\
								\
      template <class T>					\
      struct result2 <T, true> {				\
	typedef mlc::pair_<mlc::found,				\
			   typename T::TypedefName> ret;	\
      };							\
								\
      template <class T>					\
      struct result2 <T, false> {				\
	typedef mlc::pair_<mlc::not_found,			\
			   mlc::dummy> ret;			\
      };							\
								\
    };								\
								\
  }								\
								\
  struct TypedefName						\
  {								\
    template <class T>						\
    struct from_						\
    {								\
    private:							\
      typedef internal::TypedefName helper_;			\
      enum {							\
	found_ =						\
	(sizeof(helper_::selector<T>(0)) == 1)			\
      };							\
    public:							\
      typedef							\
      typename helper_::result<T, found_>::ret			\
      ret;							\
								\
      typedef							\
      typename helper_::result2<T, found_>::ret			\
      ret2;							\
    };								\
								\
    template <class T, bool b>					\
    struct from_onlyif_;					\
								\
								\
    template <class T>						\
    struct from_onlyif_ <T, true>				\
    {								\
      typedef typename from_<T>::ret ret;			\
    };								\
								\
    template <class T>						\
    struct from_onlyif_ <T, false>				\
    {								\
      typedef mlc::dummy ret;					\
    };								\
								\
  private:							\
    TypedefName() {}						\
  };								\
								\
}								\
								\
struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_o_n			\


/*! \macro mlc_typedef(Type, TypedefName)
**
** FIXME: doc
*/

# define mlc_typedef(Type, TypedefName) \
   typename typedef_::TypedefName::from_<Type>::ret

# define mlc_typedef_(Type, TypedefName) \
   typedef_::TypedefName::from_<Type>::ret



/*! \macro mlc_typedef_in(Namespace, Type, TypedefName)
**
** FIXME: doc
*/

# define mlc_typedef_in(Namespace, Type, TypedefName) \
   typename Namespace::typedef_::TypedefName::from_<Type>::ret

# define mlc_typedef_in_(Namespace, Type, TypedefName) \
   Namespace::typedef_::TypedefName::from_<Type>::ret



/*! \macro mlc_typedef_onlyif(Type, TypedefName, Bexpr)
**
** FIXME: doc
*/

# define mlc_typedef_onlyif(Type, TypedefName, Bexpr) \
   typename typedef_::TypedefName::from_onlyif_<Type, mlc_bool(Bexpr)>::ret

# define mlc_typedef_onlyif_(Type, TypedefName, Bexpr) \
   typedef_::TypedefName::from_onlyif_<Type, mlc_bool(Bexpr)>::ret


#endif // ! METALIC_TYPEDEF_HH
