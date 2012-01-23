// Copyright (C) 2007, 2008, 2009, 2012 EPITA Research and Development
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

#ifndef MLN_MORPHO_RD_HH
# define MLN_MORPHO_RD_HH

// FIXME: This file, as well its functions and classes, shall not
// contain capital letters.

/// \file
///
/// \todo Reform code!  Also go and see sandbox/theo/Rd.

# include <vector>
# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/trait/value_.hh>
# include <mln/data/fill.hh>
# include <mln/data/compare.hh>
# include <mln/util/ord.hh>


namespace mln
{

  namespace morpho
  {

    template <typename I, typename N>
    I
    Rd(const Image<I>& f, const Image<I>& g, const Neighborhood<N>& nbh);


# ifndef MLN_INCLUDE_ONLY

    namespace impl
    {

      template <typename I>
      inline
      std::vector<unsigned> compute_histo(const I& ima)
      {
	std::vector<unsigned> h(256, 0);
	mln_piter(I) p(ima.domain());
	for_all(p)
	  ++h[ima(p)];
	return h;
      }

      template <typename I>
      inline
      std::vector<mln_psite(I)> histo_reverse_sort(const I& ima)
      {
	std::vector<unsigned> h = compute_histo(ima);
	// preparing output data
	std::vector<int> loc(256);
	loc[255] = 0;
	for (int l = 254; l >= 0; --l)
	  loc[l] = loc[l+1] + h[l+1];
	std::vector<mln_psite(I)> vec(ima.domain().nsites());
	// storing output data
	mln_piter(I) p(ima.domain());
	for_all(p)
	  vec[loc[ima(p)]++] = p;
	return vec;
      }


      template <typename I, typename N>
      struct Rd
      {
	typedef mln_psite(I) point;
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

	inline
	Rd(const I& f, const I& g, const N& nbh)
	  : f(f), g(g), nbh(nbh),
	    o(f.domain()),
	    is_proc(f.domain()),
	    parent(f.domain())
	{
	  // init
	  data::fill(o, f);
	  S = histo_reverse_sort(g);
	  data::fill(is_proc, false); // FIXME: rm

	  // first pass
	  for (unsigned i = 0; i < S.size(); ++i)
	    {
	      point p = S[i];

	      make_set(p);
	      mln_niter(N) n(nbh, p);
	      for_all(n)
		{
		  if (f.domain().has(n))
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
		{
		  if (o(p) == mln_max(value))
		    o(p) = g(p);
		  else
		    o(p) = o(parent(p));
		}
	    }

	}

	inline
	bool is_proc__(const point& n, const point& p) const
	{
	  return g(n) > g(p) || (g(n) == g(p) && util::ord_strict(point(n), p));
	}

	inline
	void make_set(const point& p)
	{
	  parent(p) = p;
	}

	inline
	point find_root(const point& x)
	{
	  if (parent(x) == x)
	    return x;
	  else
	    return parent(x) = find_root(parent(x));
	}

	inline
	bool equiv(const point& r, const point& p)
	{
	  return g(r) == g(p) || g(p) >= o(r);
	}

	inline
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
    inline
    I
    Rd(const Image<I>& f, const Image<I>& g, const Neighborhood<N>& nbh)
    {
      assert(f <= g);
      impl::Rd<I,N> run(exact(f), exact(g), exact(nbh));
      return run.o;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // ! MLN_MORPHO_RD_HH
