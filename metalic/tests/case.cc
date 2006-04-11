#include <mlc/is_a.hh>
#include <mlc/case.hh>


struct test;


mlc_case_equipment_for_namespace(client);


namespace client
{

  template <class T>
  struct case_ <test, T, 1> : public mlc::where_< mlc_is_a(T, short) >
  {
  };

  template <class T>
  struct case_ <test, T, 2> : public mlc::where_< mlc_is_a(T, int) >
  {
  };


  // ok

  template <class T>
  struct case_ <test, T, 3> : public mlc::where_< mlc_is_a(T, char) >
  {
  };


//   // ko

//   template <class T>
//   struct case_ <test, T, 0> : public mlc::where_< mlc_is_a(T, char) >
//   {
//   };

//   template <class T>
//   struct case_ <test, T, 3>
//   {
//   };

//   template <class T>
//   struct default_case_ <test, T> : public mlc::where_< mlc::true_ >
//   {
//   };


} // end of namespace client


template <class T>
static void print()
{
  T tmp;
}



int main()
{
  print< client::case_<test, char>::ret >();
}
