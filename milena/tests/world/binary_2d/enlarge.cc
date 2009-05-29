#include <mln/core/image/image2d.hh>
#include <mln/data/compare.hh>
#include <mln/world/binary_2d/enlarge.hh>
#include <mln/make/image.hh>
#include <mln/value/int_u8.hh>

#include "tests/data.hh"

mln::value::int_u8 vals_i[][4] = { { 4, 4, 4, 5 },
				   { 4, 4, 4, 5 },
				   { 3, 3, 4, 3 },
				   { 3, 3, 2, 2 } };

bool vals_b[][4] = { { 0, 0, 0, 1 },
		     { 0, 0, 0, 1 },
		     { 0, 0, 0, 0 },
		     { 1, 1, 0, 0 } };

int main()
{
  using namespace mln;

  {
    value::int_u8 vals[][2] = { { 4, 5 },
				{ 3, 2 } } ;

    image2d<value::int_u8> input = make::image(vals);
    image2d<value::int_u8> input_2x = world::binary_2d::enlarge(input,1);

    image2d<value::int_u8> ref = make::image(vals_i);

    mln_assertion(ref == input_2x);
  }

  {
    bool vals[][2] = { { 0, 1 },
		       { 1, 0 } } ;

    image2d<bool> input = make::image(vals);
    image2d<bool> input_2x = world::binary_2d::enlarge(input,1);

    image2d<bool> ref = make::image(vals_b);

    mln_assertion(ref == input_2x);
  }

}
