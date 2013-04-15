// Copyright (C) 2007, 2008, 2009, 2010 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_ARITH_REVERT_HH
# define MLN_ARITH_REVERT_HH

/// \file
///
/// Point-wise revert (min -> max and max -> min) of images.
///
/// \todo Add static assertion and save one iterator in in-place version.

# include <mln/core/concept/image.hh>
# include <mln/trait/value_.hh>

// Specializations are in:
# include <mln/arith/revert.spe.hh>


// FIXME: Rely instead on mln/fun/v2v/revert.hh.
// FIXME: Revert on int value 0 does not give 0 (since min != - max;
//        idem for float etc.)


namespace mln
{

  namespace arith
  {

    /// Point-wise reversion of image \p input.
    /*!
     * \param[in] input the input image.
     * \result The result image.
     *
     * \pre \p input.is_valid
     *
     * It performs: \n
     *   for all p of input.domain \n
     *     output(p) = min + (max - input(p))
     */
    template <typename I>
    mln_concrete(I) revert(const Image<I>& input);


    /// Point-wise in-place reversion of image \p input.
    /*!
     * \param[in,out] input The target image.
     *
     * \pre \p input.is_valid
     *
     * It performs: \n
     *   for all p of input.domain \n
     *     input(p) = min + (max - input(p))
     */
    template <typename I>
    void revert_inplace(Image<I>& input);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      namespace generic
      {

	template <typename I, typename O>
	inline
	void revert(const Image<I>& input_, Image<O>& output_)
	{
	  mln_trace("arith::impl::generic::revert_");

	  const I& input = exact(input_);
	  O& output = exact(output_);

	  mln_precondition(input.is_valid());
	  mln_precondition(output.is_valid());
	  mln_precondition(input.domain() == output.domain());

	  typedef mln_value(I) V;
	  mln_piter(I) p(input.domain());
	  for_all(p)
	    output(p) = mln_min(V) + (mln_max(V) - input(p));

	}

      } // end of namespace mln::arith::impl::generic

    } // end of namespace mln::arith::impl



    // Dispatch.

    namespace internal
    {

      template <typename I, typename O>
      inline
      void
      revert_dispatch(trait::image::speed::any, const I& input, O& output)
      {
	impl::generic::revert(input, output);
      }

      template <typename I, typename O>
      inline
      void
      revert_dispatch(trait::image::speed::fastest, const I& input, O& output)
      {
	impl::revert_fastest(input, output);
      }

      template <typename I, typename O>
      inline
      void
      revert_dispatch(const Image<I>& input, Image<O>& output)
      {
	revert_dispatch(mln_trait_image_speed(I)(),
			exact(input), exact(output));

      }


    } // end of namespace mln::arith::internal



    // Facades.

    template <typename I>
    inline
    mln_concrete(I) revert(const Image<I>& input)
    {
      mln_trace("arith::revert");

      mln_precondition(exact(input).is_valid());

      mln_concrete(I) output;
      initialize(output, input);
      internal::revert_dispatch(exact(input), exact(output));

      return output;
    }

    template <typename I>
    inline
    void revert_inplace(Image<I>& input)
    {
      mln_trace("arith::revert_inplace");

      mln_precondition(exact(input).is_valid());

      internal::revert_dispatch(exact(input), exact(input));

    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::arith

} // end of namespace mln


#endif // ! MLN_ARITH_REVERT_HH
