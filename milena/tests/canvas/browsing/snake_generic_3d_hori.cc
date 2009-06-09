// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory (LRDE)
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

#include <mln/core/image/image3d.hh>
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
      moves(5),
      dps(5)
  {
    dps[0] = dpsite(1, 0, 0);
    dps[1] = dpsite(0, 1, 0);
    dps[2] = dpsite(0, -1, 0);
    dps[3] = dpsite(0, 0, 1);
    dps[4] = dpsite(0, 0, -1);

    moves[0] = &self::thr;
    moves[1] = &self::down;
    moves[2] = &self::up;
    moves[3] = &self::fwd;
    moves[4] = &self::bkd;
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
  void up() { std::cout << "up" << std::endl; next(); }
  void thr() { std::cout << "thr" << std::endl; next(); }
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
  image3d<unsigned> ima(3, 3, 3);

  ima(point3d(0,0,0)) = 42;
  std::cout << ima.bbox() << std::endl;
  my_test(ima, fun::p2v::iota(), canvas::browsing::snake_generic);
  debug::println(ima);
}
