#include "graph_labeling.hh"

int main()
{
  using namespace mln;
  using value::int_u8;

  image2d_b<int_u8> ima = io::pgm::load("/lrde/beyrouth/stage/duhamel/pub/img_1.pgm");
  image2d_b<int_u8> im (ima.domain ());
  mesh_p<point2d>* m = make_graph (ima, c4());
  draw::mesh (im, *m, 7, 1);

  debug::println (im);
}
