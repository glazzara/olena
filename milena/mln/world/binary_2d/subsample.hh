
// FIXME: Complete this file.


template <typename I>
image2d<int_u8> subsample(const image2d<bool>& input, unsigned n)
{
  typedef bool* ptr;
  ptr p = new ptr(n);
  const unsigned nrows = input.nrows() / n;

  dpoint2d dp_row(1, 0);
  const unsigned delta_row = input.delta_index(dp_row);

  for (unsigned row = 0; row < nrows; ++row) 
    {
      unsigned count = 0;
      ptr[0] = & input(point2d(n * row, 0));
      for (unsigned i = 1; i < n; ++i)
	ptr[i] = ptr[i-1] + delta_row;
      for (unsigned col = 0; col < ncols; ++col)
	{
	  for (unsigned i = 0; i < n; ++i)
	    count += *(ptr[i]++);
	  output(point2d(row, col)) = count * 255 / n / n;
	} 
    }
}
