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

  doc::pbmsave(ima, "fill-subdomain");

  // Find and label the different components.
  // \{
  label_8 nlabels;
  image2d<label_8> lbl = labeling::blobs(ima, c4(), nlabels);
  // \}

  doc::ppmsave(debug::colorize(rgb8(), lbl, nlabels), "fill-subdomain");

  // Create a new image from lbl's sites being part of component 2.
  // \{
  mln_VAR(lbl_2, lbl | (pw::value(lbl) == pw::cst(2u)));
  // \}

  image2d<label_8> tmp;
  initialize(tmp, lbl);
  level::fill(tmp, 0);
  level::fill((tmp | lbl_2.domain()).rw(), 1);
  doc::ppmsave(debug::colorize(rgb8(), tmp, 1), "fill-subdomain");

  // Create a black image from ima.
  // Fill sites being part of component 2 with red.
  // \{
  image2d<rgb8> ima2;
  initialize(ima2, ima);
  level::fill(ima2, literal::black);

  level::fill((ima2 | lbl_2.domain()).rw(), literal::red);
  // \}

  doc::ppmsave(ima2, "fill-subdomain");
}
