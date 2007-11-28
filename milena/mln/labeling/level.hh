// Copyright (C) 2007 EPITA Research and Development Laboratory
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

/*! \file mln/labeling/level.hh
 *
 * \brief Connected component labeling of the image objects at a given
 * level.
 */

# include <mln/core/concept/image.hh>
# include <mln/labeling/base.hh>
# include <mln/level/fill.hh>
# include <mln/border/adjust.hh>
# include <mln/value/other.hh>



namespace mln
{

  namespace labeling
  {

    /*! Connected component labeling of the image objects at a given
     * level.
     *
     * \param[in]  input The input image.
     * \param[in]  val   The level to consider for the labeling.
     * \param[in]  nbh   The neighborhood.
     * \param[out] nlabels The number of labels.
     * \return  The label image.
     */
    template <typename I, typename N>
    mln_ch_value(I, unsigned)
      level(const Image<I>& input, const mln_value(I)& val, const Neighborhood<N>& nbh,
	    unsigned& nlabels);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      // Functors.

      template <typename I_, typename N_, typename O_>
      struct level_t : base_<I_,N_,O_>
      {
	typedef mln_point(I_) P;

	// requirements from mln::canvas::labeling:

	typedef mln_pset(I_) S;
	const S& s;

 	inline
 	void init()                            { mln::level::fill(this->output, 0); } // FIXME: use literal::zero
	inline
	bool handles(const P& p) const         { return input(p) == val; }
	inline
	bool equiv(const P& n, const P&) const { return input(n) == val; }

	// end of requirements

	const mln_value(I_)& val;

	inline
	level_t(const I_& input, const mln_value(I_)& val, const N_& nbh, O_& output)
	  : base_<I_,N_,O_>(input, nbh, output),
	    s(input.domain()),
	    val(val)
	{}
      };

      // Routines.

      template <typename I, typename N>
      inline
      mln_ch_value(I, unsigned)
	level_(trait::image::speed::any, const I& input,
	       const mln_value(I)& val, const Neighborhood<N>& nbh,
	       unsigned& nlabels)
      {
	typedef mln_ch_value(I, unsigned) O;
	O output;
	initialize(output, input);

	typedef impl::level_t<I,N,O> F;
	F f(input, val, exact(nbh), output);
	canvas::labeling<F> run(f);

	nlabels = f.nlabels;
	// FIXME: Handle f.status
	return output;
      }

      // FIXME: Add fast versions.

      template <typename I_, typename N_, typename O_>
      struct level_fast_t : labeling::impl::base_fast_<I_,N_,O_>
      {
	typedef mln_point(I_) P;
	
	// 	typedef mln_pset(I_) S;
	// 	const S& s;
	
	inline
	void init()                            { mln::level::fill(this->output, 0); }
	inline
	bool handles(unsigned p) const         { return this->input[p] == val; }
	inline
	bool equiv(unsigned n, unsigned) const { return this->input[n] == val; }
	
	const mln_value(I_)& val;
	
	inline
	level_fast_t(const I_& input, const mln_value(I_)& val, const N_& nbh, O_& output)
	  : labeling::impl::base_fast_<I_,N_,O_>(input, nbh, output),
	    // 	    s(input.domain()),
	    val(val)
	{}
      };


      template <typename I, typename N>
      inline
      mln_ch_value(I, unsigned)
	level_(trait::image::speed::fastest, const I& input,
	       const mln_value(I)& val, const Neighborhood<N>& nbh,
	       unsigned& nlabels)
      {
	typedef mln_ch_value(I, unsigned) O;
	typedef level_fast_t<I,N,O> F;

	border::adjust(input, exact(nbh).delta());
	O output;
	initialize(output, input);
	mln_assertion(output.border() == input.border());

	F f(input, val, exact(nbh), output);
	canvas::labeling_fast<F> run(f);

	// FIXME: Handle f.status

	nlabels = f.nlabels;
	return output;
      }

    } // end of namespace mln::labeling::impl


    // Facade.

    template <typename I, typename N>
    inline
    mln_ch_value(I, unsigned)
      level(const Image<I>& input, const mln_value(I)& val, const Neighborhood<N>& nbh,
	    unsigned& nlabels)
    {
      mln_precondition(exact(input).has_data());
      return impl::level_(mln_trait_image_speed(I)(), exact(input),
			  val, nbh, nlabels);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::labeling

} // end of namespace mln


#endif // ! MLN_LABELING_LEVEL_HH
