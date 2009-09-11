#include <iostream>
#include "frac.hh"

//
// TEST INSTANTIATION
//
void test_instantiation_without_argument()
{
  const mln::algebra::frac<int> frac;

  std::cout << "frac<T> frac           : ok" << std::endl; 
}

void test_instantiation_numerator_denominator()
{
  const mln::algebra::frac<int> frac(-2,5);

  std::cout << "frac<T> frac(n,d)      : ok" << std::endl; 
}

void test_instantiation_frac()
{
  const mln::algebra::frac<int>  ref(-2,5);
  const mln::algebra::frac<int>  frac = ref;

  mln_assertion(ref == frac);

  std::cout << "frac<T> frac = frac    : ok" << std::endl;
}

void test_instantiation_zero()
{
  const mln::algebra::frac<int> ref(0,1);
  const mln::algebra::frac<int> frac = mln::literal::zero;

  mln_assertion(ref == frac);
  std::cout << "frac<T> frac = zero    : ok" << std::endl; 
}

void test_instantiation_one()
{
  const mln::algebra::frac<int> ref(1,1);
  const mln::algebra::frac<int> frac = mln::literal::one;

  mln_assertion(ref == frac);
  std::cout << "frac<T> frac = one     : ok" << std::endl; 
}

void test_instantiation_integer()
{
  const mln::algebra::frac<int> ref(5,1);
  const mln::algebra::frac<int> frac = 5;

  mln_assertion(ref == frac);
  std::cout << "frac<T> frac = integer : ok" << std::endl;
}


//
// TEST FRAC_BASE_
//
void test_frac_base_accessor()
{
  const int                     ref1 = 1;
  const int                     ref2 = 2;
  const mln::math::sign_t       ref3 = mln::math::negative;
  const float                   ref4 = -0.5;
  const mln::algebra::frac<int> frac(-1, 2);

  mln_assertion(ref1 == frac.numerator());
  mln_assertion(ref2 == frac.denominator());
  mln_assertion(ref3 == frac.sign());
  mln_assertion(ref4 == frac.eval());

  std::cout << "frac.numerator()       : ok" << std::endl;
  std::cout << "frac.denominator()     : ok" << std::endl;
  std::cout << "frac.sign()            : ok" << std::endl;
  std::cout << "frac.eval()            : ok" << std::endl;
}

void test_frac_base_manipulator()
{
  const int                     ref1 = 5;
  const int                     ref2 = 3;
  const mln::algebra::frac<int> frac(-25,15);
  
  mln_assertion(ref1 == frac.numerator());
  mln_assertion(ref2 == frac.denominator());
  std::cout << "frac.indicible()       : ok" << std::endl;
}


//
// TEST ASSIGNEMENT
//
void test_assignement_zero()
{
  const mln::algebra::frac<int> ref(0,1);
  mln::algebra::frac<int>       frac;

  frac = mln::literal::zero;
  mln_assertion(ref == frac);

  std::cout << "frac = zero            : ok" << std::endl; 
}

void test_assignement_one()
{
  const mln::algebra::frac<int> ref(1,1);
  mln::algebra::frac<int>       frac;

  frac = mln::literal::one;
  mln_assertion(ref == frac);

  std::cout << "frac = one             : ok" << std::endl; 
}

void test_assignement_frac()
{
  const mln::algebra::frac<int> ref(-2,3);
  mln::algebra::frac<int>       frac;

  frac = ref;
  mln_assertion(ref == frac);

  std::cout << "frac = frac            : ok" << std::endl; 
}

void test_assignement_integer()
{
  const mln::algebra::frac<int> ref(-5,1);
  mln::algebra::frac<int>       frac;

  frac = -5;
  mln_assertion(ref == frac);

  std::cout << "frac = integer         : ok" << std::endl;
}


//
// TEST COMPARAISON
//
void test_comparaison_equal_to()
{
  const mln::algebra::frac<int> op1(4,6);
  const mln::algebra::frac<int> op2(6,4);

  mln_assertion(op1 == op1);
  mln_assertion(!(op1 == op2));

  std::cout << "frac == frac           : ok" << std::endl;
}

void test_comparaison_not_equal_to()
{
  const mln::algebra::frac<int> op1(4,6);
  const mln::algebra::frac<int> op2(6,4);

  mln_assertion(!(op1 != op1));
  mln_assertion(op1 != op2);

  std::cout << "frac != frac           : ok" << std::endl;
}

void test_comparaison_less_equal_than()
{
  const mln::algebra::frac<int> op1(1,5);
  const mln::algebra::frac<int> op2(2,5);

  mln_assertion(op1 <= op2);
  mln_assertion(op2 <= op2);
  mln_assertion(!(op2 <= op1));

  std::cout << "frac <= frac           : ok" << std::endl;
}

void test_comparaison_less_than()
{
  const mln::algebra::frac<int> op1(1,5);
  const mln::algebra::frac<int> op2(2,5);

  mln_assertion(op1 < op2);
  mln_assertion(!(op2 < op2));
  mln_assertion(!(op2 < op1));

  std::cout << "frac <  frac           : ok" << std::endl;
}

void test_comparaison_greater_than()
{
  const mln::algebra::frac<int> op1(2,5);
  const mln::algebra::frac<int> op2(1,5);

  mln_assertion(op1 > op2);
  mln_assertion(!(op2 > op2));
  mln_assertion(!(op2 > op1));

  std::cout << "frac >  frac           : ok" << std::endl;
}

void test_comparaison_greater_equal_than()
{
  const mln::algebra::frac<int> op1(2,5);
  const mln::algebra::frac<int> op2(1,5);

  mln_assertion(op1 >= op2);
  mln_assertion(op2 >= op2);
  mln_assertion(!(op2 >= op1));

  std::cout << "frac >= frac           : ok" << std::endl;
}

//
// TEST FRAC ARITHMETIC
//
void test_frac_arithmetic_uminus()
{
  const mln::algebra::frac<int> ref(-1,4);
  const mln::algebra::frac<int> op1(1,4);
  
  mln_assertion(ref == (-op1));
  std::cout << "-frac                  : ok" << std::endl; 
}

void test_frac_arithmetic_plus()
{
  const mln::algebra::frac<int> ref1(6,4);
  const mln::algebra::frac<int> ref2(10,4);
  const mln::algebra::frac<int> ref3(14,4);
  const mln::algebra::frac<int> op1(2,4);
  const mln::algebra::frac<int> op2(1,1);
  mln::algebra::frac<int>       res = op1;

  res += op2;

  mln_assertion(ref1 == (op1 + op2));
  mln_assertion(ref1 == res);
  mln_assertion(ref1 == (res++));
  mln_assertion(ref2 == res);
  mln_assertion(ref3 == (++res));
  mln_assertion(ref3 == res);
  
  std::cout << "frac +  frac           : ok" << std::endl;
  std::cout << "frac += frac           : ok" << std::endl;
  std::cout << "frac++                 : ok" << std::endl;
  std::cout << "++frac                 : ok" << std::endl;
}

void test_frac_arithmetic_minus()
{
  const mln::algebra::frac<int> ref1(-1,4);
  const mln::algebra::frac<int> ref2(-5,4);
  const mln::algebra::frac<int> ref3(-9,4);
  const mln::algebra::frac<int> op1(3,4);
  const mln::algebra::frac<int> op2(1,1);
  mln::algebra::frac<int>       res = op1;
  
  res -= op2;

  mln_assertion(ref1 == (op1 - op2));
  mln_assertion(ref1 == res);
  mln_assertion(ref1 == (res--));
  mln_assertion(ref2 == res);
  mln_assertion(ref3 == (--res));
  mln_assertion(ref3 == res);

  std::cout << "frac -  frac           : ok" << std::endl;
  std::cout << "frac -= frac           : ok" << std::endl;
  std::cout << "frac--                 : ok" << std::endl;
  std::cout << "--frac                 : ok" << std::endl;
}

void test_frac_arithmetic_times()
{
  const mln::algebra::frac<int> ref(3,8);
  const mln::algebra::frac<int> op1(2,4);
  const mln::algebra::frac<int> op2(3,4);
  mln::algebra::frac<int>       res = op1;  

  res *= op2;

  mln_assertion(ref == (op1 * op2));
  mln_assertion(ref == res);

  std::cout << "frac *  frac           : ok" << std::endl;
  std::cout << "frac *= frac           : ok" << std::endl;
}

void test_frac_arithmetic_div()
{
  const mln::algebra::frac<int> ref(2,3);
  const mln::algebra::frac<int> op1(2,4);
  const mln::algebra::frac<int> op2(3,4);
  mln::algebra::frac<int>       res = op1;

  res /= op2;

  mln_assertion(ref == (op1 / op2));
  mln_assertion(ref == res);

  std::cout << "frac /  frac           : ok" << std::endl;
  std::cout << "frac /= frac           : ok" << std::endl;
}

//
// TEST SCALAR ARITHMETIC
//

void test_scalar_arithmetic_plus()
{
  const mln::algebra::frac<int> ref(6,4);
  const mln::algebra::frac<int> op1(2,4);
  const int                     op2 = 1;
  mln::algebra::frac<int>       res = op1;

  res += op2;

  mln_assertion(ref == (op1 + op2));
  mln_assertion(ref == res);

  std::cout << "frac +  integer        : ok" << std::endl;
  std::cout << "frac += integer        : ok" << std::endl;
}

void test_scalar_arithmetic_minus()
{
  const mln::algebra::frac<int> ref1(-1,4);
  const mln::algebra::frac<int> op1(3,4);
  const int                     op2 = 1;
  mln::algebra::frac<int>       res = op1;

  res -= op2;

  mln_assertion(ref1 == (op1 - op2));
  mln_assertion(ref1 == res);
  
  std::cout << "frac -  integer        : ok" << std::endl;
  std::cout << "frac -= integer        : ok" << std::endl;
}

void test_scalar_arithmetic_times()
{
  const mln::algebra::frac<int> ref(6,4);
  const mln::algebra::frac<int> op1(2,4);
  const int                     op2 = 3;
  mln::algebra::frac<int>       res = op1;

  res *= op2;

  mln_assertion(ref == (op1 * op2));
  mln_assertion(ref == res);
  
  std::cout << "frac *  integer        : ok" << std::endl;
  std::cout << "frac *= integer        : ok" << std::endl;
}

void test_scalar_arithmetic_div()
{
  const mln::algebra::frac<int> ref(2,12);
  const mln::algebra::frac<int> op1(2,4);
  const int                     op2 = 3;
  mln::algebra::frac<int>       res = op1;

  res /= op2;

  mln_assertion(ref == (op1 / op2));
  mln_assertion(ref == res);
  
  std::cout << "frac /  integer        : ok" << std::endl;
  std::cout << "frac /= integer        : ok" << std::endl;
}


int main2()
{
  // TEST INSTANTIATION
  std::cout << std::endl << "TEST INSTANTIATION" << std::endl;
  test_instantiation_without_argument();
  test_instantiation_numerator_denominator();
  test_instantiation_frac();
  test_instantiation_zero();
  test_instantiation_one();
  test_instantiation_integer();

  // TEST FRAC_BASE
  std::cout << std::endl << "TEST FRAC_BASE" << std::endl;
  test_frac_base_accessor();
  test_frac_base_manipulator();

  // TEST ASSIGNEMENT
  std::cout << std::endl << "TEST ASSIGNEMENT" << std::endl;
  test_assignement_zero();
  test_assignement_one();
  test_assignement_frac();
  test_assignement_integer();

  // TEST COMPARAISON
  std::cout << std::endl << "TEST COMPARAISON" << std::endl;
  test_comparaison_equal_to();
  test_comparaison_not_equal_to();
  test_comparaison_less_equal_than();
  test_comparaison_less_than();
  test_comparaison_greater_equal_than();
  test_comparaison_greater_than();

  // TEST FRAC ARITHMETIC
  std::cout << std::endl << "TEST FRAC ARITHMETIC" << std::endl;
  test_frac_arithmetic_uminus();
  test_frac_arithmetic_plus();
  test_frac_arithmetic_minus();
  test_frac_arithmetic_times();
  test_frac_arithmetic_div();

  // TEST SCALAR ARITHMETIC
  std::cout << std::endl << "TEST SCALAR ARITHMETIC" << std::endl;
  test_scalar_arithmetic_plus();
  test_scalar_arithmetic_minus();
  test_scalar_arithmetic_times();
  test_scalar_arithmetic_div();

  return 0;
}
