// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_LEVEL_TRANSFORM_HH
# define MLN_LEVEL_TRANSFORM_HH

/*! \file mln/level/transform.hh
 *
 * \brief Transform the contents of an image into another one.
 *
 * \todo Clean this file + overload with pixel iterators.
 *
 * \todo Re-activate tests and make them static.
 */

# include <mln/core/concept/image.hh>
# include <mln/core/concept/function.hh>
# include <mln/value/set.hh>

// Specializations are in:
# include <mln/level/transform.spe.hh>


namespace mln
{

  namespace level
  {

    /*! Transform the image \p input through a function \p f.
     *
     * \param[in] input The input image.
     * \param[in] f The function.
     *
     * This routine runs: \n
     * for all p of \p input, \p output(p) = \p f( \p input(p) ).
     *
     */
    template <typename I, typename F>
    mln_ch_value(I, mln_result(F))
    transform(const Image<I>& input, const Function_v2v<F>& f);


    /*! Transform two images \p input1 \p input2 through a function \p f.
     *
     * \param[in] input1 The 1st input image.
     * \param[in] input2 The 2nd input image.
     * \param[in] f The function.
     *
     * This routine runs: \n
     * for all p of \p input, \p output(p) = \p f( \p input1(p), \p input2(p) ).
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
      inline
      void transform_tests(const Image<I>& input,
                           const Function_v2v<F>& f)
      {
        // Dynamic test.
	mln_precondition(exact(input).has_data());

	// Avoid a warning about an undefined variable when NDEBUG
	// is not defined.
	(void) input;
        (void) f;
      }

      template <typename I1, typename I2, typename F>
      inline
      void transform_tests(const Image<I1>& input1,
			   const Image<I2>& input2,
                           const Function_vv2v<F>& f)
      {
        // Dynamic tests.
	mln_precondition(exact(input1).has_data());
	mln_precondition(exact(input2).has_data());
	mln_precondition(exact(input2).domain() == exact(input1).domain());

	// Avoid a warning about an undefined variable when NDEBUG
	// is not defined.
	(void) input1;
	(void) input2;
        (void) f;
      }

    } // end of namespace mln::level::internal



    namespace impl
    {


      // Generic implementations.


      namespace generic
      {

	template <typename I, typename F>
	inline
        mln_ch_value(I, mln_result(F))
        transform(const Image<I>& input_, const Function_v2v<F>& f_)
	{
          trace::entering("level::impl::generic::transform");

	  const I& input  = exact(input_);
	  const F& f      = exact(f_);

	  level::internal::transform_tests(input, f);

          mln_ch_value(I, mln_result(F)) output;
          initialize(output, input);

          mlc_is(mln_trait_image_pw_io(mln_ch_value(I, mln_result(F))),
                 trait::image::pw_io::read_write)::check();

	  mln_piter(I) p(input.domain());
	  for_all(p)
	    output(p) = f(input(p));

	  trace::exiting("level::impl::generic::transform");
          return output;
	}


	template <typename I1, typename I2, typename F>
	inline
        mln_ch_value(I1, mln_result(F))
	  transform(const Image<I1>& input1_,
		    const Image<I2>& input2_,
		    const Function_vv2v<F>& f_)
	{
          trace::entering("level::impl::generic::transform");

	  const I1& input1  = exact(input1_);
	  const I2& input2  = exact(input2_);
	  const  F& f       = exact(f_);

	  level::internal::transform_tests(input1, input2, f);

          mln_ch_value(I1, mln_result(F)) output;
          initialize(output, input1);

	  mln_piter(I1) p(input1.domain());
	  for_all(p)
	    output(p) = f(input1(p), input2(p));

	  trace::exiting("level::impl::generic::transform");
          return output;
	}

      } // end of namespace mln::level::impl::generic


    } // end of namespace mln::level::impl


    // Facades.

    template <typename I, typename F>
    inline
    mln_ch_value(I, mln_result(F))
    transform(const Image<I>& input, const Function_v2v<F>& f)
    {
      trace::entering("level::transform");

      internal::transform_tests(input, f);

      mln_ch_value(I, mln_result(F)) output;
      output = impl::internal::transform_dispatch(exact(input), exact(f));

      trace::exiting("level::transform");
      return output;
    }

    template <typename I1, typename I2, typename F>
    inline
    mln_ch_value(I1, mln_result(F))
    transform(const Image<I1>& input1,
	      const Image<I2>& input2,
	      const Function_vv2v<F>& f)
    {
      trace::entering("level::transform");

      internal::transform_tests(input1, input2, f);

      mln_ch_value(I1, mln_result(F)) output;
      output = impl::generic::transform(input1, input2, f);

      trace::exiting("level::transform");
      return output;
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::level

} // end of namespace mln


#endif // ! MLN_LEVEL_TRANSFORM_HH
