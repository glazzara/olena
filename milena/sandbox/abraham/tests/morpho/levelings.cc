#include <mln/core/image/image2d.hh>
#include <mln/io/ppm/save.hh>
#include <mln/io/ppm/load.hh>
#include <mln/value/int_u8.hh>
#include <mln/morpho/vector_median.hh>
#include <mln/morpho/autarkical_leveling.hh>
#include <mln/fun/meta/red.hh>
#include <mln/value/rgb8.hh>
#include <mln/value/mixin.hh>
#include <mln/core/image/fun_image.hh>
#include <mln/core/image/violent_cast_image.hh>

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

      win::rectangle2d rect(8, 8);

      io::ppm::save(morpho::autarkical_leveling(ima, morpho::vector_median(ima, rect), rect), name.append("_autarkical_leveled.ppm"));

    }
  return 0;
}
