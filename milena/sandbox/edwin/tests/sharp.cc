/* mln core */
#include <mln/core/image/image2d.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/core/routine/duplicate.hh>
#include <mln/core/var.hh>

/* Site set */
#include <mln/core/site_set/p_array.hh>
#include <mln/data/sort_psites.hh>

/* Component trees */
#include <mln/morpho/tree/data.hh>
#include <mln/morpho/tree/compute_attribute_image.hh>
#include <mln/morpho/tree/components.hh>
#include <mln/morpho/tree/filter.hh>
#include <mln/morpho/tree/propagate.hh>

/* Attributes */
#include <mln/morpho/attribute/mysharpness.hh>

/* io */
#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/ppm/save.hh>

/* data & pw */
#include <mln/core/concept/function.hh>
#include <mln/fun/p2v/ternary.hh>
#include <mln/data/fill.hh>
#include <mln/data/paste.hh>
#include <mln/pw/all.hh>

/* level */
//#include <mln/data/stretch.hh>

/* label */
#include <mln/labeling/blobs.hh>
#include <mln/value/label_16.hh>
#include <mln/labeling/colorize.hh>

/* trace */
#include <mln/trace/quiet.hh>
#include <mln/util/timer.hh>

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

void dsp(const std::string& str, mln::util::timer& timer)
{
  std::cout << "in " << timer.read() << std::endl;
  std::cout << std::endl
	    << "*********************" << std::endl
	    << "** " << str << std::endl
	    << "*********************" << std::endl;
  timer.restart();
}

namespace mln
{

  template <typename P2V>
  struct height_wrapper_ : Function_v2v< height_wrapper_<P2V> >
  {
    typedef unsigned result;

    height_wrapper_(const Function_v2v<P2V>& f) :
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
  height_wrapper(const Function_v2v<P2V>& f)
  {
    return height_wrapper_<P2V>(f);
  }

  template <typename P2V>
  struct card_wrapper_ : Function_v2v< card_wrapper_<P2V> >
  {
    typedef unsigned result;

    card_wrapper_(const Function_v2v<P2V>& f) :
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
  card_wrapper(const Function_v2v<P2V>& f)
  {
    return card_wrapper_<P2V>(f);
  }

  template <typename T, typename F, typename P2B>
  inline
  void
  mymin(const T& tree, Image<F>& f_, const Function_v2b<P2B>& pred_)
  {
    F& f = exact(f_);
    const P2B& pred = exact(pred_);

    mln_ch_value(F, bool) mark;
    initialize(mark, f);
    mln::data::fill(mark, false);

    mln_dn_node_piter(T) n(tree);
    for_all(n)
      if (mark(tree.parent(n)) || !pred(tree.parent(n)))
	{
	  f(n) = 0.0;
	  mark(n) = true;
	}
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
  util::timer timer;

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
    dsp("Load & Pre-treat", timer);

  typedef image2d<int_u8> I;
  I input;
  io::pgm::load(input, argv[1]);

  /***************************/
  /* Component tree creation */
  /***************************/

  if (mydebug)
    dsp("Component tree creation", timer);

  typedef p_array< mln_site_(I) > S;
  typedef morpho::tree::data<I,S> tree_t;

  S sorted_sites = data::sort_psites_decreasing(input);
  tree_t tree(input, sorted_sites, c4());


  /******************************/
  /* Compute Attribute On Image */
  /******************************/
  if (mydebug)
    dsp("Image sharp attribute", timer);


  typedef mln_ch_value_(I, double) A;
  A a;
  {
    typedef morpho::attribute::volume<I> v_attr;
    typedef mln_ch_value_(I, unsigned) H;
    typedef mln_ch_value_(I, v_attr) V;
    H h_img;
    V v_img;

    a = morpho::attribute::mysharpness(tree, &h_img, &v_img);

    // Component filtering
    if (height)
      mymin(tree, a, pw::value(h_img) > pw::cst(height));

    if (card)
      morpho::tree::filter::filter(tree, a, card_wrapper(pw::value(v_img)) > pw::cst(card), 0.0);
  }

  /************************************************/
  /* Retrieve Components (Maximising the criteria)  */
  /************************************************/

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
    dsp(s.str(), timer);
  }

  if (nb_components) {
    obj_array = morpho::tree::get_components(tree, a, nb_components);
  } else if (sharpness > 0) {
    mln_VAR(predicate, pw::value(a) > pw::cst(sharpness));
    obj_array = morpho::tree::get_components(tree, a, predicate);
  } else {
    obj_array = morpho::tree::get_components(tree, a);
  }

  /***********************************/
  /* Use components in output image  */
  /***********************************/
  if (mydebug) {
    dsp("Propagate components and stretch", timer);
  }

  // Note: now we must propagate the representant value to the other components sites.

  //a = morpho::tree::propagate_components(a, tree, obj_array, 0);
  //I output = data::stretch(int_u8(), a); //adapt to 0-255
  //io::pgm::save(output, "components.pgm");


  /* EXTRA */

  // labeling
  typedef mln_ch_value_(I, bool) M;
  M mask = morpho::tree::set_value_to_components(tree, obj_array, true, false);

  {
    typedef mln_ch_value_(I, value::label<16>) L;
    typedef mln_ch_value_(I, value::rgb<8>) O;
    value::label<16> nlabel;
    L label = labeling::blobs(mask, c4(), nlabel);
    O output = labeling::colorize(value::rgb8(), label, nlabel);
    io::ppm::save(output, "label.pgm");
  }

  if (mydebug) {
    dsp("Finish", timer);
  }

  /* Now store output image */
//   {
//    I out;
//    initialize(out, input);
//    data::fill(out, 255);
//    data::paste(input | pw::value(mask), out);
//    io::pgm::save(out, "output.pgm");
//   }

}
