#include <mlc/if.hh>
#include <mlc/cmp.hh>

struct alpha;
struct beta;
struct gamma;
struct delta;

int
main()
{
  typedef mlc_if_(mlc::true_, alpha, beta) x;
  mlc_eq(x, alpha)::ensure();

  typedef mlc_if_(mlc::false_, gamma, delta) y;
  mlc_eq(y, delta)::ensure();
}
