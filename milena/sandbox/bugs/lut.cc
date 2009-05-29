/// Passing a fun::i2v::array to data::Transform can lead to a segfault.
/// Through the dispatch of data::transform, a dispatch function detects that
/// we are using int_u8 as values, meaning a low quantification value type.
/// It tries to compute a look-up table (lut) which for each int_u8 value
/// (0-255), maps a new value computed thanks to the function.

#include <mln/core/image/image2d.hh>
#include <mln/fun/i2v/array.hh>
#include <mln/value/int_u8.hh>
#include <mln/data/transform.hh>

int main()
{
  using namespace mln;

  image2d<value::int_u8> ima(4, 4);

  /// This function can compute values for 0,1 and 2.
  fun::i2v::array<bool> f(3, true);

  /// Crash, since the lut computes values for 0-255.
  data::transform(ima, f);
}
