#include <mln/essential/2d.hh>
int main()
{
  using namespace mln;
  using value::label8;

  bool vals[6][5] = {
      {0, 1, 1, 0, 0},
      {0, 1, 1, 0, 0},
      {0, 0, 0, 0, 0},
      {1, 1, 0, 1, 0},
      {1, 0, 1, 1, 1},
      {1, 0, 0, 0, 0}
  };
  image2d<bool> ima = make::image(vals);

  label8 nlabels;
  image2d<label8> lbl = labeling::blobs(ima, c4(), nlabels);

  debug::println(lbl);
}
