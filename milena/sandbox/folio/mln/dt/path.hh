// Copyright (C) 2007 EPITA Research and Development Laboratory
//
// This file is part of the Milena Library.  This library is free
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

#ifndef MLN_DT_PATH_HH
# define MLN_DT_PATH_HH

# include "canvas_dt.hh"

namespace mln
{

  namespace dt
  {

    /*! Propagation using a single neighborhood (PSN).
     *
     * \param[in]  input_   The input image.
     * \param[in]  nbh      The chamfer window to use for distance calcul.
     * \param[in]  max      Max value of the output image.
     * \return              A distance map.
     *
     * \pre \p img has to be initialized.
     */
    template<typename I, typename N>
    mln_ch_value(I, mln_point(I))
    path(const Image<I>& input_, const N& nbh, unsigned max);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      // Generic functor.

      template <typename I_, typename N_>
      struct path_functor
      {
	typedef I_ I;
	typedef N_ N;

	const I& input;
	const N& nbh;
	unsigned max;

	path_functor(const I& input, const N& nbh, unsigned max)
	  : input(input),
	    nbh(nbh),
	    max(max)
	{}

	mln_ch_value(I, mln_point(I)) output;

	void init()
	{
	  initialize(output, input);
	}

	void init_in_for(const mln_point(I)& p)
	{
	  output(p) = p;
	}

	void run_in_for(const mln_point(I)& p, const mln_point(I)& n)
	{
	  output(n) = output(p);
	}
      };

    } // end of namespace mln::dt::impl


    // Facade.

    template<typename I, typename N>
    inline
    mln_ch_value(I, mln_point(I))
    path(const Image<I>& input, const N& nbh, unsigned max)
    {
      trace::entering("dt::path");
      mln_precondition(exact(input).is_valid());

      typedef impl::path_functor<I, N> F;
      F f(exact(input), nbh, max);
      mln::canvas::dt<F> call(f);

      trace::exiting("dt::path");
      return f.output;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::dt

} // end of namespace mln

#endif // ! MLN_DT_PATH_HH
