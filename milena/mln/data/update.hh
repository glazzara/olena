// Copyright (C) 2007, 2008, 2009, 2012 EPITA Research and Development
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

#ifndef MLN_DATA_UPDATE_HH
# define MLN_DATA_UPDATE_HH

/// \file
///
/// \brief Update an accumulator with image pixel values.

# include <mln/core/concept/accumulator.hh>
# include <mln/core/concept/image.hh>



namespace mln
{

  namespace data
  {

    /*! \brief Update an accumulator with the pixel values of the
     *  image \p input.
     *
     * \param[in] a The accumulator.
     * \param[in] input The input image.
     * \return The accumulator result.
     *
     * \ingroup mlnaccucompute
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

	/// Generic implementation of data::update.
	///
        /// \param[in] a_     The accumulator.
	/// \param[in] input_ The input image.
	///
	/// \return The accumulator result.
	//
	template <typename A, typename I>
	inline
	mln_result(A)
	update(Accumulator<A>& a_, const Image<I>& input_)
	{
	  mln_trace("data::impl::generic::update");

	  A& a = exact(a_);
	  const I& input = exact(input_);
	  data::internal::update_tests(a, input);

	  mln_piter(I) p(input.domain());
	  for_all(p)
	    a.take(input(p));

	  return a.to_result();
	}

      } // end of namespace mln::data::impl::generic


      /// Fastest implementation of data::update.
      ///
      /// \param[in] a_     The accumulator.
      /// \param[in] input_ The input image.
      ///
      /// \return The accumulator result.
      //
      template <typename A, typename I>
      inline
      mln_result(A)
      update_fastest(Accumulator<A>& a_, const Image<I>& input_)
      {
	mln_trace("data::impl::update_fastest");

	A& a = exact(a_);
	const I& input = exact(input_);
	data::internal::update_tests(a, input);

	mln_pixter(const I) pxl(input);
	for_all(pxl)
	  a.take(pxl.val());

	return a.to_result();
      }


    } // end of namespace mln::data::impl



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
      mln_trace("data::update");

      data::internal::update_tests(a, input);
      mln_result(A) r = internal::update_dispatch(a, input);

      return r;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::data

} // end of namespace mln


#endif // ! MLN_DATA_UPDATE_HH
