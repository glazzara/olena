#include <mln/core/image/image2d.hh>
#include <mln/core/image/image_if.hh>

# include <mln/core/alias/neighb2d.hh>
# include <mln/make/double_neighb2d.hh>

#include <mln/value/rgb8.hh>
#include <mln/value/int_u16.hh>
#include <mln/value/int_s16.hh>

#include <mln/linear/gaussian.hh>
#include <mln/morpho/erosion.hh>

#include <mln/core/var.hh>
#include <mln/debug/println.hh>
#include <mln/opt/at.hh>

#include "src/io.hh"
#include "src/graph.hh"
#include "src/convert.hh"


void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.ppm output.pgm" << std::endl;
  abort();
}

/// FIXME Put these elsewhere
typedef mln::image2d<mln::value::rgb8>          Icolor;
typedef mln::image2d<mln::value::int_u16>       Igray;

void process(Icolor& graph, const Icolor& input)
{
  using namespace mln;

  // Neighborhood definition

//   bool e2c_h[] = { 0, 1, 0,
//                    0, 0, 0,
//                    0, 1, 0 };
//   bool e2c_v[] = { 0, 0, 0,
//                    1, 0, 1,
//                    0, 0, 0 };

//   bool e2e_h[] = { 0, 0, 1, 0, 0,
//                    0, 1, 0, 1, 0,
//                    0, 0, 0, 0, 0,
//                    0, 1, 0, 1, 0,
//                    0, 0, 1, 0, 0 };
//   bool e2e_v[] = { 0, 0, 0, 0, 0,
//                    0, 1, 0, 1, 0,
//                    1, 0, 0, 0, 1,
//                    0, 1, 0, 1, 0,
//                    0, 0, 0, 0, 0 };

//   bool e2p_h[] = { 0, 0, 1,
//                    0, 0, 0,
//                    1, 0, 0 };
//   bool e2p_v[] = { 1, 0, 0,
//                    0, 0, 0,
//                    0, 0, 1 };
  //mln_VAR(e2c, make::double_neighb2d(is_row_odd, e2c_h, e2c_v));


  // convert the image into grey level
  Igray gray_graph(graph.bbox());
  convert_to_gl(gray_graph, graph);

  Igray gray_input(input.bbox());
  convert_to_gl(gray_input, input);


  //data::paste(morpho::dilation((gray_graph | is_cell), e2c.win()), gray_graph);
//   Igray save(gray_graph.bbox());
//   data::paste(gray_graph | is_cell, save);
//   IO::save(save, "tmp3.ppm");

  // Create the laplacian image
  //Igray laplacian(gray_input.bbox());
  image2d<int> laplacian(gray_input.bbox());
  linear::gaussian(gray_input, 2.0f, laplacian);
  //IO::save(laplacian, "tmp1.ppm");
  linear::laplacian(gray_input, 2.0f, laplacian);
  debug::println(laplacian);
  //IO::save(laplacian, "tmp2.ppm");

  // Display edge on the output images
//   debug::println(laplacian);

//   mln_piter_(Igray) p(laplacian.domain());
//   for_all(p)
//     if (laplacian(p) == 0u)
//     {
//       std::cout << "I'm here" << std::endl;
//       opt::at(graph, p.row() * 2, p.col() * 2) = value::rgb<8>(255, 0, 0);
//     }

}



int main(int argc, char* argv[])
{
  using namespace mln;

  // Initialisation
  if (argc != 3)
    usage(argv);

  std::string input_file(argv[1]);
  std::string output_file(argv[2]);


  Icolor input;

  // Load the image
  IO::load(input, input_file);

  // create a graph image from the input
  Icolor graph;
  create_graph(input, graph);

  // Process
  process(graph, input);


  // Save
  IO::save(graph, output_file);
}
