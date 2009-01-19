#include "ismm2009.hh"

#include <mln/value/int_u8.hh>
#include <mln/value/label_8.hh>
#include <mln/value/label_16.hh>

#include <mln/io/pgm/load.hh>
#include <mln/debug/println.hh>

#include <mln/morpho/elementary/gradient.hh>
#include <mln/morpho/gradient.hh>
#include <mln/morpho/tree/data.hh>
#include <mln/morpho/tree/compute_attribute_image.hh>
#include <mln/labeling/regional_minima.hh>

#include <mln/accu/count.hh>



void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pgm" << std::endl;
  std::cerr << "For Laurent's ISMM 2009 scheme: min-tree, attributes, and watershed." << std::endl;
  abort();
}



template <typename I, typename N, typename L>
void do_it(const I& g, const N& nbh, L& n_labels)
{
  using namespace mln;

  // regional minima

  mln_ch_value(I,L) regmin = labeling::regional_minima(g, nbh, n_labels);
  debug::println("regmin(g):", regmin);


  // watershed

  L n_basins;
  mln_ch_value(I,L) w = morpho::meyer_wst(g, nbh, n_basins);
  mln_invariant(n_basins == n_labels);
  debug::println("w(g):", w);


  {
    typedef p_array<mln_site(I)> S;
    S s = level::sort_psites_decreasing(g);
    
    typedef morpho::tree::data<I,S> tree_t;
    tree_t t(g, s, nbh);
    
    accu::count< util::pix<I> > a_; // Kind of attribute.
 
    mln_ch_value(I,unsigned) a = morpho::tree::compute_attribute_image(a_, t);
    debug::println("a:", a);

    debug::println("a | w line:", a | (pw::value(w) == pw::cst(0)));
    debug::println("a | w basins:", a | (pw::value(w) != pw::cst(0)));

    debug::println("a | regmin:", a | (pw::value(regmin) != pw::cst(0)));
  }

} // end of do_it



int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;


  typedef image2d<int_u8> I;
  typedef value::label_8 L;

  border::thickness = 0;

  if (argc != 2)
    usage(argv);

  I raw_f;
  io::pgm::load(raw_f, argv[1]);


  I f_ = add_edges(raw_f);
  mln_VAR(f, f_ | is_pixel);
  debug::println("f:", f);

  mln_VAR(g, f_ | is_edge);
  data::paste( morpho::gradient(extend(g, f_),
				e2p().win()),
	       g );
  debug::println("g:", g);

  L n;
  do_it(g, e2e(), n);

}
