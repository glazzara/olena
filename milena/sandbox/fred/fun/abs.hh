#ifndef ABS_HH
# define ABS_HH

# include "unary.hh"
# include <mln/value/concept/scalar.hh>
# include <mln/math/abs.hh>

namespace mln
{
  // ABS, pure
  namespace fun
  {
    template <typename T>
    struct abs : unary<abs, T> {};
  }

  namespace trait
  {
    template <typename T>
    struct set_unary_<mln::fun::abs, mln::value::Scalar, T>
    {
      typedef set_unary_ ret;
      typedef T result;
      typedef T argument;

      static result read(const argument& x)
      {
	return math::abs(x);
      }
    };
  }

  namespace meta
  {
    typedef unary<mln::fun::abs> abs;
  }
}

#endif /* ! ABS_HH */