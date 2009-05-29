#include "../exec/filetype.hh"

#include <mln/core/alias/point2d.hh>
#include <mln/core/alias/dpoint2d.hh>

#include <mln/core/image/dmorph/image_if.hh>

#include <mln/data/fill.hh>
#include <mln/data/paste.hh>
#include <mln/pw/all.hh>

// #include <mln/labeling/colorize.hh>

#include <mln/transform/influence_zone_geodesic.hh>

// #include <mln/data/transform.hh>
// #include <mln/fun/v2v/wrap.hh>


using mln::value::int_u8;



namespace mln
{


  template <typename L>
  image2d<int_u8>
  left_iz(const image2d<bool>& in, const image2d<L>& ws_txt)
  {
    box2d b = ws_txt.domain();

    image2d<int_u8> ima(b);
    data::fill(ima, 0);

    unsigned nrows = ima.nrows();
    int_u8 l = 0;
    for (unsigned row = 0; row < nrows; ++row)
      if (ws_txt.at_(row, 0) == 0)
	ima.at_(row, 0) = ++l;

    io::pgm::save(ima, "tmp_iz_start.pgm");

    data::paste( transform::influence_zone_geodesic(ima | (pw::value(ws_txt) == pw::cst(0)),
						    c8(),
						    mln_max(unsigned)),
		 ima );

    image2d<int_u8> out(in.domain());
    data::fill(out, false);
    data::fill((out | pw::value(in)).rw(), 255);

    mln_piter(box2d) p(out.domain());
    for_all(p)
//       if (in(p) == true)
	{
	  point2d p_(p.row() / 4, p.col() / 4);
	  if (! ima.has(p_) || ima(p_) == 0)
	    continue;
	  out(p) = ima(p_);
	}

    return out;
  }


} // ! mln



void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pbm ws_txt.pgm output.ppm" << std::endl
	    << "  Label lines." << std::endl
	    << "  input is the large binary image;" << std::endl
	    << "  ws_txt is a small (25p) watershed image thru text." << std::endl;
  std::abort();
}



int main(int argc, char* argv[])
{
  using namespace mln;

  if (argc != 4)
    usage(argv);

  trace::entering("main");

  image2d<bool> in;
  io::pbm::load(in, argv[1]);

  image2d<int_u8> ws_txt, out;
  io::pgm::load(ws_txt, argv[2]);

  out = left_iz(in, ws_txt);

  io::pgm::save(out, argv[3]);

//   io::ppm::save(labeling::colorize(value::rgb8(), out, 254),
// 		argv[3]);

  trace::exiting("main");
}
