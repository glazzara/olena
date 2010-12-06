// Copyright (C) 2007, 2008, 2009, 2010 EPITA Research and Development
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

#include <iostream>

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/window2d.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>

#include <mln/value/int_u8.hh>

#include <mln/morpho/includes.hh>
#include <mln/morpho/general.hh>

#include <mln/accu/logic/lor.hh>
#include <mln/accu/logic/land.hh>
#include <mln/accu/logic/land_basic.hh>
#include <mln/accu/logic/lor_basic.hh>
#include <mln/accu/stat/max.hh>
#include <mln/accu/stat/min.hh>
#include <mln/accu/stat/max_h.hh>
#include <mln/accu/stat/min_h.hh>

#include "apps/bench/minus.hh"
#include "apps/bench/and_not.hh"

#include <mln/util/timer.hh>

#include "apps/data.hh"

// With hard-coded/inlined 4-c structuring element.
template <typename I>
mln_concrete(I)
gradient_spe_0(const mln::Image<I>& ima_)
{
  using namespace mln;
  typedef mln_value(I) V;
  typedef mln_psite(I) P;

  const I& ima = exact(ima_);

  mln_concrete(I) result;
  initialize(result, ima);

  mln_piter(I) p(result.domain());
  for_all(p)
  {
    // FIXME: Use one or two accu(s) instead?
    V sup = ima(p);
    V inf = ima(p);
    {
      P q(p.row() - 1, p.col());
      if (ima.has(q))
	{
	  if (ima(q) > sup) sup = ima(q);
	  if (ima(q) < inf) inf = ima(q);
	}
    }
    {
      P q(p.row(), p.col() - 1);
      if (ima.has(q))
	{
	  if (ima(q) > sup) sup = ima(q);
	  if (ima(q) < inf) inf = ima(q);
	}
    }
    {
      P q(p.row(), p.col() + 1);
      if (ima.has(q))
	{
	  if (ima(q) > sup) sup = ima(q);
	  if (ima(q) < inf) inf = ima(q);
	}
    }
    {
      P q(p.row() + 1, p.col());
      if (ima.has(q))
	{
	  if (ima(q) > sup) sup = ima(q);
	  if (ima(q) < inf) inf = ima(q);
	}
    }
    result(p) = sup - inf;
  }
  return result;
}

template <typename I, typename W>
mln_concrete(I)
gradient_spe_1(const mln::Image<I>& ima_, const mln::Window<W>& win_)
{
  using namespace mln;
  typedef mln_value(I) V;

  const I& ima = exact(ima_);
  const W& win = exact(win_);

  mln_concrete(I) result;
  initialize(result, ima);

  mln_piter(I) p(result.domain());
  mln_qiter(W) q(win, p);
  for_all(p)
  {
    // FIXME: Use one or two accu(s) instead?
    V sup = mln_min(V);
    V inf = mln_max(V);
    for_all(q)
      if (ima.has(q))
	{
	  if (ima(q) > sup)
	    sup = ima(q);
	  if (ima(q) < inf)
	    inf = ima(q);
	}
    result(p) = sup - inf;
  }
  return result;
}

template <typename I, typename W>
mln_concrete(I)
gradient_spe_2(const mln::Image<I>& ima_, const mln::Window<W>& win_)
{
  using namespace mln;
  typedef mln_value(I) V;

  const I& ima = exact(ima_);
  const W& win = exact(win_);

  typedef mln_concrete(I) O;
  O result;
  initialize(result, ima);

  mln_pixter(const I) pi(ima);
  mln_pixter(O) po(result);
  // FIXME: The arguments of a qixter (p, win) are swapped compared to
  // a qiter (win, p).  Make this uniform?
  mln_qixter(const I, W) q(pi, win);
  for_all_2(pi, po)
  {
    // FIXME: Use one or two accu(s) instead?
    V sup = mln_min(V);
    V inf = mln_max(V);
    for_all(q)
    {
      if (q.val() > sup)
	sup = q.val();
      if (q.val() < inf)
	inf = q.val();
    }
    po.val() = sup - inf;
  }
  return result;
}

template <typename I, typename W>
mln_concrete(I)
gradient_spe_3(const mln::Image<I>& ima_, const mln::Window<W>& win_)
{
  using namespace mln;
  typedef mln_value(I) V;

  const I& ima = exact(ima_);
  const W& win = exact(win_);

  typedef mln_concrete(I) O;
  O result;
  initialize(result, ima);

  mln_pixter(const I) pi(ima);
  mln_pixter(O) po(result);
  // FIXME: The arguments of a qixter (p, win) are swapped compared to
  // a qiter (win, p).  Make this uniform?
  mln_qixter(const I, W) q(pi, win);
  for_all_2(pi, po)
  {
    // FIXME: Use one or two accu(s) instead?
    V sup = mln_min(V);
    V inf = mln_max(V);
    bool global_sup_reached_p = false;
    bool global_inf_reached_p = false;
    for_all(q)
    {
      if (!global_sup_reached_p && q.val() > sup)
	{
	  sup = q.val();
	  if (sup == mln_max(V))
	    {
	      // Global sup reached.
	      if (global_inf_reached_p)
		break;
	      global_sup_reached_p = true;
	    }
	}
      if (!global_inf_reached_p && q.val() < inf)
	{
	  inf = q.val();
	  if (inf == mln_min(V))
	    {
	      // Global inf reached.
	      if (global_sup_reached_p)
		break;
	      global_inf_reached_p = true;
	    }
	}
    }
    po.val() = sup - inf;
  }
  return result;
}


// FIXME: Unify accumulators and_not and minus?

namespace mln
{
  namespace accu
  {
    namespace meta
    {
      /// Accumulator for morphological gradient on sets.
      typedef and_not<meta::logic::lor_basic, meta::logic::land_basic> lor_basic_minus_land_basic;
      /// Accumulator for morphological gradient on functions.
      typedef minus<meta::stat::max, meta::stat::min> max_minus_min;

      /// Incremental accumulator for morphological gradient on sets.
      typedef meta::and_not<meta::logic::lor, meta::logic::land> lor_minus_land;
      /// Incremental accumulator for morphological gradient on functions.
      typedef meta::minus<meta::stat::max_h, meta::stat::min_h> max_h_minus_min_h;
    } // End of namespace mln::accu::meta.

  } // End of namespace mln::accu.


namespace morpho
{

  struct gradient_op
  {
    template <typename I>
    mln_morpho_select_accu(I, lor_basic_minus_land_basic, max_minus_min)
    accu(const Image<I>&) const
    {
      mln_morpho_select_accu(I, lor_basic_minus_land_basic, max_minus_min) tmp;
      return tmp;
    }

    template <typename I>
    mln_morpho_select_accu(I, lor_minus_land, max_h_minus_min_h)
    accu_incr(const Image<I>&) const
    {
      mln_morpho_select_accu(I, lor_minus_land, max_h_minus_min_h) tmp;
      return tmp;
    }

    // FIXME: Which neutral should we choose?
    template <typename I>
    mln_value(I) neutral(const Image<I>&) const
    {
      return internal::neutral<I>::infimum();
    }
  };

  template <typename I, typename W>
  inline
  mln_concrete(I)
  gradient_spe_4(const Image<I>& input, const Window<W>& win)
  {
    mln_trace("morpho::dilation");
    mln_precondition(exact(input).is_valid());
    mln_precondition(! exact(win).is_empty());

    mln_concrete(I) output = general(gradient_op(), input, win);

    mln_postcondition(test::positive(output));
    return output;
  }

} // End of namespace mln::morpho.

} // End of namespace mln.



int main()
{
  using namespace mln;
  using value::int_u8;

  border::thickness = 1;
  image2d<int_u8> lena;
  io::pgm::load(lena, MLN_IMG_DIR "/lena.pgm");

  image2d<int_u8> g;
  util::timer t;

  t.start();
  g = gradient_spe_0(lena);
  t.stop();
  std::cout << t.read() << std::endl;
  io::pgm::save(g, "gradient-spe-lena-out-0.pgm");

  t.restart();
  g = gradient_spe_1(lena, win_c4p());
  t.stop();
  std::cout << t.read() << std::endl;
  io::pgm::save(g, "gradient-spe-lena-out-1.pgm");

  t.restart();
  g = gradient_spe_2(lena, win_c4p());
  t.stop();
  std::cout << t.read() << std::endl;
  io::pgm::save(g, "gradient-spe-lena-out-2.pgm");

  t.restart();
  g = gradient_spe_3(lena, win_c4p());
  t.stop();
  std::cout << t.read() << std::endl;
  io::pgm::save(g, "gradient-spe-lena-out-3.pgm");

  t.restart();
  g = mln::morpho::gradient_spe_4(lena, win_c4p());
  t.stop();
  std::cout << t.read() << std::endl;
  io::pgm::save(g, "gradient-spe-lena-out-4.pgm");
}
