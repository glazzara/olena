#include <mln/essential/2d.hh>
#include <scribo/table/repair_vertical_lines.hh>
#include <scribo/table/repair_horizontal_lines.hh>
#include <scribo/table/extract_lines_with_rank.hh>
#include <scribo/debug/save_table_image.hh>
#include <mln/util/couple.hh>

int main(int argc, char *argv[])
{
  using namespace mln;

  if (argc < 2)
  {
    std::cout << "Usage: " << argv[0] << " <image.pbm>" << std::endl;
    return 1;
  }

  image2d<bool> input;
  io::pbm::load(input, argv[1]);

  typedef util::couple<util::array<box2d>,
		       util::array<box2d> > tblboxes_t;

  tblboxes_t lineboxes;
  lineboxes.first().append(make::box2d(0,0, 1,1)); // Dummy value for component 0.
  lineboxes.first().append(make::box2d(0,28, 27, 32));
  lineboxes.first().append(make::box2d(31,28, 59,32));
  lineboxes.second().append(make::box2d(0,0, 1,1)); // Dummy value for component 0.
  lineboxes.second().append(make::box2d(27,0, 31, 26));
  lineboxes.second().append(make::box2d(27,34, 31,59));


//  tblboxes_t lineboxes = scribo::table::extract_lines_with_rank(input, c8(), value::label_16(), win::vline2d(11), win::hline2d(11), 2, 2);
//
//  std::cout << lineboxes.first() << std::endl;
//  std::cout << lineboxes.second() << std::endl;

  scribo::table::repair_vertical_lines(input, lineboxes, 31);
  scribo::table::repair_horizontal_lines(input, lineboxes, 31);

  mln_assertion(lineboxes.first().nelements() == 2);
  mln_assertion(lineboxes.second().nelements() == 2);
  mln_assertion(lineboxes.first()[1] == make::box2d(0,28, 59,32));
  mln_assertion(lineboxes.second()[1] == make::box2d(27,0, 31,59));
}
