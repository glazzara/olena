/* mln core */
#include <mln/core/image/image2d.hh>
#include <mln/core/image/image_if.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/core/alias/window2d.hh>
#include <mln/core/alias/w_window2d_int.hh>
#include <mln/core/routine/duplicate.hh>
#include <mln/core/var.hh>

/* mln value */
#include <mln/value/int_u16.hh>

/* Site set */
#include <mln/core/site_set/p_array.hh>
#include <mln/level/sort_psites.hh>

/* Component trees */
#include <mln/morpho/tree/data.hh>
#include <mln/morpho/tree/compute_attribute_image.hh>
#include "../tree/propagate.hh"
#include "../tree/run.hh"
#include "../tree/accumulator/arg_max.hh"

/* Attributes */
#include <mln/transform/distance_front.hh>
#include "../attributes/bbox.hh"
#include <mln/morpho/attribute/card.hh>
#include <mln/make/w_window2d_int.hh>

/* io */
#include <mln/io/pbm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/ppm/save.hh>

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
#include <cmath>

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

template <typename P2V, typename G>
struct ratio_ : public mln::Function_p2v< ratio_<P2V, G> >
{
  typedef double result;

  ratio_(const P2V& f, const G& g) :
    f_ (f), g_ (g)
  {
  }

  template <typename P>
  double operator() (const P& p) const
  {
    mln_VAR(box, f_(p));
    double a = (double) (box.len(1)) / (double)(box.len(0));
    return a * std::log(g_(p));
  }

protected:
  const P2V& f_;
  const G& g_;
};

template <typename P2V, typename G>
ratio_<P2V, G> ratio(const mln::Function_p2v<P2V>& f, const mln::Function_p2v<G>& g)
{
  return ratio_<P2V, G>(exact(f), exact(g));
}




int main(int argc, char* argv[])
{
  using namespace mln;
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

  if (mydebug) {
    dsp("Distance geodesic");
    io::pgm::save(input, "distance.pgm");
  }

  /* Component tree creation */
  typedef p_array< mln_site_(I) > S;
  typedef morpho::tree::data<I,S> tree_t;

  S sorted_sites = level::sort_psites_decreasing(input);
  tree_t tree(input, sorted_sites, c4());

  /* Compute Attribute On Image */
  typedef morpho::attribute::bbox<I> bbox_t;
  typedef morpho::attribute::card<I> card_t;
  typedef mln_ch_value_(I, double) A;

  mln_VAR(attr_image, morpho::tree::compute_attribute_image(bbox_t (), tree));
  mln_VAR(card_image, morpho::tree::compute_attribute_image(card_t (), tree));
  A a = duplicate(ratio(pw::value(attr_image), pw::value(card_image)) | attr_image.domain());
  morpho::tree::propagate_representant(tree, a);

  if (mydebug) {
    dsp("Image sharp attribute");
  }

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
  typedef mln_ch_value_(I, value::rgb<8>) O;
  value::label<8> nlabel;
  L label = labeling::blobs(mask, c4(), nlabel);
  O output = debug::colorize(value::rgb8(), label, nlabel);
  io::ppm::save(output, "label.pgm");

  /* Now store output image image */
  O out;
  initialize(out, input);
  data::fill(out, literal::black_t());
  data::paste(output | pw::value(input_), out);

  if (mydebug) {
    dsp("Mask input");
  }

  io::ppm::save(out, "output.pgm");
}
