// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_LABELING_LEVEL_HH
# define MLN_LABELING_LEVEL_HH

/// \file mln/labeling/level.hh
///
/// Connected component labeling of the image objects at a given
/// level.

# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/canvas/labeling.hh>
# include <mln/data/fill.hh>



namespace mln
{

  namespace labeling
  {

    /*! Connected component labeling of the image objects at a given
     * level.
     *
     * \param[in]  input    The input image.
     * \param[in]  val      The level to consider for the labeling.
     * \param[in]  nbh      The connexity of the level components.
     * \param[out] nlabels  The number of labels.
     * \return              The label image.
     */
    template <typename I, typename N, typename L>
    mln_ch_value(I, L)
    level(const Image<I>& input, const mln_value(I)& val,
	  const Neighborhood<N>& nbh, L& nlabels);


# ifndef MLN_INCLUDE_ONLY


    // Tests.

    namespace internal
    {

      template <typename I, typename N, typename L>
      void
      level_tests(const Image<I>& input, const mln_value(I)& val, const Neighborhood<N>& nbh,
		  L& nlabels)
      {
	mln_precondition(exact(input).is_valid());
	// mln_precondition(exact(nbh).is_valid());

	(void) input;
	(void) val;
	(void) nbh;
	(void) nlabels;
      }

    } // end of namespace mln::labeling::internal



    namespace impl
    {

      // Generic functor.

      template <typename I>
      struct level_functor
      {
	typedef mln_psite(I) P;

	const I& input;
	const mln_value(I)& val;

	// Requirements from mln::canvas::labeling.

        typedef mln_pset(I) S;

	// Generic implementation

	void init()				  {}
	bool handles(const P& p) const		  { return input(p) == val; }
	bool equiv(const P& n, const P&) const	  { return input(n) == val; }
	bool labels(const P&) const		  { return true; }
	void do_no_union(const P& n, const P& p)  {}
	void init_attr(const P&)		  {}
	void merge_attr(const P& r, const P& p)	  {}

	// Fastest implementation

	void init_()				  {}
	bool handles_(unsigned p) const		  { return input.element(p) == val; }
	bool equiv_(unsigned n, unsigned) const	  { return input.element(n) == val; }
	bool labels_(unsigned) const		  { return true; }
	void do_no_union_(unsigned n, unsigned p) {}
	void init_attr_(unsigned)		  {}
	void merge_attr_(unsigned r, unsigned p)  {}

	// end of Requirements.

	level_functor(const Image<I>& input_, const mln_value(I)& val)
	  : input(exact(input_)),
	    val(val)
	{
	}
      };

    } // end of namespace mln::labeling::impl




    // Facade.

    template <typename I, typename N, typename L>
    mln_ch_value(I, L)
    level(const Image<I>& input, const mln_value(I)& val, const Neighborhood<N>& nbh,
	  L& nlabels)
    {
      trace::entering("labeling::level");

      internal::level_tests(input, val, nbh, nlabels);

      mln_ch_value(I, L) output;
      impl::level_functor<I> f(input, val);
      output = canvas::labeling_video(input, nbh, nlabels, f);

      trace::exiting("labeling::level");
      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::labeling

} // end of namespace mln


#endif // ! MLN_LABELING_LEVEL_HH
