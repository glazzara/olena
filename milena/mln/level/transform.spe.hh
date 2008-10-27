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
	template <typename I, typename F, typename O>
	void transform(const Image<I>& input_,
                       const Function_v2v<F>& f_,
                       Image<O>& output_);
      }



      template <typename I, typename F, typename O>
      void transform_lowq(const Image<I>& input_,
                          const Function_v2v<F>& f_,
                          Image<O>& output_)
      {
	trace::entering("level::impl::transform_lowq");

	const I& input  = exact(input_);
	const F& f      = exact(f_);
        O& output       = exact(output_);

        level::internal::transform_tests(input, f, output);
        mlc_is(mln_trait_image_pw_io(O),
               trait::image::pw_io::read_write)::check();

        value::lut_vec<mln_vset(I), mln_result(F)>
          lut(input.values_eligible(), f);

	mln_piter(I) p(input.domain());
	for_all(p)
	  output(p) = lut(input(p));

	trace::exiting("level::impl::transform_lowq");
      }

      template <typename I, typename F, typename O>
      void transform_taken(const Image<I>& input_,
                           const Function_v2v<F>& f_,
                           Image<O>& output_)
      {
        trace::entering("level::impl::transform_taken");

	const I& input  = exact(input_);
	const F& f      = exact(f_);
        O& output       = exact(output_);

        level::internal::transform_tests(input, f, output);
        mlc_is(mln_trait_image_pw_io(O),
               trait::image::pw_io::read_write)::check();

        value::lut_vec<mln_vset(I), mln_result(F)>
          lut(input.taken_values(), f);

	mln_piter(I) p(input.domain());
	for_all(p)
	  output(p) = lut(input(p));

	trace::exiting("level::impl::transform_taken");
      }


      template <typename I, typename F, typename O>
      void transform_singleton(const Image<I>& input_,
                               const Function_v2v<F>& f_,
                               Image<O>& output_)
      {
        trace::entering("level::impl::transform_singleton");

	const I& input  = exact(input_);
	const F& f      = exact(f_);

        level::internal::transform_tests(input_, f_, output_);
        mln_precondition((mlc_is(mln_trait_image_pw_io(O),
                                 trait::image::pw_io::read_write)::value ||
                          mlc_is(mln_trait_image_vw_io(O),
                                 trait::image::vw_io::read_write)::value));

        mln_result(F) val = f(input.val());
        fill_with_value(output_, val);

	trace::exiting("level::impl::transform_singleton");
      }

      template <typename I, typename F, typename O>
      void transform_fast(const Image<I>& input_,
                          const Function_v2v<F>& f_,
                          Image<O>& output_)
      {
        trace::entering("level::impl::transform_fast");

        const I& input = exact(input_);
        const F& f     = exact(f_);
        O& output      = exact(output_);

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
      }



      template <typename I, typename F, typename O>
      void transform_fast_lowq(const Image<I>& input_,
                               const Function_v2v<F>& f_,
                               Image<O>& output_)
      {
        trace::entering("level::impl::transform_fast_lowq");

        const I& input = exact(input_);
	const F& f     = exact(f_);
        O& output      = exact(output_);

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
      }



      // Dispatch
      // --------
      namespace internal
      {

        /// Deal with image not updated
        template <typename I, typename F, typename O>
        inline
        void transform_dispatch_(mln::trait::undef,
                                 mln::trait::image::quant::any,
                                 const Image<I>& input,
                                 const Function_v2v<F>& f,
                                 Image<O>& output)
        {
          impl::generic::transform(input, f, output);
        }

        template <typename I, typename F, typename O>
        inline
        void transform_dispatch_(mln::trait::image::vw_set::any,
                                 mln::trait::image::quant::any,
                                 const Image<I>& input,
                                 const Function_v2v<F>& f,
                                 Image<O>& output)
        {
          impl::generic::transform(input, f, output);
        }

        template <typename I, typename F, typename O>
        inline
        void transform_dispatch_(mln::trait::image::vw_set::uni,
                                 mln::trait::image::quant::any,
                                 const Image<I>& input,
                                 const Function_v2v<F>& f,
                                 Image<O>& output)
        {
          impl::transform_taken(input, f, output);
        }


        template <typename I, typename F, typename O>
        inline
        void transform_dispatch_(mln::trait::image::vw_set::none,
                                 mln::trait::image::quant::low,
                                 const Image<I>& input,
                                 const Function_v2v<F>& f,
                                 Image<O>& output)
        {
          impl::transform_lowq(input, f, output);
        }

        template <typename I, typename F, typename O>
        inline
        void transform_dispatch_(mln::trait::image::quant::any,
                                 mln::trait::image::value_access::direct,
                                 mln::trait::image::value_access::direct,
                                 mln::trait::image::ext_domain::some,
                                 mln::trait::image::ext_domain::some,
                                 const Image<I>& input_,
                                 const Function_v2v<F>& f,
                                 Image<O>& output_)
        {
          const I& input  = exact(input_);
          O& output       = exact(output_);


          if (input.domain() == output.domain())
            transform_fast(input, f, output);
          else
            transform_dispatch_(mln_trait_image_vw_set(I)(),
                                mln_trait_image_quant(I)(),
                                input, f, output);
        }

        template <typename I, typename F, typename O>
        inline
        void transform_dispatch_(mln::trait::image::quant::any,
                                 mln::trait::image::value_access::direct,
                                 mln::trait::image::value_access::direct,
                                 mln::trait::image::ext_domain::none,
                                 mln::trait::image::ext_domain::none,
                                 const Image<I>& input,
                                 const Function_v2v<F>& f,
                                 Image<O>& output)
        {
          transform_fast(input, f, output);
        }

        template <typename I, typename F, typename O>
        inline
        void transform_dispatch_(mln::trait::image::quant::low,
                                 mln::trait::image::value_access::direct,
                                 mln::trait::image::value_access::direct,
                                 mln::trait::image::ext_domain::some,
                                 mln::trait::image::ext_domain::some,
                                 const Image<I>& input_,
                                 const Function_v2v<F>& f,
                                 Image<O>& output_)
        {
          const I& input  = exact(input_);
          O& output       = exact(output_);

          if (input.domain() == output.domain())
            transform_fast_lowq(input, f, output);
          else
            transform_dispatch_(mln_trait_image_vw_set(I)(),
                                mln_trait_image_quant(I)(),
                                input, f, output);
        }

        template <typename I, typename F, typename O>
        inline
        void transform_dispatch_(mln::trait::image::quant::low,
                                 mln::trait::image::value_access::direct,
                                 mln::trait::image::value_access::direct,
                                 mln::trait::image::ext_domain::none,
                                 mln::trait::image::ext_domain::none,
                                 const Image<I>& input,
                                 const Function_v2v<F>& f,
                                 Image<O>& output)
        {
          transform_fast_lowq(input, f, output);
        }


        template <typename I, typename F, typename O>
        inline
        void transform_dispatch_(mln::trait::image::quant::any,
                                 mln::trait::image::value_access::any,
                                 mln::trait::image::value_access::any,
                                 mln::trait::image::ext_domain::any,
                                 mln::trait::image::ext_domain::any,
                                 const Image<I>& input,
                                 const Function_v2v<F>& f,
                                 Image<O>& output)
        {
          transform_dispatch_(mln_trait_image_vw_set(I)(),
                              mln_trait_image_quant(I)(),
                              input, f, output);
        }


        template <typename I, typename F, typename O>
        inline
        void transform_dispatch_(mln::trait::image::value_storage::any,
                                 mln::trait::image::value_storage::any,
                                 const Image<I>& input,
                                 const Function_v2v<F>& f,
                                 Image<O>& output)
        {
          transform_dispatch_(mln_trait_image_vw_set(I)(),
                              mln_trait_image_quant(I)(),
                              input, f, output);
        }

        template <typename I, typename F, typename O>
        inline
        void transform_dispatch_(mln::trait::image::value_storage::singleton,
                                 mln::trait::image::value_storage::any,
                                 const Image<I>& input,
                                 const Function_v2v<F>& f,
                                 Image<O>& output)
        {
          transform_singleton(input, f, output);
        }

        template <typename I, typename F, typename O>
        inline
        void transform_dispatch_(mln::trait::image::value_storage::one_block,
                                 mln::trait::image::value_storage::one_block,
                                 const Image<I>& input_,
                                 const Function_v2v<F>& f_,
                                 Image<O>& output_)
        {
          const I& input  = exact(input_);
          O& output       = exact(output_);


          /// Check basic properties
          if (mlc_is(mln_trait_image_value_alignement(I),
                     trait::image::value_alignement::with_grid)::value &&
              mlc_is(mln_trait_image_value_alignement(O),
                     trait::image::value_alignement::with_grid)::value)
            {
              transform_dispatch_(mln_trait_image_quant(I)(),
                                  mln_trait_image_value_access(I)(),
                                  mln_trait_image_value_access(O)(),
                                  mln_trait_image_ext_domain(I)(),
                                  mln_trait_image_ext_domain(O)(),
                                  input, f_, output);
            }
          else
            transform_dispatch_(mln_trait_image_vw_set(I)(),
                                mln_trait_image_quant(I)(),
                                input, f_, output);
        }



        template <typename I, typename F, typename O>
        inline
        void transform_dispatch_(const Image<I>& input,
                                 const Function_v2v<F>& f,
                                 Image<O>& output)
        {
          transform_dispatch_(mln_trait_image_value_storage(I)(),
                              mln_trait_image_value_storage(O)(),
                              input, f, output);
        }

      }



    } // end of namespace mln::level::impl

  } // end of namespace mln::level

} // end of namespace mln

# endif // ! MLN_INCLUDE_ONLY


#endif // ! MLN_LEVEL_TRANSFORM_SPE_HH
