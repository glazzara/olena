// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

/// \file doc/benchmark/canvas.cc
///
/// Test on mln::labeling::value.

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/value/int_u8.hh>
#include <mln/io/pgm/load.hh>

#include <mln/labeling/level.hh>
#include <mln/util/timer.hh>


namespace mln
{


  namespace old_canvas
  {

    template <typename F>
    struct labeling
    {
      // Functor.
      F& f;

      typedef typename F::I I;
      typedef typename F::N N;
      typedef typename F::L L;
      typedef typename F::S S;

      // Local type.
      typedef mln_psite(I) psite;

      // Auxiliary data.
      mln_ch_value(I, bool)  deja_vu;
      mln_ch_value(I, psite) parent;

      // Output.
      mln_ch_value(I, L) output;
      L nlabels;
      bool status;

      // Ctor.
      labeling(F& f);

      void init();

      void pass_1();

      void pass_2();


      // Auxiliary methods.

      void make_set(const psite& p);

      bool is_root(const psite& p) const;

      psite find_root(const psite& x);

      void do_union(const psite& n, const psite& p);

    };


    template <typename F>
    labeling<F>::labeling(F& f)
      : f(f)
    {
      mln_trace("canvas::labeling");

      init();
      f.init(); // Client initialization.
      pass_1();
      pass_2();

    }

    template <typename F>
    void
    labeling<F>::init()
    {
      initialize(deja_vu, f.input);
      mln::data::fill(deja_vu, false);
      initialize(parent, f.input);
      initialize(output, f.input);
      mln::data::fill(output, L(literal::zero));
      nlabels = 0;
    }

    template <typename F>
    void
    labeling<F>::pass_1()
    {
      mln_fwd_piter(S) p(f.s);
      mln_niter(N) n(f.nbh, p);
      for_all(p) if (f.handles(p))
	{
	  make_set(p);
	  for_all(n)
	    if (f.input.domain().has(n) && deja_vu(n))
	      {
		if (f.equiv(n, p))
		  do_union(n, p);
		else
		  f.do_no_union(n, p);
	      }
	  deja_vu(p) = true;
	}
    }

    template <typename F>
    void
    labeling<F>::pass_2()
    {
      mln_bkd_piter(S) p(f.s);
      for_all(p) if (f.handles(p))
	{
	  if (is_root(p))
	    {
	      if (f.labels(p))
		{
		  if (nlabels == mln_max(L))
		    {
		      status = false;
		      return;
		    }
		  output(p) = ++nlabels;
		}
	    }
	  else
	    output(p) = output(parent(p));
	}
      status = true;
    }

    template <typename F>
    void
    labeling<F>::make_set(const psite& p)
    {
      parent(p) = p;
      f.init_attr(p);
    }

    template <typename F>
    bool
    labeling<F>::is_root(const psite& p) const
    {
      return parent(p) == p;
    }

    template <typename F>
    typename labeling<F>::psite
    labeling<F>::find_root(const psite& x)
    {
      if (parent(x) == x)
	return x;
      else
	return parent(x) = find_root(parent(x));
    }

    template <typename F>
    void
    labeling<F>::do_union(const psite& n, const psite& p)
    {
      psite r = find_root(n);
      if (r != p)
	{
	  parent(r) = p;
	  f.merge_attr(r, p);
	}
    }


  } // end of namespace mln::old_canvas



  namespace old_labeling
  {


      template <typename I_, typename N_, typename L_>
      struct level_functor
      {
	typedef mln_psite(I_) P;

	// requirements from mln::canvas::labeling:

	typedef I_ I;
	typedef N_ N;
	typedef L_ L;
        typedef mln_pset(I) S;

	const I& input;
	const N& nbh;
        const S& s;

	bool handles(const P& p) const         { return input(p) == val; }
	bool equiv(const P& n, const P&) const { return input(n) == val; }

 	void init()                          {}
	bool labels(const P&) const          { return true; }
	void do_no_union(const P&, const P&) {}
	void init_attr(const P&)             {}
	void merge_attr(const P&, const P&)  {}

	// end of requirements

	const mln_value(I_)& val;

	level_functor(const I& input, const mln_value(I)& val, const N& nbh)
	  : input(input),
	    nbh(nbh),
	    s(input.domain()),
	    val(val)
	{}
      };



    template <typename I, typename N, typename L>
    mln_ch_value(I, L)
    level(const Image<I>& input, const mln_value(I)& val, const Neighborhood<N>& nbh,
	  L& nlabels)
    {
      mln_trace("labeling::value");

      typedef level_functor<I,N,L> F;
      F f(exact(input), val, exact(nbh));
      old_canvas::labeling<F> run(f);
	  
      nlabels = run.nlabels;
      
      return run.output;
    }

  } // end of namespace mln::old_labeling

} // end of namespace mln




int main()
{
  using namespace mln;
  using value::int_u8;

  image2d<int_u8> lena;
  io::pgm::load(lena, "../../img/lena.pgm");

  {
    util::timer t;
    t.start();
    unsigned n;
    for (unsigned l = 0; l <= 255; ++l)
      old_labeling::value(lena, l, c4(), n);
    std::cout << "canvas as class: " << t.read() << std::endl;
  }

  {  
    util::timer t;
    t.start();
    unsigned n;
    for (unsigned l = 0; l <= 255; ++l)
      labeling::impl::generic::data(lena, l, c4(), n);
    std::cout << "canvas as proc.: " << t.read() << std::endl;
  }

}
