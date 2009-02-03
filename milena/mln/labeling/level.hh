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

// The 'fastest' specialization is in:
# include <mln/labeling/level.spe.hh>



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



    // Generic implementation.

    namespace impl
    {


      struct labeling_functor_base
      {
 	void init()                          {}

	template <typename P>
	bool handles(const P&) const         { return true; }

	template <typename L, typename R>
	bool equiv(const L&, const R&) const { return false; }

	template <typename P>
	bool labels(const P&) const          { return true; }

	template <typename L, typename R>
	void do_no_union(const L&, const R&) {}

	template <typename P>
	void init_attr(const P&)             {}

	template <typename L, typename R>
	void merge_attr(const L&, const R&)  {}
      };


      // Generic functor.

      template <typename I>
      struct level_functor : labeling_functor_base
      {
	typedef mln_psite(I) P;

	const I& input;
	const mln_value(I)& val;

	// Requirements from mln::canvas::labeling.

        typedef mln_pset(I) S;
        const S& s;

 	void init()                             {}
	bool handles(const P& p) const          { return input(p) == val; }
	bool equiv(const P& n, const P&) const  { return input(n) == val; }
	bool labels(const P&) const             { return true; }

	// end of Requirements.

	level_functor(const Image<I>& input_, const mln_value(I)& val)
	  : input(exact(input_)),
	    val(val),
	    s(input.domain())
	{
	}
      };



      // Generic implementation.

      namespace generic
      {

	template <typename I, typename N, typename L>
	mln_ch_value(I, L)
	level(const Image<I>& input, const mln_value(I)& val,
	      const Neighborhood<N>& nbh,
	      L& nlabels)
	{
	  trace::entering("labeling::impl::generic::level");

	  internal::level_tests(input, val, nbh, nlabels);

	  level_functor<I> f(input, val);
	  mln_ch_value(I, L) output = canvas::labeling(input, nbh, f, nlabels);

	  trace::exiting("labeling::impl::generic::level");
	  return output;
	}

      } // end of namespace mln::labeling::impl::generic


    } // end of namespace mln::labeling::impl


    // Dispatch.

    namespace internal
    {

      template <typename I, typename N, typename L>
      mln_ch_value(I, L)
      level_dispatch(const Image<I>& input, const mln_value(I)& val, const Neighborhood<N>& nbh,
		     L& nlabels)
      {
	return impl::generic::level(input, val, nbh, nlabels);
      }

    } // end of namespace mln::labeling::internal



    // Facade.

    template <typename I, typename N, typename L>
    mln_ch_value(I, L)
    level(const Image<I>& input, const mln_value(I)& val, const Neighborhood<N>& nbh,
	  L& nlabels)
    {
      trace::entering("labeling::level");

      internal::level_tests(input, val, nbh, nlabels);

      mln_ch_value(I, L) output;
      output = internal::level_dispatch(input, val, nbh, nlabels);

      trace::exiting("labeling::level");
      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::labeling

} // end of namespace mln


#endif // ! MLN_LABELING_LEVEL_HH
