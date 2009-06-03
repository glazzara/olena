// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

/// \file tests/morpho/tree/compute_attribute_image.cc
///
/// Tests on mln::morpho::tree::compute_attribute_image.

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/core/site_set/p_array.hh>
#include <mln/data/sort_psites.hh>
#include <mln/data/transform.hh>

#include <mln/debug/println.hh>
#include <mln/core/var.hh>
#include <mln/value/int_u8.hh>

#include <mln/morpho/tree/data.hh>
#include <mln/morpho/tree/compute_attribute_image.hh>

#include <mln/morpho/closing_area.hh>
#include <mln/morpho/opening_area.hh>

#include <mln/accu/count.hh>
#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/pbm/save.hh>

#include <mln/arith/min.hh>
#include <mln/arith/diff_abs.hh>


struct sat_t : mln::Function_v2v<sat_t>
{
  typedef mln::value::int_u8 result;
  result operator()(unsigned u) const
  {
    return u > 255 ? 255 : u;
  }
}
  sat;



int main()
{
  using namespace mln;
  using value::int_u8;

  typedef image2d<int_u8> I;
  typedef p_array<point2d> S;
  typedef util::pix<I> Px;


  I f;
  io::pgm::load(f, "../../../img/tiny.pgm");


  image2d<unsigned> area_inc, area_dec;
  {
    S s = data::sort_psites_increasing(f);
    morpho::tree::data<I,S> t(f, s, c4());
    accu::count<Px> a;
    area_inc = morpho::tree::compute_attribute_image(a, t);
    debug::println("area_inc =", area_inc);
  }
  {
    S s = data::sort_psites_decreasing(f);
    morpho::tree::data<I,S> t(f, s, c4());
    accu::count<Px> a;
    area_dec = morpho::tree::compute_attribute_image(a, t);
    debug::println("area_dec =", area_dec);
  }

  io::pgm::save(data::transform(area_inc, sat), "inc.pgm");
  io::pgm::save(data::transform(area_dec, sat), "dec.pgm");

  image2d<unsigned> area = arith::min(area_inc, area_dec);
  io::pgm::save(data::transform(area, sat), "min.pgm");

  unsigned lambda = 20;

  {
    I clo = morpho::closing_area(f, c4(), lambda);
    io::pgm::save(clo, "clo.pgm");

    I ope(clo.domain());
    morpho::opening_area(f, c4(), lambda, ope);
    io::pgm::save(ope, "ope.pgm");

    I d_clo = arith::diff_abs(f, clo);
    io::pgm::save(d_clo, "d_clo.pgm");

    I d_ope = arith::diff_abs(f, ope);
    io::pgm::save(d_ope, "d_ope.pgm");
  }

}
