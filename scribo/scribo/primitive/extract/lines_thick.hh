// Copyright (C) 2009, 2012 EPITA Research and Development Laboratory
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

#ifndef SCRIBO_PRIMITIVE_EXTRACT_LINES_THICK_HH
# define SCRIBO_PRIMITIVE_EXTRACT_LINES_THICK_HH

/// \file
///
/// Extract thick lines in a binary image.


# include <mln/core/concept/image.hh>
# include <mln/core/concept/window.hh>
# include <mln/core/concept/neighborhood.hh>

# include <mln/morpho/opening/structural.hh>

# include <scribo/core/macros.hh>
# include <scribo/primitive/extract/components.hh>

namespace scribo
{

  namespace primitive
  {

    namespace extract
    {


      using namespace mln;

      /*! \brief Extract thick lines in a binary image.
       * Only non discontinued lines are correctly extracted with this routine.
       *
       * \param[in]     input_	  A binary image.
       * \param[in]     nbh_	  The neighborhood used for labeling image
       *				  components.
       * \param[in,out] nlines	  Type used for labeling.
       * \param[in]     win_	  Window used to extract the lines
       *
       * \return An image in which lines are labeled.
       *
       * \ingroup extractprimitiveseps
       */
      template <typename I, typename N, typename V, typename W>
      component_set<mln_ch_value(I,V)>
      lines_thick(const Image<I>& input_,
		  const Neighborhood<N>& nbh_, V& nlines,
		  const Window<W>& win_);



# ifndef MLN_INCLUDE_ONLY


      namespace internal
      {

        template <typename I, typename N, typename V, typename W>
        void
	lines_thick_tests(const Image<I>& input,
			  const Neighborhood<N>& nbh, V& nlines,
			  const Window<W>& win)
	{
	  mlc_equal(mln_value(I),bool)::check();
	  mlc_bool(mln_site_(I)::dim == 2)::check();
	  mlc_is_a(V, mln::value::Symbolic)::check();

	  mln_precondition(exact(input).is_valid());
	  mln_precondition(exact(nbh).is_valid());
	  mln_precondition(exact(win).is_valid());

	  (void) input;
	  (void) nbh;
	  (void) win;
	  (void) nlines;
	}

      } // end of namespace scribo::primitive::internal



      template <typename I, typename N, typename V, typename W>
      inline
      component_set<mln_ch_value(I,V)>
      lines_thick(const Image<I>& input_,
		  const Neighborhood<N>& nbh_, V& nlines,
		  const Window<W>& win_)
      {
	mln_trace("scribo::primitive::lines_thick");

	internal::lines_thick_tests(input_, nbh_, nlines, win_);

	const I& input = exact(input_);
	const N& nbh = exact(nbh_);
	const W& win = exact(win_);

	mln_ch_value(I,bool) filter = morpho::opening::structural(input, win);
	component_set<mln_ch_value(I,V)> output
	  = primitive::extract::components(filter, nbh, nlines);

	return output;
      }


# endif // !MLN_INCLUDE_ONLY

    } // end of namespace scribo::primitive::extract

  } // end of namespace scribo::primitive

} // end of namespace scribo

#endif // ! SCRIBO_PRIMITIVE_EXTRACT_VERTICAL_LINES_THICK_HH
