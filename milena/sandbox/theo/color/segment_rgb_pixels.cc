
#include <mln/core/image/image2d.hh>
#include <mln/core/image/image_if.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/label_16.hh>
#include <mln/value/rgb8.hh>
#include <mln/literal/colors.hh>

#include <mln/io/ppm/load.hh>
#include <mln/io/ppm/save.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/pbm/save.hh>
#include <mln/level/convert.hh>

#include <mln/morpho/meyer_wst.hh>

#include "mean_rgb_pix.hh"
#include "segment.hh"



namespace mln
{


  // Distance, stored on pixels, of neighboring colors.

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
	  int_u8 d_ = dist(input(p), input(n));
	  if (d_ > d)
	    d = d_;
	}
      output(p) = d;
    }

    io::pgm::save(output, "temp_dist.pgm");

    return output;
  }





  // From 'mean color' attributes to 'mean difference p/par(p)'.
  // -----------------------------------------------------------


  template <typename T, typename A>
  inline
  mln_ch_value(A, value::int_u8)
  dists_from_means(const T& t, const A& a, bool echo = false)
  {
    if (echo)
      {
	std::cout << "means:" << std::endl;
	display_tree_attributes(t, a);
      }

    mln_ch_value(A, value::int_u8) d;
    initialize(d, a);
    
    typedef typename T::nodes_t N;
    mln_fwd_piter(N) n(t.nodes());
    for_all(n)
      d(n) = dist(a(n), a(t.parent(n)));

    if (echo)
      {
	std::cout << "dists:" << std::endl;
	display_tree_attributes(t, d);
      }

    return d;
  }


} // mln



void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.ppm n echo output.ppm" << std::endl;
  std::cerr << "color version on pixels (without edges)" << std::endl;
  std::cerr << "  n >= 2" << std::endl;
  std::cerr << "  echo = 0 (mute) or 1 (verbose)" << std::endl;
  abort();
}



int main(int argc, char* argv[])
{
  using namespace mln;

  using value::int_u8;
  using value::rgb8;


  if (argc != 5)
    usage(argv);


  // Color debase (on pixels, not edges) version.
  // --------------------------------------------


  typedef image2d<rgb8> I;

  I input;
  io::ppm::load(input, argv[1]);

  unsigned n_objects = atoi(argv[2]);

  bool echo = atoi(argv[3]);


  // Neighborhood.
  mln_VAR(nbh, c4());


  // Changing input into 'f'.

  typedef image2d<int_u8> F;
  F f = dist_on_pixels(input, c4());


  typedef p_array<mln_psite_(F)> S;
  S s = level::sort_psites_decreasing(f);
  
  typedef morpho::tree::data<F,S> tree_t;
  tree_t t(f, s, c4());
  
  accu::rgb_image_ = input;
  accu::mean_rgb_pix< util::pix<F> > a_;
  mln_VAR(a, compute_attribute_on_nodes(a_, t));


  mln_VAR(d, dists_from_means(t, a, echo));
  make_attribute_grow(t, d);
  if (echo)
    {
      std::cout << "dists growing:" << std::endl;
      display_tree_attributes(t, d);
    }


  // BAD: extinct_attributes(t, d, echo);

  
  int_u8 lambda;
  unsigned less;
  image2d<int_u8> g = run_filter(d, t, c4(), n_objects, // input
				 less, lambda,         // output
				 echo);

  io::pbm::save( (pw::value(g) != pw::value(f)) | f.domain(),
		 "temp_activity.pbm" );

//   if (echo)
//     debug::println("activity (g != f) = ", (pw::value(g) != pw::value(f)) | f.domain());


  // Watershed transform.

  typedef value::label_16 L;
  L nbasins;
  mln_ch_value_(I, L) w = morpho::meyer_wst(g, c4(), nbasins);
  std::cout << "n basins = " << nbasins << std::endl;

  image2d<rgb8> output = duplicate(input);
  data::fill((output | (pw::value(w) == 0)).rw(), literal::black);

  io::ppm::save(output, argv[4]);
}
