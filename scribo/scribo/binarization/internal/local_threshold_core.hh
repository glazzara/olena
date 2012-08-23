// Copyright (C) 2011 EPITA Research and Development Laboratory (LRDE)
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

#ifndef SCRIBO_BINARIZATION_INTERNAL_LOCAL_THRESHOLD_CORE_HH
# define SCRIBO_BINARIZATION_INTERNAL_LOCAL_THRESHOLD_CORE_HH

/// \file
///
///

# include <mln/core/concept/image.hh>
# include <mln/data/transform.hh>
# include <mln/value/int_u8.hh>

# include <mln/border/mirror.hh>
# include <mln/border/adjust.hh>

# include <scribo/canvas/integral_browsing.hh>

# include <scribo/util/init_integral_image.hh>
# include <scribo/util/integral_sum_sum2_functor.hh>
# include <scribo/util/compute_sub_domains.hh>

# ifdef SCRIBO_LOCAL_THRESHOLD_DEBUG
#  include <scribo/binarization/internal/local_threshold_debug.hh>
#  include <mln/io/pgm/save.hh>
#  include <mln/io/pbm/save.hh>
#  include <mln/data/saturate.hh>
# endif // ! SCRIBO_LOCAL_THRESHOLD_DEBUG

# include <mln/util/timer.hh>

namespace scribo
{

  namespace binarization
  {

    namespace internal
    {

      using namespace mln;


      /*! \brief Run Sauvola core algorithm. Use functor \p f to
	  compute the threshold and perform computation.

	\input[in]  input       An image.
	\input[in]  f           Functor computing Sauvola's formula.
	\input[in]  window_size The window size.

      */
      template <typename I, typename F>
      void
      local_threshold_core(const Image<I>& input, F& f,
		   unsigned window_size);


      /// \overload
      /// The window size is set to 11.
      //
      template <typename I, typename F>
      void
      local_threshold_core(const Image<I>& input, F& f);


# ifndef MLN_INCLUDE_ONLY


      // Implementations.

      namespace impl
      {

	namespace generic
	{

	  using namespace mln;

	  template <typename I, typename F>
	  void
	  local_threshold_core(const Image<I>& input, unsigned window_size)
	  {
	    trace::entering("scribo::binarization::internal::impl::generic::local_threshold_core");

	    // Not implemented
	    mlc_abort(I)::check();

	    trace::exiting("scribo::binarization::internal::impl::generic::local_threshold_core");
	  }

	} // end of namespace scribo::binarization::internal::impl::generic


	template <typename I, typename F>
	void
	local_threshold_core_fastest(const Image<I>& input, F& f, unsigned window_size)
	{
	  trace::entering("scribo::binarization::internal::impl::generic::local_threshold_core");
	  mln_precondition(exact(input).is_valid());

	  mln::util::timer t;
	  t.start();

	  // Make sure the image sizes are a multiple of 3 in each
	  // dimension. (browsing while binarizing relies on that
	  // property).
	  mln::util::array<mln::util::couple<box2d, unsigned> >
	    sub_domains = scribo::util::compute_sub_domains(input, 1, 3);

	  border::adjust(input, sub_domains(1).second());
	  border::mirror(input);

	  scribo::util::integral_sum_sum2_functor<value::int_u8, double> fi;
	  image2d<mln::util::couple<double,double> >
	    integral = scribo::util::init_integral_image(input, 3, fi,
							 sub_domains[2].first(),
							 sub_domains[2].second());

	  t.stop();
	  std::cout << "image integrale - " << t << std::endl;

#  ifdef SCRIBO_LOCAL_THRESHOLD_DEBUG
	  initialize(internal::debug_mean, input);
	  initialize(internal::debug_stddev, input);

	  initialize(internal::debug_threshold, input);
	  initialize(internal::debug_alpham, input);
	  initialize(internal::debug_alphacond, input);
#  endif // ! SCRIBO_LOCAL_THRESHOLD_DEBUG

	  t.restart();

	  window_size /= 3;
	  if (window_size % 2)
	    window_size += 2;
	  else
	    window_size += 1;

	  scribo::canvas::integral_browsing(integral, 1, window_size,
					    window_size, 3, f);
	  t.stop();
	  std::cout << "Binarization - " << t << std::endl;

	  trace::exiting("scribo::binarization::internal::impl::generic::local_threshold_core");
	}


      } // end of namespace scribo::binarization::internal::impl



      // Dispatch

      namespace internal
      {

	template <typename I, typename F>
	void
	local_threshold_core_dispatch(const mln_value(I)&,
			      const Image<I>& input, F& f,
			      unsigned window_size)
	{
	  impl::generic::local_threshold_core(input, f, window_size);
	}

	template <typename I, typename F>
	void
	local_threshold_core_dispatch(mln::trait::image::speed::fastest,
			      const mln_value(I)&,
			      const Image<I>& input, F& f,
			      unsigned window_size)
	{
	  impl::local_threshold_core_fastest(input, f, window_size);
	}


	template <typename I, typename F>
	void
	local_threshold_core_dispatch(const Image<I>& input, F& f,
			      unsigned window_size)
	{
	  typedef mln_value(I) V;
	  local_threshold_core_dispatch(mln_trait_image_speed(I)(),
				V(), input, f, window_size);
	}

      } // end of namespace scribo::binarization::internal::internal



      // Facades

      template <typename I, typename F>
      void
      local_threshold_core(const Image<I>& input, F& f, unsigned window_size)
      {
	trace::entering("scribo::binarization::internal::local_threshold_core");

	mln_precondition(exact(input).is_valid());

	internal::local_threshold_core_dispatch(input, f, window_size);

# ifdef SCRIBO_LOCAL_THRESHOLD_DEBUG
	if (stddev_image_output)
	  io::pgm::save(data::saturate(value::int_u8(), debug_stddev),
			stddev_image_output);
	if (mean_image_output)
	  io::pgm::save(data::saturate(value::int_u8(), debug_mean),
			mean_image_output);
	if (threshold_image_output)
	  io::pgm::save(data::saturate(value::int_u8(), debug_threshold),
			threshold_image_output);

	if (alpham_image_output)
	  io::pgm::save(data::saturate(value::int_u8(), debug_alpham),
			alpham_image_output);
	if (alphacond_image_output)
	  io::pbm::save(debug_alphacond, alphacond_image_output);
# endif // ! SCRIBO_LOCAL_THRESHOLD_DEBUG

	trace::exiting("scribo::binarization::internal::local_threshold_core");
      }

      template <typename I, typename F>
      void
      local_threshold_core(const Image<I>& input, F& f)
      {
	local_threshold_core(input, f, 11);
      }


# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace scribo::binarization::internal

  } // end of namespace scribo::binarization

} // end of namespace scribo


#endif // ! SCRIBO_BINARIZATION_INTERNAL_LOCAL_THRESHOLD_CORE_HH
