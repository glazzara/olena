// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef MLN_TEST_CONVERSION_HH
# define MLN_TEST_CONVERSION_HH

/// \file
///
/// Test a conversion on the pixel values of an image.

# include <mln/core/concept/image.hh>
# include <mln/data/convert.hh>
# include <mln/math/diff_abs.hh>
# include <mln/math/sqr.hh>
# include <mln/math/sqrt.hh>
# include <mln/value/rgb8.hh>


namespace mln
{

  namespace test
  {

    /// Test if all pixel values of \p ima can correctly convert into
    /// the type of \p value and then reconvert into their original
    /// type.
    ///
    /// \param[in] ima The image.
    /// \param[in] value A value of the conversion type.
    //
    template <typename I, typename V>
    float conversion(const Image<I>& ima, const V& value);

# ifndef MLN_INCLUDE_ONLY


    // Implementations.

    namespace impl
    {

      /// Distance between two color values.
      /// We have to specialized it for each type of color.
      template <typename V>
      inline
      float color_distance(const V& color1, const V& color2)
      {
	return -1.0; // FIXME
      }

      /// Distance between two RGB values.
      /// \return Value is normalized ([0..1]).
      template <>
      inline
      float color_distance<value::rgb8>(const value::rgb8& color1, const value::rgb8& color2)
      {
	float dist = 0.0;

	dist += math::diff_abs(color1.red(), color2.red());
	dist += math::diff_abs(color1.green(), color2.green());
	dist += math::diff_abs(color1.blue(), color2.blue());

	dist /= 3 * 255;

	return dist;
      }

      /// \return Root mean square.
      template <typename I, typename V>
      inline
      float conversion_(const I& ima, const V& value)
      {
	float delta = 0;

	typedef mln_ch_value(I, V) J;
	J converted = data::convert(value, ima);
	I output = data::convert(mln_value(I)(), converted);

	mln_piter(I) p(ima.domain());
	for_all(p)
	{
	  delta += math::sqr(color_distance(ima(p), output(p)));
	}

	delta /= ima.nelements();
	delta = math::sqrt(delta);
	return delta;
      }

    } // end of namespace mln::test::impl



    // Facades.


    template <typename I, typename V>
    inline
    float conversion(const Image<I>& ima, const V& value)
    {
      trace::entering("test::conversion");

      float res = impl::conversion_(exact(ima), value);

      trace::exiting("test::conversion");
      return res;
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::test

} // end of namespace mln


#endif // ! MLN_TEST_CONVERSION_HH
