#include <mln/walue/int_u.hh>
#include <mln/literal/zero.hh>


int main()
{
  using namespace mln;
  using walue::int_u8;

  int_u8 i = 1;
  mln_assertion(i == 1);
  i = 2;
  mln_assertion(i == 2);

  mln_assertion((i - i) == 0);
  mln_assertion((i + 2 * i) == (i * 3));
  mln_assertion((+i) == i);
  mln_assertion((-i) == (0 - i));

  float f = i;
  mln_assertion(f == 2);
  mln_assertion(f == i);
  mln_assertion((f + i) == 2 * f);

  i = literal::zero;
  mln_assertion(0 == i);

  {
    int_u8 i = walue::set_encoding(51);
    mln_assertion(i.encoding() == 51);
  }
}
