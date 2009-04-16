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
#include "components.hh"
#include "propagate.hh"

/* Attributes */
#include <mln/morpho/attribute/sharpness.hh>

/* io */
#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>

/* data & pw */
#include <mln/core/concept/function.hh>
#include <mln/fun/p2v/ternary.hh>
#include <mln/data/fill.hh>
#include <mln/data/paste.hh>
#include <mln/pw/all.hh>

/* level */
#include <mln/level/stretch.hh>

/* trace */
#include <mln/trace/quiet.hh>

/* std */
#include <string>
#include <iostream>

using namespace mln;
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

namespace mln
{
  // Distance, stored on pixels, of neighboring colors.

  value::int_u8 dist_mean(const value::rgb8& c1, const value::rgb8& c2)
  {
    unsigned d = 0;
    d += (math::diff_abs(c1.red(), c2.red()) + 2) / 3;
    d += (math::diff_abs(c1.green(), c2.green()) + 2) / 3;
    d += (math::diff_abs(c1.blue(), c2.blue()) + 2) / 3;
    if (d > 255)
      d = 255;
    return d;
  }

  template <typename N>
  image2d<value::int_u8>
  dist_on_pixels(const image2d<value::rgb8>& input, const N& nbh)
  {
    using value::int_u8;
    image2d<int_u8> output(input.domain());

    mln_piter(box2d) p(input.domain());
    mln_niter(N) n(nbh, p);
    for_all(p)
    {
      int_u8 d = 0u;
      for_all(n) if (input.domain().has(n))
	{
	  int_u8 d_ = dist_mean(input(p), input(n));
	  if (d_ > d)
	    d = d_;
	}
      output(p) = 255 - d;
    }
    return output;
  }

  template <typename P2V>
  struct height_wrapper_ : Function_p2v< height_wrapper_<P2V> >
  {
    typedef unsigned result;

    height_wrapper_(const Function_p2v<P2V>& f) :
      f_ (exact(f))
    {
      mlc_is_a(mln_result(P2V), Accumulator)::check();
    }

    template <typename P>
    unsigned operator() (const P& p) const
    {
      return f_(p).height();
    }

  private:
    const P2V& f_;
  };

  template <typename P2V>
  inline
  height_wrapper_<P2V>
  height_wrapper(const Function_p2v<P2V>& f)
  {
    return height_wrapper_<P2V>(f);
  }

  template <typename P2V>
  struct card_wrapper_ : Function_p2v< card_wrapper_<P2V> >
  {
    typedef unsigned result;

    card_wrapper_(const Function_p2v<P2V>& f) :
      f_ (exact(f))
    {
      mlc_is_a(mln_result(P2V), Accumulator)::check();
    }

    template <typename P>
    unsigned operator() (const P& p) const
    {
      return f_(p).area();
    }

  private:
    const P2V& f_;
  };

  template <typename P2V>
  inline
  card_wrapper_<P2V>
  card_wrapper(const Function_p2v<P2V>& f)
  {
    return card_wrapper_<P2V>(f);
  }

}







int main(int argc, char* argv[])
{
  using value::int_u8;
  using value::rgb8;
  std::string arg;
  unsigned nb_components = 0;
  unsigned card = 0;
  unsigned height = 0;
  double sharpness = 0;

  if (argc < 2)
    usage(argv);

  for (int i = 2; i < argc; i++)
    {
      arg = std::string(argv[i]);
      if (arg == "--debug")
	mydebug = true;
      else if (arg == "-n" && i != argc)
	nb_components = atoi(argv[++i]);
      else if (arg == "-s" && i != argc)
	sharpness = atof(argv[++i]);
      else if (arg == "-c" && i != argc)
	card = atoi(argv[++i]);
      else if (arg == "--trace")
	trace::quiet = false;
      else if (arg == "-h")
	height = atoi(argv[++i]);
      else
	usage (argv);
    }
  /********************/
  /* Load & Pre-treat */
  /********************/

  if (mydebug)
    dsp("Load & Pre-treat");

  typedef image2d<int_u8> I;
  I input;
  io::pgm::load(input, argv[1]);

  /***************************/
  /* Component tree creation */
  /***************************/

  if (mydebug)
    dsp("Component tree creation");

  typedef p_array< mln_site_(I) > S;
  typedef morpho::tree::data<I,S> tree_t;

  S sorted_sites = level::sort_psites_decreasing(input);
  tree_t tree(input, sorted_sites, c4());


  /******************************/
  /* Compute Attribute On Image */
  /******************************/
  if (mydebug)
    dsp("Image sharp attribute");

  // TODO: l'attribut devrait favoriser les composantes plus larges
  // dans son calcul. Ainsi au lieu de faire un sharpness, on aurait
  // un ratio sharpness / hauteur de composante (reprendre l'idee du
  // log utilise pour INIM).

  typedef morpho::attribute::sharpness<I> sharp_t;
  typedef mln_ch_value_(I, double) A;
  typedef mln_ch_value_(I, sharp_t) B;

  B a_img;
  A a = morpho::tree::compute_attribute_image(sharp_t (), tree, &a_img);
  // Note: then we work on nodes (component representant so we don't
  // need to propagate the representant value to the component sites.


  /***********************************/
  /* Components Filtering            */
  /***********************************/

  // So we compute card attribute and we filter big components
  // FIXME: some attributes are compositions of attributes, here
  // sharpness can give area so, it would be fine if we could give an
  // optional extra argument to compute_attribute where the
  // accumulators image will be stored.

  if (card)
    {
      if (mydebug)
	dsp("Image card attribute");

      a = duplicate((fun::p2v::ternary(card_wrapper(pw::value(a_img)) > pw::cst(card),
				       pw::value(a),
				       pw::cst(0.0))) | a.domain());
    }

  if (height)
    {
      if (mydebug)
	dsp("Image height attribute");
      a = duplicate((fun::p2v::ternary(height_wrapper(pw::value(a_img)) > pw::cst(height),
				       pw::value(a),
				       pw::cst(0.0))) | a.domain());
    }

  /************************************************/
  /* Retrieve Components (Maximising the criteria)  */
  /************************************************/

  accumulator::arg_max<A> argmax(a);
  p_array< mln_psite_(A) > obj_array; // Array of object components.

  if (mydebug) {
    std::stringstream s("Run max accumulator, look for ", std::stringstream::out|std::stringstream::in|
			std::stringstream::ate);
    if (nb_components)
      s << nb_components << " components.";
    else if (sharpness)
      s << "components whose treshold > " << sharpness;
    else
      s << "components util leaves are glutted";
    dsp(s.str());
  }

  if (nb_components) {
    obj_array = morpho::tree::get_components(tree, a, nb_components);
  } else if (sharpness > 0) {
    mln_VAR(predicate, pw::value(a) > pw::cst(sharpness));
    obj_array = morpho::tree::get_components(tree, a, predicate);
  } else {
    obj_array = morpho::tree::get_components(tree, a);
  }


  /* Print them */
  if (mydebug) {
    dsp("Image Filtered Components");
    mln_fwd_piter_(p_array< mln_psite_(I) >) c(obj_array);
    for_all(c)
      std::cout << c;
  }

  /***********************************/
  /* Use components in output image  */
  /***********************************/
  if (mydebug) {
    dsp("Create mask and propagate");
  }

  // Note: now we must propagate the representant value to the other components sites.
  A pre_output = morpho::tree::set_value_to_components(a, tree, obj_array, 0);
  I output = level::stretch(int_u8(), pre_output); //adapt to 0-255
  io::pgm::save(output, "components.pgm");


  /* EXTRA */

//   typedef mln_ch_value_(I, bool) M;
//   M mask;
//   initialize(mask, a);
//   data::fill(mask, false);

//   mln_fwd_piter_(p_array< mln_psite_(I) >) c(obj_array);
//   for_all(c)
//   {
//     mask(c) = true;
//     propagate_node_to_descendants(c, tree, mask);
//   }


  // mask now contains all nodes related to objects

 //  /* Labeling */
//     typedef mln_ch_value_(I, value::label<8>) L;
//     value::label<8> nlabel;
//     L label = labeling::blobs(mask, c4(), nlabel);
//     io::ppm::save(debug::colorize(value::rgb8(), label, nlabel), "label.pgm");


//   /* Now store output image */
//     I out;
//     initialize(out, input);
//     data::fill(out, 0);
//     data::paste(input | pw::value(mask), out);
//     io::pgm::save(out, "output.pgm");


}
