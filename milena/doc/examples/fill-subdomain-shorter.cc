#include <mln/core/image/image2d.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/core/var.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/labeling/blobs.hh>

#include <mln/make/image.hh>

#include <mln/value/rgb8.hh>
#include <mln/value/label_8.hh>
#include <mln/literal/colors.hh>

#include <mln/pw/all.hh>

#include <mln/data/fill.hh>

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

  // Find and label the different components.
  // Fill the sites of component 2 with red.
  // \{
  label_8 nlabels;
  image2d<label_8> lab = labeling::blobs(ima, c4(), nlabels);

  image2d<rgb8> ima2;
  initialize(ima2, ima);
  data::fill(ima2, literal::black);

  data::fill((ima2 | (pw::value(lab) == pw::cst(2u))).rw(), literal::red);
  // \}
}
