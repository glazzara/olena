
#include <mln/core/var.hh>

#include <mln/value/label_16.hh>
#include <mln/value/int_u8.hh>
#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/debug/println.hh>

#include <mln/morpho/tree/data.hh>
#include <mln/morpho/tree/compute_attribute_image.hh>
#include <mln/morpho/attribute/height.hh>
#include <mln/labeling/regional_minima.hh>

#include "cplx2d.hh"
#include "extinct_attributes.hh"




void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pgm" << std::endl;
  std::cerr << "Laurent ISMM 2009 scheme." << std::endl;
  abort();
}




int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;
  using value::label_16;

  if (argc != 2)
    usage(argv);

  // f: regular image.

  image2d<int_u8> f;
  io::pgm::load(f, argv[1]);


  // g: weights on edges.

  mln_VAR(g, cplx2d::f_to_g(f) );
  debug::println("g:", g);


  // a: array "label -> attribute".

  typedef unsigned A;                        //  <---  Type of attributes.

  {
    typedef p_array<point2d> s_t;
    s_t s = data::sort_psites_decreasing(g); // min-tree

    typedef morpho::tree::data<g_t,s_t> tree_t;
    tree_t t(g, s, cplx2d::e2e());

    morpho::attribute::height<g_t> a;
    mln_VAR(h, morpho::tree::compute_attribute_image(a, t));
    debug::println("h:", h);

    unsigned l_max;

    mln_VAR(h_r, labeling::regional_minima(h, cplx2d::e2e(), l_max));
    debug::println("h_r:", h_r);

    extinct_attributes(t, h, /* echo = */ true);
    debug::println("he | nodes:", h | t.nodes());
  }

}
