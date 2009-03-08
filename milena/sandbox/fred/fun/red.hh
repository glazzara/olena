#ifndef RED_HH
# define RED_HH

# include "unary.hh"
# include <mln/value/rgb.hh>

namespace mln
{
  // RED, assignable
  namespace fun
  {
    template <typename T>
    struct red : unary<red, T> {};
  }

  namespace trait
  {
    template <unsigned n>
    struct set_precise_unary_<fun::red, mln::value::rgb<n> >
    {
      typedef set_precise_unary_ ret;
      typedef mln::value::rgb<n> argument;
      typedef typename argument::red_t result;
      typedef argument& lvalue;
      
      static result read(const argument& x)
      {
	return x.red();
      }
      
      static void write(lvalue l, const result& r)
      {
	l.red() = r;
      }
    };
  }
  
  namespace meta
  {
    typedef unary<fun::red> red;
  }
}

#endif /* ! RED_HH */