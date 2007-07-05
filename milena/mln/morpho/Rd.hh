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

#ifndef MLN_MORPHO_RD_HH
# define MLN_MORPHO_RD_HH

# include <vector>

# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>

# include <mln/value/props.hh>

# include <mln/level/fill.hh>
# include <mln/level/compare.hh>



namespace mln
{

  namespace morpho
  {

    template <typename I, typename N>
    I Rd(const Image<I>& f, const Image<I>& g, const Neighborhood<N>& nbh);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      template <typename I>
      std::vector<unsigned> compute_histo(const I& ima)
      {
	std::vector<unsigned> h(256, 0);
	mln_piter(I) p(ima.domain());
	for_all(p)
	  ++h[ima(p)];
	return h;
      }

      template <typename I>
      std::vector<mln_point(I)> histo_reverse_sort(const I& ima)
      {
	std::vector<unsigned> h = compute_histo(ima);
	// preparing output data
	std::vector<int> loc(256);
	loc[255] = 0;
	for (int l = 254; l >= 0; --l)
	  loc[l] = loc[l+1] + h[l+1];
	std::vector<mln_point(I)> vec(ima.domain().npoints());
	// storing output data
	mln_piter(I) p(ima.domain());
	for_all(p)
	  vec[loc[ima(p)]++] = p;
	return vec;
      }


      template <typename I, typename N>
      struct Rd
      {
	typedef mln_point(I) point;
	typedef mln_value(I) value;

	// in:
	const I& f;
	const I& g;
	const N& nbh;

	// out:
	I o;

	// aux:
	mln_ch_value(I, bool)  is_proc;
	mln_ch_value(I, point) parent;
	std::vector<point> S;

	Rd(const I& f, const I& g, const N& nbh)
	  : f(f), g(g), nbh(nbh),
	    o(f.domain()),
	    is_proc(f.domain()),
	    parent(f.domain())
	{
	  // init
	  level::fill(o, f);
	  S = histo_reverse_sort(g);
	  level::fill(is_proc, false); // FIXME: rm

	  // first pass
	  for (unsigned i = 0; i < S.size(); ++i)
	    {
	      point p = S[i];
	    
	      make_set(p);
	      mln_niter(N) n(nbh, p);
	      for_all(n)
		{
		  if (f.has(n))
		    assert(is_proc(n) == is_proc__(n, p));
		  if (f.has(n) && is_proc(n))
		    do_union(n, p);
		}
	      is_proc(p) = true;
	    }

	  // second pass
	  for (int i = S.size() - 1; i >= 0; --i)
	    {
	      point p = S[i];
	      if (parent(p) == p)
		if (o(p) == mln_max(value))
		  o(p) = g(p);
	      else
		o(p) = o(parent(p));
	      is_proc(p) = true;
	    }

	}
  
	bool is_proc__(const point& n, const point& p) const
	{
	  return g(n) > g(p) || (g(n) == g(p) && n < p);
	}

	void make_set(const point& p)
	{
	  parent(p) = p;
	}

	point find_root(const point& x)
	{
	  if (parent(x) == x)
	    return x;
	  else
	    return parent(x) = find_root(parent(x));
	}

	bool equiv(const point& r, const point& p)
	{
	  return g(r) == g(p) || g(p) >= o(r);
	}

	void do_union(const point& n, const point& p)
	{
	  point r = find_root(n);
	  if (r != p)
	    {
	      if (equiv(r, p))
		{
		  parent(r) = p;
		  if (o(r) > o(p))
		    o(p) = o(r); // increasing criterion
		}
	      else
		o(p) = mln_max(value);
	    }
	}
      
      };

    } // end of namespace mln::morpho::impl


    // facade

    template <typename I, typename N>
    I Rd(const Image<I>& f, const Image<I>& g, const Neighborhood<N>& nbh)
    {
      assert(f <= g);
      impl::Rd<I,N> run(exact(f), exact(g), exact(nbh));
      return run.o;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // ! MLN_MORPHO_RD_HH
