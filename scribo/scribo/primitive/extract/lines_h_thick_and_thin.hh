// Copyright (C) 2011, 2012, 2013 EPITA Research and Development Laboratory
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

#ifndef SCRIBO_PRIMITIVE_EXTRACT_LINES_H_THICK_AND_THIN_HH
# define SCRIBO_PRIMITIVE_EXTRACT_LINES_H_THICK_AND_THIN_HH

/// \file
///
/// \brief Find thick and thin lines.

# include <mln/io/pbm/load.hh>
# include <mln/io/pbm/save.hh>
# include <mln/io/ppm/save.hh>

# include <mln/logical/and.hh>
# include <mln/logical/or.hh>

# include <mln/make/w_window2d.hh>

# include <mln/extension/adjust_fill.hh>
# include <mln/accu/transform_line.hh>
# include <mln/accu/count_value.hh>
# include <mln/data/fill.hh>
# include <mln/literal/grays.hh>
# include <mln/literal/colors.hh>

# include <mln/pw/all.hh>
# include <mln/core/routine/duplicate.hh>
# include <mln/win/rectangle2d.hh>
# include <mln/win/hline2d.hh>
# include <mln/morpho/dilation.hh>

# include <mln/data/convert.hh>
# include <mln/data/fill.hh>
# include <mln/core/image/dmorph/image_if.hh>

# include <mln/core/alias/neighb2d.hh>
# include <mln/labeling/compute.hh>
# include <mln/labeling/foreground.hh>

# include <scribo/core/macros.hh>
# include <scribo/core/def/lbl_type.hh>
# include <scribo/primitive/internal/rd.hh>

# include <scribo/debug/logger.hh>


namespace scribo
{

  namespace primitive
  {

    namespace extract
    {
      using namespace mln;

      /*! \brief Extract both thick and thin horizontal lines.
       * \ingroup extractprimitiveseps
       */
      template <typename I>
      mln_concrete(I)
      lines_h_thick_and_thin(const Image<I>& binary_image,
			     unsigned length, unsigned delta,
			     float p_few = 0.2,     // very tolerant  (v. severe is 0.05)
			     float p_enough = 0.6, // very tolerant  (v. severe is 0.80)
			     float ratio = 8);


# ifndef MLN_INCLUDE_ONLY

      namespace internal
      {

	const unsigned
	tag_bg = 0,     // literal::black    background

	  tag_thin = 1, // literal::white    thin

	  tag_top = 2, // literal::red
	  tag_mdl = 3, // literal::green     thick
	  tag_bot = 4, // literal::blue

	  tag_spc = 5; // literal::dark_gray "white space"



	image2d<value::rgb8>
	colorize(const image2d<value::int_u8>& input)
	{
	  image2d<value::rgb8> output(input.domain());
	  mln_piter_(box2d) p(input.domain());
	  for_all(p)
	    switch (input(p))
	    {
	      // bg:
	      case tag_bg:
		output(p) = literal::black;
		break;

		// thin:
	      case tag_thin:
		output(p) = literal::white;
		break;

		// thick:
	      case tag_top:
		output(p) = literal::red;
		break;
	      case tag_mdl:
		output(p) = literal::green;
		break;
	      case tag_bot:
		output(p) = literal::blue;
		break;

		// space:
	      case tag_spc:
		output(p) = literal::dark_gray;
		break;
	    }
	  return output;
	}


	image2d<value::int_u8>
	tag_it(const image2d<bool>& input, unsigned length, unsigned delta,
	       float p_few = 0.2,     // very tolerant  (v. severe is 0.05)
	       float p_enough = 0.6)  // very tolerant  (v. severe is 0.80)
	{
	  unsigned
	    few   = unsigned(p_few * length + 0.49999),  // usage: <= few
	    enough = unsigned(p_enough * length + 0.49999), // usage: >= enough
	    a_lot  = length - few;                     // usage: >= a_lot


	  extension::adjust_fill(input, length / 2, 0);
	  accu::count_value<bool> accu(true);
	  image2d<unsigned> count = accu::transform_line(accu, input, length, 1);
	  border::fill(count, 0); // FIXME: correct?

	  image2d<value::int_u8> output;
	  initialize(output, count);
	  data::fill(output, 0);

	  const unsigned nrows = count.nrows(), ncols = count.ncols();

	  dpoint2d up(-delta, 0), down(+delta, 0);
	  const int
	    offset_up = count.delta_offset(up),
	    offset_down = count.delta_offset(down);

	  typedef const unsigned* ptr_t;
	  value::int_u8* p_out;

	  for (unsigned row = 0; row < nrows; ++row)
	  {
	    ptr_t
	      p_    = & count.at_(row, -1),
	      p_top = p_ + offset_up,
	      p_bot = p_ + offset_down;
	    p_out = & output.at_(row, -1);
	    for (unsigned col = 0; col < ncols; ++col)
	    {
	      ++p_;
	      ++p_top;
	      ++p_bot;
	      ++p_out;

	      if (*p_top <= few && *p_bot <= few)
	      {
		if (*p_ <= few)
		  *p_out = tag_spc;
		else if (*p_ >= enough)
		  *p_out = tag_thin;
		// no other case with both n_top and n_bot <= few
		continue;
	      }

	      if (*p_ < a_lot)
		continue;

	      if (*p_top <= few && *p_bot >= a_lot)
	      {
		*p_out = tag_top;
		continue;
	      }

	      if (*p_top >= a_lot && *p_bot <= few)
	      {
		*p_out = tag_bot;
		continue;
	      }

	      *p_out = tag_mdl;

	    }
	  }

	  return output;
	}


	image2d<bool>
	image_with_tag(const image2d<value::int_u8>& input, value::int_u8 tag)
	{
	  image2d<bool> output(input.domain());
	  mln_pixter_(image2d<bool>) p_o(output);
	  mln_pixter_(const image2d<value::int_u8>) p_i(input);
	  for_all_2(p_i, p_o)
	    p_o.val() = p_i.val() == tag;
	  return output;
	}




	void flush_tag(const image2d<value::int_u8>& input, unsigned col, value::int_u8 tag,
		       // out:
		       int& row, value::int_u8& next_tag)
	{
	  int row_offset = input.delta_offset(dpoint2d(+1, 0));
	  const value::int_u8* p = & input.at_(row, col);
	  while (*p == tag)
	  {
	    p += row_offset;
	    ++row;
	  }
	  next_tag = *p;
	}


	void draw_vertical(image2d<bool>& output, unsigned col, int row_start, int row_end)
	{
	  const unsigned offset = output.delta_offset(dpoint2d(+1, 0)); // next row
	  bool* p_out = & output.at_(row_start, col);
	  for (int row = row_start; row < row_end; ++row, p_out += offset)
	    *p_out = true;
	}


	image2d<bool>
	detect_thick(const image2d<value::int_u8>& input)
	{
	  image2d<bool> output;
	  initialize(output, input);
	  data::fill(output, false);

	  const mln::def::coord
	    maxrow = geom::max_row(input),
	    maxcol = geom::max_col(input);
	  mln::def::coord row, col;

	  col = geom::min_col(input);
	  do
	  {
	    row = geom::min_row(input) - 1;
	    do
	    {
	      ++row;
	      const value::int_u8& t = input.at_(row, col);
	      if (t != tag_top)
		continue;
	      assert(t == tag_top);
	      int r = row;
	      value::int_u8 next_tag;
	      flush_tag(input, col, tag_top, r, next_tag); // top
	      if (next_tag == tag_mdl || next_tag == tag_thin)
	      {
		flush_tag(input, col, next_tag, r, next_tag); // mdl or thin
		if (next_tag != tag_bot)
		  continue;
		flush_tag(input, col, tag_bot, r, next_tag); // bot
		draw_vertical(output, col, row, r);
		row = r - 1;
	      }
	      else if (next_tag == tag_bot)
	      {
		flush_tag(input, col, tag_bot, r, next_tag); // bot
		draw_vertical(output, col, row, r);
		row = r - 1;
	      }
	    }
	    while (row <= maxrow);
	  }
	  while (++col <= maxcol);

	  return output;
	}


	void
	add_thin(const image2d<value::int_u8>& input, image2d<bool>& output)
	{
	  unsigned N = input.nelements();
	  if (output.nelements() != N)
	    std::abort();
	  const value::int_u8* p_in = input.buffer();
	  bool* p_out = output.buffer();
	  for (unsigned i = 0; i < N; ++i)
	  {
	    if (*p_in == tag_thin)
	      *p_out = true;
	    ++p_in;
	    ++p_out;
	  }
	}


	template <typename I, typename J,
		  typename N, typename W, typename D>
	mln_concrete(I)
	rd3_fast(const Image<I>& f_,
		 const Image<J>& g_,
		 const Neighborhood<N>& nbh_,
		 const Weighted_Window<W>& w_win_,
		 D max)
	{
	  const I& f = exact(f_);
	  const J& g = exact(g_);
	  const N& nbh   = exact(nbh_);
	  const W& w_win = exact(w_win_);

	  mln_precondition(f.is_valid());
	  mln_precondition(w_win.is_valid());

	  // Handling w_win.
	  extension::adjust(f, w_win);
	  border::resize(g, f.border());
	  const unsigned n_ws = w_win.size();
	  util::array<int> dp = offsets_wrt(f, w_win.win());
	  mln_invariant(dp.nelements() == n_ws);

	  // Output.
	  mln_concrete(I) o = duplicate(f);
	  if (o.border() != f.border())
	    std::abort();

	  // Distance map.
	  mln_ch_value(I, D) dmap;
	  initialize(dmap, f);
	  data::fill(dmap, max);

	  // Mod determination.
	  unsigned mod;
	  {
	    accu::stat::max<unsigned> m;
	    for (unsigned i = 0; i < w_win.size(); ++i)
	      m.take(w_win.w(i));
	    mod = unsigned(m) + 1;
	  }

	  // Aux data.
	  typedef std::vector<unsigned> bucket_t;
	  std::vector<bucket_t> bucket(mod);
	  unsigned bucket_size = 0;

	  // Initialization.
	  {
	    // For the extension to be ignored:
	    extension::fill(f, true);
	    extension::fill(dmap, D(0));

	    mln_pixter(const I)    p(f);
	    mln_nixter(const I, N) n(p, nbh);
	    for_all(p)
	      if (p.val() == true)
	      {
		dmap.element(p.offset()) = 0;
		for_all(n)
		  if (n.val() == false)
		  {
		    bucket[0].push_back(p.offset());
		    ++bucket_size;
		    break;
		  }
	      }
	  } // end of Initialization.

	  // Propagation.
	  {
	    unsigned p;

	    for (unsigned d = 0; bucket_size != 0; ++d)
	    {
	      bucket_t& bucket_d = bucket[d % mod];
	      for (unsigned i = 0; i < bucket_d.size(); ++i)
	      {
		p = bucket_d[i];

		if (dmap.element(p) == max)
		{
		  // Saturation so stop.
		  bucket_size = bucket_d.size(); // So at end bucket_size == 0.
		  break;
		}

		if (dmap.element(p) < d)
		  // p has already been processed, having a distance less than d.
		  continue;

		for (unsigned i = 0; i < n_ws; ++i)
		{
		  unsigned q = p + dp[i];
		  if (dmap.element(q) > d)
		  {
		    unsigned d_ = d + w_win.w(i);
		    if (d_ < dmap.element(q) && g.element(q) == true)
		    {
		      o.element(q) = true;
		      dmap.element(q) = d_;
		      bucket[d_ % mod].push_back(q);
		      ++bucket_size;
		    }
		  }
		}
	      }
	      bucket_size -= bucket_d.size();
	      bucket_d.clear();
	    }
	  } // end of Propagation.

	  return o;
	}


	template <typename I, typename J>
	inline
	mln_concrete(I)
	rd3_fast(const I& f, const J& g, unsigned length, unsigned delta)
	{
	  unsigned mean = (length + delta) / 2;
	  unsigned ws[] = {  mean, length,  mean,
			     delta,   0,   delta,
			     mean,  length, mean };
	  w_window<dpoint2d,unsigned> w_win = mln::make::w_window2d(ws);
	  unsigned max = length * delta + 1;

	  mln_concrete(I) output = rd3_fast(f, g, c4(), w_win, max);

	  return output;
	}


      } // scribo::primitive::extract::internal


      template <typename I>
      mln_concrete(I)
      lines_h_thick_and_thin(const Image<I>& binary_image_,
			     unsigned length, unsigned delta,
			     float p_few, float p_enough,
			     float ratio)
      {
	mln_trace("scribo::primitive::extract::lines_h_thick_and_thin");

	mlc_is(mln_value(I), bool)::check();

	const I& binary_image = exact(binary_image_);
	mln_precondition(binary_image.is_valid());

	if (length % 2 == 0)
	  ++length;

	// Find lines
	mln_ch_value(I,value::int_u8)
	  tags = internal::tag_it(binary_image, length, delta, p_few, p_enough);
	mln_concrete(I) mask = internal::detect_thick(tags);
	internal::add_thin(tags, mask);


	debug::logger().log_image(debug::AuxiliaryResults,
				  mask, "lines_h_thick_and_thin_mask");

	image2d<bool> output = internal::rd3_fast(mask, binary_image,
						  2 * length, 2 * delta);

	debug::logger().log_image(debug::AuxiliaryResults,
				  output, "lines_h_thick_and_thin_output_before_filter");

	// Remove invalid lines
	typedef scribo::def::lbl_type V;
	V nlabels;
	mln_ch_value(I,V) lbl = labeling::foreground(output, c8(), nlabels);
	mln::util::array<box2d>
	  bbox = labeling::compute(accu::shape::bbox<point2d>(), lbl, nlabels);

	image2d<value::int_u8> debug;
	initialize(debug, binary_image);
	data::fill(debug, 0);
	for_all_ncomponents(e, nlabels)
	{
	  if (bbox(e).width() < length ||
	      (std::max(bbox(e).width(), bbox(e).height()) /
	       std::min(bbox(e).width(), bbox(e).height()) + 0.49999) < ratio)
	    data::fill(((output | bbox(e)).rw()
			| (pw::value(lbl) == pw::cst(e))).rw(), false);
	}

	debug::logger().log_image(debug::Results,
				  output, "lines_h_thick_and_thin_output");


	return output;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::primitive::extract

  } // end of namespace scribo::primitive

} // end of namespace scribo

#endif // ! SCRIBO_PRIMITIVE_EXTRACT_LINES_H_THICK_AND_THIN_HH
