// Copyright (C) 2009, 2010, 2011, 2012 EPITA Research and Development
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

#ifndef SCRIBO_PRIMITIVE_EXTRACT_LINES_H_SINGLE_HH
# define SCRIBO_PRIMITIVE_EXTRACT_LINES_H_SINGLE_HH

/// \file
///
/// \brief Fast Extraction of single horizontal thick lines.

# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>

# include <scribo/core/component_set.hh>
# include <scribo/primitive/extract/components.hh>


namespace scribo
{

  namespace primitive
  {

    namespace extract
    {


      using namespace mln;

      /*! \brief Fast Extraction of single horizontal thick lines.
       *
       * Only single non discontinued lines are correctly extracted
       * with this routine.
       *
       * \param[in]     input	    A binary image.
       * \param[in]     nbh	    The neighborhood used for labeling image
       *			    components.
       * \param[in,out] nlines	    Type used for labeling.
       * \param[in]     min_line_length The minimum line length.
       * \param[in]     w_h_ratio   The minimum ratio width/height object
       *                            bounding boxes to consider an
       *                            object as a single line.
       *
       * \return An image in which only horizontal single lines are
       * labeled.
       *
       * \ingroup extractprimitiveseps
       */
      template <typename I, typename N, typename V>
      component_set<mln_ch_value(I,V)>
      lines_v_single(const Image<I>& input,
		     const Neighborhood<N>& nbh, const V& nlines,
		     unsigned min_line_length,
		     float w_h_ratio);


      /*! \brief Fast Extraction of single horizontal thick lines.
       *
       * Only single non discontinued lines are correctly extracted
       * with this routine.
       *
       * \param[in]     components     A labeled image.
       * \param[in]     min_line_length The minimum line length.
       * \param[in]     w_h_ratio   The minimum ratio width/height object
       *                            bounding boxes to consider an
       *                            object as a single line.
       *
       * \return An image in which only horizontal single lines are
       * labeled.
       *
       * \ingroup extractprimitiveseps
       */
      template <typename L>
      component_set<L>
      lines_h_single(const component_set<L>& components,
		     unsigned min_line_length,
		     float w_h_ratio);


# ifndef MLN_INCLUDE_ONLY

      namespace internal
      {

	template <typename L>
	struct is_line_h_single
	  : Function_v2b<is_line_h_single<L> >
	{
	  typedef bool result;

	  is_line_h_single(const component_set<L>& components,
			   float w_h_ratio, unsigned min_line_length)
	    : w_h_ratio_(w_h_ratio), min_line_length_(min_line_length),
	      components_(components)
	  {
	  }


	  bool operator()(const mln_value(L)& label) const
	  {
	    const mln_domain(L)& box = components_(label).bbox();

	    unsigned
	      height = box.pmax().row() - box.pmin().row() + 1,
	      width = box.pmax().col() - box.pmin().col() + 1;

	    return ((width / (float)height) > w_h_ratio_
		    && width > min_line_length_);
	  }

	  float w_h_ratio_;
	  unsigned min_line_length_;

	  component_set<L> components_;
	};


      } // end of namespace mln::internal




      template <typename I, typename N, typename V>
      component_set<mln_ch_value(I,V)>
      lines_h_single(const Image<I>& input_,
		     const Neighborhood<N>& nbh_, const V& nlines,
		     unsigned min_line_length,
		     float w_h_ratio)
      {
	mln_trace("scribo::primitive::lines_h_single");

	const I& input = exact(input_);
	const N& nbh = exact(nbh_);
	mln_precondition(input.is_valid());
	mln_precondition(nbh.is_valid());

	typedef mln_ch_value(I,V) L;
  V ncomps;
	component_set<L>
	  output = primitive::extract::components(input, nbh, ncomps);
  (void) ncomps;

	internal::is_line_h_single<L>
	  is_line(output, w_h_ratio, min_line_length);

	output.update_tags(is_line, component::Ignored);

	return output;
      }




      template <typename L>
      component_set<L>
      lines_h_single(const component_set<L>& components,
		     unsigned min_line_length,
		     float w_h_ratio)
      {
	mln_trace("scribo::primitive::lines_h_single");

	mln_precondition(components.is_valid());

	internal::is_line_h_single<L>
	  is_line(components, w_h_ratio, min_line_length);

	component_set<L> output = components.duplicate();
	output.update_tags(is_line, component::Ignored);

	return output;
      }


# endif // ! MLN_INCLUDE_ONLY


    } // end of namespace scribo::primitive::extract

  } // end of namespace scribo::primitive

} // end of namespace scribo

#endif // ! SCRIBO_PRIMITIVE_EXTRACT_LINES_H_SINGLE_HH
