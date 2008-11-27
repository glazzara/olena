#include <mln/essential/2d.hh>
int main()
{
  using namespace mln;

  // \{
  bool vals[3][3] = { { 0, 1, 1 },
		      { 1, 0, 0 },
		      { 1, 1, 0 } };

  image2d<bool> ima_def = make::image(vals);
  debug::println_with_border(ima_def);

  std::cout << "===========" << std::endl << std::endl;

  border::thickness = 0;
  image2d<bool> ima_bt0 = make::image(vals);
  debug::println_with_border(ima_bt0);
  // \}

}
