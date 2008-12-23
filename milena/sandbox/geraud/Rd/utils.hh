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

#ifndef	MLN_MORPHO_RD_UTILS_HH
# define MLN_MORPHO_RD_UTILS_HH

# include <vector>

# include <mln/core/concept/image.hh>
# include <mln/core/routine/duplicate.hh>

# include <mln/util/ord.hh>

# include <mln/data/fill_with_value.hh>
# include <mln/data/paste.hh>
# include <mln/level/compare.hh>



namespace mln
{

  namespace morpho
  {

    namespace Rd
    {


      template <typename T>
      T min(T v1, T v2)
      {
	return v1 < v2 ? v1 : v2;
      }


      template <typename I>
      I minimun(const I& ima1, const I& ima2)
      {
	mln_precondition(ima1.has_data() && ima2.has_data());
	mln_precondition(ima1.domain() == ima2.domain());
	I out(ima1.domain());
	mln_piter(I) p(ima1.domain());
	for_all(p)
	  out(p) = ima1(p) < ima2(p) ? ima1(p) : ima2(p);
	return out;
      }


      template <typename I, typename P, typename N>
      mln_value(I) max_N(const I& ima, const P& p, const N& nbh)
      {
	mln_value(I) v = ima(p);
	mln_niter(N) n(nbh, p);
	for_all(n)
	  if (ima.has(n) && ima(n) > v)
	    v = ima(n);
	return v;
      }


      template <typename I, typename P, typename N>
      mln_value(I) max_Nminus(const I& ima, const P& p, const N& nbh)
      {
	mln_value(I) v = ima(p);
	mln_niter(N) n(nbh, p);
	for_all(n)
	  if (ima.has(n) &&
	      util::ord_strict(p.to_site(), n.to_site()) &&
	      ima(n) > v)
	    v = ima(n);
	return v;
      }

      template <typename I, typename P, typename N>
      mln_value(I) max_Nplus(const I& ima, const P& p, const N& nbh)
      {
	mln_value(I) v = ima(p);
	mln_niter(N) n(nbh, p);
	for_all(n)
	  if (ima.has(n) &&
	      util::ord_strict(n.to_site(), p.to_site()) &&
	      ima(n) > v)
	    v = ima(n);
	return v;
      }


      template <typename I>
      std::vector<unsigned> compute_histo(const I& ima)
      {
	std::vector<unsigned> h(256, 0);
	mln_piter(I) p(ima.domain());
	for_all(p)
	  ++h[ima(p)];
	return h;
      }


//       template <typename I>
//       std::vector<mln_site(I)> histo_sort(const I& ima)
//       {
// 	std::vector<unsigned> h = compute_histo(ima);
// 	// preparing output data
// 	std::vector<int> loc(256);
// 	loc[0] = 0;
// 	for (int l = 1; l < 256; ++l)
// 	  loc[l] = loc[l-1] + h[l-1];
// 	std::vector<mln_site(I)> vec(ima.nsites());
// 	// storing output data
// 	mln_piter(I) p(ima.domain());
// 	for_all(p)
// 	  vec[loc[ima(p)]++] = p;
// 	return vec;
//       }


      template <typename I>
      std::vector<mln_site(I)> histo_reverse_sort(const I& ima)
      {
	std::vector<unsigned> h = compute_histo(ima);
	// preparing output data
	std::vector<int> loc(256);
	loc[255] = 0;
	for (int l = 254; l >= 0; --l)
	  loc[l] = loc[l+1] + h[l+1];
	std::vector<mln_site(I)> vec(ima.nsites());
	// storing output data
	mln_piter(I) p(ima.domain());
	for_all(p)
	  vec[loc[ima(p)]++] = p;
	return vec;
      }



      template <typename I, typename N>
      struct regional_maxima_t
      {
	typedef mln_site(I) point;
	typedef mln_ch_value(I, bool)  image_bool;
	typedef mln_ch_value(I, point) image_point;

	// in:
	I f;
	N nbh;

	// out:
	I o;

	// aux:
	std::vector<point> S;
	image_bool  deja_vu;
	image_bool  attr;
	image_point parent;

	regional_maxima_t(const I& f, const N& nbh)
	  : f(f), nbh(nbh)
	{
	  initialize(o, f);
	  initialize(parent, f);
	  initialize(attr, f);
	  initialize(deja_vu, f);
	
	  // init

	  data::fill_with_value(deja_vu, false);
	  S = histo_reverse_sort(f);

	  // first pass

	  for (unsigned i = 0; i < S.size(); ++i)
	    {
	      point p = S[i];
	    
	      make_set(p);
	      mln_niter(N) n(nbh, p);
	      for_all(n)
		if (f.has(n) && deja_vu(n))
		  {
		    if (f(n) == f(p))
		      do_union(n, p);
		    else // f(n) > f(p)
		      attr(p) = false;
		  }
	      deja_vu(p) = true;
	    }

	  // second pass

	  const mln_value(I) zero = 0;	  
	  for (int i = S.size() - 1; i >= 0; --i)
	    {
	      point p = S[i];
	      if (parent(p) == p)
		o(p) = attr(p) ? f(p) : zero;
	      else
		o(p) = o(parent(p));
	    }
	}
 
	void make_set(const point& p)
	{
	  parent(p) = p;
	  attr(p) = true;
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
	      parent(r) = p;
	      attr(p) = attr(p) && attr(r);
	    }
	}
     
      };


      template <typename I, typename N>
      I
      regional_maxima(const I& f, const N& nbh)
      {
	regional_maxima_t<I, N> run(f, nbh);
	return run.o;
      }


    } // end of namespace mln::morpho::Rd

  } // end of namespace mln::morpho

} // end of namespace mln


#endif // ! MLN_MORPHO_RD_UTILS_HH
