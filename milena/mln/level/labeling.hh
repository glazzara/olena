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

#ifndef MLN_LEVEL_LABELING_HH
# define MLN_LEVEL_LABELING_HH

# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/value/props.hh>
# include <mln/level/fill.hh>



namespace mln
{

  namespace level
  {

    template <typename I, typename N, typename O>
    void labeling(const Image<I>& input, const Neighborhood<N>& nbh, Image<O>& output);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      template <typename I, typename N, typename O>
      struct component_labeling
      {
	typedef mln_point(I) point;
	typedef mln_value(I) value;

	// in:
	const I& input;
	const N& nbh;

	// out:
	O& output;

	// aux:
	mln_ch_value(I, point) parent;
	mln_ch_value(I, bool)  deja_vu;
	mln_value(O) current_l;

	component_labeling(const I& input_, const N& nbh_, O& output_)
	  : // in
	    input(input_),
	    nbh(nbh_),
	    // out
	    output(output_),
	    // aux
	    parent(input.domain()),
	    deja_vu(input.domain())
	{
	}

	void run()
	{
	  // init
	  {
	    level::fill(deja_vu, false);
	    current_l = 0;
	    level::fill(output, 0);
	  }

	  // first pass
	  {
	    mln_bkd_piter(I) p(input.domain());
	    mln_niter(N) n(nbh, p);
	    for_all(p)
	      if (input(p))
		{
		  parent(p) = p;
		  for_all(n)
		    if (input.has(n) && input(n) && deja_vu(n))
		      {
			point r = find_root(n);
			if (r != p)
			  parent(r) = p;
		      }
		  deja_vu(p) = true;
		}
	  }

	  // second pass
	  {
	     mln_fwd_piter(I) p(input.domain());
	     for_all(p)
	       if (input(p))
		 if (parent(p) == p)
		   output(p) = ++current_l;
		 else
		   output(p) = output(parent(p));
	  }

	}

	point find_root(const point& x)
	{
	  if (parent(x) == x)
	    return x;
	  else
	    return parent(x) = find_root(parent(x));
	}
      
      }; // end of component_labeling



      template <typename I, typename N, typename O>
      void labeling(value::binary_kind,
		    const Image<I>& input, const Neighborhood<N>& nbh, Image<O>& output)
      {
	impl::component_labeling<I,N,O> f(exact(input), exact(nbh), exact(output));
	f.run();
      }

      /*
	template <typename K, typename I, typename N, typename O>
	void labeling(K,
	              const Image<I>& input, const Neighborhood<N>& nbh, Image<O>& output)
	{
	  impl::flat_labeling<I,N,O> f(exact(input), exact(nbh), exact(output));
	  f.run();
	}
      */


    } // end of namespace mln::level::impl


    // facade

    template <typename I, typename N, typename O>
    void labeling(const Image<I>& input, const Neighborhood<N>& nbh, Image<O>& output)
    {
      mln_assertion(exact(output).domain() == exact(input).domain());
      impl::labeling(mln_value_kind(I)(),
		     exact(input), exact(nbh), exact(output));
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::level

} // end of namespace mln


#endif // ! MLN_LEVEL_LABELING_HH
