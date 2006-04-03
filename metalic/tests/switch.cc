#include <mlc/is_a.hh>
#include <mlc/case.hh>


struct test;


mlc_case_equipment_for_namespace(client);


namespace client
{

  template <class T>
  struct case_ <test, T, 1> : public mlc::where_< mlc_is_a(T, short) >
  {
    typedef float ret;
  };

  template <class T>
  struct case_ <test, T, 2> : public mlc::where_< mlc_is_a(T, int) >
  {
    typedef double ret;
  };

//   // ok

  template <class T>
  struct case_ <test, T, 3> : public mlc::where_< mlc_is_a(T, char) >
  {
    typedef int ret;
  };

//   // ok

//   template <class T>
//   struct case_ <test, T, 3> : public mlc::where_< mlc_is_a(T, char) >
//   {
//     typedef mlc::not_found ret;
//   };

//   // ok

//   template <class T>
//   struct case_ <test, T, 3> : public mlc::where_< mlc_is_a(T, char) >
//   {
//     struct protected_ {
//       typedef mlc::not_found ret;
//     };
//   };






//   // ko

//   template <class T>
//   struct case_ <test, T, 0> : public mlc::where_< mlc_is_a(T, char) >
//   {
//     typedef int ret;
//   };

//   template <class T>
//   struct case_ <test, T, 3> : public mlc::where_< mlc_is_a(T, char) >
//   {
//   };

//   template <class T>
//   struct case_ <test, T, 3> : public mlc::where_< mlc_is_a(T, char) >
//   {
//     struct protected_ {
//     };
//     typedef int ret;
//   };

//   template <class T>
//   struct case_ <test, T, 3> : public mlc::where_< mlc_is_a(T, char) >
//   {
//     struct protected_ {
//     };
//   };

//   template <class T>
//   struct case_ <test, T, 3>
//   {
//     typedef int ret;
//   };

//   template <class T>
//   struct default_case_ <test, T> : public mlc::where_< mlc::true_ >
//   {
//     typedef int ret;
//   };

//   template <class T>
//   struct default_case_ <test, T>
//   {
//   };

//   template <class T>
//   struct default_case_ <test, T> : public mlc::where_< mlc_is_a(T, char) >
//   {
//     typedef int ret;
//   };

//   template <class T>
//   struct default_case_ <test, T>
//   {
//     struct protected_ {
//       typedef int ret;
//     }
//     typedef int ret;
//   };

//   template <class T>
//   struct default_case_ <test, T>
//   {
//     struct protected_ {
//     }
//   };


} // end of namespace client



template <class T>
void print()
{
  int tmp;
}



int main()
{
  print< client::switch_<test, char>::ret >();
}
