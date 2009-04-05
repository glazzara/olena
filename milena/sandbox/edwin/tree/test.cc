/* mln core */
#include <mln/core/image/image2d.hh>
#include <mln/core/image/image_if.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/core/routine/duplicate.hh>
#include <mln/core/alias/window2d.hh>
#include <mln/core/var.hh>
#include <mln/value/int_u16.hh>

/* Site set */
#include <mln/core/site_set/p_array.hh>
#include <mln/level/sort_psites.hh>

/* Component trees */
#include <mln/morpho/tree/data.hh>
#include <mln/morpho/tree/compute_attribute_image.hh>
#include "propagate.hh"
#include "run.hh"
#include "accumulator/arg_max.hh"

/* morpho closing */
// #include <mln/morpho/opening/structural.hh>
// #include <mln/morpho/closing/structural.hh>

/* Attributes */
// #include <mln/transform/distance_geodesic.hh>
#include <mln/core/alias/window2d.hh>
#include <mln/core/alias/w_window2d_int.hh>
#include <mln/transform/distance_front.hh>
#include <mln/morpho/attribute/card.hh>
#include "../attributes/bbox.hh"
#include <mln/make/w_window2d_int.hh>

/* io */
#include <mln/io/pbm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/ppm/save.hh>
//#include <../../theo/color/change_attributes.hh>

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

/* Draw debug */
#include <mln/draw/box.hh>

/* std */
#include <string>
#include <iostream>

bool mydebug = false;

void usage(char** argv)
{
  std::cerr << "usage: " << argv[0] << " input [--debug] [-n nbr_components | -s sharpness] [-c card]" << std::endl;
  abort();
}

void dsp(const std::string& str)
{
  std::cout << std::endl
	    << "*********************" << std::endl
	    << "** " << str << std::endl
	    << "*********************" << std::endl;
}

template <typename P2V>
struct ratio_ : public mln::Function_p2v< ratio_<P2V> >
{
  typedef double result;

  ratio_(const P2V& f) :
    f_ (f)
  {
  }

  template <typename P>
  double operator() (const P& p) const
  {
    return (double) (f_(p).len(1)) / (double)(f_(p).len(0));
  }

protected:
  const P2V& f_;
};

template <typename P2V>
ratio_<P2V> ratio(const mln::Function_p2v<P2V>& f)
{
  return ratio_<P2V>(exact(f));
}




int main(int argc, char* argv[])
{
  using namespace mln;
//   using value::int_u8;
  using value::int_u16;

  std::string arg;
  unsigned nb_components = 0;
  unsigned card = 0;
  double treshold = 0;

  if (argc < 2)
    usage(argv);

  for (int i = 2; i < argc; i++)
    {
      arg = std::string(argv[i]);
      if (arg == "--debug")
	mydebug = true;
      else if (arg == "-n" && i != argc)
	nb_components = atoi(argv[++i]);
      else if (arg == "-t" && i != argc)
	treshold = atof(argv[++i]);
      else if (arg == "-c" && i != argc)
	card = atoi(argv[++i]);
      else if (arg == "--trace")
	trace::quiet = false;
      else
	usage (argv);
    }


  /* Image loadin' */
  typedef image2d<int_u16> I;

  image2d<bool> input_;
  io::pbm::load(input_, argv[1]);

  /* Work on geodesic distance image */
//   I input = transform::distance_geodesic(input_, c8(), mln_max(int_u8));
  I input;
  {
    const int weights[9] =
      { 10, 9, 10,
        1, 0, 1,
	10, 9, 10 };

    w_window2d_int win;
    mln::convert::from_to(weights, win);
    input = transform::distance_front(input_, c8(), win, mln_max(int_u16));
  }

  if (mydebug)
    dsp("Distance geodesic");

  /* Closing */
  {
    bool w[3][1];

    for (int i = 0; i < 3; i++)
      for (int j = 0; j < 1; j++)
	w[i][j] = 1;

//     input = morpho::closing::structural(input, convert::to<window2d>(w));
  }

  /* Opening */
  {
    bool w[1][15];

    for (int i = 0; i < 1; i++)
      for (int j = 0; j < 15; j++)
	w[i][j] = 1;

//     input = morpho::opening::structural(input, convert::to<window2d>(w));
  }

  /* Component tree creation */
  typedef p_array< mln_site_(I) > S;
  typedef morpho::tree::data<I,S> tree_t;

  S sorted_sites = level::sort_psites_decreasing(input);
  tree_t tree(input, sorted_sites, c4());


  io::pgm::save(input, "distance.pgm");

  /* Compute Attribute On Image */
  typedef morpho::attribute::bbox<I> bbox_t;
  typedef mln_ch_value_(I, double) A;

  mln_VAR(attr_image, morpho::tree::compute_attribute_image(bbox_t (), tree));
  A a = duplicate(ratio(pw::value(attr_image)) | attr_image.domain());
  morpho::tree::propagate_representant(tree, a);

  if (mydebug) {
    dsp("Image sharp attribute");
  }

  /* We don't want little components */

  // So we compute card attribute and we filter big components
  // FIXME: some attributes are compositions of attributes, here
  // sharpness can give area so, it would be fine if we could give an
  // optional extra argument to compute_attribute where the
  // accumulators image will be stored.

//   typedef morpho::attribute::card<I> card_t;
//   typedef mln_ch_value_(tree_t::function, mln_result_(card_t)) B;

//   B b = morpho::tree::compute_attribute_image(card_t (), tree);
//   morpho::tree::propagate_representant(tree, b);

//   if (mydebug) {
//     dsp("Image card attribute"); display_tree_attributes(tree, b);
//   }

//   a = duplicate((fun::p2v::ternary(pw::value(b) > pw::cst(2), pw::value(a), pw::cst(0.0))) | a.domain());


  /* Run max accumulator */
  accumulator::arg_max<A> argmax(a);
  p_array< mln_psite_(A) > obj_array; // Array of object components.

  if (mydebug) {
    std::stringstream s("Run max accumulator, look for ");
    if (nb_components)
      s << nb_components << " components.";
    else
      s << "components whose treshold > " << treshold;
    dsp(s.str());
  }

  if (!nb_components) {
    mln_VAR(predicate, pw::value(a) > pw::cst(treshold));
    obj_array = morpho::tree::run_while(tree, a, argmax, predicate);
  } else {
    obj_array = morpho::tree::run_ntimes(tree, a, argmax, nb_components);
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
    dsp("Create mask and propagate");
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
    mln_fwd_piter_(p_array< mln_psite_(I) >) c(obj_array);
    for_all(c)
      draw::box(out, attr_image(c), mln_max(int_u16));
    dsp("Mask input");
  }

  io::pgm::save(out, "output.pgm");
}
