#include <mln/core/image/image2d.hh>
#include <mln/core/concept/function.hh>
#include <mln/io/pgm/load.hh>
#include <mln/io/pbm/save.hh>
#include <mln/data/transform.hh>
#include <mln/value/int_u8.hh>

namespace mln
{

  struct to_bin : public Function_v2v<to_bin>
  {
    typedef bool result;

    bool operator()(const value::int_u8& v) const
    {
      return v != 0;
    }

  };

}

int main(int argc, char *argv[])
{
  using namespace mln;

  if (argc < 3)
  {
    std::cout << "Usage: " << argv[1] << " input.pgm out.pbm" << std::endl;
    return 1;
  }

  image2d<value::int_u8> input;
  io::pgm::load(input, argv[1]);

  to_bin f;
  io::pbm::save(data::transform(input, f), argv[2]);
}
