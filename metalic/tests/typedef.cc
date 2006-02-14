#include <mlc/typedef.hh>
#include <mlc/cmp.hh>

struct foo
{
  typedef int good_type;
};

mlc_decl_typedef(good_type);
mlc_decl_typedef(bad_type);

int
main ()
{
  mlc_eq(mlc_typedef_(foo, good_type), foo::good_type)::ensure();
  mlc_eq(mlc_typedef_(foo, bad_type), mlc::not_found)::ensure();
}
