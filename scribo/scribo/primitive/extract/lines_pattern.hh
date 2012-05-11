// Copyright (C) 2009, 2010 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef SCRIBO_PRIMITIVE_EXTRACT_LINES_PATTERN_HH
# define SCRIBO_PRIMITIVE_EXTRACT_LINES_PATTERN_HH

/// \file
///
/// Extract lines matching a specific pattern.
///
/// \todo Make a more generic canvas.

# include <mln/core/concept/image.hh>
# include <mln/core/concept/window.hh>
# include <mln/core/routine/duplicate.hh>

# include <mln/extension/adjust_fill.hh>

# include <mln/geom/ncols.hh>

# include <mln/accu/transform_line.hh>
# include <mln/accu/count_value.hh>

# include <mln/data/paste.hh>


namespace scribo
{

  namespace primitive
  {

    namespace extract
    {

      using namespace mln;

      /// Extract lines with a specific pattern
      ///
      /// \param[in] input_ A binary image.
      /// \param[in] length The minimum line length.
      /// \param[in] dir    The direction of the lines.
      /// \param[in] win_   A window corresponding to the line pattern.
      ///
      /// \return A image with lines of direction \p dir.
      ///
      //
      template <typename I, typename W>
      mln_concrete(I)
      lines_pattern(const Image<I>& input_, unsigned length,
		    unsigned dir, const Window<W>& win_);


# ifndef MLN_INCLUDE_ONLY


      // Implementations

      namespace impl
      {

	namespace generic
	{

	  template <typename I, typename W>
	  mln_concrete(I)
	  lines_pattern(const Image<I>& input_, unsigned length,
			unsigned dir, const Window<W>& win_)
	  {
	    trace::entering("scribo::primitive::extract::impl::generic::lines_pattern");

	    const I& input = exact(input_);
	    const W& win = exact(win_);
	    mlc_is(mln_value(I), bool)::check();
	    mln_precondition(input.is_valid());

	    // Adjusting extension.
	    extension::adjust_fill(input, length / 2, 0);

	    accu::count_value<bool> accu(true);
	    mln_ch_value(I,unsigned)
	      tmp = accu::transform_line(accu, input, length, dir);

	    mln_concrete(I) output;
	    initialize(output, input);

	    mln_piter(I) p(input.domain());
	    mln_qiter(W) q(win, p);
	    bool is_foreground;
	    for_all(p)
	    {

	      // If the foreground part of the pattern has more than 20%
	      // of background pixels, the current pixel is considered as
	      // background pixel.
	      if (length - tmp(p) > unsigned(0.2f * length) + 1)
	      {
		output(p) = false;
		continue;
	      }

	      // If the background parts of the pattern have exactly or
	      // less than 95% of background pixels, the current pixel is
	      // considered as part of the background.
	      is_foreground = true;
	      for_all(q)
		if ((length - tmp(q)) < unsigned(length * 0.95f) + 1)
		{
		  is_foreground = false;
		  break;
		}

	      output(p) = is_foreground;
	    }

	    trace::exiting("scribo::primitive::extract::impl::generic::lines_pattern");
	    return output;
	  }

	} // end of namespace scribo::primitive::extract::impl::generic



	template <typename I, typename W>
	mln_concrete(I)
	lines_pattern_fast(const Image<I>& input_, unsigned length,
			   unsigned dir, const Window<W>& win_)
	{
	  trace::entering("scribo::primitive::extract::impl::lines_pattern_fast");

	  const I& input = exact(input_);
	  const W& win = exact(win_);
	  mlc_is(mln_value(I), bool)::check();
	  mln_precondition(input.is_valid());

	  // Adjusting extension.
	  extension::adjust_fill(input, length / 2, 0);

	  accu::count_value<bool> accu(true);
	  mln_ch_value(I,unsigned)
	    tmp = accu::transform_line(accu, input, length, dir);

	  mln_concrete(I) output;
	  initialize(output, input);

	  mln::util::array<int>
	    q_arr = offsets_wrt(output, win);

	  bool is_foreground;
	  unsigned ncols = geom::ncols(output);
	  unsigned hit_ratio = unsigned(0.2f * length + 1);
	  unsigned miss_ratio = unsigned(0.95f * length + 1);

	  mln_box_runstart_piter(I) p(output.domain());
	  for_all(p)
	  {
	    unsigned pi = output.offset_of_point(p);
	    unsigned *tmp_ptr = &tmp.element(pi);
	    unsigned *end_ptr = tmp_ptr + ncols;

	    mln_value(I) *out_ptr = &output.element(pi);

	    for (; tmp_ptr < end_ptr; ++out_ptr, ++tmp_ptr)
	    {

	      // If the foreground part of the pattern has more than 20%
	      // of background pixels, the current pixel is considered as
	      // background pixel.
	      if (length - *tmp_ptr > hit_ratio)
	      {
		*out_ptr = false;
		continue;
	      }

	      // If the background parts of the pattern have exactly
	      // or less than 95% of background pixels each, the
	      // current pixel is considered as part of the
	      // background.
	      is_foreground = true;
	      for (unsigned i = 0; i < q_arr.size(); ++i)
		if ((length - *(tmp_ptr + q_arr[i])) < miss_ratio)
		{
		  is_foreground = false;
		  break;
		}

	      *out_ptr = is_foreground;
	    }
	  }

	  trace::exiting("scribo::primitive::extract::impl::lines_pattern_fast");
	  return output;
	}

      } // end of namespace scribo::primitive::extract::impl



      // Dispatch

      namespace internal
      {

	template <typename I, typename W>
	mln_concrete(I)
	lines_pattern_dispatch(mln::trait::image::value_storage::any,
			       mln::trait::image::value_access::any,
			       mln::trait::image::ext_domain::any,
			       const Image<I>& input, unsigned length,
			       unsigned dir, const Window<W>& win)
	{
	  return impl::generic::lines_pattern(input, length, dir, win);
	}


	template <typename I, typename W>
	mln_concrete(I)
	lines_pattern_dispatch(mln::trait::image::value_storage::one_block,
			       mln::trait::image::value_access::direct,
			       mln::trait::image::ext_domain::some,
			       const Image<I>& input, unsigned length,
			       unsigned dir, const Window<W>& win)
	{
	  return impl::lines_pattern_fast(input, length, dir, win);
	}


	template <typename I, typename W>
	mln_concrete(I)
	lines_pattern_dispatch(const Image<I>& input, unsigned length,
			       unsigned dir, const Window<W>& win)
	{
	  return lines_pattern_dispatch(mln_trait_image_value_storage(I)(),
					mln_trait_image_value_access(I)(),
					mln_trait_image_ext_domain(I)(),
					input,
					length,
					dir, win);
	}

      } // end of namespace scribo::primitive::extract::internal


      // Facade

      template <typename I, typename W>
      mln_concrete(I)
      lines_pattern(const Image<I>& input, unsigned length,
		    unsigned dir, const Window<W>& win)
      {
	trace::entering("scribo::primitive::extract::lines_pattern");

	mlc_is(mln_value(I), bool)::check();
	mln_precondition(exact(input).is_valid());
	mln_precondition(exact(win).is_valid());
	mln_precondition(length != 0);
	mln_precondition(dir == 0 || dir == 1);

	mln_concrete(I)
	  output = internal::lines_pattern_dispatch(input, length, dir, win);

	trace::exiting("scribo::primitive::extract::lines_pattern");
	return output;
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::primitive::extract

  } // end of namespace scribo::primitive

} // end of namespace scribo


#endif // ! SCRIBO_PRIMITIVE_EXTRACT_LINES_PATTERN_HH
