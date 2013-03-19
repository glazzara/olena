// Copyright (C) 2010, 2011 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef SCRIBO_DEBUG_HIGHLIGHT_TEXT_AREA_HH
# define SCRIBO_DEBUG_HIGHLIGHT_TEXT_AREA_HH

/// \file
///
/// Darken an image and highlight the text areas.

# include <mln/core/concept/image.hh>
# include <mln/core/concept/function.hh>

# include <mln/draw/box.hh>

# include <mln/data/fill.hh>
# include <mln/data/transform.hh>

# include <mln/literal/colors.hh>

# include <mln/value/rgb8.hh>

# include <scribo/core/component_set.hh>
# include <scribo/core/line_set.hh>

# include <mln/geom/rotate.hh>

namespace scribo
{

  namespace debug
  {

    using namespace mln;

    /*! \brief Darken an image and highlight valid lines.

      \param[in] input An image.
      \param[in] bbox An array of text area bounding boxes.

      \return a color image with highlighted text areas.

      \ingroup grpalgodebug
     */
    template <typename I>
    mln_ch_value(I, value::rgb8)
    highlight_text_area(const Image<I>& input,
			const mln::util::array<box<mln_site(I)> >& bbox);


    /*! \brief Darken an image and highlight valid lines.

      \param[in] input An image.
      \param[in] lines A line set.

      \return a color image with highlighted text areas.

      \ingroup grpalgodebug
     */
    template <typename I, typename L>
    mln_ch_value(I, value::rgb8)
    highlight_text_area(const Image<I>& input,
			const line_set<L>& lines);

    /*! \brief Darken an image and highlight valid lines.

      This overload is useful if you do know that all the components
      are text.

      \param[in] input An image.
      \param[in] components A component set.

      \return a color image with highlighted text areas.

      \ingroup grpalgodebug
     */
    template <typename I, typename L>
    mln_ch_value(I, value::rgb8)
    highlight_text_area(const Image<I>& input,
			const scribo::component_set<L>& components);


# ifndef MLN_INCLUDE_ONLY

    namespace internal
    {

      template <typename M, typename R>
      struct mask_non_text
	: Function_v2v<mask_non_text<M, R> >,
	  private mlc_is(mln_value(M), bool)::check_t
      {
	typedef R result;

	mask_non_text(const Image<M>& mask)
	  : mask_(exact(mask)), p_(mask_)
	{
	  p_.start();
	}

	result operator()(const result& v) const
	{
	  bool b = p_.val();
	  p_.next();
	  if (!b)
	    return v / 2;
	  else
	    return v;

	}

	M mask_;
	mutable mln_pixter(M) p_;
      };

    } // end of namespace scribo::debug::internal



    template <typename I>
    mln_ch_value(I, value::rgb8)
    highlight_text_area(const Image<I>& input_,
			const mln::util::array<box<mln_site(I)> >& bbox)
    {
      mln_trace("scribo::debug::highlight_text_area");

      const I& input = exact(input_);

      mln_precondition(input.is_valid());
      mlc_is(mln_value(I), value::rgb8)::check();

      typedef mln_ch_value(I, bool) mask_t;
      mask_t mask;
      initialize(mask, input);
      data::fill(mask, false);

      for_all_elements(i, bbox)
	data::fill((mask | bbox(i)).rw(), true);

      internal::mask_non_text<mask_t, mln_value(I)> f(mask);
      mln_concrete(I) output = data::transform(input, f);

      for_all_elements(i, bbox)
	mln::draw::box(output, bbox(i), literal::red);

      return output;
    }


    template <typename I, typename L>
    mln_ch_value(I, value::rgb8)
    highlight_text_area(const Image<I>& input_,
			const line_set<L>& lines)
    {
      mln_trace("scribo::debug::highlight_text_area");

      const I& input = exact(input_);

      mln_precondition(input.is_valid());
      mlc_is(mln_value(I), value::rgb8)::check();

      typedef mln_ch_value(I, bool) mask_t;
      mask_t mask;
      initialize(mask, input);
      data::fill(mask, false);

      for_all_lines(i, lines)
      {
	if (! lines(i).is_valid()
	    || lines(i).tag() != line::None
	    || lines(i).type() != line::Text)
	  continue;

	data::fill((mask | lines(i).bbox()).rw(), true);
      }

      internal::mask_non_text<mask_t, mln_value(I)> f(mask);
      mln_concrete(I) output = data::transform(input, f);

      for_all_lines(i, lines)
      {
	if (! lines(i).is_valid()
	    || lines(i).tag() != line::None
	    || lines(i).type() != line::Text)
	  continue;

	mln::draw::box(output, lines(i).bbox(), literal::red);
      }

      return output;
    }


    template <typename I, typename L>
    mln_ch_value(I, value::rgb8)
    highlight_text_area(const Image<I>& input_,
			const scribo::component_set<L>& components)
    {
      mln_trace("scribo::debug::highlight_text_area");

      const I& input = exact(input_);

      mln_precondition(input.is_valid());
      mlc_is(mln_value(I), value::rgb8)::check();

      typedef mln_ch_value(I, bool) mask_t;
      mask_t mask;
      initialize(mask, input);
      data::fill(mask, false);

      for_all_comps(i, components)
	if (components(i).is_valid())
	  data::fill((mask | components(i).bbox()).rw(), true);

      internal::mask_non_text<mask_t, mln_value(I)> f(mask);
      mln_ch_value(I, value::rgb8) output = data::transform(input, f);

      for_all_comps(i, components)
	if (components(i).is_valid())
	  mln::draw::box(output, components(i).bbox(), literal::red);

      return output;
    }


    template <typename I, typename L>
    mln_ch_value(I, value::rgb8)
    highlight_text_area_rotated(const Image<I>& input_,
				const scribo::component_set<L>& components,
				double angle, box2d rb)
    {
      mln_trace("scribo::debug::highlight_text_area");

      const I& input = exact(input_);

      mln_precondition(input.is_valid());
      mlc_is(mln_value(I), value::rgb8)::check();

      typedef mln_ch_value(I, bool) mask_t;
      mask_t mask;
      initialize(mask, input);
      data::fill(mask, false);

      mln::util::array<mln_box(I)> bbox(value::next(components.nelements()));

      for_all_comps(i, components)
	if (components(i).is_valid())
	{
	  bbox(i) = components(i).bbox();
	  bbox(i).pmin().row() += rb.pmin().row();
	  bbox(i).pmin().col() += rb.pmin().col();
	  bbox(i).pmax().row() += rb.pmin().row();
	  bbox(i).pmax().col() += rb.pmin().col();

	  bbox(i) = mln::geom::rotate(bbox(i), - angle,
				      input.domain().pcenter());
	}

      for_all_comps(i, components)
	if (components(i).is_valid())
	  data::fill((mask | bbox(i)).rw(), true);

      internal::mask_non_text<mask_t, mln_value(I)> f(mask);
      mln_ch_value(I, value::rgb8) output = data::transform(input, f);

      for_all_comps(i, components)
	if (components(i).is_valid())
	  mln::draw::box(output, bbox(i), literal::red);

      return output;
    }


# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace scribo::debug

} // end of namespace scribo


#endif // ! SCRIBO_DEBUG_HIGHLIGHT_TEXT_AREA_HH
