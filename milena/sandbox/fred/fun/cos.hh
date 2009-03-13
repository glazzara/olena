#ifndef COS_HH
# define COS_HH

# include "unary.hh"
# include <mln/value/concept/floating.hh>
# include <mln/math/acos.hh>
# include <mln/math/cos.hh>

namespace mln
{
  // COS, bijective
  namespace fun
  {
    struct cos : unary<cos> {};
  }

  namespace trait
  {

    namespace next
    {

      template <typename T>
      struct set_unary_<mln::fun::cos, mln::value::Floating, T>
      {
	typedef set_unary_ ret;
	typedef T result;
	typedef T argument;
	typedef T& lvalue;

	static result read(const argument& x)
	{
	  return math::cos(x);
	}

	static void write(lvalue l, const result& x)
	{
	  l = math::acos(x);
	}
      };

    }

  }

}

#endif /* ! COS_HH */