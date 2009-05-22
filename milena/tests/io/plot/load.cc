#include <mln/io/plot/load.hh>
#include <mln/util/array.hh>


using namespace mln;

int main(int argc, char* argv[])
{
  util::array<float> arr;

  io::plot::load(arr, argv[1]);

  for (unsigned i = 0; i < arr.nelements(); ++i)
    std::cout << arr[i] << std::endl;

  return 0;
}
