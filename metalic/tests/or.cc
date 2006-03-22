#include <mlc/cmp.hh>
#include <mlc/logic.hh>
#include <mlc/assert.hh>

struct alpha;
struct beta;
struct gamma;
struct delta;

int
main()
{
  typedef beta x;

  mlc::assert_< mlc::or_< mlc_eq(x, alpha),
                          mlc_eq(x, beta) >
              >::check();

  mlc::assert_< mlc::or_list_< mlc_eq(x, alpha),
                               mlc_eq(x, beta),
                               mlc_eq(x, gamma) >
              >::check();
}
