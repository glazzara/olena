
#include <mln/core/image/image2d.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/label_16.hh>
#include <mln/value/rgb8.hh>
#include <mln/literal/colors.hh>

#include <mln/io/ppm/load.hh>
#include <mln/io/ppm/save.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/pbm/save.hh>
#include <mln/data/convert.hh>

#include <mln/morpho/watershed/flooding.hh>

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



  // Distance between 2 rgb8 colors.
  // -----------------------------------------------------------


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

  value::int_u8 dist_sat(const value::rgb8& c1, const value::rgb8& c2)
  {
    unsigned d = 0;
    d += math::diff_abs(c1.red(), c2.red());
    d += math::diff_abs(c1.green(), c2.green());
    d += math::diff_abs(c1.blue(), c2.blue());
    if (d > 255)
      d = 255;
    return d;
  }

  value::int_u8 dist_max(const value::rgb8& c1, const value::rgb8& c2)
  {
    unsigned d = 0, d_;
    d_ = math::diff_abs(c1.red(), c2.red());
    if (d_ > d) d = d_;
    d_ = math::diff_abs(c1.green(), c2.green());
    if (d_ > d) d = d_;
    d_ = math::diff_abs(c1.blue(), c2.blue());
    if (d_ > d) d = d_;
    return d;
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
    
    mln_up_node_piter(T) n(t);
    for_all(n)
      d(n) = dist_max(a(n), a(t.parent(n)));

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
  std::cerr << "usage: " << argv[0] << " input.ppm lambda echo output.ppm" << std::endl;
  std::cerr << "color version on pixels (without edges)" << std::endl;
  std::cerr << "attribute is 'distance of color means'" << std::endl;
  std::cerr << "  lambda >= 0 and <= 255" << std::endl;
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

  int lambda = atoi(argv[2]);
  if (lambda < 0 || lambda > 255)
    usage(argv);

  bool echo = atoi(argv[3]);


  // Neighborhood.
  mln_VAR(nbh, c4());


  // Changing input into 'f'.

  typedef image2d<int_u8> F;
  F f = dist_on_pixels(input, c4());


  typedef p_array<mln_psite_(F)> S;
  S s = data::sort_psites_decreasing(f);
  
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
  
  image2d<int_u8> g = do_filter(d, t, c4(), lambda);


  io::pbm::save( (pw::value(g) != pw::value(f)) | f.domain(),
		 "temp_activity.pbm" );


//     debug::println("activity (g != f) = ", (pw::value(g) != pw::value(f)) | f.domain());


  // Watershed transform.

  typedef value::label_16 L;
  L nbasins;
  mln_ch_value_(I, L) w = morpho::watershed::flooding(g, c4(), nbasins);
  std::cout << "n basins = " << nbasins << std::endl;

  image2d<rgb8> output = duplicate(input);
  data::fill((output | (pw::value(w) == 0)).rw(), literal::black);

  io::ppm::save(output, argv[4]);
}
