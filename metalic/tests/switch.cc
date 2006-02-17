#include <mlc/is_a.hh>
#include <mlc/switch.hh>


struct test;


namespace mlc
{

  template <class T>
  struct case_ <test, T, 1> : public where_< mlc_is_a(T, short) >
  {
    typedef float ret;
  };

  template <class T>
  struct case_ <test, T, 2> : public where_< mlc_is_a(T, int) >
  {
    typedef double ret;
  };


//   // ok

//   template <class T>
//   struct case_ <test, T, 3> : public where_< mlc_is_a(T, char) >
//   {
//     typedef int ret;
//   };


//   // ko

//   template <class T>
//   struct case_ <test, T, 3> : public where_< mlc_is_a(T, char) >
//   {
//     typedef not_found ret;
//   };

//   template <class T>
//   struct case_ <test, T, 3> : public where_< mlc_is_a(T, char) >
//   {
//   };

//   template <class T>
//   struct case_ <test, T, 3>
//   {
//     typedef int ret;
//   };

//   template <class T>
//   struct default_case_ <test, T> : public where_< mlc::true_ >
//   {
//     typedef int ret;
//   };

//   template <class T>
//   struct default_case_ <test, T>
//   {
//   };


} // end of namespace mlc


template <class T>
void print()
{
  int tmp;
}



int main()
{
  print< mlc::switch_<test, char>::ret >();
}
