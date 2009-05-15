#include <mln/io/tiff/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/ppm/save.hh>
#include <mln/value/int_u8.hh>
#include <mln/value/label_16.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/make/rag_and_labeled_wsl.hh>
#include <mln/labeling/blobs.hh>
#include <mln/morpho/watershed/all.hh>

#include <graph.hh>
#include <get_lines.hh>
#include <superpose.hh>

// Graph dot output
void graph_dot_output(util::graph g, std::ostream& o)
{
  o << "digraph G {" << std::endl;

  // Display graph
  mln_edge_iter_(util::graph) e(g);
  mln_vertex_iter_(util::graph) v(g);

  for_all(v)
  {
    o << v << " [label=\"" << v << "\"]" << std::endl;
  }
  for_all(e)
  {
    o << e.v1() << "->" << e.v2()
      << " [label=\"" << e << "\"]" << std::endl;
  }

  o << "}" << std::endl;
}

// Convenient overload
void graph_dot_output(util::graph g, std::string filename)
{
  std::ofstream f;

  f.open(filename.c_str());
  graph_dot_output(g, f);
}


int main(int argc, char *argv[])
{
  using namespace mln;

  if (argc != 2)
  {
    std::cerr << "Usage: ./main file.tif" << std::endl;
    return 1;
  }

  std::cout << "Loading image..." << std::endl;
  // the max value is 255
  image2d<value::int_u8> ima;
  io::tiff::load(ima, argv[1]);
  std::cout << "[DONE]" << std::endl;

  std::cout << "Watershed..." << std::endl;
  // Calculating watershed.
  value::label_16 nb;
  image2d<value::label_16> ima_flooded =
    mln::morpho::watershed::flooding(ima, c8(), nb);
  std::cout << "[DONE]" << std::endl;

  // Result type (!)
  util::couple<util::graph,
    util::couple<image2d<value::label_16>,
    image2d<value::label_16> > > res;

  std::cout << "Building the CAG..." << std::endl;
  // build the cag
  res = graph::make_cag(ima_flooded, c8(), nb);

  // Graph output
  graph_dot_output(res.first(), "graph.dot");

  image2d<value::label_16> edges = res.second().first();
  image2d<value::label_16> vertices = res.second().second();
  std::cout << "[DONE]" << std::endl;

  std::cout << "Labeling lines..." << std::endl;

  // Remove vertical edges, label lines.
  image2d<value::label_16> lines = graph::get_lines(res.first(), edges,
						    vertices, 1.f);
  std::cout << "[DONE]" << std::endl;

  // Save results
  std::cout << "Saving images..." << std::endl;

  io::pgm::save(lines, "lines.pgm");
  io::pgm::save(ima_flooded, "ima_flooded.pgm");
  io::ppm::save(morpho::watershed::superpose(ima, ima_flooded, literal::red),
		"ima_with_flooded.ppm");

  io::pgm::save(edges, "edges.pgm");
  io::pgm::save(vertices, "vertices.pgm");

  image2d<value::rgb8> ima_spp;
  ima_spp = cag::superpose(ima, vertices, literal::green);
  ima_spp = cag::superpose(ima_spp, edges, literal::green);
  ima_spp = cag::superpose(ima_spp, lines, literal::red);
  io::ppm::save(ima_spp, "ima_spp.ppm");

  std::cout << "[DONE]" << std::endl;

  return 0;
}
