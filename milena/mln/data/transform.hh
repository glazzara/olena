// Copyright (C) 2007, 2008, 2009, 2011 EPITA Research and Development
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

#ifndef MLN_DATA_TRANSFORM_HH
# define MLN_DATA_TRANSFORM_HH

/// \file
///
/// Transform the contents of an image into another one.
///
/// \todo Clean this file + overload with pixel iterators.
///
/// \todo Re-activate tests and make them static.

# include <mln/core/concept/image.hh>
# include <mln/core/concept/function.hh>
# include <mln/value/set.hh>

// Specializations are in:
# include <mln/data/transform.spe.hh>


namespace mln
{

  namespace data
  {

    /*! \brief Transform the image \p input through a function \p f.
     *
     * \param[in] input The input image.
     * \param[in] f The function.
     *
     * This routine runs: \n
     * for all p of \p input, \p output(p) = \p f( \p input(p) ).
     *
     * \ingroup mlndata
     */
    template <typename I, typename F>
    mln_ch_value(I, mln_result(F))
    transform(const Image<I>& input, const Function_v2v<F>& f);


    /*! \brief Transform two images \p input1 \p input2 through a function \p f.
     *
     * \param[in] input1 The 1st input image.
     * \param[in] input2 The 2nd input image.
     * \param[in] f The function.
     *
     * This routine runs: \n
     * for all p of \p input, \p output(p) = \p f( \p input1(p), \p input2(p) ).
     *
     * \ingroup mlndata
     */
    template <typename I1, typename I2, typename F>
    mln_ch_value(I1, mln_result(F))
    transform(const Image<I1>& input1,
	      const Image<I2>& input2,
	      const Function_vv2v<F>& f);



# ifndef MLN_INCLUDE_ONLY

    namespace internal
    {

      template <typename I, typename F>
      void transform_tests(const Image<I>& input,
                           const Function_v2v<F>& f)
      {
        // Dynamic test.
	mln_precondition(exact(input).is_valid());

	// Avoid a warning about an undefined variable when NDEBUG
	// is not defined.
	(void) input;
        (void) f;
      }

      template <typename I1, typename I2, typename F>
      void transform_tests(const Image<I1>& input1, const Image<I2>& input2,
                           const Function_vv2v<F>& f)
      {
        // Dynamic tests.
	mln_precondition(exact(input1).is_valid());
	mln_precondition(exact(input2).is_valid());
	mln_precondition(exact(input2).domain() == exact(input1).domain());

	// Avoid a warning about an undefined variable when NDEBUG
	// is not defined.
	(void) input1;
	(void) input2;
        (void) f;
      }

    } // end of namespace mln::data::internal



    namespace impl
    {


      // Generic implementations.


      namespace generic
      {

	/// Generic implementation of data::transform.
	///
        /// \param[in] input_ The input image.
	/// \param[in] f_     The function.
	//
	template <typename I, typename F>
        mln_ch_value(I, mln_result(F))
        transform(const Image<I>& input_, const Function_v2v<F>& f_)
	{
          mln_trace("data::impl::generic::transform");

	  const I& input  = exact(input_);
	  const F& f      = exact(f_);

	  data::internal::transform_tests(input, f);

          mln_ch_value(I, mln_result(F)) output;
          initialize(output, input);

	  mln_piter(I) p(input.domain());
	  for_all(p)
	    output(p) = f(input(p));

          return output;
	}


	/// Generic implementation of data::transform.
	///
	/// \param[in] input1_ The 1st input image.
	/// \param[in] input2_ The 2nd input image.
	/// \param[in] f_      The function.
	//
	template <typename I1, typename I2, typename F>
        mln_ch_value(I1, mln_result(F))
	transform(const Image<I1>& input1_,
		  const Image<I2>& input2_,
		  const Function_vv2v<F>& f_)
	{
          mln_trace("data::impl::generic::transform");

	  const I1& input1  = exact(input1_);
	  const I2& input2  = exact(input2_);
	  const  F& f       = exact(f_);

	  data::internal::transform_tests(input1, input2, f);

          mln_ch_value(I1, mln_result(F)) output;
          initialize(output, input1);

	  mln_piter(I1) p(input1.domain());
	  for_all(p)
	    output(p) = f(input1(p), input2(p));

          return output;
	}

      } // end of namespace mln::data::impl::generic


    } // end of namespace mln::data::impl



    // Facades.


    template <typename I, typename F>
    inline
    mln_ch_value(I, mln_result(F))
    transform(const Image<I>& input, const Function_v2v<F>& f)
    {
      mln_trace("data::transform");

      internal::transform_tests(input, f);

      mln_ch_value(I, mln_result(F)) output;
      output = internal::transform_dispatch(input, f);

      return output;
    }


    template <typename I1, typename I2, typename F>
    inline
    mln_ch_value(I1, mln_result(F))
    transform(const Image<I1>& input1, const Image<I2>& input2,
	      const Function_vv2v<F>& f)
    {
      mln_trace("data::transform");

      internal::transform_tests(input1, input2, f);

      mln_ch_value(I1, mln_result(F)) output;
      output = internal::transform_dispatch(input1, input2, f);

      return output;
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::data

} // end of namespace mln


#endif // ! MLN_DATA_TRANSFORM_HH
