// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_LEVEL_UPDATE_HH
# define MLN_LEVEL_UPDATE_HH

/*! \file mln/level/update.hh
 *
 * \brief Update an accumulator with image pixel values.
 */

# include <mln/core/concept/accumulator.hh>
# include <mln/core/concept/image.hh>



namespace mln
{

  namespace level
  {

    /*! Update an accumulator with the pixel values of the image \p input.
     *
     * \param[in] a The accumulator.
     * \param[in] input The input image.
     * \return The accumulator result.
     */
    template <typename A, typename I>
    mln_result(A)
    update(Accumulator<A>& a, const Image<I>& input);



# ifndef MLN_INCLUDE_ONLY


    // Tests.

    namespace internal
    {

      template <typename A, typename I>
      inline
      void
      update_tests(Accumulator<A>& a_, const Image<I>& input_)
      {
	A& a = exact(a_);
	const I& input = exact(input_);
	mln_precondition(input.is_valid());
	// sizeof(a.take(mln_value(I)()));

	(void) a;
	(void) input;
      }

    } // end of namespace internal



    // Implementations.

    namespace impl
    {

      namespace generic
      {

	/// Generic implementation of level::update.
	template <typename A, typename I>
	inline
	mln_result(A)
	update(Accumulator<A>& a_, const Image<I>& input_)
	{
	  trace::entering("level::impl::generic::update");

	  A& a = exact(a_);
	  const I& input = exact(input_);
	  level::internal::update_tests(a, input);

	  mln_piter(I) p(input.domain());
	  for_all(p)
	    a.take(input(p));

	  trace::exiting("level::impl::generic::update");
	  return a.to_result();
	}

      } // end of namespace mln::level::impl::generic


      /// Fastest implementation of level::update.
      template <typename A, typename I>
      inline
      mln_result(A)
      update_fastest(Accumulator<A>& a_, const Image<I>& input_)
      {
	trace::entering("level::impl::update_fastest");

	A& a = exact(a_);
	const I& input = exact(input_);
	level::internal::update_tests(a, input);

	mln_pixter(const I) pxl(input);
	for_all(pxl)
	  a.take(pxl.val());

	trace::exiting("level::impl::update_fastest");
	return a.to_result();
      }


    } // end of namespace mln::level::impl



    // Dispatch.

    namespace internal
    {

      template <typename A, typename I>
      inline
      mln_result(A)
      update_dispatch(trait::image::speed::any,
		      Accumulator<A>& a, const Image<I>& input)
      {
	return impl::generic::update(a, input);
      }

      template <typename A, typename I>
      inline
      mln_result(A)
      update_dispatch(trait::image::speed::fastest,
		      Accumulator<A>& a, const Image<I>& input)
      {
	return impl::update_fastest(a, input);
      }

      template <typename A, typename I>
      inline
      mln_result(A)
      update_dispatch(Accumulator<A>& a, const Image<I>& input)
      {
	return update_dispatch(mln_trait_image_speed(I)(),
			       a, input);
      }

    } // end of namespace internal



    // Facades.

    template <typename A, typename I>
    inline
    mln_result(A)
    update(Accumulator<A>& a, const Image<I>& input)
    {
      trace::entering("level::update");

      level::internal::update_tests(a, input);
      mln_result(A) r = internal::update_dispatch(a, input);

      trace::exiting("level::update");
      return r;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::level

} // end of namespace mln


#endif // ! MLN_LEVEL_UPDATE_HH
