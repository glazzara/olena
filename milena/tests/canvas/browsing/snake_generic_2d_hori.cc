// Copyright (C) 2007, 2008, 2009 EPITA Research and Development
// Laboratory (LRDE)
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

/// \file tests/canvas/browsing/snake_generic_3d_hori.cc
///
/// \brief Tests on mln::canvas::browsing::snake_generic.

#include <mln/core/image/image2d.hh>
#include <mln/canvas/browsing/snake_generic.hh>
#include <mln/fun/p2v/iota.hh>
#include <mln/debug/println.hh>


template <typename I, typename F>
struct assign_browsing_functor
{
  typedef mln_site(I) S;
  enum { dim = S::dim };

  typedef assign_browsing_functor<I, F> self;
  typedef mln_deduce(I, psite, delta) dpsite;
  typedef void (assign_browsing_functor<I,F>::*move_fun)();

  I input;
  F f;
  std::vector<move_fun> moves;
  std::vector<dpsite> dps;

  assign_browsing_functor(I& input, F f = F())
    : input(input),
      f(f),
      moves(3),
      dps(3)
  {
    dps[0] = dpsite(1, 0);
    dps[1] = dpsite(0, 1);
    dps[2] = dpsite(0, -1);
    moves[0] = &self::down;
    moves[1] = &self::fwd;
    moves[2] = &self::bkd;
  }

  mln_psite(I) p;

  void init()  {}
  void final() {}
  void next()
  {
    input(p) = f(p);
  }
  void fwd()  { std::cout << "fwd" << std::endl; next(); }
  void bkd()  { std::cout << "bkd" << std::endl; next(); }
  void down() { std::cout << "down" << std::endl; next(); }
};

namespace mln
{

  template <typename I, typename F, typename B>
  void my_test(Image<I>& ima_,
	       const Function_v2v<F>& f_,
	       const Browsing<B>& browse_)
  {
    I& ima = exact(ima_);
    const F& f = exact(f_);
    const B& browse = exact(browse_);

    assign_browsing_functor<I, F> fun(ima, f);
    browse(fun);
  }

}


int main()
{
  using namespace mln;
  image2d<unsigned> ima2(3, 3);

  std::cout << ima2.bbox() << std::endl;
  my_test(ima2, fun::p2v::iota(), canvas::browsing::snake_generic);
  debug::println(ima2);
}
