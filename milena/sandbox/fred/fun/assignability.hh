#ifndef ASSIGNABILITY_HH
# define ASSIGNABILITY_HH

# include <mln/metal/bexpr.hh>
# include <mln/metal/if.hh>

#define mln_trait_fun_is_assignable(Fun) typename mln::trait::fun::is_assignable< Fun >::ret
#define mln_trait_fun_is_assignable_(Fun) mln::trait::fun::is_assignable< Fun >::ret
#define mln_trait_fun_is_assignable__1comma(A, B) mln_trait_fun_is_assignable(A, B)
#define mln_trait_fun_is_assignable__1comma_(A, B) mln_trait_fun_is_assignable_(A, B)

#define mln_trait_fun_is_parametrable(Fun) typename mln::trait::fun::is_parametrable< Fun >::ret
#define mln_trait_fun_is_parametrable_(Fun) mln::trait::fun::is_parametrable< Fun >::ret

#define mln_trait_fun_lvalue(Fun) typename mln::trait::fun::get_lvalue< Fun >::ret
#define mln_trait_fun_param(Fun) typename mln::trait::fun::get_param< Fun >::ret
namespace mln
{

  namespace trait
  {

    namespace fun
    {

      namespace internal
      {

	namespace introspect
	{

	  template <typename T>
	  struct except_void_t
	  {
	    typedef void ret;
	  };

	  template <>
	  struct except_void_t<void>;

	  template <typename T, typename V>
	  struct has_lvalue_t
	  {
	    typedef metal::false_ ret;
	    typedef void type;
	  };

	  template <typename T>
	  struct has_lvalue_t<T, typename except_void_t<typename T::lvalue>::ret>
	  {
	    typedef metal::true_ ret;
	    typedef typename T::lvalue type;
	  };

	  template <typename T, typename V>
	  struct has_param_t
	  {
	    typedef metal::false_ ret;
	    typedef void type;
	  };

	  template <typename T>
	  struct has_param_t<T, typename except_void_t<typename T::param>::ret>
	  {
	    typedef metal::true_ ret;
	    typedef typename T::param type;
	  };

	}

      } // end of namespace mln::trait::fun::internal

      template <typename F>
      struct is_assignable
      {
	typedef typename internal::introspect::has_lvalue_t<F, void>::ret ret;
      };

      template <typename F>
      struct is_parametrable
      {
	typedef typename internal::introspect::has_param_t<F, void>::ret ret;
      };

      template <typename F>
      struct get_lvalue
      {
	typedef typename internal::introspect::has_lvalue_t<F, void>::type ret;
      };

      template <typename F>
      struct get_param
      {
	typedef typename internal::introspect::has_param_t<F, void>::type ret;
      };

    } // end of namespace mln::trait::fun

  } // end of namespace mln::trait

} // end of namespace mln

#endif // ! ASSIGNABILITY_HH