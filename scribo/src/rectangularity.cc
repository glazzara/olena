#include <mln/essential/2d.hh>
#include <mln/transform/distance_geodesic.hh>
#include <mln/core/var.hh>
#include <mln/fun/l2l/wrap.hh>
#include <mln/win/hline2d.hh>
#include <mln/morpho/watershed/flooding.hh>
#include <mln/morpho/watershed/superpose.hh>

#include <scribo/make/debug_filename.hh>

int main(int argc, char *argv[])
{
  using namespace mln;

  using value::int_u16;
  using value::label_16;
  using value::label_8;
  using value::rgb8;

  image2d<bool> input;
  io::pbm::load(input, argv[1]);

  scribo::make::internal::debug_filename_prefix = "morpho";

  label_16 nlabels;
  image2d<label_16> lbl = labeling::blobs(input, c8(), nlabels);

  mln_VAR(dmap, transform::distance_geodesic(lbl, c8(), mln_max(unsigned)));

  io::pgm::save(level::transform(dmap, fun::l2l::wrap<label_8>()), "dmap.pgm");

  {
    image2d<unsigned> clo = morpho::closing::structural(dmap, win::disk2d(51));
    io::pgm::save(clo, scribo::make::debug_filename("clo_disk_51.pgm"));
    image2d<float> r = morpho::tree:compute_attribute_image(accu::site_set::rectangularity<point2d>(),clo);
    image2d<point2d> r = morpho::tree:max(r,c8());
  }
}
