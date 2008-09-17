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

#ifndef	MLN_MORPHO_RD_UNION_FIND_HH
# define MLN_MORPHO_RD_UNION_FIND_HH

# include "utils.hh"


namespace mln
{

  namespace morpho
  {

    namespace Rd
    {


      template <typename I, typename N>
      struct union_find_t
      {
	typedef mln_point(I) point;
	typedef mln_value(I) value;

	// in:
	const I f, g;
	N nbh;

	// out:
	I o;

	// aux:
	std::vector<point> S;
	// was: I data;
	mln_ch_value(I, bool) isproc;
	mln_ch_value(I, point) parent;

	union_find_t(const I& f, const I& g, const N& nbh)
	  : f(f), g(g), nbh(nbh)
	{
	  f.name_it("f");
	  g.name_it("g");
	  initialize(o, f);
	  o.name_it("o");
	  initialize(parent, f);
	  parent.name_it("parent");
	  initialize(isproc, f);
	  isproc.name_it("isproc");
	  // was: initialize(data, f);
	
	  // init

	  level::fill(isproc, false);
	  S = histo_reverse_sort(g);
	  level::paste(f, o); // new: replace make_set(p) { data(p) = f(p) }

	  // first pass

	  for (unsigned i = 0; i < S.size(); ++i)
	    {
	      point p = S[i];
	    
	      make_set(p);
	      mln_niter(N) n(nbh, p);
	      for_all(n)
		{
		  if (f.has(n))
		    mln_invariant(isproc(n) == is_proc(n, p));
		  if (f.has(n) and isproc(n))
		    do_union(n, p);
		}
	      isproc(p) = true;
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
	    }

	  print_counts();

	}
 
	bool is_proc(const point& n, const point& p) const
	{
	  return g(n) > g(p) or (g(n) == g(p) and n < p);
	}

	void make_set(const point& p)
	{
	  parent(p) = p;
	  // was: data(p) = f(p);
	  // now: in "initialization"
	}

	point find_root(const point& x)
	{
	  if (parent(x) == x)
	    return x;
	  else
	    return parent(x) = find_root(parent(x));
	}

	void do_union(const point& n, const point& p)
	{
	  point r = find_root(n);
	  if (r != p)
	    {
	      // NEW: o replaces data

	      if (g(r) == g(p) or g(p) >= o(r)) // equiv test
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


      template <typename I, typename N>
      I union_find(const I& f, const I& g, const N& nbh)
      {
	mln_precondition(f <= g);
	union_find_t<I, N> run(f, g, nbh);
	return run.o;
      }


    } // end of namespace mln::morpho::Rd

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // ! MLN_MORPHO_RD_UNION_FIND_HH
