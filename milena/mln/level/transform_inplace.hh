// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_LEVEL_TRANSFORM_INPLACE_HH
# define MLN_LEVEL_TRANSFORM_INPLACE_HH

/// \file mln/level/transform_inplace.hh
///
/// Transform inplace the contents of an image through a function.
///
/// \todo Take into account more properties; see level/transform.hh.

# include <mln/core/concept/image.hh>
# include <mln/core/concept/function.hh>
# include <mln/value/set.hh>
# include <mln/value/lut_vec.hh>
# include <mln/opt/value.hh>


namespace mln
{

  namespace level
  {

    /*! Transform inplace the image \p ima through a function \p f.
     *
     * \param[in,out] ima The image to be transformed.
     * \param[in] f The function.
     *
     * This routine runs: \n
     * for all p of \p ima, \p ima(p) = \p f( \p ima(p) ).
     *
     */
    template <typename I, typename F>
    void
    transform_inplace(Image<I>& ima, const Function_v2v<F>& f);


    /*! Transform inplace the image \p ima with the image \p aux
        through a function \p f.
     *
     * \param[in] ima1 The image to be transformed.
     * \param[in] aux The auxiliary image.
     * \param[in] f The function.
     *
     * This routine runs: \n
     * for all p of \p ima, \p ima(p) = \p f( \p ima(p), \p aux(p) ).
     */
    template <typename I1, typename I2, typename F>
    void
    transform_inplace(Image<I1>& ima, const Image<I2>& aux,
		      const Function_vv2v<F>& f);



# ifndef MLN_INCLUDE_ONLY


    // Tests.

    namespace internal
    {

      template <typename I, typename F>
      inline
      void transform_inplace_tests(const Image<I>& ima,
				   const Function_v2v<F>& f)
      {
        // Properties checks.
        mlc_or(mlc_is(mln_trait_image_pw_io(I),
		      trait::image::pw_io::read_write),
	       mlc_is(mln_trait_image_vw_io(I),
		      trait::image::vw_io::read_write))::check();
	mlc_converts_to(mln_result(F), mln_value(I))::check();

        // Dynamic test.
	mln_precondition(exact(ima).is_valid());

	// Avoid a warning.
	(void) ima;
        (void) f;
      }

      template <typename I1, typename I2, typename F>
      inline
      void transform_tests(const Image<I1>& ima,
			   const Image<I2>& aux,
                           const Function_v2v<F>& f)
      {
        // Properties checks.
        mlc_or(mlc_is(mln_trait_image_pw_io(I1),
		      trait::image::pw_io::read_write),
	       mlc_is(mln_trait_image_vw_io(I1),
		      trait::image::vw_io::read_write))::check();
	mlc_converts_to(mln_result(F), mln_value(I1))::check();

        // Dynamic test.
	mln_precondition(exact(ima).is_valid());
	mln_precondition(exact(aux).is_valid());
	mln_precondition(exact(aux).domain() == exact(ima).domain());

	// Avoid warnings.
	(void) ima;
	(void) aux;
        (void) f;
      }

    } // end of namespace mln::level::internal


    namespace impl
    {

      // Generic implementations.

      namespace generic
      {

	template <typename I, typename F>
        void
        transform_inplace(Image<I>& ima_, const Function_v2v<F>& f_)
	{
          trace::entering("level::impl::generic::transform_inplace");

          mlc_is(mln_trait_image_pw_io(I),
                 trait::image::pw_io::read_write)::check();

	  I& ima = exact(ima_);
	  const F& f = exact(f_);

          level::internal::transform_inplace_tests(ima, f);

	  mln_piter(I) p(ima.domain());
	  for_all(p)
	    ima(p) = f(ima(p));

	  trace::exiting("level::impl::generic::transform_inplace");
	}

	template <typename I1, typename I2, typename F>
        void
	transform_inplace(Image<I1>& ima_, const Image<I2>& aux_,
			  const Function_vv2v<F>& f_)
	{
          trace::entering("level::impl::generic::transform_inplace");

          mlc_is(mln_trait_image_pw_io(I1),
                 trait::image::pw_io::read_write)::check();

	  I1&       ima = exact(ima_);
	  const I2& aux = exact(aux_);
	  const F&  f   = exact(f_);

	  level::internal::transform_inplace_tests(ima, aux, f);

	  mln_piter(I1) p(ima.domain());
	  for_all(p)
	    ima(p) = f(ima(p), aux(p));

	  trace::exiting("level::impl::generic::transform_inplace");
	}

      } // end of namespace mln::level::impl::generic

      /// Specialized implementation

      template <typename I, typename F>
      void
      transform_inplace_lowq(Image<I>& input_,
                             const Function_v2v<F>& f_)
      {
	trace::entering("level::impl::transform_inplace_lowq");

        mlc_is(mln_trait_image_pw_io(I),
               trait::image::pw_io::read_write)::check();

	I& input  = exact(input_);
	const F& f      = exact(f_);

        level::internal::transform_inplace_tests(input, f);

        value::lut_vec<mln_vset(I), mln_result(F)>
          lut(input.values_eligible(), f);

	mln_piter(I) p(input.domain());
	for_all(p)
	  input(p) = lut(input(p));

	trace::exiting("level::impl::transform_inplace_lowq");
      }

      template <typename I, typename F>
      void
      transform_inplace_taken(Image<I>& input_,
                              const Function_v2v<F>& f_)
      {
        trace::entering("level::impl::transform_inplace_taken");

        mlc_is(mln_trait_image_pw_io(I),
               trait::image::pw_io::read_write)::check();

	I& input  = exact(input_);
	const F& f      = exact(f_);

        level::internal::transform_inplace_tests(input, f);

        value::lut_vec<mln_vset(I), mln_result(F)>
          lut(input.taken_values(), f);

	mln_piter(I) p(input.domain());
	for_all(p)
	  input(p) = lut(input(p));

	trace::exiting("level::impl::transform_inplace_taken");
      }


      template <typename I, typename F>
      void
      transform_inplace_singleton(Image<I>& input_,
                                  const Function_v2v<F>& f_)
      {
        trace::entering("level::impl::transform_inplace_singleton");

	I& input  = exact(input_);
	const F& f      = exact(f_);

        level::internal::transform_inplace_tests(input, f);

        opt::value(input) = f(opt::value(input));

	trace::exiting("level::impl::transform_inplace_singleton");
      }

      template <typename I, typename F>
      void
      transform_inplace_fastest(Image<I>& ima_, const Function_v2v<F>& f_)
      {
	trace::entering("level::impl::transform_inplace_fastest");

	I& ima = exact(ima_);
	const F& f = exact(f_);

	level::internal::transform_inplace_tests(ima, f);

	mln_pixter(I) p(ima);
	for_all(p)
	  p.val() = f(p.val());

	trace::exiting("level::impl::transform_inplace_fastest");
      }


      template <typename I, typename F>
      void
      transform_inplace_fastest_lowq(Image<I>& input_,
                                     const Function_v2v<F>& f_)
      {
        trace::entering("level::impl::transform_inplace_fastest_lowq");

        I& input = exact(input_);
	const F& f     = exact(f_);

        level::internal::transform_inplace_tests(input, f);

        value::lut_vec<mln_vset(I), mln_result(F)>
          lut(input.values_eligible(), f);

        mln_pixter(I) pi(input);
        for_all(pi)
          pi.val() = lut(pi.val());

	trace::exiting("level::impl::transform_inplace_fastest_lowq");
      }


      template <typename I1, typename I2, typename F>
      void
      transform_inplace_fastest(Image<I1>& ima_, const Image<I2>& aux_,
				const Function_vv2v<F>& f_)
      {
	trace::entering("level::impl::transform_inplace_fastest");

	mlc_is(mln_trait_image_pw_io(I1),
	       trait::image::pw_io::read_write)::check();

	I1&       ima = exact(ima_);
	const I2& aux = exact(aux_);
	const F&  f   = exact(f_);

	level::internal::transform_inplace_tests(ima, aux, f);

	mln_pixter(I1) pi(ima);
	mln_pixter(const I2) pa(aux);
	for_all_2(pi, pa)
	  pi.val() = f(pi.val(), pa.val());

	trace::exiting("level::impl::transform_inplace_fastest");
      }


    } // end of namespace mln::level::impl



    // Dispatch.

    namespace internal
    {

      // (ima, f) version.

      /// Dispatch on quantization
      template <typename I, typename F>
      void
      transform_inplace_dispatch(trait::image::vw_set::any,
                                 trait::image::quant::any,
                                 Image<I>& ima, const Function_v2v<F>& f)
      {
        level::impl::generic::transform_inplace(ima, f);
      }

      template <typename I, typename F>
      void
      transform_inplace_dispatch(trait::image::vw_set::uni,
                                 trait::image::quant::any,
                                 Image<I>& ima, const Function_v2v<F>& f)
      {
        level::impl::transform_inplace_taken(ima, f);
      }

      template <typename I, typename F>
      void
      transform_inplace_dispatch(trait::image::vw_set::any,
                                 trait::image::quant::low,
                                 Image<I>& ima, const Function_v2v<F>& f)
      {
        level::impl::transform_inplace_lowq(ima, f);
      }



      /// Dispatch for fast image
      template <typename I, typename F>
      void
      transform_inplace_dispatch_fast(trait::image::quant::any,
                                      Image<I>& ima, const Function_v2v<F>& f)
      {
        level::impl::transform_inplace_fastest(ima, f);
      }

      template <typename I, typename F>
      void
      transform_inplace_dispatch_fast(trait::image::quant::low,
                                      Image<I>& ima, const Function_v2v<F>& f)
      {
        level::impl::transform_inplace_fastest_lowq(ima, f);
      }




      /// Dispatch on value storage
      template <typename I, typename F>
      void
      transform_inplace_dispatch(trait::image::value_storage::any,
                                 trait::image::value_access::any,
                                 Image<I>& ima, const Function_v2v<F>& f)
      {
        transform_inplace_dispatch(mln_trait_image_vw_set(I)(),
                                   mln_trait_image_quant(I)(),
                                   ima, f);
      }

      template <typename I, typename F>
      void
      transform_inplace_dispatch(trait::image::value_storage::singleton,
                                 trait::image::value_access::any,
                                 Image<I>& ima, const Function_v2v<F>& f)
      {
        level::impl::transform_inplace_singleton(ima, f);
      }


      template <typename I, typename F>
      void
      transform_inplace_dispatch(trait::image::value_storage::one_block,
                                 trait::image::value_access::direct,
                                 Image<I>& ima, const Function_v2v<F>& f)
      {
        transform_inplace_dispatch_fast(mln_trait_image_quant(I)(),
                                        ima, f);
      }




      /// First dispatch
      template <typename I, typename F>
      void
      transform_inplace_dispatch(Image<I>& ima, const Function_v2v<F>& f)
      {
	transform_inplace_dispatch(mln_trait_image_value_storage(I)(),
                                   mln_trait_image_value_access(I)(),
				   ima, f);
      }



      // (ima, aux, f) version.

      template <typename I1, typename I2, typename F>
      void
      transform_inplace_dispatch(trait::image::value_alignement::any,
                                 trait::image::value_alignement::any,
                                 trait::image::speed::any,
				 trait::image::speed::any,
				 Image<I1>& ima, const Image<I2>& aux,
                                 const Function_vv2v<F>& f)
      {
	level::impl::generic::transform_inplace(ima, aux, f);
      }

      template <typename I1, typename I2, typename F>
      void
      transform_inplace_dispatch(trait::image::value_alignement::with_grid,
                                 trait::image::value_alignement::with_grid,
                                 trait::image::speed::fastest,
				 trait::image::speed::fastest,
				 Image<I1>& ima, const Image<I2>& aux,
                                 const Function_vv2v<F>& f)
      {
	level::impl::transform_inplace_fastest(ima, aux, f);
      }

      template <typename I1, typename I2, typename F>
      void
      transform_inplace_dispatch(Image<I1>& ima, const Image<I2>& aux,
                                 const Function_vv2v<F>& f)
      {
	transform_inplace_dispatch(mln_trait_image_value_alignement(I1)(),
                                   mln_trait_image_value_alignement(I2)(),
                                   mln_trait_image_speed(I1)(),
				   mln_trait_image_speed(I2)(),
				   ima, aux, f);
      }

    } // end of namespace mln::level::internal



    // Facades.

    template <typename I, typename F>
    void
    transform_inplace(Image<I>& ima, const Function_v2v<F>& f)
    {
      trace::entering("level::transform_inplace");

      internal::transform_inplace_tests(ima, f);
      internal::transform_inplace_dispatch(ima, f);

      trace::exiting("level::transform_inplace");
    }

    template <typename I1, typename I2, typename F>
    void
    transform_inplace(Image<I1>& ima, const Image<I2>& aux,
		      const Function_vv2v<F>& f)
    {
      trace::entering("level::transform_inplace");

      internal::transform_inplace_tests(ima, aux, f);
      internal::transform_inplace_dispatch(ima, aux, f);

      trace::exiting("level::transform_inplace");
    }


# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::level

} // end of namespace mln


#endif // ! MLN_LEVEL_TRANSFORM_INPLACE_HH
