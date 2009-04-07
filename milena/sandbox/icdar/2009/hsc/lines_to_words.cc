#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/value/int_u8.hh>
#include <mln/value/label_8.hh>
#include <mln/value/label_16.hh>
#include <mln/io/pgm/all.hh>
#include <mln/io/pbm/all.hh>
#include <mln/io/ppm/all.hh>
#include <mln/accu/bbox.hh>
#include <mln/labeling/compute.hh>
#include <mln/pw/all.hh>
#include <mln/core/image/image_if.hh>
#include <mln/data/fill.hh>
#include <mln/core/routine/extend.hh>
#include <mln/transform/distance_geodesic.hh>
#include <mln/win/disk2d.hh>
#include <mln/morpho/watershed/flooding.hh>
#include <mln/morpho/closing/structural.hh>
#include <mln/transform/distance_geodesic.hh>
#include <mln/core/image/extended.hh>
#include <mln/core/var.hh>
#include <mln/level/convert.hh>
#include <mln/level/compute.hh>
#include <mln/accu/count_labels.hh>
#include <mln/labeling/relabel.hh>
#include <mln/labeling/wrap.hh>
#include <mln/core/routine/extend.hh>
#include <mln/morpho/watershed/superpose.hh>

void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << "lines.pgm nlines output.pgm" << std::endl
	    << "  HSC @ ICDAR'2009" << std::endl
	    << " Split lines into words." << std::endl
	    << "  input.pbm:   input 2D binary image (text is black; background is white)" << std::endl
	    << "  lines.pgm:   output image where line components are labeled (int_u8)" << std::endl
	    << "	       0 is the background label." << std::endl
	    << " output.pgm:   output image where line components are labeled (int_u8)" << std::endl
	    << "	       0 is the background label." << std::endl;
  std::abort();
}


int main(int argc, char *argv[])
{
  using namespace mln;
  using namespace mln::value;

  if (argc < 4)
    usage(argv);

  image2d<bool> input;
  io::pbm::load(input, argv[1]);

  image2d<int_u8> lines_;
  io::pgm::load(lines_, argv[2]);
  image2d<label_16> lines = level::convert(label_16(), lines_);

  unsigned delta = 10;

  label_16 nlines;
  labeling::relabel_inplace(lines, nlines);


  std::cout << "nlines = " << nlines << std::endl;
  util::array<box2d> bboxes = labeling::compute(accu::meta::bbox(), lines, nlines);

  std::cout << "BBoxes computed" << std::endl;

  for (unsigned i = 1; i < nlines; ++i)
  {
    std::cout << "* Processing line " << i << std::endl;
    typedef image2d<bool> line_t;
    line_t line = duplicate((pw::value(lines) == i) |  (lines | bboxes[i]).domain());
//    io::pbm::save(line, "l2w_line.pbm");

    box2d b = geom::bbox(line);
    b.enlarge(delta);

    mln_VAR(line_ext, extended_to(extend(line,false),b));

//    io::pbm::save(line_ext, "l2w_line_ext.pbm");

    std::cout << "   Distance geodesic" << std::endl;
    mln_VAR(line_dmap, transform::distance_geodesic(line_ext, c4(), mln_max(unsigned)));

    std::cout << "   closing structural" << std::endl;
    mln_VAR(line_clo, morpho::closing::structural(line_dmap, win::disk2d(21)));


    std::cout << "   Watershed" << std::endl;
    label_16 nwords;
    mln_VAR(wst, morpho::watershed::flooding(line_clo, c4(), nwords));

//    io::ppm::save(morpho::watershed::superpose(input | bboxes[i], wst), "l2w_wst.ppm");
//    abort();
  }
}
