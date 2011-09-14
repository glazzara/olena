// Copyright (C) 2007 EPITA Research and Development Laboratory
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

#ifndef MLN_DISPLAY_COLOR_PRETTY_HH
# define MLN_DISPLAY_COLOR_PRETTY_HH

/*! \file mln/display/color_pretty.hh
 *
 * \brief Create a pretty image, which display its content
 *  with red value for undefined point site.
 */

# include <mln/trait/image_from_grid.hh>
# include <mln/core/image/image2d.hh>
# include <mln/value/rgb8.hh>
# include <mln/data/fill.hh>
# include <mln/data/paste.hh>
# include <mln/core/site_set/p_set.hh>
# include <mln/metal/is_not.hh>

namespace mln
{

  namespace display
  {

    /*! Return new image colored from an image \p input_. with the
     *  color red for the pixels not in the domain of \p input_.
     *
     * \param[in] input_ The image in gray level to color.
     *
     * \return colored image from \p input_ in rgb8 with red for the
     * color filled when the point is in bounding box of \p input_ but
     * not defined.
     *
     * \pre \p input_ has to be initialized.
     * \pre \p input_ values isn't color.
     *
     */
    template <typename I>
    typename trait::image_from_grid < mln_mesh(I), value::rgb8 >::ret
    color_pretty(const Image<I>& input_);


    /*! Return new image colored from an image \p input_. with the
     *  color red for the pixels not in the domain of \p input_.
     *
     * \param[in] input_ The image whose domain let to define the domain
     * of the returned image.
     * \param[in] s1_ The p_set where the points will be colored in red.
     * \param[in] s2_ The p_set where the points will be colored in green.
     * \param[in] s3_ The p_set where the points will be colored in blue.
     *
     * \return colored image from \p input_ domain which be filled in rgb8.
     *
     */
    template <typename I>
    typename trait::image_from_grid < mln_mesh(I), value::rgb8 >::ret
    color_pretty_rgb(const Image<I>& input_,
		     const p_set<mln_psite(I) >& s1_,
		     const p_set<mln_psite(I) >& s2_,
		     const p_set<mln_psite(I) >& s3_);

# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {
      template <typename V>
      inline
      value::rgb8
      color_value(V v)
      {
	return value::rgb8(v, v, v);
      }

      template <unsigned int n>
      inline
      value::rgb8
      color_value(value::rgb<n> v)
      {
	return v;
      }

      inline
      value::rgb8
      color_value(bool b)
      {
	value::int_u8 r = b ? 255 : 0;
	return value::rgb8(r, r, r);
      }


      template <typename I>
      inline
      typename trait::image_from_grid < mln_mesh(I), value::rgb8 >::ret
      color_pretty(const Image<I>& input_)
      {
	trace::entering("display::impl::color_pretty");

	const I& input = exact (input_);

	image2d<value::rgb8> output(input.domain().bbox());
	data::fill(output, value::rgb8(255, 0, 0));

	{
	  mln_piter(I) p(input.domain());

	  for_all(p)
	    output(p) = value::rgb8(color_value(input(p)));
	}

	trace::exiting("display::impl::color_pretty");
	return output;
      }

      template <typename I>
      inline
      typename trait::image_from_grid < mln_mesh(I), value::rgb8 >::ret
      color_pretty_rgb(const Image<I>& input_,
		       const p_set<mln_psite(I) >& s1_,
		       const p_set<mln_psite(I) >& s2_,
		       const p_set<mln_psite(I) >& s3_)
      {
	trace::entering("display::impl::color_pretty_rgb");

	const I& input = exact (input_);

	image2d<value::rgb8> output(input.domain().bbox());
	data::fill(output, value::rgb8(0, 0, 0));

	{
	  mln_piter(p_set<mln_psite(I) >) p(s1_);

	  for_all(p)
	    output(p).red() = 255;
	}

	{
	  mln_piter(p_set<mln_psite(I) >) p(s2_);

	  for_all(p)
	    output(p).green() = 255;
	}

	{
	  mln_piter(p_set<mln_psite(I) >) p(s3_);

	  for_all(p)
	    output(p).blue() = 255;
	}

	trace::exiting("display::impl::color_pretty_rgb");
	return output;
      }


    } // end of namespace mln::display::impl


    /// Facade.

    template <typename I>
    inline
    typename trait::image_from_grid < mln_mesh(I), value::rgb8 >::ret
    color_pretty(const Image<I>& input_)
    {
      trace::entering("display::color_pretty");

      const I& input = exact(input_);
      mln_precondition(input.is_valid());

      mlc_is_not(mln_trait_value_kind(mln_value(I)) (),
		 trait::value::kind::color)::check();

      image2d<value::rgb8> output = impl::color_pretty(input);

      trace::exiting("display::color_pretty");
      return output;
    }

    template <typename I>
    inline
    typename trait::image_from_grid < mln_mesh(I), value::rgb8 >::ret
    color_pretty_rgb(const Image<I>& input_,
		     const p_set<mln_psite(I) >& s1_,
		     const p_set<mln_psite(I) >& s2_,
		     const p_set<mln_psite(I) >& s3_)
    {
      trace::entering("display::color_pretty_rgb");

      image2d<value::rgb8> output = impl::color_pretty_rgb(input_, s1_, s2_, s3_);

      trace::exiting("display::color_pretty_rgb");
      return output;
    }

# endif // !MLN_INCLUDE_ONLY

  } // end of namespace mln::display

} // end of namespace mln


#endif //  ! MLN_DISPLAY_COLOR_PRETTY_HH
