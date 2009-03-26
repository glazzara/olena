#include <mln/essential/2d.hh>
#include <mln/transform/distance_and_influence_zone_geodesic.hh>
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

  mln_VAR(res, transform::distance_and_influence_zone_geodesic(lbl, c8(), mln_max(unsigned)));

  io::pgm::save(level::transform(res.first(), fun::l2l::wrap<label_8>()), "dmap.pgm");
  io::ppm::save(debug::colorize(rgb8(), res.second(), nlabels), "iz.ppm");

  image2d<unsigned>& dmap = res.first();
  {
    image2d<unsigned> clo = morpho::closing::structural(dmap, win::hline2d(51));
    io::pgm::save(clo, scribo::make::debug_filename("clo_line_51.pgm"));
    label_16 nlabels;
    image2d<label_16> wsd = morpho::watershed::flooding(clo, c8(), nlabels);

    io::ppm::save(morpho::watershed::superpose(input, wsd),
		  scribo::make::debug_filename("wsd_line_51.ppm"));
  }

  {
    image2d<unsigned> clo = morpho::closing::structural(dmap, win::rectangle2d(11, 101));
    io::pgm::save(clo, scribo::make::debug_filename("clo_rectangle_11_101.pgm"));
    label_16 nlabels;
    image2d<label_16> wsd = morpho::watershed::flooding(clo, c8(), nlabels);

    io::ppm::save(morpho::watershed::superpose(input, wsd),
		  scribo::make::debug_filename("wsd_rectangle_11_101.ppm"));
  }

  {
    image2d<unsigned> clo = morpho::closing::structural(dmap, win::disk2d(51));
    io::pgm::save(clo, scribo::make::debug_filename("clo_disk_51.pgm"));
    label_16 nlabels;
    image2d<label_16> wsd = morpho::watershed::flooding(clo, c8(), nlabels);

    io::ppm::save(morpho::watershed::superpose(input, wsd),
		  scribo::make::debug_filename("wsd_disk_51.ppm"));
  }

}
