#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/core/site_set/p_array.hh>

#include <mln/data/sort_psites.hh>

#include <mln/morpho/tree/data.hh>
#include <mln/morpho/tree/compute_attribute_image.hh>
#include "export_leaves.hh"

#include <mln/morpho/attribute/sharpness.hh>

#include <mln/io/pbm/save.hh>
#include <mln/io/pgm/load.hh>

#include <mln/value/int_u8.hh>

#include <iostream>



void usage(char** argv)
{
  std::cout << "Retrieve leaves of the component tree using sharpness"
	    << " attribute. It returns a binary image of leaves and creates"
	    << " txt file that contains nodes of the branch that the leaf belongs to."
	    << std::endl;
  std::cout << "Usage: " << argv[0] << " input.pgm" << std::endl;
  abort();
}


int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;

  if (argc == 0)
    usage(argv);


  // Load a pgm img (Gray level Or Gradient Image)
  typedef image2d<int_u8> I;
  I input;
  io::pgm::load(input, argv[1]);

  // Tree Creation
  typedef p_array<point2d> S;
  typedef morpho::tree::data<I,S> tree_t;
  S s = data::sort_psites_decreasing(input);
  tree_t t(input, s, c4());

  // Attribute pruning.
  typedef morpho::attribute::sharpness<I> sharp_t;
  typedef mln_ch_value_(I, double) A;

  A a = morpho::tree::compute_attribute_image(sharp_t (), t);


  //Export Leaves
  typedef mln_ch_value_(I, bool) B;
  B b_img = export_leaves_to_img(t, a);
  io::pbm::save(b_img, "leaves.pbm");
  export_leaves_to_file(t, a);
}
