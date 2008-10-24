#ifndef ENLARGE_HH
# define ENLARGE_HH

# include <iostream>

# include <mln/core/image/image2d.hh>
# include <mln/value/int_u8.hh>


float val(bool b) { return b ? 1 : 0; }

int do_threshold(float value)
{
  return 255.f * value;
}


mln::image2d<mln::value::int_u8>
enlargex2(mln::image2d<bool> input)
{
  using namespace mln;
  using value::int_u8;

  unsigned nrows, ncols;

  nrows = input.nrows();
  ncols = input.ncols();

  image2d<int_u8> output(2 * nrows, 2 * ncols);
  float value;

  // row 0

  output.at(0, 0) = do_threshold(input.at(0, 0));

  for (int col = 2; col < output.ncols(); col += 2)
    {
      value = val(input.at(0, col / 2));
      value += val(input.at(0, col / 2 - 1));
      output.at(0, col) = do_threshold(value / 2);
    }

  for (int col = 1; col < output.ncols(); col += 2)
    output.at(0, col) = do_threshold(input.at(0, col / 2));

  // col 0

  for (int row = 2; row < output.nrows(); row += 2)
    {
      value = val(input.at(row / 2, 0));
      value += val(input.at(row / 2 - 1, 0));
      output.at(row, 0) = do_threshold(value / 2);
    }

  for (int row = 1; row < output.nrows(); row += 2)
    output.at(row, 0) = do_threshold(input.at(row / 2, 0));

  // others

  for (int row = 2; row < output.nrows(); row += 2)
    {
      for (int col = 2; col < output.ncols(); col += 2)
        {
          value = val(input.at(row / 2, col / 2));
          value += val(input.at(row / 2 - 1, col / 2));
          value += val(input.at(row / 2, col / 2 - 1));
          value += val(input.at(row / 2 - 1, col / 2 - 1));
          output.at(row, col) = do_threshold(value / 4);
        }
      for (int col = 1; col < output.ncols(); col += 2)
        {
          value = val(input.at(row / 2, col / 2));
          value += val(input.at(row / 2 - 1, col / 2));
          output.at(row, col) = do_threshold(value / 2);
        }
    }

  for (int row = 1; row < output.nrows(); row += 2)
    {
      for (int col = 2; col < output.ncols(); col += 2)
        {
          value = val(input.at(row / 2, col / 2));
          value += val(input.at(row / 2, col / 2 - 1));
          output.at(row, col) = do_threshold(value / 2);
        }
      for (int col = 1; col < output.ncols(); col += 2)
        output.at(row, col) = do_threshold(input.at(row / 2, col / 2));
    }

  return output;
}


// enlarge 2^n times
mln::image2d<mln::value::int_u8>
enlarge(mln::image2d<bool> input, unsigned int n)
{
  using namespace mln;
  using value::int_u8;

  image2d<int_u8> output;

  do
  {
    output = enlargex2(input);

    if (--n > 0)
    {
      initialize(input, output);
      mln_piter_(image2d<bool>) p(input.domain());
      for_all(p)
	input(p) = output(p) > 0;
    }
    else
      break;
  }
  while (1);
  return output;
}

#endif // ! ENLARGE_HH
