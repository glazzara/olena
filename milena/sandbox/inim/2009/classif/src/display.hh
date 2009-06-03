// Copyright (C) 2008 EPITA Research and Development Laboratory
//
// This file is part of the Milena Library.  This library is free
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

#ifndef MLN__DISPLAY_HH
# define MLN__DISPLAY_HH

#include <mln/io/ppm/save.hh>
#include <mln/geom/nrows.hh>
#include <mln/geom/ncols.hh>
#include <mln/geom/nslis.hh>
#include <mln/value/all.hh>
#include <mln/core/image/image3d.hh>
#include <mln/literal/colors.hh>

#include <sys/stat.h>
#include <sstream>
#include <string.h>
#include <stdlib.h>


namespace mln
{

  template <typename I, typename J, typename K>
  void display(const I& histo, const J& ws, K mean, const char * dir)
  {
    mkdir(dir, 0777);
    chdir(dir);

    image2d< value::rgb8 > out(geom::nrows(histo), geom::ncols(histo));
    data::fill(out, literal::white);

    for (int s = 0; s < geom::nslis(histo); ++s)
    {
      for (int r = 0; r < geom::nrows(histo); ++r)
        for (int c = 0; c < geom::ncols(histo); ++c)
          if (histo(point3d(s,r,c)) > 0)
          {
            if (ws(point3d(s,r,c)) > 0)
              out(point2d(r, c)) = convert::to<value::rgb8>(mean[ws(point3d(s,r,c))]);
            else
              out(point2d(r, c)) = literal::red;
          }

        std::ostringstream is;
        is << "out_00" << s << ".ppm";
        io::ppm::save(out, is.str());
      }

    chdir("..");
  }

} // end of namespace mln

#endif /* MLN__DISPLAY_HH */

