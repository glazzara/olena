// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
// (LRDE)
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

/*! \file tests/morpho/erosion.cc
 *
 * \brief Test on mln::morpho::erosion.
 */

#include <mln/core/image/image2d.hh>
#include <mln/win/all.hh>

#include <mln/debug/iota.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>

#include <mln/value/int_u8.hh>
#include <mln/morpho/erosion.hh>
#include <mln/arith/diff_abs.hh>

#include "tests/data.hh"
#include "tests/timer.hh"


int main()
{
  using namespace mln;
  using value::int_u8;

//   unsigned
//     l_oct = 11,  L_oct = 6 * l_oct + 1,
//     l_rec = 29,  L_rec = 2 * l_rec + 1;

  //    l_
  // oct rec  err
  //  0   0    3
  //  0   1    5
  //  1   2   15
  //  1   3    9
  //  2   5   11
  //  3   8    9
  //  5  13   15
  //  8  21   11
  // 11  29    1
  // 25  66   15

  border::thickness = 20;
  image2d<int_u8> lena;
  io::pgm::load(lena, MLN_IMG_DIR "/lena.pgm");

  win::rectangle2d rec(21, 21);
  win::hline2d hline(31);
  win::vline2d vline(31);
  win::diag2d diag2d(31);
  win::backdiag2d backdiag2d(31);
  win::octagon2d oct(6 * 3 + 1);
  win::disk2d disk(6 * 3 + 1);
  image2d<int_u8> out;
  image2d<int_u8> ref;
//   trace::quiet = false;
  timer t;

  /*
  // Rectangle
  std::cout << "-------------------------- Rectangle: " << std::endl;

  {
    t.start();
    ref = morpho::impl::generic::erosion_on_function(lena, rec);
    std::cout << "generic on rectangle2d: " << t << std::endl;
  }

  {
    t.start();
    out = morpho::erosion(lena, rec);
    std::cout << "dispach on rectangle2d: " << t << std::endl;
    bool test = out == ref;
    mln_assertion(test);
    std::cout << "     " << (test ? "OK" : "KO!!!") << std::endl;
  }

  {
    t.start();
    out = morpho::impl::erosion_arbitrary_2d_fastest(lena, rec);
    std::cout << "erosion_arbitrary_2d_fastest on rectangle2d: " << t << std::endl;
    bool test = out == ref;
    mln_assertion(test);
    std::cout << "     " << (test ? "OK" : "KO!!!") << std::endl;
  }

  {
    t.start();
    out = morpho::impl::erosion_arbitrary_2d(lena, rec);
    std::cout << "erosion_arbitrary_2d on rectangle2d: " << t << std::endl;
    bool test = out == ref;
    mln_assertion(test);
    std::cout << "     " << (test ? "OK" : "KO!!!") << std::endl;
  }

  //Hline

  std::cout << "-------------------------- Hline2d: "  << std::endl;

  {
    t.start();
    ref = morpho::impl::generic::erosion_on_function(lena, hline);
    std::cout << "generic on hline2d: " << t << std::endl;
  }

  {
    t.start();
    out = morpho::erosion(lena, hline);
    std::cout << "dispach on hline2d : " << t << std::endl;
    bool test = out == ref;
    mln_assertion(test);
    std::cout << "     " << (test ? "OK" : "KO!!!") << std::endl;
  }

  {
    t.start();
    out = morpho::impl::erosion_arbitrary_2d_fastest(lena, hline);
    std::cout << "erosion_arbitrary_2d_fastest on hline2d: " << t << std::endl;
    bool test = out == ref;
    mln_assertion(test);
    std::cout << "     " << (test ? "OK" : "KO!!!") << std::endl;
  }


  {
    t.start();
    out = morpho::impl::erosion_arbitrary_2d(lena, hline);
    std::cout << "erosion_arbitrary_2d on hline2d: " << t << std::endl;
    bool test = out == ref;
    mln_assertion(test);
    std::cout << "     " << (test ? "OK" : "KO!!!") << std::endl;
  }

  std::cout << "-------------------------- Vline2d: "<< std::endl;

  //Vline
  {
    t.start();
    ref = morpho::impl::generic::erosion_on_function(lena, vline);
    std::cout << "generic on vline2d: " << t << std::endl;
  }

  {
    t.start();
    out = morpho::erosion(lena, vline);
    std::cout << "dispach on vline2d : " << t << std::endl;
    bool test = out == ref;
    mln_assertion(test);
    std::cout << "     " << (test ? "OK" : "KO!!!") << std::endl;
  }


  {
    t.start();
    out = morpho::impl::erosion_arbitrary_2d_fastest(lena, vline);
    std::cout << "erosion_arbitrary_2d_fastest on vline2d: " << t << std::endl;
    bool test = out == ref;
    mln_assertion(test);
    std::cout << "     " << (test ? "OK" : "KO!!!") << std::endl;
  }


  {
    t.start();
    out = morpho::impl::erosion_arbitrary_2d(lena, vline);
    std::cout << "erosion_arbitrary_2d on vline2d: " << t << std::endl;
    bool test = out == ref;
    mln_assertion(test);
    std::cout << "     " << (test ? "OK" : "KO!!!") << std::endl;
  }


  //Diag2d
  std::cout << "-------------------------- Diag2d: "<< std::endl;
  {
    t.start();
    ref = morpho::impl::generic::erosion_on_function(lena, diag2d);
    std::cout << "generic on diag2d: " << t << std::endl;
  }

  {
    t.start();
    out = morpho::erosion(lena, diag2d);
    std::cout << "dispach on diag2d : " << t << std::endl;
    bool test = out == ref;
    mln_assertion(test);
    std::cout << "     " << (test ? "OK" : "KO!!!") << std::endl;
  }


  {
    t.start();
    out = morpho::impl::erosion_arbitrary_2d_fastest(lena, diag2d);
    std::cout << "erosion_arbitrary_2d_fastest on diag2d: " << t << std::endl;
    bool test = out == ref;
    mln_assertion(test);
    std::cout << "     " << (test ? "OK" : "KO!!!") << std::endl;
  }


  {
    t.start();
    out = morpho::impl::erosion_arbitrary_2d(lena, diag2d);
    std::cout << "erosion_arbitrary_2d on diag2d: " << t << std::endl;
    bool test = out == ref;
    mln_assertion(test);
    std::cout << "     " << (test ? "OK" : "KO!!!") << std::endl;
  }

  //Backdiag2d
  std::cout << "-------------------------- Backdiag2d: "<< std::endl;
  {
    t.start();
    ref = morpho::impl::generic::erosion_on_function(lena, backdiag2d);
    std::cout << "generic on backdiag2d: " << t << std::endl;
  }

  {
    t.start();
    out = morpho::erosion(lena, backdiag2d);
    std::cout << "dispach on backdiag2d : " << t << std::endl;
    bool test = out == ref;
    mln_assertion(test);
    std::cout << "     " << (test ? "OK" : "KO!!!") << std::endl;
  }


  {
    t.start();
    out = morpho::impl::erosion_arbitrary_2d_fastest(lena, backdiag2d);
    std::cout << "erosion_arbitrary_2d_fastest on backdiag2d: " << t << std::endl;
    bool test = out == ref;
    mln_assertion(test);
    std::cout << "     " << (test ? "OK" : "KO!!!") << std::endl;
  }


  {
    t.start();
    out = morpho::impl::erosion_arbitrary_2d(lena, backdiag2d);
    std::cout << "erosion_arbitrary_2d on backdiag2d: " << t << std::endl;
    bool test = out == ref;
    mln_assertion(test);
    std::cout << "     " << (test ? "OK" : "KO!!!") << std::endl;
  }
  */
  std::cout << "-------------------------- Octagon: " << std::endl;
  border::thickness = 0;
  // Octagon
  {
    t.start();
    ref = morpho::impl::generic::erosion_on_function(lena, oct);
    std::cout << "generic on octagon: " << t << std::endl;
    io::pgm::save(ref, "out_oct_ref.pgm");
  }

  {
    t.start();
    out = morpho::erosion(lena, oct);
    std::cout << "dispach on octagon: " << t << std::endl;
    io::pgm::save(out, "out_oct.pgm");
    io::pgm::save(arith::diff_abs(out, ref), "diff.pgm");

    bool test = out == ref;
    mln_assertion(test);
    std::cout << "     " << (test ? "OK" : "KO!!!") << std::endl;
  }


  std::cout << "-------------------------- Disk: " << std::endl;

  // Disk
  {
    t.start();
    ref = morpho::impl::generic::erosion_on_function(lena, disk);
    std::cout << "generic on disk: " << t << std::endl;
    io::pgm::save(ref, "out_disk_ref.pgm");
  }

  {
    t.start();
    out = morpho::erosion(lena, disk);
    std::cout << "dispach on disk: " << t << std::endl;
    bool test = out == ref;
    mln_assertion(test);
    std::cout << "     " << (test ? "OK" : "KO!!!") << std::endl;
  }

}
