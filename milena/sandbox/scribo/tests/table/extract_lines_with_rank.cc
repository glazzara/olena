#include <mln/essential/2d.hh>
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
  lineboxes.first().append(make::box2d(0,0, 59,59));
  lineboxes.first().append(make::box2d(0,28, 27, 32));
  lineboxes.first().append(make::box2d(31,28, 59,32));
  lineboxes.second().append(make::box2d(0,0, 59,59));
  lineboxes.second().append(make::box2d(27,0, 31, 26));
  lineboxes.second().append(make::box2d(27,34, 31,59));


  tblboxes_t lineboxes_test = scribo::table::extract_lines_with_rank(input, c8(),
				  value::label_16(), win::vline2d(11),
				  win::hline2d(11), 2, 2);

  mln_assertion(lineboxes == lineboxes_test);
}
