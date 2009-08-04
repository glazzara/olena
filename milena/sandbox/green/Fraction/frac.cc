#include <iostream>
#include "frac.hh"

//
// TEST INSTANTIATION
//
void test_instantiation_without_argument()
{
  const mln::algebra::frac<int> constant;
  mln::algebra::frac<int>       variable;

  mln_assertion(constant == variable);
  std::cout << "const frac<T> f      : ok" << std::endl; 
  std::cout << "frac<T>       f      : ok" << std::endl; 
}

void test_instantiation_numerator_denominator()
{
  const mln::algebra::frac<int> constant(2,5);
  mln::algebra::frac<int>       variable(2,5);

  mln_assertion(constant == variable);
  std::cout << "const frac<T> f(n,d) : ok" << std::endl; 
  std::cout << "frac<T>       f(n,d) : ok" << std::endl; 
}

void test_instantiation_frac()
{
  const mln::algebra::frac<int>  ref_int(2,5);
  const mln::algebra::frac<long> ref_long(2,5);

  const mln::algebra::frac<int>  constant1 = ref_int;
  const mln::algebra::frac<int>  constant2 = ref_long;
  mln::algebra::frac<int>        variable1 = ref_int;
  mln::algebra::frac<int>        variable2 = ref_long;

  mln_assertion(constant1 == ref_int);
  mln_assertion(constant2 == ref_long);
  mln_assertion(variable1 == ref_int);
  mln_assertion(variable2 == ref_long);
  std::cout << "const frac<T> f = frac<T> : ok" << std::endl;
  std::cout << "const frac<T> f = frac<F> : ok" << std::endl;
  std::cout << "frac<T>       f = frac<T> : ok" << std::endl;
  std::cout << "frac<T>       f = frac<F> : ok" << std::endl;
}

void test_instantiation_zero()
{
  const mln::algebra::frac<int> zero(0,0);
  const mln::algebra::frac<int> f_cst = mln::literal::zero;
  mln::algebra::frac<int>       f_var = mln::literal::zero;

  mln_assertion(zero == f_cst);
  mln_assertion(zero == f_var);
  std::cout << "const frac<T> f = zero   : ok" << std::endl; 
  std::cout << "frac<T>       f = zero   : ok" << std::endl; 
}

void test_instantiation_one()
{
  const mln::algebra::frac<int> one(1,0);
  const mln::algebra::frac<int> f_cst = mln::literal::one;
  mln::algebra::frac<int>       f_var = mln::literal::one;

  mln_assertion(one == f_cst);
  mln_assertion(one == f_var);
  std::cout << "const frac<T> f = one    : ok" << std::endl; 
  std::cout << "frac<T>       f = one    : ok" << std::endl; 
}

void test_instantiation_integer()
{
  const mln::algebra::frac<int> ref(5,0);
  const mln::algebra::frac<int> f_cst = 5;
  mln::algebra::frac<int>       f_var = 5;

  mln_assertion(ref == f_cst);
  mln_assertion(ref == f_var);
  std::cout << "const frac<T> f = integer : ok" << std::endl;
  std::cout << "frac<T>       f = integer : ok" << std::endl;
}


//
// TEST ASSIGNEMENT
//
void test_assignement_zero()
{
  const mln::algebra::frac<int> zero(0,0);
  mln::algebra::frac<int>       f;

  f = mln::literal::zero;
  mln_assertion(zero == f);
  std::cout << "frac = zero   : ok" << std::endl; 
}

void test_assignement_one()
{
  const mln::algebra::frac<int> one(1,0);
  mln::algebra::frac<int>       f;

  f = mln::literal::one;
  mln_assertion(one == f);
  std::cout << "frac = one    : ok" << std::endl; 
}

void test_assignement_frac()
{
  const mln::algebra::frac<int> ref(2,3);
  mln::algebra::frac<int>       f;

  f = ref;
  mln_assertion(ref == f);
  std::cout << "frac = frac   : ok" << std::endl; 
}

void test_assignement_integer()
{
  const mln::algebra::frac<int> ref(5,0);
  mln::algebra::frac<int>       f;

  f = 5;
  mln_assertion(ref == f);
  std::cout << "frac = integer: ok" << std::endl;
}

int main()
{
  using namespace std;
  using namespace mln;
  using namespace mln::algebra;

  trace::quiet = true;

  // Testing instanciation
  frac<int> f0;
  frac<int> op1a(5,10);
  frac<int> op1b(op1a);
  frac<int> op2a(1,2);
  frac<int> op2b = op2a;
  int       op3a = 1;
  int       op3b = 1;
  int       op4a = 2;
  int       op4b = 2;
  frac<int> resPlus(literal::one);
  frac<int> resMinus(literal::zero);
  frac<int> resTimes(1,4);
  frac<int> resDiv(literal::one);
  frac<int> resPlusScalar(3,2);
  frac<int> resMinusScalar(-1,2);
  frac<int> resTimesScalar(literal::one);
  frac<int> resDivScalar(1,4);
  frac<int> resUminus(-1,4);
  frac<int> resZero(0,0);
  frac<int> resOne(1,0);
  cout << "frac<T> f     : ok" << endl; 

  test_instantiation_without_argument();
  test_instantiation_numerator_denominator();
  test_instantiation_frac();
  test_instantiation_zero();
  test_instantiation_one();
  test_instantiation_integer();

  //  trace::quiet = false;

  //trace::quiet = true;

  test_assignement_zero();
  test_assignement_one();
  test_assignement_frac();
  test_assignement_integer();


  // Test frac.eval()
  mln_assertion(0.5 == op2b.eval());
  mln_assertion(op2a == op2b);
  cout << "frac.eval()   : ok" << endl; 

  // Test - frac
  mln_assertion(resUminus == (-op2b));
  cout << "- frac()      : ok" << endl; 

  // Test frac + frac
  mln_assertion(resPlus == (op1b + op2b));
  mln_assertion(op1a == op1b);
  mln_assertion(op2a == op2b);
  cout << "frac + frac   : ok" << endl;

  // Test frac - frac
  mln_assertion(resMinus == (op1b - op2b));
  mln_assertion(op1a == op1b);
  mln_assertion(op2a == op2b);
  cout << "frac - frac   : ok" << endl;

  // Test frac * frac
  mln_assertion(resTimes == (op1b * op2b));
  mln_assertion(op1a == op1b);
  mln_assertion(op2a == op2b);
  cout << "frac * frac   : ok" << endl;

  // Test frac / frac
  mln_assertion(resDiv == (op1b / op2b));
  mln_assertion(op1a == op1b);
  mln_assertion(op2a == op2b);
  cout << "frac / frac   : ok" << endl;  

  // Test frac + scalar
  mln_assertion(resPlusScalar == (op1b + op3b));
  mln_assertion(op1a == op1b);
  mln_assertion(op3a == op3b);
  cout << "frac + scalar : ok" << endl;  

  // Test frac - scalar
  mln_assertion(resMinusScalar == (op1b - op3b));
  mln_assertion(op1a == op1b);
  mln_assertion(op3a == op3b);
  cout << "frac - scalar : ok" << endl;  

  // Test frac * scalar
  mln_assertion(resTimesScalar == (op1b * op4b));
  mln_assertion(op1a == op1b);
  mln_assertion(op4a == op4b);
  cout << "frac * scalar : ok" << endl;  

  // Test frac / scalar
  mln_assertion(resDivScalar == (op1b / op4b));
  mln_assertion(op1a == op1b);
  mln_assertion(op4a == op4b);
  cout << "frac / scalar : ok" << endl;

  return 0;
}

#include <mln/core/image/image2d.hh>
#include <mln/value/int_u8.hh>
#include <mln/io/pgm/load.hh>
#include <mln/data/compute.hh>
#include <mln/accu/math/sum.hh>
#include <mln/accu/stat/mean.hh>
#include <mln/accu/stat/variance.hh>


int main2()
{
  using namespace mln;
  using namespace std;
  using namespace mln::algebra;

  image2d<value::int_u8> input;
  io::pgm::load(input, "/usr/local/share/olena/images/small.pgm");

  /*
  accu::math::sum< value::int_u8 >               sum0;
  accu::math::sum< value::int_u8, frac<long> >   sum1;

  cout << data::compute(sum0, input) << endl;
  cout << data::compute(sum1, input) << endl;

  accu::stat::mean< value::int_u8 >              mean0;
  accu::stat::mean< value::int_u8, frac<long> >  mean1;
  */
  //  cout << data::compute(mean0, input) << endl;
  //cout << data::compute(mean1, input) << endl;

  //frac<long> count = data::compute(accu::math::count< frac<long> >(), input);
  //frac<long> variance = data::compute(accu::stat::variance< frac<long>, frac<long>, frac<long> >(), input);


  return 0;
}
