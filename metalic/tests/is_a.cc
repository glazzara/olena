#include <mlc/is_a.hh>

struct A {};
struct B : public A {};

template <class T> struct C {};
template <class T> struct D : public C<T> {};

int
main ()
{
  mlc_is_a_(B, A)::ensure();
  mlc_is_a_(D<int>, C<int>)::ensure();
}
