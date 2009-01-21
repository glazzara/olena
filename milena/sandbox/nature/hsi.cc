// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory
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

#include <iterator>
#include <string>

#include <mln/core/image/image2d.hh>
#include <mln/core/image/fun_image.hh>
#include <mln/core/alias/window2d.hh>
#include <mln/win/all.hh>

#include <mln/value/rgb8.hh>
#include <mln/value/hsi.hh>

#include <mln/fun/v2v/rgb_to_hsi.hh>
#include <mln/fun/meta/inty.hh>

#include <mln/level/compare.hh>

#include <mln/io/ppm/load.hh>
#include <mln/io/ppm/save.hh>

#include <mln/morpho/erosion.hh>

#include <mln/accu/histo.hh>
#include <mln/histo/compute.hh>
#include <mln/level/transform.hh>
#include <mln/data/paste.hh>

using namespace mln;
using namespace value;

int main (int argc, const char* argv [])
{
  if (argc < 2) {
    std::cerr << "usage: " << argv[0] << " in.ppm [other_files.ppm]" << std::endl;
    return 1;
  }

  for (int i = 1; i < argc; ++i)
    {
      image2d<rgb8> input;
      io::ppm::load(input, argv[i]);

      image2d<hsi_f> hsi = level::transform(input, fun::v2v::f_rgb_to_hsi_f);
      fun_image<mln::meta::inty<hsi_f>, image2d<hsi_f> > l(hsi);

      win::rectangle2d rect(7,7);

      std::string name = argv[i];
      name.erase(name.length() - 4);
      data::paste(morpho::erosion(l, rect), l);
      input = level::transform(hsi, fun::v2v::f_hsi_to_rgb_3x8);
      io::ppm::save(input, name.append("_tst.ppm"));
    }
}
