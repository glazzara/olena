#include <mln/core/image/image2d.hh>
#include <mln/core/image/attribute_image.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/value/int_u8.hh>

#include <mln/util/ctree/ctree.hh>

#include <mln/morpho/tree/component_tree.hh>
#include <mln/morpho/tree/compute_attribute_image.hh>
#include <mln/morpho/tree/filter/min.hh>
#include <mln/morpho/attribute/volume.hh>
#include <mln/morpho/attribute/card.hh>


#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>

#include <mln/pw/all.hh>
#include <mln/core/var.hh>

#include <mln/util/timer.hh>
#include <iostream>

#include <mln/debug/ctree.hh>
#include <mln/debug/println.hh>

void usage(char** argv)
{
  std::cout << "Usage: " << argv[0] << " image lambda [new.pgm]"
	    << std::endl;
  abort();
}

static mln::util::timer tm;

void bench(const char* msg)
{
  std::cout << msg << " in " << tm << " s" << std::endl;
  tm.restart();
}

int main(int argc, char** argv)
{
  using namespace mln;
  using value::int_u8;

  if (argc < 3)
    usage(argv);

  const char* finput = argv[1];
  const unsigned lambda = atoi(argv[2]);
  const char* foutput = (argc > 3) ? argv[3] : "new.pgm";

  tm.start();


  typedef image2d<int_u8> I;
  I input;
  io::pgm::load(input, finput);
  bench("Image Loadin");

  // Tree Computation
  typedef util::ctree::ctree<I> tree_t;
  tree_t tree = morpho::tree::max_tree(input, c4());
  bench("Tree construction");

  // Compute Attribute
  typedef attribute_image<tree_t, unsigned> A;
  morpho::attribute::volume<I> attribute;
  A attr_img = morpho::tree::compute_attribute_image(tree, attribute);
  bench("Attribute Computation (volume)");

  // Filtering
  attribute_image<tree_t, int_u8>  out = tree.to_image();
  mln_VAR(predicate, pw::value(attr_img) > pw::cst(lambda));
  morpho::tree::filter::min(out, predicate);
  bench("Filtering");

  // Converting
  I output;
  convert::over_load::from_to_(out, output); // FIXME !
  bench("Restore");

  io::pgm::save(output, foutput);
  bench("Image Savin");
}
