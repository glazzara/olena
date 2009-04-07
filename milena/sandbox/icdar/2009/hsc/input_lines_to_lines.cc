#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/labeling/background.hh>
#include <mln/value/int_u8.hh>
#include <mln/value/label_8.hh>
#include <mln/value/label_16.hh>
#include <mln/io/pbm/load.hh>
#include <mln/io/pgm/all.hh>
#include <mln/accu/lbl_maj.hh>
#include <mln/labeling/compute.hh>
#include <mln/pw/all.hh>
#include <mln/core/image/image_if.hh>
#include <mln/data/fill.hh>

void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pbm lines.pgm output.pgm" << std::endl
	    << "  HSC @ ICDAR'2009" << std::endl
	    << " Rebuild splitted components from a label image." << std::endl;
	    << "  input.pbm:   input 2D binary image (text is black; background is white)" << std::endl
	    << "  lines.pgm:   output image where line components are labeled (int_u8)" << std::endl
	    << "	       0 is the background label." << std::endl
	    << " output.pgm    output image where line components are labeled (int_u8)" << std::endl
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

  image2d<int_u8> lines;
  io::pgm::load(lines, argv[2]);

  label_16 nlabels;
  image2d<label_16> lbl = labeling::background(input, c8(), nlabels);
  util::array<box2d> bboxes = labeling::compute(accu::meta::bbox(), lbl, nlabels);

  /// Compute the most represented label for each component.
  accu::lbl_maj<label_16, int_u8> accu(nlabels.next());
  mln_piter_(image2d<int_u8>) p(lines.domain());
  for_all(p)
    if (lines(p) != 0u)
      accu.take(lbl(p), lines(p));


  // Rebuild components.
  util::array<util::couple<int_u8, float> > res = accu.to_result();
  for (unsigned i = 1; i < res.nelements(); ++i)
    if (res[i].second() >= 0.9f)
      data::fill(((lines | bboxes[i]).rw() | (pw::value(lbl) != 0u)).rw(), res[i].first());

  // Save result.
  io::pgm::save(lines, argv[3]);
}
