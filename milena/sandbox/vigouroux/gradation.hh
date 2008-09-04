#ifndef GRADATION_HH
# define GRADATION_HH

# include <mln/core/image/image2d.hh>
# include <mln/value/rgb8.hh>
# include <mln/debug/println.hh>

#include <iostream>
#include <cstdlib>

namespace mln
{
  template <typename I, typename O>
  void gradation(const Image<I>& input_, Image<O>& output_)
  {
    const I& input = exact(input_);
    O& output = exact(output_);

    mln_piter(I) p(input.domain());
    for_all(p)
    {
      mln_value(I) i = input(p);

      if (i > 0)
	{
	  output(p).red() = 0;
	  output(p).green() = i;
	  output(p).blue() = 0;
	}
      else if (i < 0)
	{
	  output(p).red() = -i;
	  output(p).green() = 0;
	  output(p).blue() = 0;
	}
      else
	{
	  output(p).red() = 0;
	  output(p).green() = 0;
	  output(p).blue() = 0;
	}
    }
  }

  template <typename I, typename O>
  void gradation2(const Image<I>& input_, Image<O>& output_)
  {
    const I& input = exact(input_);
    O& output = exact(output_);
    float max = 0;

    mln_piter(I) p(input.domain());
    for_all(p)
    {
      mln_value(I) i = input(p);

      if (abs(i) > max)
	max = abs(i);
    }

    for_all(p)
    {
      mln_value(I) i = input(p);

      if (i > 0)
	{
	  output(p).hue() = 90.0027;
	  output(p).sat() = (i / max) * 208.207;
	  output(p).inty() = (i / max) * 147.224;
	}
      else if (i < 0)
	{
	  output(p).hue() = 329.999;
	  if ((-i/max)*208.207 <= 0)
	    std::cout << (-i/max)*208.207 << std::endl;
	  if ((-i/max)*208.207 > 208.207)
	    std::cout << (-i/max)*208.207 << std::endl;
	  output(p).sat() = (-i / max) * 208.207;
	  output(p).inty() = ((-i) / max) * 147.224;
	}
      else
	{
	  output(p).hue() = 0;
	  output(p).sat() = 0;
	  output(p).inty() = 0;
	}
    }
  }
}

#endif // !GRADATION_HH
