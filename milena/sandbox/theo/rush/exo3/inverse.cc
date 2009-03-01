#include <mln/fun/all.hh>

// Some definitions
struct not_found {};
struct impossible {};

namespace inverse
{
  template <typename F>
  struct solve_inverse
  {
    typedef not_found ret;
    typedef not_found inversekind;
  };

  struct FInvKind_
  {
    struct lazy
    {
    };

    struct computed
    {
    };

    struct computed_w
    {
    };

  };

  namespace decompose
  {
    template <typename Class, typename Member>
    struct Functor;

    template <typename Class, typename Res, typename Arg>
    struct Functor<Class, Result (Class::*)(Arg)>
    {
      typedef Res fun_result;
      typedef Arg fun_argument;
    };

    template <typename T>
    struct Dec
    {
      typedef Functor<T, &(const T)::operator() > const_tool;
      typedef Functor<T, &T::operator() > mutab_tool;

      typedef tool::fun_result   result;
      typedef tool::fun_argument argument;

      T inst_;

      result
      operator() (argument a) const
      {
	return inst_(a);
      };

      result
      operator() (argument a)
      {
	return inst_(a);
      };
    };

  }

  template <typename F, typename F_1 = typename solve_inverse<F>::ret,
	    typename InvKind = typename solve_inverse<F>::InvKind>
  struct invertible : typename solve_kind<InvKind, invertible>::ret
  {

    typedef invertible<F_1, F> inverse;
  };
}

int main()
{
  using namespace mln;


};