#include <mln/core/image/image2d.hh>
#include <mln/io/ppm/save.hh>
#include <mln/io/ppm/load.hh>
#include <mln/value/int_u8.hh>
#include <mln/morpho/erosion.hh>
#include <mln/fun/meta/red.hh>
#include <mln/value/rgb8.hh>
#include <mln/value/mixin.hh>
#include <mln/core/image/dmorph/fun_image.hh>
#include <mln/core/image/vmorph/violent_cast_image.hh>

namespace mln
{
  struct red_only
  {
    template <unsigned n>
    value::rgb<n> max_() const
    {
      return value::rgb<n>(mln_max(typename value::rgb<n>::red_t), 0, 0);
    }

    value::rgb8 max () const
    {
      return value::rgb8(255, 0, 0);
    }

    template <unsigned n>
    bool less(const value::rgb<n>& a, const value::rgb<n>& b)
    {
      return a.red() < b.red();
    }
  };

}


int main (int argc, const char * argv[])
{
  using namespace mln;
  using namespace value;

  if (argc < 2) {
    std::cerr << "usage: " << argv[0] << " in.pgm [other_files.pgm]" << std::endl;
    return 1;
  }

  typedef value::mixin<value::rgb8,red_only> Rgb;

  for (int i = 1; i < argc; ++i)
    {
      border::thickness = 2;

      image2d<rgb8> ima;
      io::ppm::load(ima, argv[i]);

      std::string name(argv[i]);
      name.erase(name.length() - 4);

      win::rectangle2d rect(5, 5);

      io::ppm::save(violent_cast_image_<rgb8>(morpho::erosion(violent_cast_image_<Rgb>(ima), rect)), name.append("_red_eroded.pgm"));

    }
  return 0;
}
