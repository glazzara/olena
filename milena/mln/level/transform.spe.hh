// Copyright (C) 2007 EPITA Research and Development Laboratory
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

#ifndef MLN_LEVEL_TRANSFORM_SPE_HH
# define MLN_LEVEL_TRANSFORM_SPE_HH

/*! \file mln/level/transform.spe.hh
 *
 * \brief Specializations for mln::level::transform.
 *
 */

# ifndef MLN_LEVEL_TRANSFORM_HH
#  error "Forbidden inclusion of *.spe.hh"
# endif // ! MLN_LEVEL_TRANSFORM_HH

# include <mln/core/concept/image.hh>
# include <mln/core/concept/function.hh>

# include <mln/level/fill_with_value.hh>

# include <mln/value/set.hh>
# include <mln/value/lut_vec.hh>



# ifndef MLN_INCLUDE_ONLY

namespace mln
{

  namespace level
  {

    namespace internal
    {
      template <typename I, typename F, typename O>
      inline
      void transform_tests(const Image<I>& input,
                           const Function_v2v<F>& f,
                           Image<O>& output);
    }


    // Implementation
    // --------------
    namespace impl
    {


      namespace generic
      {
	template <typename I, typename F>
        mln_ch_value(I, mln_result(F))
        transform(const Image<I>& input_,
                  const Function_v2v<F>& f_);
      }


      template <typename I, typename F>
      mln_ch_value(I, mln_result(F))
      transform_lowq(const Image<I>& input_,
                     const Function_v2v<F>& f_)
      {
	trace::entering("level::impl::transform_lowq");

	const I& input  = exact(input_);
	const F& f      = exact(f_);

        mln_precondition(exact(input).has_data());
        mln_ch_value(I, mln_result(F)) output;
        initialize(output, input);

        level::internal::transform_tests(input, f, output);
        mlc_is(mln_trait_image_pw_io(mln_ch_value(I, mln_result(F))),
               trait::image::pw_io::read_write)::check();

        value::lut_vec<mln_vset(I), mln_result(F)>
          lut(input.values_eligible(), f);

	mln_piter(I) p(input.domain());
	for_all(p)
	  output(p) = lut(input(p));

	trace::exiting("level::impl::transform_lowq");

        return output;
      }

      template <typename I, typename F>
      mln_ch_value(I, mln_result(F))
      transform_taken(const Image<I>& input_,
                      const Function_v2v<F>& f_)
      {
        trace::entering("level::impl::transform_taken");

	const I& input  = exact(input_);
	const F& f      = exact(f_);

        mln_precondition(exact(input).has_data());
        mln_ch_value(I, mln_result(F)) output;
        initialize(output, input);

        level::internal::transform_tests(input, f, output);
        mlc_is(mln_trait_image_pw_io(mln_ch_value(I, mln_result(F))),
               trait::image::pw_io::read_write)::check();

        value::lut_vec<mln_vset(I), mln_result(F)>
          lut(input.taken_values(), f);

	mln_piter(I) p(input.domain());
	for_all(p)
	  output(p) = lut(input(p));

	trace::exiting("level::impl::transform_taken");

        return output;
      }


      template <typename I, typename F>
      mln_ch_value(I, mln_result(F))
      transform_singleton(const Image<I>& input_,
                          const Function_v2v<F>& f_)
      {
        trace::entering("level::impl::transform_singleton");

	const I& input  = exact(input_);
	const F& f      = exact(f_);

        mln_precondition(exact(input).has_data());
        mln_ch_value(I, mln_result(F)) output;
        initialize(output, input);

        level::internal::transform_tests(input_, f_, output);

        mln_result(F) val = f(input.val());
        fill_with_value(output, val);

	trace::exiting("level::impl::transform_singleton");

        return output;
      }

      template <typename I, typename F>
      mln_ch_value(I, mln_result(F))
      transform_fast(const Image<I>& input_,
                     const Function_v2v<F>& f_)
      {
        trace::entering("level::impl::transform_fast");

        typedef mln_ch_value(I, mln_result(F)) O;

        const I& input = exact(input_);
        const F& f     = exact(f_);

        mln_precondition(exact(input).has_data());
        mln_ch_value(I, mln_result(F)) output;
        initialize(output, input);


        level::internal::transform_tests(input, f, output);

        mln_pixter(const I) pi(input);
        mln_pixter(O) po(output);
        po.start();
        for_all(pi)
        {
          po.val() = f(pi.val());
          po.next();
        }

	trace::exiting("level::impl::transform_fast");

        return output;
      }



      template <typename I, typename F>
      mln_ch_value(I, mln_result(F))
      transform_fast_lowq(const Image<I>& input_,
                          const Function_v2v<F>& f_)
      {
        trace::entering("level::impl::transform_fast_lowq");
        typedef mln_ch_value(I, mln_result(F)) O;

        const I& input = exact(input_);
	const F& f     = exact(f_);

        mln_precondition(exact(input).has_data());
        mln_ch_value(I, mln_result(F)) output;
        initialize(output, input);

        level::internal::transform_tests(input, f, output);

        value::lut_vec<mln_vset(I), mln_result(F)>
          lut(input.values_eligible(), f);

        mln_pixter(const I) pi(input);
        mln_pixter(O) po(output);

        po.start();
        for_all(pi)
        {
          po.val() = lut(pi.val());
          po.next();
        }

	trace::exiting("level::impl::transform_fast_lowq");

        return output;
      }



      // Dispatch
      // --------
      namespace internal
      {

        /// Deal with image not updated
        template <typename I, typename F>
        inline
        mln_ch_value(I, mln_result(F))
        transform_dispatch_(mln::trait::undef,
                            mln::trait::image::quant::any,
                            const Image<I>& input,
                            const Function_v2v<F>& f)
        {
          return impl::generic::transform(input, f);
        }

        template <typename I, typename F>
        inline
        mln_ch_value(I, mln_result(F))
        transform_dispatch_(mln::trait::image::vw_set::any,
                            mln::trait::image::quant::any,
                            const Image<I>& input,
                            const Function_v2v<F>& f)
        {
          return impl::generic::transform(input, f);
        }

        template <typename I, typename F>
        inline
        mln_ch_value(I, mln_result(F))
        transform_dispatch_(mln::trait::image::vw_set::uni,
                            mln::trait::image::quant::any,
                            const Image<I>& input,
                            const Function_v2v<F>& f)
        {
          return impl::transform_taken(input, f);
        }


        template <typename I, typename F>
        inline
        mln_ch_value(I, mln_result(F))
        transform_dispatch_(mln::trait::image::vw_set::any,
                            mln::trait::image::quant::low,
                            const Image<I>& input,
                            const Function_v2v<F>& f)
        {
          return impl::transform_lowq(input, f);
        }


        /// FIXME check that is right
        template <typename I, typename F>
        inline
        mln_ch_value(I, mln_result(F))
        transform_dispatch_(mln::trait::image::quant::any,
                            mln::trait::image::value_access::direct,
                            const Image<I>& input,
                            const Function_v2v<F>& f)
        {
          return transform_fast(input, f);
        }

        /// FIXME check that is right
        template <typename I, typename F>
        inline
        mln_ch_value(I, mln_result(F))
        transform_dispatch_(mln::trait::image::quant::low,
                            mln::trait::image::value_access::direct,
                            const Image<I>& input,
                            const Function_v2v<F>& f)
        {
          return transform_fast_lowq(input, f);
        }



        template <typename I, typename F>
        inline
        mln_ch_value(I, mln_result(F))
        transform_dispatch_(mln::trait::image::quant::any,
                            mln::trait::image::value_access::any,
                            const Image<I>& input,
                            const Function_v2v<F>& f)
        {
          return transform_dispatch_(mln_trait_image_vw_set(I)(),
                                     mln_trait_image_quant(I)(),
                                     input, f);
        }


        template <typename I, typename F>
        inline
        mln_ch_value(I, mln_result(F))
        transform_dispatch_(mln::trait::image::value_storage::any,
                            const Image<I>& input,
                            const Function_v2v<F>& f)
        {
          return transform_dispatch_(mln_trait_image_vw_set(I)(),
                                     mln_trait_image_quant(I)(),
                                     input, f);
        }

        template <typename I, typename F>
        inline
        mln_ch_value(I, mln_result(F))
        transform_dispatch_(mln::trait::image::value_storage::singleton,
                            const Image<I>& input,
                            const Function_v2v<F>& f)
        {
          return transform_singleton(input, f);
        }

        template <typename I, typename F>
        inline
        mln_ch_value(I, mln_result(F))
        transform_dispatch_(mln::trait::image::value_storage::one_block,
                            const Image<I>& input_,
                            const Function_v2v<F>& f_)
        {
          const I& input  = exact(input_);

          /// Check alignement property
          if (mlc_is(mln_trait_image_value_alignement(I),
                     trait::image::value_alignement::with_grid)::value)
            return transform_dispatch_(mln_trait_image_quant(I)(),
                                       mln_trait_image_value_access(I)(),
                                       input, f_);
          else
            return transform_dispatch_(mln_trait_image_vw_set(I)(),
                                       mln_trait_image_quant(I)(),
                                       input, f_);
        }



        template <typename I, typename F>
        mln_ch_value(I, mln_result(F))
        transform_dispatch_(const Image<I>& input,
                            const Function_v2v<F>& f)
        {
          return transform_dispatch_(mln_trait_image_value_storage(I)(),
                                     input, f);
        }

      }



    } // end of namespace mln::level::impl

  } // end of namespace mln::level

} // end of namespace mln

# endif // ! MLN_INCLUDE_ONLY


#endif // ! MLN_LEVEL_TRANSFORM_SPE_HH
