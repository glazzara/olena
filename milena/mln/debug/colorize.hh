// Copyright (C) 2008 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_DEBUG_COLORIZE_HH
# define MLN_DEBUG_COLORIZE_HH

/// \file mln/debug/colorize.hh
///
/// Fill an image with successive values.

# include <mln/core/concept/image.hh>
# include <mln/fun/i2v/array.hh>
# include <mln/value/rgb8.hh>
# include <mln/literal/black.hh>
# include <mln/level/transform.hh>


namespace mln
{

  namespace debug
  {

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
     * variables mln::debug::colorize_::min_value and
     * mln::debug::colorize_::max_value.
     *
     * \param[in] labeled_image A labeled image (\sa labeling::blobs).
     * \param[in] nlabels Number of labels.
     */
    template <typename I, typename L>
    mln_concrete(I) colorize(const Image<L>& labeled_image, mln_value(L) nlabels);


# ifndef MLN_INCLUDE_ONLY

    namespace colorize_
    {
      unsigned min_value = 20;
      unsigned max_value = 220;
    }


    namespace internal
    {

      template <typename V>
      V random_color();

      mln::value::rgb8
      random_color()
      {
	return mln::value::rgb8(colorize_::min_value + (rand() % colorize_::max_value),
				colorize_::min_value + (rand() % colorize_::max_value),
				colorize_::min_value + (rand() % colorize_::max_value));
      }

    }

    template <typename I, typename L>
    inline
    mln_concrete(I)
    colorize(const Image<L>& input, mln_value(L) nlabels)
    {
      trace::entering("debug::colorize");
      mln_precondition(exact(input).has_data());
      // FIXME: we want to be sure that this is a label.
      mlc_is_a(mln_value(L), mln::value::Symbolic)::check();

      unsigned label_count = nlabels.next();
      static fun::i2v::array<mln_value(I)> f(0);
      int diff_size = f.size() - label_count;
      if (diff_size < 0)
      {
	f.resize(label_count);
	unsigned i = f.size() + diff_size;
	// We want to treat comp 0 differently since it is the background.
	if (i == 0)
	{
	  i = 1;
	  f(0) = literal::black;
	}
	for (; i < f.size(); ++i)
	  f(i) = internal::random_color();
      }
      mln_precondition(f.size() == (label_count));
      mln_concrete(I) output = level::transform(input, f);

      trace::exiting("debug::colorize");
      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::debug

} // end of namespace mln


#endif // ! MLN_DEBUG_COLORIZE_HH
