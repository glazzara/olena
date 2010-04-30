# include <mln/core/image/image2d.hh>
# include <mln/core/alias/neighb2d.hh>
# include <mln/topo/skeleton/crest.hh>
# include <mln/topo/skeleton/is_simple_point.hh>
# include <mln/transform/distance_front.hh>
# include <mln/logical/not.hh>
# include <mln/make/w_window2d_int.hh>
# include <mln/value/int_u8.hh>
# include <mln/arith/revert.hh>
# include <mln/io/pgm/save.hh>
# include <mln/io/pgm/load.hh>
# include <mln/io/pbm/save.hh>
# include <mln/io/pbm/load.hh>
# include <mln/morpho/skeleton_constrained.hh>
# include <mln/core/routine/extend.hh>

int main(int argc, char *argv[])
{
  using namespace mln;

  image2d<bool> input;
  io::pbm::load(input, argv[1]);

  int vals[] = { 0, 9, 0, 9, 0,
		 9, 6, 4, 6, 9,
		 0, 4, 0, 4, 0,     // Values of distances.
		 9, 6, 4, 6, 9,
		 0, 9, 0, 9, 0 };

  image2d<value::int_u8>
    dist_map_n = transform::distance_front(logical::not_(input), c8(),
					   make::w_window2d_int(vals),
					   mln_max(value::int_u8));

  image2d<value::int_u8> dist_map = arith::revert(dist_map_n);

  io::pgm::save(dist_map, "distance.pgm");
  io::pgm::save(dist_map_n, "distance_n.pgm");

  // Constraint
  image2d<bool> K = topo::skeleton::crest(input, dist_map_n, c8(), atoi(argv[2]));
  io::pbm::save(K, "constraint.pbm");

  typedef image2d<bool> I;

  I skel =
    morpho::skeleton_constrained(input, c8(),
				 topo::skeleton::is_simple_point<I,neighb2d>,
				 extend(K, false), dist_map);

  io::pbm::save(skel, "skeleton.pbm");
}
