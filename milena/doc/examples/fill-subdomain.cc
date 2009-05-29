#include <mln/core/image/image2d.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/core/var.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/make/image.hh>

#include <mln/value/rgb8.hh>
#include <mln/value/label_8.hh>
#include <mln/literal/colors.hh>

#include <mln/pw/all.hh>

#include <mln/labeling/blobs.hh>
#include <mln/labeling/colorize.hh>

#include <mln/data/fill.hh>

#include <doc/tools/sample_utils.hh>

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

  doc::ppmsave(labeling::colorize(rgb8(), lbl, nlabels), "fill-subdomain");

  // Create a new image from lbl's sites being part of component 2.
  // \{
  mln_VAR(lbl_2, lbl | (pw::value(lbl) == pw::cst(2u)));
  // \}

  image2d<label_8> tmp;
  initialize(tmp, lbl);
  data::fill(tmp, 0);
  data::fill((tmp | lbl_2.domain()).rw(), 1);
  doc::ppmsave(labeling::colorize(rgb8(), tmp, 1), "fill-subdomain");

  // Create a black image from ima.
  // Fill sites being part of component 2 with red.
  // \{
  image2d<rgb8> ima2;
  initialize(ima2, ima);
  data::fill(ima2, literal::black);

  data::fill((ima2 | lbl_2.domain()).rw(), literal::red);
  // \}

  doc::ppmsave(ima2, "fill-subdomain");
}
