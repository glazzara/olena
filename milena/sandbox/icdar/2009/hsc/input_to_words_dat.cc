// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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


// Results published in:
//
// `ICDAR2009 Handwriting Segmentation Contest`.
// B. Gatos, N. Stamatopoulos and G. Louloudis
// 2009 10th International Conference on Document Analysis and Recognition
//
// http://www.cvc.uab.es/icdar2009/papers/3725b393.pdf


#include <mln/value/int_u16.hh>
#include <mln/value/rgb8.hh>

#include <mln/io/pbm/load.hh>
#include <mln/io/ppm/save.hh>
#include <mln/labeling/colorize.hh>
#include <mln/estim/min_max.hh>

#include <sandbox/icdar/2009/hsc/input_to_lines.hh>
#include <sandbox/icdar/2009/hsc/line_to_words.hh>
#include <sandbox/icdar/2009/hsc/get_line_images.hh>
#include <sandbox/icdar/2009/hsc/io/icdar/save.hh>


namespace mln
{

  template <typename L>
  void
  paste_words(image2d<L>& output,
	      const image2d<L>& words,
	      L l_1st)
  {
    mln_piter(box2d) p(words.domain());
    for_all(p)
    {
      if (words(p) == 0)
	continue;
      output(p) = l_1st.to_enc() + words(p).to_enc();
    }
  }

} // mln



void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pbm output.dat [output.ppm]" << std::endl
	    << "  HSC @ ICDAR'2009" << std::endl
	    << "  input.pbm:  a raw binary 2D image (either a line or a page)" << std::endl
	    << "  output.dat: output image where words are labeled (int_u8)" << std::endl
	    << "              0 is the background label." << std::endl;
  std::abort();
}



int main(int argc, char* argv[])
{
  using namespace mln;

  if (argc != 3 && argc != 4)
    usage(argv);


  trace::entering("main");

  typedef image2d<bool> I_bw;

  I_bw input;
  io::pbm::load(input, argv[1]);


  typedef value::label<12> Ll;
  Ll n_lines;
  image2d<Ll> lines = input_to_lines(input, n_lines, 0.501); // with majoritary

  // std::cout << "n lines = " << n_lines << std::endl;

  typedef value::int_u16 L;

  image2d<L> output(input.domain());
  data::fill(output, 0);

  util::array<I_bw> line_ = get_line_images(lines, n_lines, 51);

  L n_words = 0;
  for (unsigned l = 1; l <= n_lines; ++l)
    {
      I_bw line = line_[l]; // A line image.

      L n_line_words = 0;
      image2d<L> words = line_to_words(line, n_line_words, false);
      paste_words(output, words, /* 1st label is */ n_words);

      // std::cout << "line = " << l
      //	<< "  n_words = " << n_line_words << std::endl;

      n_words += n_line_words;
    }

  {
    // L l_min, l_max;
    // estim::min_max(output, l_min, l_max);
    // if (l_max != n_words)
    //  std::cout << "oops: " << l_max << ' ' << n_words << std::endl;
  }

  io::icdar::save(output, argv[2]);

  if (argc == 4)
    {
      io::ppm::save(labeling::colorize(value::rgb8(),
				    output,
				    n_words),
		    argv[3]);
    }

  trace::exiting("main");
}
