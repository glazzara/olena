#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/data/fill.hh>
#include <mln/data/paste.hh>
#include <mln/data/convert.hh>
#include <mln/data/saturate.hh>

#include <mln/labeling/value.hh>
#include <mln/labeling/colorize.hh>

#include <mln/morpho/watershed/superpose.hh>
#include <mln/morpho/watershed/flooding.hh>
#include <mln/morpho/elementary/dilation.hh>
#include <mln/morpho/closing/area.hh>
#include <mln/morpho/closing/structural.hh>

#include <mln/transform/distance_front.hh>
#include <mln/transform/influence_zone_front.hh>

#include <mln/pw/all.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/core/image/dmorph/sub_image.hh>

#include <mln/make/w_window2d.hh>

#include "apps/morphers/lazy_recorder.hh"

#include <mln/io/pbm/load.hh>
#include <mln/io/pbm/save.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/ppm/save.hh>
#include <mln/io/magick/save.hh>

#include <mln/literal/colors.hh>


// Save as PNG directly, instead of using PPM as intermediate format.
template <typename I>
inline
void
save_colorized(const mln::decorated_image< I, lazy_recorder<I> >& rec,
	       const std::string& prefix)
{
  mln_concrete(I) frame = mln::duplicate(rec.decoration().initial);
  std::stringstream s;
  s << std::setfill ('0') << std::setw (6) << 0;
  mln::io::magick::save(mln::labeling::colorize(mln::value::rgb8(), frame),
			prefix + s.str() + ".png");
  for (size_t i = 0; i < rec.decoration().sequence.size(); ++i)
    {
      // Next change (`frame(p)' is assigned the value `v' in the next
      // frame).
      mln_psite(I) p = rec.decoration().sequence[i].first;
      mln_value(I) v = rec.decoration().sequence[i].second;
      // Skip consecutive identical frames.
      if (frame(p) == v)
	continue;
      frame(p) = v;
      std::stringstream s;
      s << std::setfill ('0') << std::setw (6) << i + 1;
      mln::io::magick::save(mln::labeling::colorize(mln::value::rgb8(), frame),
			    prefix + s.str() + ".png");
    }
}


void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pbm output.ppm" << std::endl;
  std::abort();
}


int main(int argc, char* argv[])
{
  // Initialize Magick++.
  Magick::InitializeMagick(*argv);

  using namespace mln;
  using value::int_u8;

  if (argc != 3)
    usage(argv);

  image2d<bool> input, clo;
  io::pbm::load(input, argv[1]);


  /// Structural closing.
  clo = morpho::closing::structural(input,
				    win::rectangle2d(5, 17));

  io::pbm::save(clo, "tmp_clo.pgm");


  /// Distance map computation.
  int ww[] = { 00, 11,  0, 11,  0,
  	       11,  7,  5,  7, 11,
  	       00,  5,  0,  5,  0,
  	       11,  7,  5,  7, 11,
  	       00, 11,  0, 11,  0 };


  image2d<int_u8> dmap;
  dmap = transform::distance_front(clo,
				   c4(), make::w_window2d(ww),
				   mln_max(int_u8));
  dmap = morpho::closing::area(dmap, c4(), 500);
  
  io::pgm::save(dmap, "tmp_dmap.pgm");


  unsigned nbasins;
  image2d<unsigned> ws = morpho::watershed::flooding(dmap,
						     c4(),
						     nbasins);

  io::ppm::save(labeling::colorize(value::rgb8(), ws), "tmp_ws.ppm");

  {
    image2d<value::rgb8> ws_ = data::convert(value::rgb8(), input);
    data::fill((ws_ | (pw::value(ws) == pw::cst(0))).rw(), literal::red);
    io::ppm::save(ws_, "tmp_ws_superpose.ppm");

    // test% g++ -I. main.cc -DNDEBUG -O2
    // main.cc: In function ‘int main(int, char**)’:
    // main.cc:85: error: no matching function for call to ‘convert(mln::image2d<bool>&, mln::value::rgb8)’

    // /// Convert the image \p input by changing the value type.
    // ///
    // /// \param[in] v A value of the destination type.
    // /// \param[in] input The input image.
    // //
    // template <typename V, typename I>
    // mln_ch_value(I, V)
    // convert(const V& v, const Image<I>& input);
  }


  image2d<unsigned> lab(input.domain());
  data::fill(lab, 0);

  unsigned nlabels;
  data::paste(labeling::value(ws | make::box2d(0,0,input.nrows()-1,0),
			      0, c4(), nlabels),
	      lab);

  decorated_image< image2d<unsigned>,
		   lazy_recorder< image2d<unsigned> > > rec_lab =
    lazy_record(lab);

  io::pgm::save(data::saturate(value::int_u8(), lab), "tmp_lab.pgm");


  image_if< decorated_image< image2d<unsigned int>,
			     lazy_recorder< image2d<unsigned int> > >,
	    fun::eq_v2b_expr_< pw::value_< image2d<unsigned int> >,
			       pw::cst_<int> > > rec_iz =
    transform::influence_zone_front(rec_lab | (pw::value(ws) == pw::cst(0)),
				    c8(),
				    make::w_window2d(ww));
  data::paste(rec_iz, lab);
  io::pgm::save(data::saturate(value::int_u8(), lab), "tmp_iz.pgm");

  save_colorized(rec_iz.unmorph_(), "tmp_iz");

  image2d<value::rgb8> output = labeling::colorize(value::rgb8(), lab);
  io::ppm::save(output, "tmp_iz.ppm");

  data::fill((output | pw::value(input)).rw(), literal::white);
  io::ppm::save(output, "tmp_iz_input.ppm");
}
