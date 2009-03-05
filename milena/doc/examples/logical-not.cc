#include <mln/essential/2d.hh>
#include <doc/tools/sample_utils.hh>
int main()
{
  using namespace mln;

  // \{
  bool vals[5][5] = {
    {1, 0, 1, 0, 0},
    {0, 1, 0, 1, 0},
    {1, 0, 1, 0, 0},
    {0, 1, 0, 1, 0},
    {0, 1, 0, 1, 0}
  };
  image2d<bool> ima = make::image(vals);
  // \}
  doc::pbmsave(ima, "logical-not");


  // \{
  image2d<bool> ima_neg = logical::not_(ima);
  // \}

  doc::pbmsave(ima_neg, "logical-not");

  // \{
  logical::not_inplace(ima);
  // \}

  doc::pbmsave(ima, "logical-not");
}
