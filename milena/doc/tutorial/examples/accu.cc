# include <mln/accu/mean.hh>


int main()
{
  using namespace mln;

  mlc_bool(accu::mean<int>::proxy_level == 1)::check();

  accu::mean<int> m;
  m.take(100);
  m.take(2);
  mln_assertion(m == 51);

  // Just for fun:
  mln_assertion(literal::zero != m);
  mln_assertion(m != literal::zero);

  std::cout << (-m) << std::endl;
}
