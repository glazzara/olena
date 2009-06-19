#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/core/var.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/label_16.hh>

#include <mln/io/pgm/all.hh>
#include <mln/io/ppm/save.hh>

#include <mln/debug/int2rgb.hh>
#include <mln/display/display_region.hh>
#include <mln/literal/colors.hh>
#include <mln/math/diff_abs.hh>
#include <mln/morpho/elementary/gradient.hh>
#include <mln/morpho/watershed/flooding.hh>
#include <mln/world/inter_pixel/compute.hh>
#include <mln/world/inter_pixel/display_region.hh>
#include <mln/world/inter_pixel/immerse.hh>
#include <mln/world/inter_pixel/is_pixel.hh>
#include <mln/world/inter_pixel/is_separator.hh>
#include <mln/world/inter_pixel/neighb2d.hh>


using namespace mln;
using value::int_u8;
using value::label_16;


struct dist_t : Function_vv2v<dist_t>
{
  typedef int_u8 result;

  template <typename V>
  int_u8 operator()(const V v1, const V v2) const
  {
    return math::diff_abs(v1, v2);
  }
} dist;



int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cout << "Usage: " << argv[0] << " input.pgm" << std::endl;
    return 1;
  }

  image2d<int_u8> input;
  io::pgm::load(input, argv[1]);

  // Gradient.
  image2d<int_u8> grad = morpho::elementary::gradient(input, c4());

  label_16 nbasins;

  // Normal watershed.
  image2d<label_16> normal_wst = morpho::watershed::flooding(input, c4(), nbasins);
  io::ppm::save(display::display_region(input, normal_wst, literal::red), "normal.ppm");



  // Distance on edges.
  typedef image_if<image2d<int_u8>, world::inter_pixel::is_pixel> Ix;
  Ix imax = world::inter_pixel::immerse(input);
  image_if<image2d<int_u8>, world::inter_pixel::is_separator> edges;
  edges = world::inter_pixel::compute(imax, dist);

  // On edges watershed.
  mln_VAR(edge_wst, morpho::watershed::flooding(edges, world::inter_pixel::e2e(), nbasins));
  io::ppm::save(world::inter_pixel::display_region(input, edge_wst.unmorph_(), literal::red), "edges.ppm");

  return 0;
}
