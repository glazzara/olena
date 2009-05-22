#include <mln/essential/2d.hh>
#include <mln/core/var.hh>
#include <mln/fun/l2l/wrap.hh>
#include <mln/transform/distance_and_influence_zone_geodesic.hh>

int main(int, char *argv[])
{
  using namespace mln;

  using value::int_u16;
  using value::label_16;
  using value::label_8;
  using value::rgb8;

  image2d<bool> ima;
  io::pbm::load(ima, argv[1]);

  label_16 nlabels;
  image2d<label_16> lbl = labeling::blobs(ima, c8(), nlabels);

  mln_VAR(res, transform::distance_and_influence_zone_geodesic(lbl, c8(), mln_max(unsigned)));

  io::pgm::save(level::transform(res.first(), fun::l2l::wrap<label_8>()), "dmap.pgm");
  io::ppm::save(labeling::colorize(value::rgb8(), res.second(), nlabels), "iz.ppm");

}
