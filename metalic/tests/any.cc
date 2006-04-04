/// Test mlc::any.

#include <mlc/any.hh>
#include <mlc/cmp.hh>
#include <mlc/assert.hh>
#include <mlc/logic.hh>
#include <mlc/is_a.hh>

// FIXME: Test other flavors of mlc::any.

/* Linear hierachy.

   - Abstractions.

                           mlc::any<Exact>
                                  ^
                                  |
                               A<Exact>
                                  ^
                                  |
                               B<Exact>
                                  ^
                                  |
                                Exact


   -  Concrete class C, deriving from B.

                             mlc::any<C>
                                  ^
                                  |
                                 A<C>
                                  ^
                                  |
                                 B<C>
                                  ^
                                  |
                                  C

*/

template <typename Exact>
struct A : public mlc::any__simple< Exact > {};

template <typename Exact>
struct B : public A< Exact > {};

struct C : public B<C> {};


/* A more complex hierarchy (several subclasses, diamond inheritance).

   - Abstractions.

                           mlc::any<Exact>
                                  ^
                                  |
                               T<Exact>
                               ^  ^   ^
                              /   |    \
                             /    |     \
                     U<Exact>  X<Exact>  Y<Exact>
                        ^         ^         ^
                        |         |         |
                      Exact     Exact     Exact

   - Concrete classes V and Z, deriving from U and from X and Y resp.


                 mlc::any<Z>          mlc::any<Z>
                     ^                    ^
                     |                    |
                    T<Z>                 T<Z>
                     ^                    ^
                     |                  /   \
                    U<Z>              X<Z>  Y<Z>
                     ^                 ^     ^
                     |                  \   /
                     V                    Z

*/

template <typename Exact>
struct T : public mlc::any__simple< Exact > {};

template <typename Exact>
struct U : public T< Exact > {};

struct V : public U<V> {};

template <typename Exact>
struct X : virtual public T< Exact > {};

template <typename Exact>
struct Y : virtual public T< Exact > {};

struct Z : public X<Z>, public Y<Z> {};

// The struct enclosing these two functions (methods) is used to
// factor the code w.r.t. the tested hierarchy.
template <template < class > class Top>
struct assuming_top_class_is
{
  template <template < class > class T, typename E>
  static void
  test_exact_is_a(Top<E>& a)
  {
    typedef typeof(a.exact()) exact_type;
    
    mlc::assert_< mlc::eq_<exact_type, E> >::check();
    mlc::assert_< mlc_is_a(exact_type, T) >::check();
  };

  template <template <class> class T, typename E>
  static void
  test_exact_is_not_a(Top<E>& a)
  {
    typedef typeof(a.exact()) exact_type;
    
    mlc::assert_< mlc::eq_<exact_type, E> >::check();
    mlc::assert_< mlc_is_not_a(exact_type, T) >::check();
  }
};

int
main()
{
  C c;
  assuming_top_class_is<A>::test_exact_is_a<A>(c);
  assuming_top_class_is<A>::test_exact_is_a<B>(c);

  Z z;
  assuming_top_class_is<T>::test_exact_is_a<T>(z);
  assuming_top_class_is<T>::test_exact_is_a<X>(z);
  assuming_top_class_is<T>::test_exact_is_a<Y>(z);

  X<Z> x;
  assuming_top_class_is<T>::test_exact_is_a<T>(x);
  // Bad branch.
  assuming_top_class_is<T>::test_exact_is_not_a<U>(x);
  // Bad hierachy.
  assuming_top_class_is<T>::test_exact_is_not_a<B>(x);
}
