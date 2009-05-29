#include <mln/core/image/image2d.hh>
#include <mln/value/rgb8.hh>


namespace mln
{

  namespace debug
  {

    using value::rgb8;

    template <typename V>
    image2d<rgb8>
    int2rgb(const image2d<V>& input)
    {
      image2d<rgb8> output;
      initialize(output, input);

      mln_piter(image2d<V>) p(input.domain());
      for_all(p)
      {
	unsigned value = input(p);
	output(p).blue() = value % 256;
	value /= 256;
	output(p).green() = value % 256;
	value /= 256;
	output(p).red() = value % 256;
      }

      return output;
    }

} // end of namespace mln::debug

} // end of namespace mln
