/* mln core */
#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
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

/* io */
#include <mln/io/pgm/load.hh>
#include <../../theo/color/change_attributes.hh>
#include <iostream>

/* std */
#include <string>

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
  typedef morpho::attribute::sharpness<I> accu_t;
  typedef mln_ch_value_(tree_t::function, mln_result_(accu_t)) A;

  A a = morpho::tree::compute_attribute_image(accu_t (), tree);
  morpho::tree::propagate_representant(tree, a);

  if (mydebug) {
    dsp("Image attribute"); display_tree_attributes(tree, a);
  }

  /* Run max accumulator, looking for 5 objects */
  accumulator::arg_max<A> argmax(a);
  p_array< mln_psite_(A) > obj_array; // Array of object components.
  obj_array = morpho::tree::run_ntimes(tree, a, argmax, 5);

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


}
