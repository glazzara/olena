#include <mlc/if.hh>
#include <mlc/cmp.hh>
#include <mlc/assert.hh>

struct alpha;
struct beta;
struct gamma;
struct delta;

int
main()
{
  typedef mlc_if_(mlc::true_, alpha, beta) x;
  mlc::assert_< mlc_eq(x, alpha) >::check();

  typedef mlc_if_(mlc::false_, gamma, delta) y;
  mlc::assert_< mlc_eq(y, delta) >::check();
}
