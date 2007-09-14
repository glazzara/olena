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

# include <mln/labeling/base.hh>
# include <mln/level/fill.hh>


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
     * \param[out] output  The label image.
     * \param[out] nlabels The number of labels.
     *
     * \return Succeed or not.
     */
    template <typename I, typename N, typename O>
    bool level(const Image<I>& input, const mln_value(I)& val, const Neighborhood<N>& nbh,
	       Image<O>& output, unsigned& nlabels);



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

 	void init()                            { mln::level::fill(this->output, 0); }
	bool handles(const P& p) const         { return input(p) == val; }
 	bool equiv(const P& n, const P&) const { return input(n) == val; }
 
	// end of requirements

	const mln_value(I_)& val;

	level_t(const I_& input, const mln_value(I_)& val, const N_& nbh, O_& output)
	  : base_<I_,N_,O_>(input, nbh, output),
	    s(input.domain()),
	    val(val)
	{}
      };

      // Routines.

      template <typename I, typename N, typename O>
      bool level_(const Image<I>& input, const mln_value(I)& val, const Neighborhood<N>& nbh,
		  Image<O>& output, unsigned& nlabels)
      {
	typedef impl::level_t<I,N,O> F;
	F f(exact(input), val, exact(nbh), exact(output));
	canvas::labeling<F> run(f);
	nlabels = f.nlabels;
	return f.status;
      }

      // FIXME: Add fast versions.

      // FIXME (ADD)
      //
      template <typename I, typename N, typename O>
      bool level_(const Fast_Image<I>& input, const mln_value(I)& val, const Neighborhood<N>& nbh,
		  Image<O>& output, unsigned& nlabels)
      {
	typedef impl::level_t<I,N,O> F;
	F f(exact(input), val, exact(nbh), exact(output));
	canvas::labeling_fast<F> run(f);
	nlabels = f.nlabels;
	return f.status;
      }

      //
      //END FIXME (ADD)

    } // end of namespace mln::labeling::impl


    // Facade.

    template <typename I, typename N, typename O>
    bool level(const Image<I>& input, const mln_value(I)& val, const Neighborhood<N>& nbh,
	       Image<O>& output, unsigned& nlabels)
    {
      mln_precondition(exact(output).domain() == exact(input).domain());
      return impl::level_(exact(input), val, nbh, output, nlabels);
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::labeling

} // end of namespace mln


#endif // ! MLN_LABELING_LEVEL_HH
