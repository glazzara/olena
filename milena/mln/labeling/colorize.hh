// Copyright (C) 2008, 2009, 2010, 2011, 2013 EPITA Research and
// Development Laboratory (LRDE)
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

#ifndef MLN_LABELING_COLORIZE_HH
# define MLN_LABELING_COLORIZE_HH

/// \file
///
/// Fill an image with successive values.

# include <mln/core/concept/image.hh>
# include <mln/fun/i2v/array.hh>
# include <mln/value/rgb8.hh>
# include <mln/literal/black.hh>
# include <mln/data/transform.hh>
# include <mln/data/compute.hh>
# include <mln/accu/stat/max.hh>
# include <mln/util/array.hh>
# include <mln/util/set.hh>
# include <mln/value/next.hh>


namespace mln
{

  namespace labeling
  {

    // Forward declaration.
    namespace colorize_
    {
      extern unsigned min_value;
      extern unsigned max_value;
    }


    /// Create a new color image from a labeled image and fill each component
    /// with a random color.
    /*!
     * litera::black is used for component 0, e.g. the background.
     * Min and max values for RGB values can be set through the global
     * variables mln::labeling::colorize_::min_value and
     * mln::labeling::colorize_::max_value.
     *
     * \param[in] value value type used in the returned image.
     * \param[in] labeled_image A labeled image (\sa labeling::blobs).
     * \param[in] nlabels Number of labels.
     */
    template <typename V, typename L>
    mln_ch_value(L, V)
    colorize(const V& value,
	     const Image<L>& labeled_image,
	     const mln_value(L)& nlabels);


    /// \overload
    //
    template <typename V, typename L>
    mln_ch_value(L, V)
    colorize(const V& value,
	     const Image<L>& labeled_image);


    /// \overload
    //
    template <typename L>
    mln_ch_value(L, mln::value::rgb8)
    colorize(const Image<L>& input,
	     const mln_value(L)& nlabels);


# ifndef MLN_INCLUDE_ONLY

#  ifndef MLN_WO_GLOBAL_VARS

    namespace colorize_
    {
      unsigned min_value = 20;
      unsigned max_value = 220;
    }

#  endif // ! MLN_WO_GLOBAL_VARS

    namespace internal
    {

      inline
      unsigned random_number()
      {
	unsigned last = colorize_::min_value + (colorize_::max_value - colorize_::min_value + 1) * rand();

	return math::min(colorize_::min_value + last % colorize_::max_value, colorize_::max_value);
      }


      // No random color generator is available for the value type V.
      template <typename V>
      V random_color(const V&);


      template <typename RGB>
      RGB
      random_color_rgb(const RGB&)
      {
	static unsigned
	  nelements = colorize_::max_value - colorize_::min_value + 1;
	static util::array<util::set<unsigned> >
	  red_(nelements),
	  green_(nelements);

	unsigned red, green, blue;

	unsigned ntries = 0;
	do
	{
	  red = random_number();
	  ++ntries;
	}
	while (red_[red - colorize_::min_value].nelements() == nelements
	       && ntries < nelements);

	if (ntries == nelements)
	{
	  trace::warning("labeling::colorize - Can't find a new unique color. Returning black.");
	  return literal::black;
	}


	do
	  green = random_number();
	while (red_[red - colorize_::min_value].has(green)
	       || green_[green - colorize_::min_value].nelements() == nelements);
	red_[red - colorize_::min_value].insert(green);

	do
	  blue = random_number();
	while (green_[green - colorize_::min_value].has(blue));
	green_[green - colorize_::min_value].insert(blue);

	return RGB(red, green, blue);
      }

      template <unsigned n>
      mln::value::rgb<n>
      random_color(const mln::value::rgb<n>& v)
      {
	return random_color_rgb(v);
      }


# ifdef MLN_VALUE_QT_RGB32_HH

      inline
      mln::value::qt::rgb32
      random_color(const mln::value::qt::rgb32& v)
      {
	return random_color_rgb(v);
      }

# endif // ! MLN_VALUE_QT_RGB32_HH

    }

    template <typename V, typename L>
    inline
    mln_ch_value(L, V)
    colorize(const V& value,
	     const Image<L>& input,
	     const mln_value(L)& nlabels)
    {
      trace::entering("labeling::colorize");
      mln_precondition(exact(input).is_valid());
      // FIXME: check that V is a color type.
      // FIXME: we want to be sure that this is a label.
      // mlc_is_a(mln_value(L), mln::value::Symbolic)::check();
      (void) value;

      unsigned label_count = value::next(nlabels);
      static fun::i2v::array<V> f(0);
      int diff_size = f.size() - label_count;
      if (diff_size < 0)
      {
	srand(1);
	unsigned i = f.size();
	f.resize(label_count);
	// We want to treat comp 0 differently since it is the background.
	if (i == 0)
	{
	  i = 1;
	  f(0) = literal::black;
	}
	for (; i < f.size(); ++i)
	  f(i) = internal::random_color(value);
      }
      mln_assertion(f.size() >= (label_count));
      mln_ch_value(L, V) output = data::transform(input, f);

      trace::exiting("labeling::colorize");
      return output;
    }

    template <typename V, typename L>
    inline
    mln_ch_value(L, V)
    colorize(const V& value,
	     const Image<L>& input)
    {
      trace::entering("labeling::colorize");
      mln_precondition(exact(input).is_valid());

      accu::stat::max<mln_value(L)> accu;
      mln_value(L) nlabels = data::compute(accu, input);

      mln_ch_value(L,V) output = colorize(value, input, nlabels);

      trace::exiting("labeling::colorize");
      return output;
    }


    template <typename L>
    inline
    mln_ch_value(L, mln::value::rgb8)
    colorize(const Image<L>& input,
	     const mln_value(L)& nlabels)
    {
      return colorize(mln::value::rgb8(), input, nlabels);
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::labeling

} // end of namespace mln


#endif // ! MLN_LABELING_COLORIZE_HH
