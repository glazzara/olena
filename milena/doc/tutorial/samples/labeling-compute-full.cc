#include <mln/essential/2d.hh>
int main()
{
  using namespace mln;
  using value::int_u8;
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

  util::array<box2d> boxes =
      labeling::compute(accu::meta::bbox(),
			lbl,
			nlabels);

  for (unsigned i = 1; i <= nlabels; ++i)
    std::cout << boxes[i] << std::endl;
}
