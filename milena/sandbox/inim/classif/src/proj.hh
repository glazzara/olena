// Copyright (C) 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_PROJ_HH
# define MLN_PROJ_HH

#include <mln/io/ppm/save.hh>
#include <mln/io/pgm/save.hh>
#include <mln/data/paste.hh>
#include <mln/accu/mean.hh>
#include <mln/accu/maj_h.hh>
#include <mln/literal/white.hh>
#include <mln/literal/colors.hh>
#include <mln/make/vec.hh
#include <mln/opt/at.hh>

namespace mln
{

  template <typename T, typename A>
  image2d<mln_result(A)>
  proj(const image3d<T>& input, A)
  {
    image2d<A> acc(geom::nslis(input), geom::nrows(input));
    mln_piter(image3d<T>) p(input.domain());
    for_all(p) // 3d
      if (input(p) != literal::zero)
        opt::at(acc, p.sli(), p.row()).take(input(p));

    image2d<mln_result(A)> output(acc.domain());
    data::paste(acc, output);
    return output;
  }

  template <typename T, typename U, typename K>
  void
  save_class(const image3d<T>& histo, const image3d<U>& ws,
             K mean, const char * fn)
  {
    accu::maj_h<value::int_u8> maj_1;
    image2d<value::int_u8> hproj = proj(histo, maj_1);

    accu::maj_h<value::int_u8> maj_2;
    image2d<value::int_u8> proj_class = proj(ws, maj_2);

    //std::cout << histo;

    image2d<value::rgb8> out(proj_class.domain());
    data::fill(out, literal::white);

    mln_piter(image2d<value::int_u8>) p(proj_class.domain());
    for_all(p)
      if (hproj(p) > 0)
      {
        if (proj_class(p) > 0)
          out(p) = convert::to<value::rgb8>(mean[proj_class(p)]);
        else
          out(p) = literal::red;
      }
    io::ppm::save(out, fn);
  }

  template <typename T, typename I, typename A>
  image2d<mln_result(A)>
  proj_vec(const image3d<T>& input, const I& histo, A)
  {
    image2d<A> acc(geom::nslis(input), geom::nrows(input));
    mln_piter(image3d<T>) p(input.domain());
    for_all(p) // 3d
      if (histo(p) != literal::zero)
        opt::at(acc, p.sli(), p.row()).take(input(p));

    image2d<mln_result(A)> output(acc.domain());
    data::paste(acc, output);
    return output;
  }

  template <typename I, typename T, typename U, typename V>
  void
  save_class_v2(const I& ima, const T& histo, image3d<U>& mean,
                const image3d<V>& parent, unsigned f,
                const char * fn)
  {
    image3d<value::rgb8> out3d(histo.domain());

    mln_piter(I) p(ima.domain());
    for_all(p)
    {
      point3d p3 = point3d(ima(p).red() / f,
                           ima(p).green() / f,
                           ima(p).blue() / f);
      out3d(p3) = convert::to<value::rgb8>(mean(parent(p3)));
    }

    //FIXME: maj_h implies set is finite, mx set is ordered
    accu::mean<mln::algebra::vec<3u, float> > maj_2;
    image2d<mln::algebra::vec<3u, float> > mproj =
      proj_vec(out3d, histo, maj_2);

    image2d<value::rgb8> out(geom::nslis(histo), geom::nrows(histo));
    data::fill(out, literal::white);

    mln_piter(image2d<value::int_u8>) p1(out.domain());
    for_all(p1)
    {
      out(p1) = convert::to<value::rgb8>(mproj(p1));
    }

    io::ppm::save(out, fn);
  }

} // end of namespace mln

#endif /* MLN_PROJ_HH */

