#include <iostream>
#include "frac.hh"

#include <mln/core/image/image2d.hh>
#include <mln/value/int_u8.hh>
#include <mln/io/pgm/load.hh>
#include <mln/accu/stat/variance.hh>
#include <mln/data/compute.hh>

int main2()
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

int main()
{
  using namespace mln;
  using namespace std;
  using namespace mln::algebra;

  image2d<value::int_u8> input;
  io::pgm::load(input, "/usr/local/share/olena/images/small.pgm");


  frac<long> variance = data::compute(accu::stat::variance< frac<long>, frac<long>, frac<long> >(), input);


  return 0;
}
