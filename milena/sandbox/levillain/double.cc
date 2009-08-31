#include <mln/value/int_u8.hh>
#include <mln/core/image/image2d.hh>

#include <mln/debug/iota.hh>
#include <mln/debug/println.hh>

int main()
{
  using namespace mln;
  using mln::value::int_u8;

  image2d<int_u8> ima(5, 5);
  debug::iota(ima);
  debug::println(ima);
  /* ima =

     1  2  3  4  5
     6  7  8  9  10
     11 12 13 14 15
     16 17 18 19 20
     21 22 23 24 25 */

  image2d<int_u8> ima_x2 (10, 10);
  mln_piter_(image2d<int_u8>) p(ima_x2.domain());
  for_all(p)
  {
    /* This conversion from ``piter'' to ``point'' is required, since
       an iterator does not expose the interface of the underlying
       point (among which the methods row(), col(), etc.).  */
    point2d p_ = p;
    point2d q(p_.row() / 2, p_.col() / 2);
    ima_x2(p) = ima(q);
  }
  debug::println(ima_x2);
  /* ima_x2 =

     1  1  2  2  3  3  4  4  5  5
     1  1  2  2  3  3  4  4  5  5
     6  6  7  7  8  8  9  9  10 10
     6  6  7  7  8  8  9  9  10 10
     11 11 12 12 13 13 14 14 15 15
     11 11 12 12 13 13 14 14 15 15
     16 16 17 17 18 18 19 19 20 20
     16 16 17 17 18 18 19 19 20 20
     21 21 22 22 23 23 24 24 25 25
     21 21 22 22 23 23 24 24 25 25 */
}
