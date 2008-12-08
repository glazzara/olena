#include <mln/essential/2d.hh>
#include <doc/tutorial/tools/sample_utils.hh>
int main()
{
  using namespace mln;
  using value::rgb8;
  using value::label_8;


  // \{
  bool vals[6][5] = {
      {0, 1, 1, 0, 0},
      {0, 1, 1, 0, 0},
      {0, 0, 0, 0, 0},
      {1, 1, 0, 1, 0},
      {1, 0, 1, 1, 1},
      {1, 0, 0, 0, 0}
  };
  image2d<bool> ima = make::image(vals);
  // \}
  doc::pbmsave(ima, "labeling-compute");

  // \{
  label_8 nlabels;
  image2d<label_8> lbl = labeling::blobs(ima, c4(), nlabels);
  // \}
  doc::ppmsave(debug::colorize(rgb8(), lbl, nlabels),
	       "labeling-compute");


  // \{
  util::array<box2d> boxes =
      labeling::compute(accu::meta::bbox(),
			lbl,
			nlabels);
  // \}


  // \{
  for (unsigned i = 1; i <= nlabels; ++i)
    std::cout << boxes[i] << std::endl;
  // \}
}
