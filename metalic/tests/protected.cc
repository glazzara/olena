#include <mlc/switch.hh>
#include <mlc/cmp.hh>
#include <mlc/typedef.hh>
#include <mlc/assert.hh>
#include <mlc/implies.hh>


mlc_case_equipment_for_namespace(client);
mlc_decl_typedef(protected_);

template <typename T>
void reveal()
{
  T tmp;
}



namespace client
{

  struct A
  {
    typedef bool typedef_in_A_only;
  };

  struct B
  {
    typedef char typedef_in_B_only;
  };



  // simple_test

  struct simple_test;

  template <class T>
  struct case_ <simple_test, T, 1> : public mlc::where_< mlc_eq(T, A) >
  {
    typedef A ret;
  };

  template <class T>
  struct case_ <simple_test, T, 2> : public mlc::where_< mlc_eq(T, B) >
  {
    typedef B ret;
  };


  // test_ok

  struct test_ok;

  template <class T>
  struct case_ <test_ok, T, 1> : public mlc::where_< mlc_eq(T, A) >
  {
    typedef typename A::typedef_in_A_only ret;
  };

  template <class T>
  struct case_ <test_ok, T, 2> : public mlc::where_< mlc_eq(T, B) >
  {
    typedef typename B::typedef_in_B_only ret;
  };



  // test_KO

  struct test_KO;

  template <class T>
  struct case_ <test_KO, T, 1> : public mlc::where_< mlc_eq(T, A) >
  {
    typedef typename T::typedef_in_A_only ret;
  };

  template <class T>
  struct case_ <test_KO, T, 2> : public mlc::where_< mlc_eq(T, B) >
  {
    typedef typename T::typedef_in_B_only ret;
  };


  // test_soluce

  struct test_soluce;

  template <class T>
  struct case_ <test_soluce, T, 1> : public mlc::where_< mlc_eq(T, A) >
  {
    struct protected_ {
      typedef typename T::typedef_in_A_only ret;
    };
  };

  template <class T>
  struct case_ <test_soluce, T, 2> : public mlc::where_< mlc_eq(T, B) >
  {
    // typedef mlc::dummy ret;
    struct protected_ {
      typedef typename T::typedef_in_B_only ret;
    };
  };



} // end of namespace client


struct ERROR_USE_EITHER_PROTECT_OR_RET_IN_A_CASE;



int main()
{

  using namespace client;

  typedef  switch_<simple_test, B>::ret  type;
  reveal<type>();
  

  // typedef  switch_<test, B>    ::ret  type;
  //  which is equiv to:
  // typedef  case_<test, B>::ret ::ret  type;

//   typedef case_<test_soluce, B>::ret ::protect::ret type;




//   typedef case_<test_soluce, B>::ret         the_case;
//   typedef mlc_typedef_(the_case, protected_) the_case_protected;
//   typedef mlc_ret_(the_case)                 the_case_ret;

//   mlc::assert_< mlc::xor_< mlc_is_found(the_case_protected),
//                            mlc_is_found(the_case_ret) >,
//                 ERROR_USE_EITHER_PROTECT_OR_RET_IN_A_CASE
//               >::check();

//   typedef mlc::if_< mlc::eq_< the_case_protected, mlc::not_found >,
//                     the_case_ret,
//                     mlc_ret_(the_case_protected) >::ret result;


//   reveal<result>();
}
