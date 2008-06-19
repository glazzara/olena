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

#ifndef SANDBOX_MLN_LABELING_LEVEL_HH
# define SANDBOX_MLN_LABELING_LEVEL_HH

/*! \file sandbox/duhamel/labeling_level.hh
 *
 * \brief Connected component labeling of the image objects at a given
 * level.
 */

# include <mln/labeling/base.hh>
# include <mln/debug/println.hh>
# include <mln/core/window2d.hh>
# include <mln/convert/to_window.hh>
# include <mln/core/concept/dpoint.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/core/window.hh>
# include <mln/pw/image.hh>
# include <mln/pw/cst.hh>
# include <mln/metal/is_a.hh>



namespace mln
{

  namespace convert
  {
    template <typename N>
    window<mln_dpoint(N)> to_upper_window(const Neighborhood<N>& nbh_)
    {
      const N& nbh = exact(nbh_);
      typedef mln_dpoint(N) D;
      typedef mln_point(D) P;
      window<D> win;
      mln_niter(N) n(nbh, P::zero);
      for_all(n)
	if (n > P::zero)
	  win.insert(n - P::zero);
      return win;
    }

  } // end of namespace convert

  template <typename F>
  struct labeling_fast_try2
  {
    F& f;
    
    typedef typename F::I I;
    typedef typename F::N N;
    typedef typename F::O O;
    
    // aux:
    mln_ch_value(O, unsigned) parent;
    
    labeling_fast_try2(F& f)
      : f(f),
	parent(f.output.domain(), f.input.border())
    {
      run();
    }
    
    void run()
    {
//       std::cout << "fast"
// 		<< std::endl;
      // init
      {
	f.nlabels = 0;
	f.init();
      }
      // first pass
      {
	mln_bkd_pixter(const I) p(f.input);
	//	mln_nixter(const I, N) n(p, f.nbh);

	typedef window<mln_dpsite(I)> W;
	W win = convert::to_upper_window(f.nbh);
	mln_qixter(const I, W) n(p, win);
	
	for_all(p) if (f.handles(p))
	  {
	    make_set(p);
	    for_all(n)
	      if (f.equiv(n, p))
		do_union(n, p);
	      else
		f.do_no_union(n, p);
	  }
      }
      
      // second pass
      {
	mln_fwd_pixter(const I) p(f.input);
	for_all(p) if (f.handles(p))
	  {
	    if (is_root(p))
	      {
		if (f.labels(p))
		  {
		    if (f.nlabels == mln_max(mln_value(O)))
		      {
			f.status = false;
			return;
		      }
		    f.output[p] = ++f.nlabels;
		  }
	      }
	    else
	      f.output[p] = f.output[parent[p]];
	  }
	f.status = true;
      }
      
    } // end of run()
    
    void make_set(unsigned p)
    {
      parent[p] = p;
      f.init_attr(p);
    }
    
    bool is_root(unsigned p) const
    {
      return parent[p] == p;
    }
    
    unsigned find_root(unsigned x)
    {
      if (parent[x] == x)
	return x;
      else
	return parent[x] = find_root(parent[x]);
    }
    
    void do_union(unsigned n, unsigned p)
    {
      unsigned r = find_root(n);
      if (r != p)
	{
	  parent[r] = p;
	  f.merge_attr(r, p);
	}
    }
    
  };
  
  
  
  template <typename I_, typename N_, typename O_>
  struct level_fast_t : labeling::impl::base_fast_<I_,N_,O_>
  {
    typedef mln_point(I_) P;
    
    // 	typedef mln_pset(I_) S;
    // 	const S& s;
    
    void init()                            { level::fill(this->output, 0); }
    bool handles(unsigned p) const         { return this->input[p] == val; }
    bool equiv(unsigned n, unsigned) const { return this->input[n] == val; }

    const mln_value(I_)& val;

    level_fast_t(const I_& input, const mln_value(I_)& val, const N_& nbh, O_& output)
      : labeling::impl::base_fast_<I_,N_,O_>(input, nbh, output),
	// 	    s(input.domain()),
	val(val)
    {}
  };


  template <typename I, typename N, typename O>
  bool labeling_level_fast(const Fast_Image<I>& input, const mln_value(I)& val, const Neighborhood<N>& nbh,
			   Fast_Image<O>& output, unsigned& nlabels)
  {
    typedef level_fast_t<I,N,O> F;
    F f(exact(input), val, exact(nbh), exact(output));
    labeling_fast_try2<F> run(f);
    nlabels = f.nlabels;
    return f.status;
  }

} // end of namespace mln


#endif // ! SANDBOX_MLN_LABELING_LEVEL_HH
