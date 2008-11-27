#include <mln/essential/2d.hh>
#include <mln/pw/all.hh>
#include <mln/core/var.hh>

struct my_values_t : mln::Function_p2v<my_values_t>
{
  typedef int result;

  int operator()(const mln::point2d&) const
  {
    return 0;
  }
};

int main()
{
  using namespace mln;

  box2d arr(4, 2);
  my_values_t my_values;

  // \{
  mln_VAR(ima, my_values | arr);
  // \}

  (void) ima;

  // \{
  mln_VAR(imab1, ima | (pw::value(ima) == pw::cst(1u)));
  // \}

  (void) imab1;

  // \{
  box2d b2(1, 2);
//FIXME: enable these lines.
//  mln_VAR(imad, imab1 / b2);

//  debug::println(imad);
  // \}

}
