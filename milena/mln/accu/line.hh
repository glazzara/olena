// Copyright (C) 2007, 2008, 2009, 2011, 2012 EPITA Research and
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

#ifndef MLN_ACCU_LINE_HH
# define MLN_ACCU_LINE_HH

/// \file
///
/// Run an accumulator on a line of images.
///
/// \todo Suppress integer manipulations (redundant with ptr arith).

# include <mln/core/concept/meta_accumulator.hh>
# include <mln/core/concept/image.hh>



namespace mln
{

  namespace accu
  {

    // FIXME:

    /*! Line an accumulator onto the pixel values of the image \p input.
     *
     * \param[in] input The input image.
     * \param[in] p_start The starting site of the line.
     * \param[in] len The line length.
     * \param[in] half_length The half length of the line.
     * \param[in,out] output The resulting image.
     *
     * This routine runs: \n
     *   tmp = \p a \n
     *   tmp.init() \n
     *   accu::take(\p input, tmp) \n
     *   return tmp.to_result() \n
     */
    template <typename Meta_Accu, unsigned Dir, // Free parameters.
	      typename I, typename O>
    void
    line(const Image<I>& input,
	 const mln_site(I)& p_start, def::coord len,
	 def::coord half_length,
	 Image<O>& output);



# ifndef MLN_INCLUDE_ONLY

    namespace internal
    {

      template <typename Meta_Accu, unsigned Dir, // Free parameters.
		typename I, typename O>
      void
      line_tests(const Image<I>& input_,
		 const mln_site(I)&, unsigned,
		 unsigned,
		 Image<O>& output_)
      {
	mlc_is_a(Meta_Accu, Meta_Accumulator)::check();

	typedef mln_accu_with(Meta_Accu, mln_value(I)) A;
	mlc_converts_to(mln_result(A), mln_value(O))::check();

	typedef mln_site(I) P;
	mlc_bool(Dir < P::dim)::check();

	mlc_is(mln_trait_image_value_io(O),
	       mln::trait::image::value_io::read_write)::check();

	const I& input = exact(input_);
	O& output = exact(output_);
	mln_precondition(input.is_valid());
	mln_precondition(output.is_valid());
      }

    } // end of namespace mln::accu::internal


    namespace impl
    {

      namespace generic
      {

	template <typename Meta_Accu, unsigned Dir,
		  typename I, typename O>
	void
	line(const Image<I>& input_,
	     const mln_site(I)& p_start, unsigned len_,
	     unsigned half_length_,
	     Image<O>& output_)
	{
	  typedef mln_site(I) P;
	  typedef mln_accu_with(Meta_Accu, mln_value(I)) A;

	  const I& input = exact(input_);
	  O& output = exact(output_);

	  // Avoid warning in signed/unsigned comparison.
	  const def::coord len = static_cast<def::coord>(len_);
	  const def::coord half_length = static_cast<def::coord>(half_length_);

	  // Checks and tests.
	  internal::line_tests<Meta_Accu, Dir>(input,
					       p_start, len,
					       half_length,
					       output);

	  // Initialization.
	  const def::coord start = p_start[Dir];
	  const def::coord last = static_cast<def::coord>(start + len - 1);
	  A a; // Accumulator.
	  a.init();
	  P p; // Current point.

	  // Start (first line point).
	  p = p_start;
	  P q = p;
	  for (def::coord c = 0; c <= half_length && c < len; ++c)
	    {
	      a.take(input(q));
	      ++q[Dir];
	    }
	  output(p) = a;

	  def::coord& cur = p[Dir];
	  if (cur == last)
	    return;

	  // Begin of line.

	  P p_plus = p_start;
	  def::coord& plus = p_plus[Dir];
	  plus = static_cast<def::coord>(plus + half_length);

	  while (cur < start + half_length && cur < last)
	    {
	      ++cur;
	      ++plus;
	      if (plus <= last)
		a.take(input(p_plus));
	      output(p) = a;
	    }

	  if (cur == last)
	    return;

	  // Middle of line.

	  P p_minus = p_start;
	  def::coord& minus = p_minus[Dir];
	  --minus;

	  while (cur < last - half_length)
	    {
	      ++cur;
	      ++plus;
	      ++minus;
	      mln_invariant(plus >= start && plus <= last);
	      a.take(input(p_plus));
	      mln_invariant(minus >= start && minus <= last);
	      a.untake(input(p_minus));
	      output(p) = a;
	    }

	  if (cur == last)
	    return;

	  // End of line.

	  while (cur < last)
	    {
	      ++cur;
	      ++minus;
	      mln_invariant(minus >= start && minus <= last);
	      a.untake(input(p_minus));
	      output(p) = a;
	    }
	}

      } // end of namespace mln::accu::impl::generic



      template <typename Meta_Accu, unsigned Dir,
		typename I, typename O>
      void
      line_fastest(const Image<I>& input_,
		   const mln_site(I)& p_start, def::coord len,
		   def::coord half_length,
		   Image<O>& output_)
      {
	typedef mln_site(I)  P;
	typedef mln_value(I) V;
	typedef mln_accu_with(Meta_Accu, V) A;

	const I& input = exact(input_);
	O& output = exact(output_);

	// Checks and tests.
	internal::line_tests<Meta_Accu, Dir>(input,
					     p_start, len,
					     half_length,
					     output);

	// Initialization.

	const def::coord start = p_start[Dir];
	const def::coord last = start + len - 1;

	const V* p_in    = & input(p_start);
	V* p_out   = & output(p_start);

	mln_delta(P) dp = literal::zero;
	dp[Dir] = 1;
	unsigned offset = input.delta_offset(dp);

	A a; // Accumulator.
	a.init();

	// Start (first line point).

	const V* q = p_in;
	for (def::coord c = 0; c <= half_length && c < len; ++c)
	  {
	    a.take(*q);
	    q += offset;
	  }
	*p_out = a;

	def::coord cur = p_start[Dir];
	if (cur == last)
	  return;

	// Begin of line.

	def::coord plus = start + half_length;
	const V* p_plus = p_in + half_length * offset;

	while (cur < start + half_length && cur < last)
	  {
	    ++cur;
	    p_in += offset;
	    ++plus;
	    p_plus += offset;
	    if (plus <= last)
	      a.take(*p_plus);
	    p_out += offset;
	    *p_out = a;
	  }

	if (cur == last)
	  return;

	// Middle of line.

	def::coord minus = start - 1;
	const V* p_minus = p_in - offset;

	while (cur < last - half_length)
	  {
	    ++cur;
	    p_in += offset;
	    ++plus;
	    p_plus += offset;
	    ++minus;
	    p_minus += offset;
	    mln_invariant(plus >= start && plus <= last);
	    a.take(*p_plus);
	    mln_invariant(minus >= start && minus <= last);
	    a.untake(*p_minus);
	    p_out += offset;
	    *p_out = a;
	  }

	if (cur == last)
	  return;

	// End of line.

	while (cur < last)
	  {
	    ++cur;
	    p_in += offset;
	    ++minus;
	    p_minus += offset;
	    mln_invariant(minus >= start && minus <= last);
	    a.untake(*p_minus);
	    p_out += offset;
	    *p_out = a;
	  }

      }




    } // end of namespace mln::accu::impl


    namespace internal
    {

      template <typename Meta_Accu, unsigned Dir, // Free parameters.
		typename I, typename O>
      void
      line_dispatch(trait::image::speed::any,
		    const Image<I>& input,
		    const mln_site(I)& p_start, unsigned len,
		    unsigned half_length,
		    Image<O>& output)
      {
	impl::generic::line<Meta_Accu, Dir>(input,
					    p_start, len,
					    half_length,
					    output);
      }

//       template <typename Meta_Accu, unsigned Dir, // Free parameters.
// 		typename I, typename O>
//       void
//       line_dispatch(trait::image::speed::fastest,
// 		    const Image<I>& input,
// 		    const mln_site(I)& p_start, unsigned len,
// 		    unsigned half_length,
// 		    Image<O>& output)
//       {
// 	impl::line_fastest<Meta_Accu, Dir>(input,
// 					   p_start, len,
// 					   half_length,
// 					   output);
//       }

      template <typename Meta_Accu, unsigned Dir, // Free parameters.
		typename I, typename O>
      void
      line_dispatch(const Image<I>& input,
		    const mln_site(I)& p_start, unsigned len,
		    unsigned half_length,
		    Image<O>& output)
      {
	line_dispatch<Meta_Accu, Dir>(mln_trait_image_speed(I)(),
				      input,
				      p_start, len,
				      half_length,
				      output);
      }

    } // end of namespace mln::accu::internal


    template <typename Meta_Accu, unsigned Dir, // Free parameters.
	      typename I, typename O>
    void
    line(const Image<I>& input,
	 const mln_site(I)& p_start, unsigned len,
	 unsigned half_length,
	 Image<O>& output)
    {
      internal::line_tests<Meta_Accu, Dir>(input,
					   p_start, len,
					   half_length,
					   output);
      internal::line_dispatch<Meta_Accu, Dir>(input,
					      p_start, len,
					      half_length,
					      output);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_LINE_HH
