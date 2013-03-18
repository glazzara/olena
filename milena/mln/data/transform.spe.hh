// Copyright (C) 2007, 2008, 2009 EPITA Research and Development
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

#ifndef MLN_DATA_TRANSFORM_SPE_HH
# define MLN_DATA_TRANSFORM_SPE_HH

/// \file
///
/// \brief Specializations for mln::data::transform.
///
/// \todo The implementation (and thus the dispatch) involving a lut
/// has been de-activated; the patch of Z does not work anymore since
/// we do not make the difference between i2v and v2v functions.
/// Actually the patch was no acceptable solution to the lut problem
/// (for some values, we cannot compute f(v) to put into the lut...)

# ifndef MLN_DATA_TRANSFORM_HH
#  error "Forbidden inclusion of *.spe.hh"
# endif // ! MLN_DATA_TRANSFORM_HH

# include <mln/core/concept/image.hh>
# include <mln/core/concept/function.hh>

# include <mln/data/fill_with_value.hh>

# include <mln/value/set.hh>
# include <mln/value/lut_vec.hh>
# include <mln/opt/value.hh>



# ifndef MLN_INCLUDE_ONLY

namespace mln
{

  namespace data
  {

    // Forward declarations.

    namespace internal
    {

      template <typename I, typename F>
      void transform_tests(const Image<I>& input,
                           const Function_v2v<F>& f);

      template <typename I1, typename I2, typename F>
      void transform_tests(const Image<I1>& input1, const Image<I2>& input2,
                           const Function_vv2v<F>& f);

    }


    // Implementations.


    namespace impl
    {


      namespace generic
      {
	// Forward declaration.

	template <typename I, typename F>
        mln_ch_value(I, mln_result(F))
	  transform(const Image<I>& input_, const Function_v2v<F>& f_);

        template <typename I1, typename I2, typename F>
        mln_ch_value(I1, mln_result(F))
          transform(const Image<I1>& input1,
                    const Image<I2>& input2,
                    const Function_vv2v<F>& f);

      }


//       template <typename I, typename F>
//       mln_ch_value(I, mln_result(F))
// 	transform_lowq_v2v(const Image<I>& input_, const Function_v2v<F>& f_)
//       {
// 	mln_trace("data::impl::transform_lowq_v2v");

//         mlc_is(mln_trait_image_pw_io(mln_ch_value(I, mln_result(F))),
//                trait::image::pw_io::read_write)::check();

// 	const I& input  = exact(input_);
// 	const F& f      = exact(f_);
//         data::internal::transform_tests(input, f);

//         mln_ch_value(I, mln_result(F)) output;
//         initialize(output, input);

//         value::lut_vec<mln_vset(I), mln_result(F)>
//           lut(input.values_eligible(), f);

// 	mln_piter(I) p(input.domain());
// 	for_all(p)
// 	  output(p) = lut(input(p));

//         return output;
//       }


      template <typename I, typename F>
      mln_ch_value(I, mln_result(F))
	transform_lowq_i2v(const Image<I>& input_, const Function_v2v<F>& f_)
      {
	mln_trace("data::impl::transform_lowq");

        mlc_is(mln_trait_image_pw_io(mln_ch_value(I, mln_result(F))),
               trait::image::pw_io::read_write)::check();

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


//       template <typename I, typename F>
//       mln_ch_value(I, mln_result(F))
// 	transform_taken_v2v(const Image<I>& input_, const Function_v2v<F>& f_)
//       {
//         mln_trace("data::impl::transform_taken_v2v");

//         mlc_is(mln_trait_image_pw_io(mln_ch_value(I, mln_result(F))),
//                trait::image::pw_io::read_write)::check();

// 	const I& input  = exact(input_);
// 	const F& f      = exact(f_);
//         data::internal::transform_tests(input, f);

//         mln_ch_value(I, mln_result(F)) output;
//         initialize(output, input);

//         value::lut_vec<mln_vset(I), mln_result(F)>
//           lut(input.taken_values(), f);

// 	mln_piter(I) p(input.domain());
// 	for_all(p)
// 	  output(p) = lut(input(p));

//         return output;
//       }


      template <typename I, typename F>
      mln_ch_value(I, mln_result(F))
	transform_taken_i2v(const Image<I>& input_, const Function_v2v<F>& f_)
      {
        mln_trace("data::impl::transform_taken_i2v");

        mlc_is(mln_trait_image_pw_io(mln_ch_value(I, mln_result(F))),
               trait::image::pw_io::read_write)::check();

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


      template <typename I, typename F>
      mln_ch_value(I, mln_result(F))
	transform_singleton(const Image<I>& input_, const Function_v2v<F>& f_)
      {
        mln_trace("data::impl::transform_singleton");

	const I& input  = exact(input_);
	const F& f      = exact(f_);
        data::internal::transform_tests(input, f);

        mln_ch_value(I, mln_result(F)) output;
        initialize(output, input);

        mln_result(F) val = f(opt::value(input));
	data::fill_with_value(output, val);

        return output;
      }


      template <typename I, typename F>
      mln_ch_value(I, mln_result(F))
	transform_fast(const Image<I>& input_, const Function_v2v<F>& f_)
      {
        mln_trace("data::impl::transform_fast");

        const I& input = exact(input_);
        const F& f     = exact(f_);
        data::internal::transform_tests(input, f);

        typedef mln_ch_value(I, mln_result(F)) O;
        O output;
        initialize(output, input);

        mln_pixter(const I) pi(input);
        mln_pixter(O) po(output);
	for_all_2(pi, po)
          po.val() = f(pi.val());

        return output;
      }


      template <typename I, typename F>
      mln_ch_value(I, mln_result(F))
	transform_fast_lowq(const Image<I>& input_, const Function_v2v<F>& f_)
      {
        mln_trace("data::impl::transform_fast_lowq");

        const I& input = exact(input_);
	const F& f     = exact(f_);
        data::internal::transform_tests(input, f);

        typedef mln_ch_value(I, mln_result(F)) O;
        O output;
        initialize(output, input);

        value::lut_vec<mln_vset(I), mln_result(F)>
          lut(input.values_eligible(), f);

        mln_pixter(const I) pi(input);
        mln_pixter(O) po(output);
        for_all_2(pi, po)
          po.val() = lut(pi.val());

        return output;
      }


      template <typename I1, typename I2, typename F>
      mln_ch_value(I1, mln_result(F))
	transform_fastest(const Image<I1>& input1_, const Image<I2>& input2_,
			  const Function_vv2v<F>& f_)
      {
        mln_trace("data::impl::transform_fastest");

        const I1& input1 = exact(input1_);
        const I2& input2 = exact(input2_);
        const  F& f      = exact(f_);
        data::internal::transform_tests(input1, input2, f);

        typedef mln_ch_value(I1, mln_result(F)) O;
        O output;
        initialize(output, input1);
        mln_pixter(O) po(output);

        mln_pixter(const I1) pi1(input1);
        mln_pixter(const I2) pi2(input2);
        for_all_3(pi1, pi2, po)
          po.val() = f(pi1.val(), pi2.val());

        return output;
      }



    } // end of namespace mln::data::impl



    // Dispatch.

    namespace internal
    {

      /// Deal with image not updated
      template <typename I, typename F>
      inline
      mln_ch_value(I, mln_result(F))
	transform_dispatch(trait::undef,
			   trait::image::quant::any,
			   const Image<I>& input, const Function_v2v<F>& f)
      {
	return data::impl::generic::transform(input, f);
      }

      template <typename I, typename F>
      inline
      mln_ch_value(I, mln_result(F))
	transform_dispatch(trait::image::vw_set::any,
			   trait::image::quant::any,
			   const Image<I>& input, const Function_v2v<F>& f)
      {
	return data::impl::generic::transform(input, f);
      }

//       template <typename I, typename F>
//       inline
//       mln_ch_value(I, mln_result(F))
// 	transform_dispatch(trait::image::vw_set::uni,
// 			   trait::image::quant::low,
// 			   const Image<I>& input, const Function_v2v<F>& f)
//       {
// 	return data::impl::transform_taken_v2v(input, f);
//       }

      template <typename I, typename F>
      inline
      mln_ch_value(I, mln_result(F))
	transform_dispatch(trait::image::vw_set::uni,
			   trait::image::quant::low,
			   const Image<I>& input, const Function_v2v<F>& f)
      {
	return data::impl::transform_taken_i2v(input, f);
      }

//       template <typename I, typename F>
//       inline
//       mln_ch_value(I, mln_result(F))
// 	transform_dispatch(trait::image::vw_set::any,
// 			   trait::image::quant::low,
// 			   const Image<I>& input, const Function_v2v<F>& f)
//       {
// 	return data::impl::transform_lowq_v2v(input, f);
//       }

      template <typename I, typename F>
      inline
      mln_ch_value(I, mln_result(F))
	transform_dispatch(trait::image::vw_set::any,
			   trait::image::quant::low,
			   const Image<I>& input, const Function_v2v<F>& f)
      {
	return data::impl::transform_lowq_i2v(input, f);
      }

      template <typename I, typename F>
      inline
      mln_ch_value(I, mln_result(F))
	transform_dispatch(trait::image::quant::any,
			   trait::image::value_access::direct,
			   const Image<I>& input, const Function_v2v<F>& f)
      {
	return data::impl::transform_fast(input, f);
      }

      template <typename I, typename F>
      inline
      mln_ch_value(I, mln_result(F))
	transform_dispatch(trait::image::quant::low,
			   trait::image::value_access::direct,
			   const Image<I>& input, const Function_v2v<F>& f)
      {
	return data::impl::transform_fast_lowq(input, f);
      }

//       template <typename I, typename F>
//       inline
//       mln_ch_value(I, mln_result(F))
// 	transform_dispatch(trait::image::quant::low,
// 			   trait::image::value_access::direct,
// 			   const Image<I>& input, const Function_v2v<F>& f)
//       {
// 	return data::impl::transform_fast(input, f);
//       }

      template <typename I, typename F>
      inline
      mln_ch_value(I, mln_result(F))
	transform_dispatch(trait::image::quant::any,
			   trait::image::value_access::any,
			   const Image<I>& input, const Function_v2v<F>& f)
      {
	return transform_dispatch(mln_trait_image_vw_set(I)(),
				  mln_trait_image_quant(I)(),
				  input, exact(f));
      }


      template <typename I, typename F>
      inline
      mln_ch_value(I, mln_result(F))
	transform_dispatch(trait::image::value_storage::any,
			   const Image<I>& input, const Function_v2v<F>& f)
      {
	return transform_dispatch(mln_trait_image_vw_set(I)(),
				  mln_trait_image_quant(I)(),
				  input, exact(f));
      }

      template <typename I, typename F>
      inline
      mln_ch_value(I, mln_result(F))
	transform_dispatch(trait::image::value_storage::singleton,
			   const Image<I>& input, const Function_v2v<F>& f)
      {
	return data::impl::transform_singleton(input, f);
      }

      template <typename I, typename F>
      inline
      mln_ch_value(I, mln_result(F))
	transform_dispatch(trait::image::value_storage::one_block,
			   const Image<I>& input_, const Function_v2v<F>& f_)
      {
	const I& input  = exact(input_);

	/// Check alignment property
	if (mlc_is(mln_trait_image_value_alignment(I),
		   trait::image::value_alignment::with_grid)::value)
	  return transform_dispatch(mln_trait_image_quant(I)(),
				    mln_trait_image_value_access(I)(),
				    input, exact(f_));
	else
	  return transform_dispatch(mln_trait_image_vw_set(I)(),
				    mln_trait_image_quant(I)(),
				    input, exact(f_));
      }



      // Dispatch for transformation from a couple of images.

      template <typename I1, typename I2, typename F>
      mln_ch_value(I1, mln_result(F))
	transform_dispatch_2(trait::image::value_alignment::any,
                             trait::image::value_alignment::any,
                             trait::image::speed::any,
			     trait::image::speed::any,
			     const Image<I1>& input1, const Image<I2>& input2,
			     const Function_vv2v<F>& f)
      {
	return data::impl::generic::transform(input1, input2, f);
      }

      template <typename I1, typename I2, typename F>
      mln_ch_value(I1, mln_result(F))
	transform_dispatch_2(trait::image::value_alignment::with_grid,
                             trait::image::value_alignment::with_grid,
                             trait::image::speed::fastest,
			     trait::image::speed::fastest,
			     const Image<I1>& input1, const Image<I2>& input2,
			     const Function_vv2v<F>& f)
      {
	return data::impl::transform_fastest(input1, input2, f);
      }

      // end of Dispatch for transformation from a couple of images.



      // Dispatch entry points.

      template <typename I, typename F>
      mln_ch_value(I, mln_result(F))
	transform_dispatch(const Image<I>& input, const Function_v2v<F>& f)
      {
	return transform_dispatch(mln_trait_image_value_storage(I)(),
				  input, exact(f));
      }

      template <typename I1, typename I2, typename F>
      mln_ch_value(I1, mln_result(F))
	transform_dispatch(const Image<I1>& input1, const Image<I2>& input2,
			   const Function_vv2v<F>& f)
      {
	return transform_dispatch_2(mln_trait_image_value_alignment(I1)(),
                                    mln_trait_image_value_alignment(I2)(),
                                    mln_trait_image_speed(I1)(),
				    mln_trait_image_speed(I2)(),
				    input1, input2, f);
      }

    } // end of namespace mln::data::internal



  } // end of namespace mln::data

} // end of namespace mln

# endif // ! MLN_INCLUDE_ONLY


#endif // ! MLN_DATA_TRANSFORM_SPE_HH
