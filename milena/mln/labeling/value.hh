// Copyright (C) 2007, 2008, 2009, 2010, 2013 EPITA Research and
// Development Laboratory (LRDE)
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

#ifndef MLN_LABELING_VALUE_HH
# define MLN_LABELING_VALUE_HH

/// \file
///
/// Connected component labeling of image sites at a given value.

# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/canvas/labeling/video.hh>
# include <mln/data/fill.hh>



namespace mln
{

  namespace labeling
  {

    /*! \brief Connected component labeling of the image sites at a
     given value.

     \param[in]  input    The input image.
     \param[in]  val      The value to consider.
     \param[in]  nbh      The connectivity of components.
     \param[out] nlabels  The number of labels.
     \return              The label image.

     \ingroup labeling
    */
    template <typename I, typename N, typename L>
    mln_ch_value(I, L)
    value(const Image<I>& input, const mln_value(I)& val,
	  const Neighborhood<N>& nbh, L& nlabels);


# ifndef MLN_INCLUDE_ONLY


    // Tests.

    namespace internal
    {

      template <typename I, typename N, typename L>
      void
      value_tests(const Image<I>& input, const mln_value(I)& val, const Neighborhood<N>& nbh,
		  L& nlabels)
      {
	mln_precondition(exact(input).is_valid());
	mln_precondition(exact(nbh).is_valid());

	(void) input;
	(void) val;
	(void) nbh;
	(void) nlabels;
      }

    } // end of namespace mln::labeling::internal



    namespace impl
    {

      // Generic functor.

      template <typename I, typename L>
      struct value_functor
      {
	typedef mln_psite(I) P;

	const I& input;
	const mln_value(I)& val;

	// Requirements from mln::canvas::labeling.

        typedef mln_domain(I) S;

	// Generic implementation

	void init()				  {}
	bool handles(const P& p) const		  { return input(p) == val; }
	bool equiv(const P& n, const P&) const	  { return input(n) == val; }
	bool labels(const P&) const		  { return true; }
	void do_no_union(const P&, const P&)      {}
	void init_attr(const P&)		  {}
	void merge_attr(const P&, const P&)	  {}
	void set_new_label(const P&, const L&)    {}
	void set_label(const P&, const L&)        {}
	void finalize()                           {}

	// Fastest implementation

	void init_()				  {}
	bool handles_(unsigned p) const		  { return input.element(p) == val; }
	bool equiv_(unsigned n, unsigned) const	  { return input.element(n) == val; }
	bool labels_(unsigned) const		  { return true; }
	void do_no_union_(unsigned, unsigned)	  {}
	void init_attr_(unsigned)		  {}
	void merge_attr_(unsigned, unsigned)	  {}
	void set_new_label_(unsigned, const L&)   {}
	void set_label_(unsigned, const L&)       {}
	void finalize_()                          {}

	// end of Requirements.

	value_functor(const Image<I>& input_, const mln_value(I)& val)
	  : input(exact(input_)),
	    val(val)
	{
	}
      };

    } // end of namespace mln::labeling::impl




    // Facade.

    template <typename I, typename N, typename L>
    mln_ch_value(I, L)
    value(const Image<I>& input, const mln_value(I)& val,
	  const Neighborhood<N>& nbh, L& nlabels)
    {
      mln_trace("labeling::value");

      internal::value_tests(input, val, nbh, nlabels);

      mln_ch_value(I, L) output;
      impl::value_functor<I,L> f(input, val);
      output = canvas::labeling::video(input, nbh, nlabels, f);

      return output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::labeling

} // end of namespace mln


#endif // ! MLN_LABELING_VALUE_HH
