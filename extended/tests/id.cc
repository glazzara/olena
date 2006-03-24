#include <iostream>


#include <xtd/math/id.hh>



// namespace xtd 
// {
//   struct xfail;

//   template <>
//   struct fun_traits_< xfail >
//   {
//     typedef int res_type;
//   };
//   struct xfail : public xtd::abstract::plain_fun_< xfail >
//   {
//   };
  
// }



int main()
{

//   {
//     xtd::xfail test;
//   }


  {
    xtd::plain_id_<int> id_int;
    std::cout << id_int(51) << std::endl;

    xtd::plain_id_<float> id_float;
    std::cout << id_float(5.1f) << std::endl;

    // xfail tests:
    // xtd::id(id_int);
    // xtd::id(xtd::id);
  }

}
