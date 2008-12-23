// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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

# include <string>
# include <mln/core/image/image2d.hh>
# include <mln/io/pbm/load.hh>
# include <mln/debug/println.hh>
# include <mln/logical/not.hh>
# include <mln/logical/or.hh>
# include <mln/io/pbm/save.hh>
# include <mln/io/pbm/load.hh>
# include <mln/io/ppm/save.hh>
# include <mln/io/pgm/save.hh>

# include <mln/morpho/erosion.hh>
# include <mln/morpho/dilation.hh>
# include <mln/make/w_window2d.hh>
# include <mln/win/rectangle2d.hh>
# include <mln/win/vline2d.hh>
# include <mln/win/hline2d.hh>
# include <mln/data/fill.hh>
# include <mln/literal/all.hh>
# include <mln/value/rgb8.hh>


using namespace mln;

template <typename I, typename W>
mln_concrete(I)
pseudo(const I& input, const W& win, const float ratio)
{
  I output = duplicate (input);
  unsigned ko = win.length () - (unsigned)(ratio * win.length ());
  std::cout << "ko : " << ko << std::endl;
  mln_pixter(const I) p(input);
  mln_pixter(I) p_out(output);
  mln_qixter(const I, W) q(p, win);

  for_all_2(p, p_out)
    {
      unsigned cpt = 0;

      for_all(q)
      {
	if (! q.val ())
	  {
	    if (++cpt > ko)
	      break;
	  }
      }
      p_out.val() = (cpt <= ko);
    }
  return output;
}

template <typename I, typename J>
void
draw_array(I input, J image_bool)
{
  mln_piter(I) p (input.domain ());

  for_all(p)
  {
    if (image_bool(p))
      input(p) = literal::red;
  }
}


int
main(int argc, char** argv)
{
  using value::int_u8;
  using value::rgb8;

  typedef point2d P;
  typedef image2d<bool> I;
  typedef image2d<unsigned> I_LABEL;
  typedef image2d<value::rgb8> O;

  if (argc != 3)
    {
      std::cerr << "Usage : " << argv[0]
		<< " input.pbm output" << std::endl;
      exit(1);
    }

  std::string path_input = argv[1];
  std::string path_output = argv[2];


  I input = io::pbm::load (path_input);
  O output (input.domain ());

  /// Inversion video.
  I inv_image = logical::not_(input);


  mln_piter_(I) p (input.domain());

  for_all(p)
  {
    if (inv_image(p))
      output(p) = literal::black;
    else
      output(p) = literal::white;
  }


  /// Extract horizontal axis of array.
  win::hline2d h_line(51);
  I h_axis_image = pseudo(inv_image, h_line, 0.7);
  h_axis_image = morpho::erosion(h_axis_image, h_line);
  draw_array(output, h_axis_image);

  /// Extract vertical axis of array.
  win::vline2d v_line(11);
  I v_axis_image = pseudo(inv_image, v_line, 0.75);
  v_axis_image = morpho::erosion(v_axis_image, v_line);
  draw_array(output, v_axis_image);

  io::ppm::save (output, path_output + ".ppm");
  std::cout << path_output + ".ppm generated" << std::endl;
}

