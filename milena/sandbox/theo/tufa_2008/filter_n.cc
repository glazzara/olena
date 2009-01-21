// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

/// \file sandbox/geraud/tufa/soluce.cc


#include <mln/core/image/image2d.hh>
#include <mln/value/int_u8.hh>
#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/debug/println.hh>

#include <mln/core/site_set/p_array.hh>
#include <mln/level/sort_psites.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/morpho/tree/data.hh>

#include <mln/accu/count.hh>
#include <mln/util/set.hh>

#include <mln/labeling/regional_minima.hh>
#include <mln/morpho/tree/compute_attribute_image.hh>
#include <mln/morpho/closing_area.hh>
#include <mln/data/fill.hh>
#include <mln/opt/at.hh>



namespace mln
{

  template <typename I>
  void println_par(const I& par)
  {
    int nr = par.nrows(), nc = par.ncols();
    for (int r = 0; r < nr; ++r)
      {
	for (int c = 0; c < nc; ++c)
	  if (opt::at(par, r,c) == point2d(r,c))
	    std::cout << "(    ) ";
	  else
	    std::cout << opt::at(par, r,c) << ' ';
	std::cout << std::endl;
      }
  }

  template <typename I>
  struct my_less_
  {
    const I& ima_;

    inline
    my_less_(const I& ima)
      : ima_(ima)
    {
    }

    inline
    bool operator()(const mln_site(I)& lhs,
		    const mln_site(I)& rhs) const
    {
      return util::ord_strict(ima_(lhs), ima_(rhs))
	|| (ima_(lhs) == ima_(rhs)
	    &&
	    util::ord_strict(lhs, rhs));
    }
  };


  template <typename I, typename S>
  p_array<mln_site(I)> my_sort_increasing(const I& ima, const S& s)
  {
    p_array<mln_site(I)> v;
    convert::from_to(s, v);
    std::sort(v.hook_std_vector_().begin(), v.hook_std_vector_().end(),
	      my_less_<I>(ima));
    return v;
  }



  // FIXME: HERE

  template <typename A, typename T, typename N>
  inline
  mln_concrete(typename T::function)
  get_attribute_image(const A& a, const T& t, const N& nbh,
		      unsigned n_objects,
		      bool echo = false)
  {
    typedef typename T::function I;


    unsigned n_regmins_f;
    mln_ch_value(I, unsigned) regmins_f = labeling::regional_minima(t.f(), nbh, n_regmins_f);
    if (echo)
      {
	debug::println("f =", t.f());
	debug::println("regmins(f) =", regmins_f);
	debug::println("par on nodes = ", t.parent_image() | t.nodes());
	std::cout << "n regmins(f) = " << n_regmins_f << std::endl
		  << std::endl;
      }
    if (n_objects >= n_regmins_f)
      {
	std::cout << "warning: number of expected objects is greater than number of regmins!" << std::endl;
	std::cout << "aborting..." << std::endl;
	return duplicate(t.f());
      }

    // Compute attribute on nodes.
    // ---------------------------


    mln_ch_value(I, A) acc;
    initialize(acc, t.f());


    mln_ch_value(I, unsigned) nchildren;
    initialize(nchildren, t.f());
    data::fill(nchildren, 0);

    {
      // Transmit "dynamic data" (state) of 'a' to every values of
      // 'acc'.  It is usually a no-op (so useless) except for a
      // few accumulators, e.g., for accu::rank which has the 'k'
      // attribute.
      data::fill(acc, a);
    }
    {
      // Initialize every attribute with the corresponding pixel.
      mln_piter(I) p(t.f().domain());
      for_all(p)
	acc(p).take_as_init(make::pix(t.f(), p));
    }
    {
      mln_fwd_piter(T) p(t.domain());
      // Propagate attribute from a site to its parent.
      for_all(p)
	if (! t.is_root(p))
	  {
	    acc(t.parent(p)).take(acc(p));
	    if (t.is_a_node(p))
	      ++nchildren(t.parent(p)); // so parent(p) is a node
	  }
      // Back-propagate attribute from a node to sites of its
      // component.  Below, p is a non-node component site and
      // parent(p) is a node, that is, the site representative of
      // the component p belongs to.
      for_all(p)
	if (! t.is_a_node(p))
	  {
	    mln_assertion(t.is_a_node(t.parent(p)));
	    acc(p) = acc(t.parent(p));
	  }
    }

    if (echo)
      debug::println("nchildren =", nchildren | t.nodes());

    typedef typename T::function I;
    mln_ch_value(I, mln_result(A)) attr;
    {
      // Change accumulator into its result.
      initialize(attr, acc);
      mln_piter(I) p(t.f().domain());
      for_all(p)
	if (t.is_a_node(p))
	  attr(p) = acc(p).to_result();
    }

    if (echo)
      debug::println("attr on nodes = ", attr | t.nodes());



    // ...
    // ---------------------------


    unsigned lambda = mln_max(unsigned);


    typedef p_array<mln_site(I)> S;
    S s = my_sort_increasing(attr, t.nodes());

    const typename T::parent_t& par = t.parent_image();

    unsigned
      count = n_regmins_f,
      less  = 0;
    mln_fwd_piter(S) p(s);
    for_all(p)
    {
      if (attr(p) < lambda && par(p) != p)
	{
	  mln_assertion(nchildren(par(p)) > 0);
	  --nchildren(par(p));
	  if (nchildren(par(p)) != 0)
	    {
	      if (count <= n_objects)
		{
		  ++less; // minus 1 object wrt the expected number!
		}
	      --count;
	      if (count == n_objects)
		{
		  lambda = attr(p) + 1;
		  std::cout << "lambda = " << lambda << std::endl
			    << std::endl;
		  // break; // Stop iterations.
		}
	    }
	}
    }

    if (less != 0)
      std::cerr << "WARNING: less objects (" << less << ") than expected..." << std::endl
		<< std::endl;

    if (echo)
      debug::println("nchildren =", nchildren | t.nodes());


    // Filtering.
    mln_concrete(I) g;
    {
      initialize(g, t.f());
      mln_bkd_piter(T) p(t.domain());
      for_all(p)
	if (t.is_a_node(p) && attr(p) >= lambda)
	  g(p) = t.f(p);
	else
	  g(p) = g(par(p));

      if (echo)
	debug::println("g =", g);
    }

    // Test!
    {
      mln_concrete(I) g_ref = morpho::closing_area(t.f(), nbh, lambda);
      if (echo)
	debug::println("g_ref =", g_ref);

      unsigned n_regmins_g_ref;
      mln_ch_value(I, unsigned) regmin_g = labeling::regional_minima(g_ref, nbh, n_regmins_g_ref);
      if (echo)
	std::cout << "n_regmins(g_ref) = " << n_regmins_g_ref << std::endl
		  << std::endl;

      if (g != g_ref)
	std::cerr << "OOPS: g DIFFERS FROM ref!" << std::endl
		  << std::endl;

      bool consistency = (n_regmins_g_ref + less == n_objects);
      if (consistency == false)
	std::cerr << "OOPS: INCONSISTENCY (BUG...)!" << std::endl
		  << std::endl;
    }

    return g;
  }


} // mln



void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pgm n output.pgm echo" << std::endl;
  std::cerr << "n: number of expected objects (n > 0)" << std::endl;
  std::cerr << "echo: 0 (silent) or 1 (verbose)" << std::endl;
  std::cerr << "filter" << std::endl;
  std::abort();
}


int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;

  if (argc != 5)
    usage(argv);

  typedef image2d<int_u8> I;
  I f;
  // input image
  io::pgm::load(f, argv[1]);

  // n
  int n = std::atoi(argv[2]);
  if (n <= 0)
    usage(argv);

  // echo
  int echo = std::atoi(argv[4]);
  if (echo != 0 && echo != 1)
    usage(argv);

  neighb2d nbh = c4();

  typedef p_array<point2d> S;
  S s = level::sort_psites_decreasing(f);

  // Children go towards lower levels so leafs are regional minima.
  // We get a min-tree so that we can perform morphological closings.

  typedef morpho::tree::data<I,S> tree_t;
  tree_t t(f, s, nbh);

  accu::count< util::pix<I> > attr;
  I g = get_attribute_image(attr, t, nbh, n, echo);
  io::pgm::save(g, argv[3]);
}
