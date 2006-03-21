#include <mlc/is_a.hh>
#include <mlc/assert.hh>

struct A {};
struct B : public A {};

template <class T> struct C {};
template <class T> struct D : public C<T> {};

int
main ()
{
  mlc::assert_< mlc_is_a_(B, A) >::check();
  mlc::assert_< mlc_is_a_(D<int>, C<int>) >::check();
}
