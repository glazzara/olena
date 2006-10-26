// Copyright (C) 2006 EPITA Research and Development Laboratory
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

/// Test the look-up table morpher.

#include <oln/basics2d.hh>
#include <oln/morpher/with_lut.hh>
#include <oln/value/color/rgb.hh>
#include <oln/level/fill.hh>
#include <oln/debug/print.hh>


// A simple (fake) routine.
template <typename I>
void image_test(const oln::abstract::image<I>&)
{
  // Do nothing.
}


/*--------------------------------------------------------------.
| Equipment of the new value type of the look-up table (rgb8).  |
`--------------------------------------------------------------*/

oln::value::color::rgb8 white(255, 255, 255);
oln::value::color::rgb8 blue (  0,   0, 255);
oln::value::color::rgb8 red  (255,   0,   0);

// We don't want to equip rgb8 with an operator<, nor to specialize
// std::less, so we pass the functor rgb8_less_t as fourth parameter
// of the look-up table.
xtd::lexi_less_t lexi_less;
struct rgb8_less_t
{
  bool operator()(const oln::value::color::rgb8& lhs,
		  const oln::value::color::rgb8& rhs) const
  {
    return lexi_less(lhs.to_vec(), rhs.to_vec());
  }
};

// FIXME: Move this elsewhere?
namespace oln
{
  std::ostream&
  operator<< (std::ostream& ostr, value::color::rgb8 val)
  {
    // The static_cast are needed because components of rgb8 are
    // unsigned chars, printed as a classic chars.
    ostr << "("
	 << static_cast<int>(val.red()) << ", "
	 << static_cast<int>(val.green()) << ", "
	 << static_cast<int>(val.blue())
	 << ")";
    return ostr;
  }

} // end of namespace oln


/*---------------------.
| Look-up table type.  |
`---------------------*/

typedef oln::lookup_table< unsigned, oln::value::color::rgb8,
			   std::less<unsigned>, rgb8_less_t > lut_t;


/*-------.
| Test.  |
`-------*/

int main()
{
  using oln::value::color::rgb8;

  unsigned data[] = { 0, 1, 2, 0, 1, 2, 0, 1, 2};

  typedef oln::image2d<unsigned> image_t;
  image_t ima(3, 3);
  oln::level::fill(ima, data);
  oln::debug::print(ima);

  lut_t lut;
  rgb8 c(16, 6, 4);
  lut.
    add(0, blue).
    add(1, white).
    add(2, c);

  typedef oln::morpher::with_lut<image_t, lut_t> image_with_lut_t;
  // FIXME: ``ima + lut'' doesn't work.  Maybe a `using' statement is
  // required.
  image_with_lut_t ima_with_lut(ima, lut);

#if 0
  // Disable the display to speed up the test.
  std::cout << "lut = "  << std::endl << ima_with_lut.lut() << std::endl;
  oln::debug::print(ima_with_lut);
  std::cout << std::endl;
#endif

  image_test(ima_with_lut);

#if 0
  std::cout << ima_with_lut.value(c) << std::endl;
#endif

  // Change a value in the LUT.
  ima_with_lut.value(c) = red;

#if 0
  // Disable the display to speed up the test.
  std::cout << "lut = "  << std::endl << ima_with_lut.lut() << std::endl;
  oln::debug::print(ima_with_lut);
  std::cout << std::endl;
#endif

  oln::point2d p(1, 2);
  assert(ima_with_lut(p) == red);
  assert(ima_with_lut.at(1, 2) == red);

  // FIXME: To be enabled later, when oln::level::fill is specialized
  // for abstract::mutable_image_being_value_wise_random_accessible.
#if 0
  oln::level::apply(ima_with_lut, fun); // 3 ops only !!!
#endif
}
