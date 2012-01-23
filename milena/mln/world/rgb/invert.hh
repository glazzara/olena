// Copyright (C) 2010, 2012, 2013 EPITA Research and Development
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

#ifndef MLN_WORLD_RGB_INVERT_HH
# define MLN_WORLD_RGB_INVERT_HH

/// \file
///
/// Point-wise invert (min -> max and max -> min) of RGB images.
///
/// \todo Move somewhere else?

# include <mln/core/concept/image.hh>
# include <mln/trait/value_.hh>
# include <mln/value/rgb.hh>

// Specializations are in:
# include <mln/world/rgb/invert.spe.hh>


namespace mln
{

  namespace world
  {

    namespace rgb
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
       *   {\n
       *     output(p).red() = min(red) + (max(red) - input(p).red()) \n
       *     output(p).green() = min(green) + (max(green) - input(p).green()) \n
       *     output(p).blue() = min(blue) + (max(blue) - input(p).blue()) \n
       *   }
       */
      template <typename I>
      mln_concrete(I) invert(const Image<I>& input);


      /// Point-wise in-place reversion of image \p input.
      /*!
       * \param[in,out] input The target image.
       *
       * \pre \p input.is_valid
       *
       * It performs: \n
       *   for all p of input.domain \n
       *   {\n
       *     output(p).red() = min(red) + (max(red) - input(p).red()) \n
       *     output(p).green() = min(green) + (max(green) - input(p).green()) \n
       *     output(p).blue() = min(blue) + (max(blue) - input(p).blue()) \n
       *   }
       */
      template <typename I>
      void invert_inplace(Image<I>& input);


# ifndef MLN_INCLUDE_ONLY

      namespace impl
      {

	namespace generic
	{

	  template <typename I, typename O>
	  inline
	  void invert(const Image<I>& input_, Image<O>& output_)
	  {
	    trace::entering("world::rgb::impl::generic::invert");

	    const I& input = exact(input_);
	    O& output = exact(output_);

	    mln_precondition(input.is_valid());
	    mln_precondition(output.is_valid());

	    typedef mln_value(I) V;
	    mln_piter(I) p(input.domain());
	    for_all(p)
	      output(p) = mln_min(V) + (mln_max(V) - input(p));

	    trace::exiting("world::rgb::impl::generic::invert");
	  }

	} // end of namespace mln::world::rgb::impl::generic


      } // end of namespace mln::world::rgb::impl



      // Dispatch.

      namespace internal
      {

	template <typename I, typename O>
	inline
	void
	invert_dispatch(trait::image::speed::any, const mln_value(I)&,
			const I& /* input*/ , O& /* output */)
	{
	  // No implementation found.  Maybe `input' is not an RGB image?
	  mlc_abort(I)::check();
	}


	template <typename I, typename O, unsigned n>
	inline
	void
	invert_dispatch(trait::image::speed::any, const value::rgb<n>&,
			const I& input, O& output)
	{
	  impl::invert_rgb(input, output);
	}

	template <typename I, typename O, unsigned n>
	inline
	void
	invert_dispatch(trait::image::speed::fastest, const value::rgb<n>&,
			const I& input, O& output)
	{
	  impl::invert_rgb_fastest(input, output);
	}


	template <typename I, typename O>
	inline
	void
	invert_dispatch(const Image<I>& input, Image<O>& output)
	{
	  typedef mln_value(I) V;
	  invert_dispatch(mln_trait_image_speed(I)(), V(),
			  exact(input), exact(output));

	}


      } // end of namespace mln::world::rgb::internal




      // Facades.

      template <typename I>
      inline
      mln_concrete(I) invert(const Image<I>& input)
      {
	trace::entering("world::rgb::invert");

	mln_precondition(exact(input).is_valid());

	mln_concrete(I) output;
	initialize(output, input);
	internal::invert_dispatch(input, output);

	trace::exiting("world::rgb::invert");
	return output;
      }

      template <typename I>
      inline
      void invert_inplace(Image<I>& input)
      {
	trace::entering("world::rgb::invert_inplace");

	mln_precondition(exact(input).is_valid());

	internal::invert_dispatch(input, input);

	trace::exiting("world::rgb::invert_inplace");
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::world::rgb

  } // end of namespace mln::world

} // end of namespace mln


#endif // ! MLN_WORLD_RGB_INVERT_HH
