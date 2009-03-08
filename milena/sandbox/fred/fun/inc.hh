#ifndef INC_HH
# define INC_HH

# include "unary.hh"

namespace mln
{
  // INC, bijective
  namespace fun
  {
    template <typename T>
    struct inc : unary<inc, T> {};
  }

  namespace trait
  {
    template <typename T>
    struct set_unary_<fun::inc, mln::value::Scalar, T>
    {
      typedef set_unary_ ret;
      typedef T result;
      typedef T argument;
      typedef T& lvalue;
      
      static result read(const argument& x)
      {
	return x + 1;
      }
      
      static void write(lvalue l, const result& r)
      {
	l = r - 1;
      }
    };
  } 
  
  namespace meta
  {
    typedef unary<fun::inc> inc;
  }
}

#endif /* ! INC_HH */