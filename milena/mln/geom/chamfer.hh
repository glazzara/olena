// Copyright (C) 2007, 2008, 2009, 2011, 2012 EPITA Research and
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

#ifndef MLN_GEOM_CHAMFER_HH
# define MLN_GEOM_CHAMFER_HH

/// \file
///
/// Connected component chamfer of the image objects.

# include <mln/canvas/chamfer.hh>


namespace mln
{

  namespace geom
  {

    /*! \brief Apply chamfer algorithm to a binary image.

      \ingroup mlngeom
     */
    template <typename I, typename W>
    mln_ch_value(I, unsigned)
    chamfer(const Image<I>& input_, const W& w_win_,
	    unsigned max = mln_max(unsigned));


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      // Functors.

      template <typename I_, typename W_>
      struct chamfer_t
      {
	typedef I_ I;
	typedef mln_ch_value(I, unsigned) O;
	typedef W_ W;
	typedef mln_site(I_) P;

	// requirements from mln::canvas::chamfer:

	const I& input;
	const W& win;

	O output;
	bool status;
	unsigned max;

	void fill_output()
	{
	  mln_piter(O) p(output.domain());
	  for_all(p)
	  {
	    if (input(p))
	      output(p) = 0u;
	    else
	      output(p) = max;
	  }
	}

	inline
	void init()                            { initialize(output, exact(input));
						 fill_output();
						 // Was :
						 // data::fill((output | (input | pw::cst(true)).domain()).rw(), 0);
						 // data::fill((output | (input | pw::cst(false)).domain()).rw(), max);
	}
	inline
	bool handles(const P& p) const         { return input(p) == false; }

	// end of requirements

	inline
	chamfer_t(const I_& input, const W_& win, unsigned max)
	  : input (input),
	    win (win),
	    max (max)
	{}
      };

    } // end of namespace mln::geom::impl



    // Facade.

    template <typename I, typename W>
    mln_ch_value(I, unsigned)
    chamfer(const Image<I>& input_, const W& w_win_,
	    unsigned max)
    {
      mln_trace("mln::geom::chamfer");
      // FIXME: check that input_ is binary.

      const I& input = exact(input_);
      const W& w_win = exact(w_win_);

      typedef impl::chamfer_t<I, W> F;

      F f(input, w_win, max);
      canvas::chamfer<F> run(f);

      return f.output;
    }

#endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::geom

}  // end of namespace mln

#endif // ! MLN_GEOM_CHAMFER_HH
