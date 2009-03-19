#include <mln/essential/2d.hh>
#include <scribo/text/extract_lines.hh>
#include <scribo/filter/thin_bboxes.hh>

int usage(const char *name)
{
  std::cout << "Usage: " << name << " <input.pbm> " << std::endl;
  return 1;
}

int main(int argc, char *argv[])
{
  using namespace mln;

  if (argc < 1)
    return usage(argv[0]);

  scribo::make::internal::debug_filename_prefix = "thin_bboxes";

  image2d<bool> input;
  io::pbm::load(input, argv[1]);

  value::label_16 nlines;
  typedef scribo::util::text<image2d<value::label_16> > text_t;
  text_t lines = scribo::text::extract_lines(input, c8(), nlines);

  text_t filtered_lines = scribo::filter::thin_bboxes(lines, 5);

  scribo::debug::save_textbboxes_image(input, filtered_lines.bboxes(),
				       literal::red,
				       scribo::make::debug_filename("thickness_filter"));
}
