#ifndef NORM_HH
# define NORM_HH

# include "unary.hh"
# include <mln/norm/all.hh>

namespace mln
{
  // NORMS, reversible
  namespace fun
  {
    namespace norm
    {
      template <typename T>
      struct l1 : unary<l1, T> {};

      template <typename T>
      struct l2 : unary<l2, T> {};

      template <typename T>
      struct linfty : unary<linfty, T> {};
    }
  }

  namespace trait
  {
    template <unsigned n, typename T>
    struct set_precise_unary_<mln::fun::norm::l1, mln::algebra::vec<n, T> >
    {
      typedef set_precise_unary_ ret;
      typedef mln::algebra::vec<n, T> argument;
      typedef argument& lvalue;
      typedef mln_sum_product(argument,argument) result;
      
      static result read(const argument& x)
      {
	return mln::norm::l1(x);
      }
      
      static void write(lvalue l, const result& x)
      {
	l = l / read(l) * x;
      }
    };

    template <unsigned n, typename T>
    struct set_precise_unary_<mln::fun::norm::l2, mln::algebra::vec<n, T> >
    {
      typedef set_precise_unary_ ret;
      typedef mln::algebra::vec<n, T> argument;
      typedef argument& lvalue;
      typedef mln_sum_product(argument,argument) result;
      
      static result read(const argument& x)
      {
	return mln::norm::l2(x);
      }
      
      static void write(lvalue l, const result& x)
      {
	l = l / read(l) * x;
      }
    };

    template <unsigned n, typename T>
    struct set_precise_unary_<mln::fun::norm::linfty, mln::algebra::vec<n, T> >
    {
      typedef set_precise_unary_ ret;
      typedef mln::algebra::vec<n, T> argument;
      typedef argument& lvalue;
      typedef mln_sum_product(argument,argument) result;
      
      static result read(const argument& x)
      {
	return mln::norm::linfty(x);
      }
      
      static void write(lvalue l, const result& x)
      {
	l = l / read(l) * x;
      }
    };
  } 
  
  namespace meta
  {
    namespace norm
    {
      typedef unary<mln::fun::norm::l1> l1;
      typedef unary<mln::fun::norm::l2> l2;
      typedef unary<mln::fun::norm::linfty> linfty;
    }
  }
}

#endif /* ! NORM_HH */