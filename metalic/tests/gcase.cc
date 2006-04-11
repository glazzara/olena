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

} // end of namespace client


template <class T>
void print()
{
  int tmp;
}



int main()
{
  print< client::get_case_<test, char>::ret >();
}
