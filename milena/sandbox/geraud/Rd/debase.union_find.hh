// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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

#ifndef	OLN_MORPHO_RD_UNION_FIND_HH
# define OLN_MORPHO_RD_UNION_FIND_HH

# include <oln/morpho/Rd/utils.hh>


namespace oln
{

  namespace morpho
  {

    namespace Rd
    {


      template <typename I, typename N>
      struct union_find_t
      {
	typedef oln_point(I) point;

	// in:
	I f, g;
	N nbh;

	// out:
	I o;

	// aux:
	std::vector<point> S;
	I data;
	oln_plain_value(I, bool) is_proc;
	oln_plain_value(I, point) parent;

	union_find_t(const I& f, const I& g, const N& nbh)
	  : f(f), g(g), nbh(nbh)
	{
	  prepare(o, with, f);
	  prepare(parent, with, f);
	  prepare(is_proc, with, f);
	  prepare(data, with, f);
	
	  // init

	  std::cout << "0 ";
	  data::fill(is_proc, false);
	  S = histo_reverse_sort(g);

	  // first pass

	  std::cout << "1 ";
	  for (unsigned i = 0; i < S.size(); ++i)
	    {
	      point p = S[i];
	    
	      make_set(p);
	      oln_niter(N) n(nbh, p);
	      for_all(n)
		if (f.has(n) and is_proc(n))
		  do_union(n, p);
	      is_proc(p) = true;
	    }

	  // second pass

	  std::cout << "2 ";
	  data::fill(is_proc, false);
	  for (int i = S.size() - 1; i >= 0; --i)
	    {
	      point p = S[i];
	      assert(is_proc(p) == false);
	      if (parent(p) == p)
		o(p) = data(p) == 255 ? g(p) : data(p);
	      else
		{
		  assert(is_proc(parent(p)) == true);
		  o(p) = o(parent(p));
		}
	      is_proc(p) = true;
	    }

	}
 
	void make_set(const point& p)
	{
	  parent(p) = p;
	  data(p) = f(p);
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
	  return g(r) == g(p) or g(p) >= data(r);
	}

	void do_union(const point& n, const point& p)
	{
	  point r = find_root(n);
	  if (r != p)
	    {
	      if (equiv(r, p))
		{
		  parent(r) = p;
		  if (data(r) > data(p))
		    data(p) = data(r); // increasing criterion
		}
	      else
		data(p) = 255;
	    }
	}
      
      };


      template <typename I, typename N>
      I union_find(const I& f, const I& g, const N& nbh)
      {
	precondition(f <= g);
	union_find_t<I, N> run(f, g, nbh);
	return run.o;
      }


    } // end of namespace oln::morpho::Rd

  } // end of namespace oln::morpho

} // end of namespace oln


#endif // ! OLN_MORPHO_RD_UNION_FIND_HH
