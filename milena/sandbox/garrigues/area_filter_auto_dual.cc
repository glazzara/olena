// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_AREA_FILTER_HH
# define MLN_AREA_FILTER_HH

# include <mln/core/image/image2d.hh>
# include <mln/data/fill.hh>
# include <mln/literal/zero.hh>
# include <mln/convert/to_upper_window.hh>
# include <mln/accu/count.hh>
# include <mln/util/pix.hh>

# include <mln/core/alias/neighb2d.hh>

# include <mln/io/pbm/save.hh>
# include <mln/io/pbm/load.hh>

namespace mln
{


  template <typename I>
  static inline
  mln_psite(I)
  find_root(I& parent,
	    const mln_psite(I)& x)
  {
    if (parent(x) == x)
      return x;
    else
      return parent(x) = find_root(parent, parent(x));
  }

  template <typename I, typename N, typename F, typename O>
  void
  algebraic_union_find(const Image<I>& input_,
		       const Neighborhood<N>& nbh_,
		       F& f,
		       Image<O>& output_)
  {
    trace::entering("canvas::morpho::algebraic_union_find");

    // FIXME: Tests?

    typedef typename F::S S;
    typedef typename F::A A;

    const I& input = exact(input_);
    const N& nbh = exact(nbh_);
    O& output = exact(output_);

    mln_precondition(output.domain() == input.domain());

    // Local type.
    typedef mln_psite(I) P;

    // Auxiliary data.
    mln_ch_value(O, bool)  deja_vu;
    mln_ch_value(O, P)     parent;
    mln_ch_value(O, A)     data;

    // init
    {
      initialize(deja_vu, input);
      mln::data::fill(deja_vu, false);
      initialize(parent, input);
      initialize(data, input);
      f.init(); // init required.
    }

    // first pass
    {
      mln_fwd_piter(I) p(input.domain());
      mln_niter(N) n(nbh, p);
      for_all(p)
      {

	if (input(p) == true)
	  // Body for foreground
	{
	  // Make set.
	  parent(p) = p;
	  data(p).take_as_init(make::pix(input, p));

	  for_all(n)
	    if (input.domain().has(n) && input(n) && deja_vu(n))
	    {
	      //do_union(n, p);
	      P r = find_root(parent, n);
	      if (r != p)
	      {
		if (input(r) == input(p) || f.is_active(data(r))) // Equiv(r, p)
		  // Either a flat zone or the component of r is still growing.
		{
		  // p is now the new root.
		  data(p).take(data(r));
		  parent(r) = p;
		}
		else
		  // Stop the growing connected component.
		  f.inactivate(data(p));
	      }
	    }
	  deja_vu(p) = true;
	}
	else
	  // Body for background
	{
	  // Make set.
	  parent(p) = p;
	  data(p).take_as_init(make::pix(input, p));

	  for_all(n)
	    if (input.domain().has(n) && !input(n) && deja_vu(n))
	    {
	      //do_union(n, p);
	      P r = find_root(parent, n);
	      if (r != p)
	      {
		if (input(r) == input(p) || f.is_active(data(r))) // Equiv(r, p)
		  // Either a flat zone or the component of r is still growing.
		{
		  data(p).take(data(r));
		  parent(r) = p;
		}
		else
		  f.inactivate(data(p));
	      }
	    }
	  deja_vu(p) = true;

	}


      }
    }

    // second pass
    {
      mln_bkd_piter(I) p(input.domain());
      for_all(p)
	if (input(p)) // Foreground
	  if (parent(p) == p)
	    output(p) = !f.is_active(data(p));
	  else
	    output(p) = output(parent(p));
	else          // Background.
	  if (parent(p) == p)
	    output(p) = f.is_active(data(p));
	  else
	    output(p) = output(parent(p));
    }

    trace::exiting("canvas::morpho::algebraic_union_find");
  }


  struct area_filter_t
  {
    // requirements from mln::canvas::morpho::algebraic_union_find

    typedef image2d<bool> I;
    typedef util::pix<I> pix_t;
    typedef accu::count<pix_t> A;
    typedef mln_psite_(I) P;
    typedef p_array<P> S;

    mln_result_(A) lambda;

    inline
    void init()
    {
    }

    inline
    bool is_active(const A& attr) const
    {
      return attr.to_result() < lambda;
    }

    inline
    void inactivate(A& attr)
    {
      attr.set_value(lambda);
    }

    // end of requirements

    inline
    area_filter_t(const image2d<bool>& input, mln_result_(A) lambda)
      : lambda(lambda)
    {
    }

  };



  template <typename N>
  inline
  void area_filter(const image2d<bool>& input,
		   const Neighborhood<N>& nbh, unsigned lambda,
		   image2d<bool>& output)
  {
    typedef area_filter_t F;
    F f(input, lambda);
    algebraic_union_find(input, nbh, f, output);
  }

} // end of namespace mln

int main(int argc, char** argv)
{
  using namespace mln;
  image2d<bool> input;

  if (argc < 2)
  {
    std::cerr << "Usage: " << argv[0] << " in.pbm area out.pbm" << std::endl;
    return 1;
  }

  io::pbm::load(input, argv[1]);


  image2d<bool> output1(input.domain());
  image2d<bool> output2(input.domain());
  image2d<bool> output3(input.domain());
  image2d<bool> output4(input.domain());

  area_filter(input,   c4(), atoi(argv[2]), output1);
  area_filter(output1, c4(), atoi(argv[2]), output2);
  area_filter(output2, c4(), atoi(argv[2]), output3);
  area_filter(output3, c4(), atoi(argv[2]), output4);

  io::pbm::save(output1, "out1.pgm");
  io::pbm::save(output2, "out2.pgm");
  io::pbm::save(output3, "out3.pgm");
  io::pbm::save(output3, "out4.pgm");
}

#endif // ! MLN_AREA_FILTER_HH
