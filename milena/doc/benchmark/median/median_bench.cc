// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory (LRDE)
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

#include <stdio.h>
#include <time.h>

#include <mln/core/image/image2d.hh>
#include <mln/win/all.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>

#include <mln/value/int_u8.hh>
#include <mln/debug/iota.hh>
#include <mln/debug/println.hh>

#include <mln/data/approx/median.hh>
#include <mln/data/fast_median.hh>
#include <mln/data/median.hh>

#include <mln/core/dpoints_pixter.hh>
#include <mln/core/pixel.hh>

#include "tests/data.hh"

using namespace mln;
using value::int_u8;

class timer
{
public:
  void start()
  {
    start_ = clock();
  }

  void stop()
  {
    end_ = clock();
    len = float(end_ - start_) / CLOCKS_PER_SEC;
  }

  float lenght()
  {
    return len;
  }

private:
  clock_t start_;
  clock_t end_;
  float len;
};


std::ostream& operator<<(std::ostream& ostr, timer t)
{
  return ostr << t.lenght() << "s";
}

template <typename I, typename W, typename O>
void tests(const Image<I>& input, const Window<W>& win,
	   Image<O>& output)
{
  timer chrono;

  chrono.start();
  data::fast_median(input, win, output);
  chrono.stop();
  std::cout << "Fast median : " << chrono << std::endl;

  chrono.start();
  data::median(input, win, output);
  chrono.stop();
  std::cout << "Median : " << chrono << std::endl;

  chrono.start();
  data::approx::median(input, exact(win), output);
  chrono.stop();
  std::cout << "Approx median : " << chrono << std::endl;

}

int main()
{
  {
    std::cout << "-----------------------" << std::endl;
    std::cout << "-----With rectangle 21x21" << std::endl;
    std::cout << "-----------------------" << std::endl;

    win::rectangle2d rect(21, 21);
    border::thickness = 50;

    image2d<int_u8> lena;
    io::pgm::load(lena, MLN_IMG_DIR "/lena.pgm");
    image2d<int_u8>  out(lena.domain());

    tests(lena, rect, out);
    io::pgm::save(out, "out.pgm");

  }

  {
    std::cout << "-----------------------" << std::endl;
    std::cout << "-----With octogone 13" << std::endl;
    std::cout << "-----------------------" << std::endl;

    win::octagon2d oct(13);
    border::thickness = 50;

    image2d<int_u8> lena;
    io::pgm::load(lena, MLN_IMG_DIR "/lena.pgm");
    image2d<int_u8>  out(lena.domain());

    tests(lena, oct, out);
    io::pgm::save(out, "out_oct.pgm");
  }


  {
    std::cout << "-----------------------" << std::endl;
    std::cout << "-----With disk2d 10" << std::endl;
    std::cout << "-----------------------" << std::endl;

    win::disk2d win(10);
    border::thickness = 50;

    image2d<int_u8> lena;
    io::pgm::load(lena, MLN_IMG_DIR "/lena.pgm");
    image2d<int_u8>  out(lena.domain());

    tests(lena, win, out);
    io::pgm::save(out, "out_oct.pgm");
  }

//   {
//     std::cout << "-----------------------" << std::endl;
//     std::cout << "-----With hline 10" << std::endl;
//     std::cout << "-----------------------" << std::endl;

//     win::hline2d win(10);
//     border::thickness = 50;

//     image2d<int_u8> lena;
//     io::pgm::load(lena, MLN_IMG_DIR "/lena.pgm");
//     image2d<int_u8>  out(lena.domain());

//     tests(lena, win, out);
//     io::pgm::save(out, "out_oct.pgm");
//   }

}
