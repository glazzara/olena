# include "cs2d.hh"
# include "cs2d_morpho.hh"
# include "dbl_neighb.hh"

# include <mln/debug/println.hh>

# include <mln/io/pgm/load.hh>
# include <mln/io/pgm/save.hh>
# include <mln/io/ppm/save.hh>

# include <mln/data/paste.hh>
# include <mln/data/fill.hh>

# include <mln/morpho/dilation.hh>
# include <mln/morpho/meyer_wst.hh>
# include <mln/morpho/closing_area.hh>

# include <mln/logical/not.hh>
# include <mln/labeling/blobs.hh>
# include <mln/accu/stat/mean.hh>



void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pgm lambda output.pgm" << std::endl;
  abort();
}


struct is_row_odd_t
{ 
  bool operator()(const mln::point2d& p) const
  {
    return p.row() % 2;
  }
} is_row_odd;



int main(int argc, char* argv[])
{
  if (argc != 4)
    usage(argv);


  using namespace mln;
  using value::int_u8;

  // trace::quiet = false;

  cmorpho::dbl_neighb2d nbh_e;
  nbh_e
    .insert_even(dpoint2d(-1,-1))
    .insert_even(dpoint2d(-1,+1))
    .insert_even(dpoint2d( 0,-2))
    .insert_odd(dpoint2d(-2, 0))
    .insert_odd(dpoint2d(-1,-1))
    .insert_odd(dpoint2d(-1,+1));

  // display_masks(make::box2d(2,3));

  image2d<int_u8> lena;
  io::pgm::load(lena, argv[1]);

  cs2d<int_u8, int_u8, int_u8> cs(lena.domain());

  fill_cells_with(cs, lena);
  data::fill(cs.vertex, 0);

  cmorpho::dbl_neighb2d nbh_g;
  nbh_g.insert_odd(up).insert_even(left);
  data::paste(cmorpho::gradient(cs.edge, nbh_g),
	       cs.edge);
  // io::pgm::save(cs.impl, "grad.pgm");

  int lambda = atoi(argv[2]);
  if (lambda < 0)
    usage(argv);

  if (lambda != 0)
    data::paste(morpho::closing_area(cs.edge, nbh_e, lambda), cs.edge);

  cs2d<int_u8, bool, bool> cs_(lena.domain());
  data::fill(cs_.impl, false);
  fill_cells_with(cs_, lena);

  unsigned l;
  data::fill(cs_.edge,
	      pw::value(cmorpho::meyer_wst(cs.edge, nbh_e, l)) == pw::cst(0));
  std::cout << "l = " << l << std::endl;

  data::paste(morpho::dilation(cs_.vertex,
				convert::to_window(c4())),
	       cs_.vertex);

  unsigned L;
  // debug::println(cs_.impl);
  image2d<unsigned> label = labeling::blobs(logical::not_(cs_.impl), c4(), L);
  // debug::println(label);
  mln_invariant(L == l);

  std::vector<int_u8> v(l + 1);
  accu::compute<accu::stat::mean>(cs_.cell, label, v);

  data::fill(cs_.cell, data::transform(label, v));
  io::pgm::save(cell_image(cs_), argv[3]);

//   io::ppm::save(pack(cs_, colorize, 7), "log.ppm");
}
