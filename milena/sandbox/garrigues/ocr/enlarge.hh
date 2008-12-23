# include <iostream>

# include <mln/core/image/image2d.hh>
# include <mln/core/routine/initialize.hh>

# include <mln/value/int_u8.hh>
# include <mln/fun/p2v/ternary.hh>

# include <mln/pw/image.hh>
# include <mln/pw/cst.hh>
# include <mln/pw/value.hh>
# include <mln/opt/at.hh>

# include <mln/core/routine/duplicate.hh>


float val(bool b) { return b ? 1 : 0; }

int do_threshold(float value)
{
  return 255.f * value;
}

namespace mln
{

  image2d<value::int_u8>
  enlargex2(const image2d<bool>& input)
  {
    using value::int_u8;

    unsigned nrows, ncols;

    nrows = input.nrows();
    ncols = input.ncols();

    image2d<int_u8> output(2 * nrows, 2 * ncols);
    float value;

    // row 0

    opt::at(output, 0, 0) = do_threshold(opt::at(input, 0, 0));

    for (int col = 2; col < output.ncols(); col += 2)
      {
	value = val(opt::at(input, 0, col / 2));
	value += val(opt::at(input, 0, col / 2 - 1));
	opt::at(output, 0, col) = do_threshold(value / 2);
      }

    for (int col = 1; col < output.ncols(); col += 2)
      opt::at(output, 0, col) = do_threshold(opt::at(input, 0, col / 2));

    // col 0

    for (int row = 2; row < output.nrows(); row += 2)
      {
	value = val(opt::at(input, row / 2, 0));
	value += val(opt::at(input, row / 2 - 1, 0));
	opt::at(output, row, 0) = do_threshold(value / 2);
      }

    for (int row = 1; row < output.nrows(); row += 2)
      opt::at(output, row, 0) = do_threshold(opt::at(input, row / 2, 0));

    // others

    for (int row = 2; row < output.nrows(); row += 2)
      {
	for (int col = 2; col < output.ncols(); col += 2)
	  {
	    value = val(opt::at(input, row / 2, col / 2));
	    value += val(opt::at(input, row / 2 - 1, col / 2));
	    value += val(opt::at(input, row / 2, col / 2 - 1));
	    value += val(opt::at(input, row / 2 - 1, col / 2 - 1));
	    opt::at(output, row, col) = do_threshold(value / 4);
	  }
	for (int col = 1; col < output.ncols(); col += 2)
	  {
	    value = val(opt::at(input, row / 2, col / 2));
	    value += val(opt::at(input, row / 2 - 1, col / 2));
	    opt::at(output, row, col) = do_threshold(value / 2);
	  }
      }

    for (int row = 1; row < output.nrows(); row += 2)
      {
	for (int col = 2; col < output.ncols(); col += 2)
	  {
	    value = val(opt::at(input, row / 2, col / 2));
	    value += val(opt::at(input, row / 2, col / 2 - 1));
	    opt::at(output, row, col) = do_threshold(value / 2);
	  }
	for (int col = 1; col < output.ncols(); col += 2)
	  opt::at(output, row, col) = do_threshold(opt::at(input, row / 2, col / 2));
      }

    return output;
  }



  image2d<value::int_u8>
  enlargex2(const image2d<value::int_u8>& input)
  {
    using value::int_u8;

    unsigned nrows, ncols;

    nrows = input.nrows();
    ncols = input.ncols();

    image2d<int_u8> output(2 * nrows, 2 * ncols);
    unsigned value;

    // row 0

    opt::at(output, 0, 0) = (opt::at(input, 0, 0));

    for (int col = 2; col < output.ncols(); col += 2)
      {
	value = (opt::at(input, 0, col / 2));
	value += (opt::at(input, 0, col / 2 - 1));
	opt::at(output, 0, col) = (value / 2);
      }

    for (int col = 1; col < output.ncols(); col += 2)
      opt::at(output, 0, col) = (opt::at(input, 0, col / 2));

    // col 0

    for (int row = 2; row < output.nrows(); row += 2)
      {
	value = (opt::at(input, row / 2, 0));
	value += (opt::at(input, row / 2 - 1, 0));
	opt::at(output, row, 0) = (value / 2);
      }

    for (int row = 1; row < output.nrows(); row += 2)
      opt::at(output, row, 0) = (opt::at(input, row / 2, 0));

    // others

    for (int row = 2; row < output.nrows(); row += 2)
      {
	for (int col = 2; col < output.ncols(); col += 2)
	  {
	    value = (opt::at(input, row / 2, col / 2));
	    value += (opt::at(input, row / 2 - 1, col / 2));
	    value += (opt::at(input, row / 2, col / 2 - 1));
	    value += (opt::at(input, row / 2 - 1, col / 2 - 1));
	    opt::at(output, row, col) = ((unsigned(value)+2) / 4);
	  }
	for (int col = 1; col < output.ncols(); col += 2)
	  {
	    value = (opt::at(input, row / 2, col / 2));
	    value += (opt::at(input, row / 2 - 1, col / 2));
	    opt::at(output, row, col) = (value / 2);
	  }
      }

    for (int row = 1; row < output.nrows(); row += 2)
      {
	for (int col = 2; col < output.ncols(); col += 2)
	  {
	    value = (opt::at(input, row / 2, col / 2));
	    value += (opt::at(input, row / 2, col / 2 - 1));
	    opt::at(output, row, col) = (value / 2);
	  }
	for (int col = 1; col < output.ncols(); col += 2)
	  opt::at(output, row, col) = (opt::at(input, row / 2, col / 2));
      }

    return output;
  }






  // enlarge 2^n times
  image2d<value::int_u8>
  enlarge(const image2d<bool>& input, unsigned int n)
  {
    using value::int_u8;

    if (n == 0)
      return duplicate(fun::p2v::ternary(pw::value(input),
				     pw::cst(int_u8(255)),
				     pw::cst(int_u8(0)))
		   | input.domain());

    image2d<int_u8> output = enlargex2(input);

    while (--n)
      output = enlargex2(output);

    return output;
  }

} // mln
