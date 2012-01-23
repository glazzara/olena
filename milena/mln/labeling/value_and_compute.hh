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

#ifndef MLN_LABELING_VALUE_AND_COMPUTE_HH
# define MLN_LABELING_VALUE_AND_COMPUTE_HH

/// \file
///
/// Connected component labeling of image sites at a given value.

# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/canvas/labeling/video.hh>
# include <mln/data/fill.hh>
# include <mln/util/array.hh>
# include <mln/util/couple.hh>



namespace mln
{

  namespace labeling
  {

    /// \brief Connected component labeling of the image sites at a given
    /// value.
    ///
    /// \param[in]  input    The input image.
    /// \param[in]  val      The value to consider.
    /// \param[in]  nbh      The connectivity of components.
    /// \param[out] nlabels  The number of labels.
    /// \param[in]  accu     The accumulator to be computed.
    /// \return              The label image.
    //
    template <typename I, typename N, typename L, typename A>
    util::couple<mln_ch_value(I,L),
		 util::couple<util::array<mln_result(A)>,
			      util::array<A> > >
    value_and_compute(const Image<I>& input, const mln_value(I)& val,
		      const Neighborhood<N>& nbh, L& nlabels,
		      const Accumulator<A>& accu);


# ifndef MLN_INCLUDE_ONLY


    // Tests.

    namespace internal
    {

      template <typename I, typename N, typename L, typename A>
      void
      value_and_compute_tests(const Image<I>& input,
			      const mln_value(I)& /* val */,
			      const Neighborhood<N>& nbh, L& /* nlabels */,
			      const Accumulator<A>& /* accu */)
      {
	mln_precondition(exact(input).is_valid());
	mln_precondition(exact(nbh).is_valid());

	(void) input;
	(void) nbh;
      }

    } // end of namespace mln::labeling::internal



    namespace impl
    {

      // Generic functor.

      template <typename I, typename L, typename A>
      struct value_and_compute_functor
      {
	typedef mln_psite(I) P;

	util::array<mln_result(A)> result_;
	util::array<A> accus_;

	const I& input;
	const mln_value(I)& val;

	typedef mln_result(A) accu_result;
	typedef mln_argument(A) accu_argument;
	typedef util::couple<util::array<accu_result>,
			     util::array<A> > result;


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
	void set_new_label(const P& p, const L& l)
	{
	  accus_.append(A());
	  process__(accu_argument(), p, l);
	}
	void set_label(const P& p, const L& l)    { process__(accu_argument(), p, l); };
	void finalize() { convert::from_to(accus_, result_); }



	// Fastest implementation

	void init_()				  { accus_.append(A()); }
	bool handles_(unsigned p) const		  { return input.element(p) == val; }
	bool equiv_(unsigned n, unsigned) const	  { return input.element(n) == val; }
	bool labels_(unsigned) const		  { return true; }
	void do_no_union_(unsigned, unsigned)	  {}
	void init_attr_(unsigned)		  {}
	void merge_attr_(unsigned, unsigned)	  {}
	void set_new_label_(unsigned p, const L& l)
	{
	  accus_.append(A());
	  process__(accu_argument(), p, l);
	};
	void set_label_(unsigned p, const L& l)   { process__(accu_argument(), p, l); };
	void finalize_() { convert::from_to(accus_, result_); }

	// end of Requirements.



	value_and_compute_functor(const Image<I>& input_, const mln_value(I)& val)
	  : input(exact(input_)),
	    val(val)
	{
	}


      private:
	inline
	void process__(const unsigned&, unsigned p, const L& l)
	{
	  accus_[l].take(p);
	}

	inline
	void process__(const mln_psite(I)&, unsigned p, const L& l)
	{
	  accus_[l].take(input.point_at_offset(p));
	}


	inline
	void process__(const mln_psite(I)&, const mln_site(I)& p, const L& l)
	{
	  accus_[l].take(p);
	}

	inline
	void process__(const mln_value(I)&, const mln_site(I)&, const L& l)
	{
	  accus_[l].take(l);
	}

	template <typename V>
	inline
	void process__(const V&, const mln_site(I)&, const L& /* l */)
	{
	  mlc_abort(V)::check();
	}


      };

    } // end of namespace mln::labeling::impl




    // Facade.

    template <typename I, typename N, typename L, typename A>
    util::couple<mln_ch_value(I,L),
		 util::couple<util::array<mln_result(A)>,
			      util::array<A> > >
    value_and_compute(const Image<I>& input, const mln_value(I)& val,
		      const Neighborhood<N>& nbh, L& nlabels,
		      const Accumulator<A>& accu)
    {
      trace::entering("labeling::value_and_compute");

      internal::value_and_compute_tests(input, val, nbh, nlabels, accu);

      typedef mln_ch_value(I,L) out_t;
      typedef impl::value_and_compute_functor<I, L, A> func_t;
      func_t f(input, val);
      out_t output = canvas::labeling::video(input, nbh, nlabels, f);

      util::couple<out_t, typename func_t::result>
	result = make::couple(output,
			      make::couple(f.result_, f.accus_));


      trace::exiting("labeling::value_and_compute");
      return result;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::labeling

} // end of namespace mln


#endif // ! MLN_LABELING_VALUE_AND_COMPUTE_HH
