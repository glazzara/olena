#include <iostream>
#include <cassert>




void window_sliding(unsigned nr, unsigned nc,
		    unsigned h,  unsigned w)
{
  const unsigned h_2 = h / 2;
  const int d_row_up = - 1 - h_2, d_row_down = h_2;

  // top part
  for (int row = 0; row <= h_2; ++row)
    {
      assert(row + d_up < 0);
      assert(row + d_down >= 0  &&  row + d_down < nr);
      // top left
      // ...

      // top middle
      // ...

      // top right
      // ...
    }
  // main part
  for (int row = h_2 + 1; row <= nr - 1 - h_2; ++row)
    {
      assert(row + d_up   >= 0  &&  row + d_up   < nr);
      assert(row + d_down >= 0  &&  row + d_down < nr);
    }
  // bottom part
  for (int row = nr - h_2; row < nr; ++row)
    {
      assert(row + d_up >= 0  &&  row + d_up < nr);
      assert(row + d_down >= nr);
    }
}



int main()
{

//   unsigned nr = 50, nc = 100;

  unsigned nr = 10, nc = 10;
  unsigned h = 3, w = 5;
  window_sliding(nr, nc, h, w);

}
