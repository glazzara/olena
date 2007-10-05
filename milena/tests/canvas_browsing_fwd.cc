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

/*! \file tests/canvas_browsing_fwd.cc
 *
 * \brief Tests on mln::canvas::browsing::fwd.
 */

#include <mln/core/image2d_b.hh>
#include <mln/core/image3d_b.hh>
#include <mln/canvas/browsing/fwd.hh>
#include <mln/canvas/browsing/snake_fwd.hh>
#include <mln/canvas/browsing/directional.hh>
#include <mln/fun/p2v/iota.hh>
#include <mln/pw/image.hh>
#include <mln/debug/println.hh>
#include <mln/level/fill.hh>



// FIXME: Move code below into mln/canvas/browsing/iota.hh.


template <typename I_, typename F>
struct assign_browsing_functor
{
  typedef I_ I;
  enum { dim = I::point::dim };


  I input;
  F f;
  int dir;

  assign_browsing_functor(I& input, F f = F(), int dir_ = 0)
    : input(input),
      f(f),
      dir(dir_)
  {}

  mln_psite(I) p;

  void init()  {}
  void final() {}
  void next()
  {
    input(p) = f(p);
  }
  void fwd()  { next(); }
  void bkd()  { next(); }
  void down() { next(); }
};

namespace mln
{

  template <typename I, typename F, typename B>
  void my_test(Image<I>& ima_,
	       const Function_p2v<F>& f_,
	       const Browsing<B>& browse_,
	       int dir = 0)
  {
    I& ima = exact(ima_);
    const F& f = exact(f_);
    const B& browse = exact(browse_);

    assign_browsing_functor<I, F> fun(ima, f, dir);
    browse(fun);
  }

}


int main()
{
  using namespace mln;
  image2d_b<unsigned> ima2(3, 3);
  image3d_b<unsigned> ima3(3, 3, 3);

  my_test(ima2, fun::p2v::iota, canvas::browsing::fwd);
  debug::println(ima2);

  my_test(ima2, fun::p2v::iota, canvas::browsing::snake_fwd);
  debug::println(ima2);


  level::fill(ima2, 0);
  my_test(ima2, fun::p2v::iota, canvas::browsing::directional, 0);
  debug::println(ima2);

  level::fill(ima2, 0);
  my_test(ima2, fun::p2v::iota, canvas::browsing::directional, 1);
  debug::println(ima2);


  level::fill(ima3, 0);
  my_test(ima3, fun::p2v::iota, canvas::browsing::directional, 0);
  debug::println(ima3);

  level::fill(ima3, 0);
  my_test(ima3, fun::p2v::iota, canvas::browsing::directional, 1);
  debug::println(ima3);

  level::fill(ima3, 0);
  my_test(ima3, fun::p2v::iota, canvas::browsing::directional, 2);
  debug::println(ima3);
}
