/* mln core */
#include <mln/core/image/image2d.hh>
#include <mln/core/image/image_if.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/core/routine/duplicate.hh>
#include <mln/core/var.hh>

/* Site set */
#include <mln/core/site_set/p_array.hh>
#include <mln/level/sort_psites.hh>

/* Component trees */
#include <mln/morpho/tree/data.hh>
#include <mln/morpho/tree/compute_attribute_image.hh>
#include "propagate.hh"
#include "run.hh"
#include "accumulator/arg_max.hh"

/* Attributes */
#include <mln/morpho/attribute/sharpness.hh>
#include <mln/morpho/attribute/card.hh>


/* io */
#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/ppm/save.hh>
#include <../../theo/color/change_attributes.hh>

/* data & pw */
#include <mln/core/concept/function.hh>
#include <mln/fun/p2v/ternary.hh>
#include <mln/data/fill.hh>
#include <mln/data/paste.hh>
#include <mln/pw/all.hh>

/* labeling */
#include <mln/value/label.hh>
#include <mln/labeling/blobs.hh>
#include <mln/debug/colorize.hh>


/* std */
#include <string>
#include <iostream>

bool mydebug = false;


void usage(char** argv)
{
  std::cerr << "usage: " << argv[0] << " input [--debug]" << std::endl;
  abort();
}

void dsp(const char* str)
{
  std::cout << std::endl
	    << "*********************" << std::endl
	    << "** " << str << std::endl
	    << "*********************" << std::endl;
}



int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;

  if (argc < 2)
    usage(argv);

  mydebug = (argc >= 3 && std::string(argv[2]) == "--debug");


  /* Image loadin' */
  typedef image2d<int_u8> I;

  I input;
  io::pgm::load(input, argv[1]);

  /* Component tree creation */
  typedef p_array< mln_site_(I) > S;
  typedef morpho::tree::data<I,S> tree_t;

  S sorted_sites = level::sort_psites_decreasing(input);
  tree_t tree(input, sorted_sites, c4());

  /* Compute Attribute On Image */
  typedef morpho::attribute::sharpness<I> sharp_t;
  typedef mln_ch_value_(I, double) A;

  A a = morpho::tree::compute_attribute_image(sharp_t (), tree);
  morpho::tree::propagate_representant(tree, a);

  if (mydebug) {
    dsp("Image sharp attribute"); display_tree_attributes(tree, a);
  }

  /* We don't want little components */

  // So we compute card attribute and we filter big components
  // FIXME: some attributes are compositions of attributes, here
  // sharpness can give area so, it would be fine if we could give an
  // optional extra argument to compute_attribute where the
  // accumulators image will be stored.

  typedef morpho::attribute::card<I> card_t;
  typedef mln_ch_value_(tree_t::function, mln_result_(card_t)) B;

  B b = morpho::tree::compute_attribute_image(card_t (), tree);
  morpho::tree::propagate_representant(tree, b);

  if (mydebug) {
    dsp("Image card attribute"); display_tree_attributes(tree, b);
  }

  a = duplicate((fun::p2v::ternary(pw::value(b) > pw::cst(100), pw::value(a), pw::cst(0.0))) | a.domain());


  /* Run max accumulator */
  accumulator::arg_max<A> argmax(a);
  p_array< mln_psite_(A) > obj_array; // Array of object components.

  mln_VAR(predicate, pw::value(a) > pw::cst(0.7));
  obj_array = morpho::tree::run_while(tree, a, argmax, predicate);

  if (mydebug) {
    dsp("Run max accumulator, lk 4 5 objs"); display_tree_attributes(tree, a);
  }

  /* Print them */
  if (mydebug) {
    dsp("Image Filtered Components");
    mln_fwd_piter_(p_array< mln_psite_(I) >) c(obj_array);
    for_all(c)
      std::cout << c;
  }

  /* Now Back Propagate to component */
  typedef mln_ch_value_(I, bool) M;
  M mask;
  initialize(mask, a);
  data::fill(mask, false);

  mln_fwd_piter_(p_array< mln_psite_(I) >) c(obj_array);
  for_all(c)
  {
    mask(c) = true;
    propagate_node_to_descendants(c, tree, mask);
  }
  morpho::tree::propagate_representant(tree, mask);

  // mask now contains all nodes related to objects

  if (mydebug) {
    dsp("Create mask and propagate"); display_tree_attributes(tree, mask);
  }

  /* Labeling */
  typedef mln_ch_value_(I, value::label<8>) L;
  value::label<8> nlabel;
  L label = labeling::blobs(mask, c4(), nlabel);
  io::ppm::save(debug::colorize(value::rgb8(), label, nlabel), "label.pgm");

  /* Now store output image image */
  I out;
  initialize(out, input);
  data::fill(out, 0);
  data::paste(input | pw::value(mask), out);

  if (mydebug) {
    dsp("Mask input"); display_tree_attributes(tree, out);
  }

  io::pgm::save(out, "output.pgm");
}
