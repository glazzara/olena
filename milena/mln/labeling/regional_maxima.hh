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

#ifndef MLN_LABELING_REGIONAL_MAXIMA_HH
# define MLN_LABELING_REGIONAL_MAXIMA_HH

/*! \file mln/labeling/regional_maxima.hh
 *
 * \brief Connected component labeling of the regional maxima of an
 * image.
 */

# include <mln/labeling/base.hh>
# include <mln/level/sort_points.hh>


namespace mln
{

  namespace labeling
  {

    /*! Connected component labeling of the regional maxima of an
     * image.
     *
     * \param[in]  input The input image.
     * \param[in]  nbh The neighborhood to consider.
     * \param[out] output The label image.
     *
     * \return The number of labels.
     */
    template <typename I, typename N, typename O>
    bool regional_maxima(const Image<I>& input, const Neighborhood<N>& nbh,
			 Image<O>& output, unsigned& nlabels);



# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      template <typename I_, typename N_, typename O_>
      struct regional_maxima_ : base_<I_,N_,O_>
      {
	typedef mln_point(I_) P;
	
	// requirements from mln::canvas::labeling:

	typedef vec_p<P> S;
	S s;

 	void init()                              { mln::level::fill(this->output, 0);
	                                           level::fill(attr, true); }
	bool labels(const P& p) const            { return attr(p);  }
	bool equiv(const P& n, const P& p) const { return input(n) == input(p); }
	void do_no_union(const P& n, const P& p) { mln_invariant(input(n) > input(p));
	                                           attr(p) = false; }
	void merge_attr(const P& r, const P& p)  { attr(p) = attr(p) && attr(r); }

	// end of requirements

	mln_ch_value(O_, bool) attr;

	regional_maxima_(const I_& input, const N_& nbh, O_& output)
	  : base_<I_,N_,O_>(input, nbh, output),
	    s(level::sort_points_decreasing(input)),
	    attr(output.domain())
	{
	}
      };

    } // end of namespace mln::labeling::impl


    // Facade.

    template <typename I, typename N, typename O>
    bool regional_maxima(const Image<I>& input, const Neighborhood<N>& nbh,
			 Image<O>& output, unsigned& nlabels)
    {
      mln_precondition(exact(output).domain() == exact(input).domain());
      typedef impl::regional_maxima_<I,N,O> F;
      F f(exact(input), exact(nbh), exact(output));
      canvas::labeling<F> run(f);
      nlabels = f.nlabels;
      return f.status;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::labeling

} // end of namespace mln


#endif // ! MLN_LABELING_REGIONAL_MAXIMA_HH
