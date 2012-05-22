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

#include <cmath>

#include <mln/core/image/image1d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/pw/image.hh>
#include <mln/core/image/flat_image.hh>
#include <mln/core/image/vmorph/cast_image.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/core/image/dmorph/sub_image.hh>
#include <mln/core/image/dmorph/extension_val.hh>

#include <mln/data/fill.hh>
#include <mln/data/transform.hh>
#include <mln/data/paste.hh>
#include <mln/fun/p2b/chess.hh>
#include <mln/fun/p2v/iota.hh>


namespace my
{

  // FIXME: Use debug::iota instead of this hand-made version.
  template <typename I>
  void iota(I& ima)
  {
    unsigned i = 0;
    mln_piter(I) p(ima.domain());
    for_all(p)
    {
      ima(p) = i * i;
      i += 1;
    }
  }

  // FIXME: Shouldn't this functor be part of Milena?
  struct sqrt : mln::Function_v2v<sqrt>
  {
    typedef unsigned short result;

    template <typename T>
    result operator()(T c) const
    {
      return static_cast<result>( std::sqrt(float(c)) );
    }
  };

} // end of namespace my



int main()
{
  using namespace mln;
  const unsigned size = 5;
  box2d b = make::box2d(1,1, 3,3);


  /// image 1d test
  {
    image1d<unsigned short> ima(size);
    image1d<unsigned short> out(size);

    my::iota(ima);
    out = data::transform(ima, my::sqrt());

    // FIXME: Use mln_piter().
    // FIXME: Or use mln::test instead?
    // (And so on for the rest of the file.)
    box_fwd_piter_<point1d> p(out.domain());
    for_all(p)
      mln_assertion(ima(p) == out(p) * out(p));
  }


  /// image 2d test
  {
    image2d<unsigned short> ima(size, size);
    image2d<unsigned short> out(size, size);

    my::iota(ima);
    out = data::transform(ima, my::sqrt());

    box_fwd_piter_<point2d> p(out.domain());
    for_all(p)
      mln_assertion(ima(p) == out(p) * out(p));
  }

  /// Another image2d test
  {
     image2d<unsigned short> ima(size, size);

     data::fill_with_value(ima, 51);
     data::transform(ima, my::sqrt());
  }

  /// image 3d test
  {
    image3d<unsigned short> ima(size, size, size);
    image3d<unsigned short> out(size, size, size);

    my::iota(ima);
    out = data::transform(ima, my::sqrt());

    box_fwd_piter_<point3d> p(out.domain());
    for_all(p)
      mln_assertion(ima(p) == out(p) * out(p));
  }

  /// pw image test
  {
    fun::p2v::iota f;
    const pw::image<fun::p2v::iota, box2d> ima(f, b);
    image2d<unsigned short> out(size, size);

    data::fill(out, 0u);
    out = data::transform(ima, my::sqrt());
  }

  // flat image test
  {
    flat_image<short, box2d> ima(5, b);
    image2d<unsigned short> out(size, size);

    data::fill_with_value(ima, 169);
    out = data::transform(ima, my::sqrt());

    box2d::piter p(out.domain());
    for_all(p)
      mln_assertion(ima(p) == out(p) * out(p));
  }

  // image if test
  {
    typedef image2d<unsigned short> I;
    typedef image_if<I, fun::p2b::chess> II;

    I ima(size, size);
    II ima_if = ima | fun::p2b::chess();

    data::fill_with_value(ima, 0);
    my::iota(ima);
    II out = data::transform(ima_if, my::sqrt());

    II::piter p(ima_if.domain());
    for_all(p)
      mln_assertion(ima_if(p) == out(p) * out(p));
  }

  // cast image test
  {
    typedef image2d<unsigned short> I;
    typedef cast_image_<int, I> II;
    typedef image2d<unsigned short> III;

    I in(size, size);
    II cast(in);
    III out(size, size);

    data::fill(in, 169u);
    data::fill(out, 81u);

    out = data::transform(cast, my::sqrt());

    II::piter p(cast.domain());
    for_all(p)
      mln_assertion(cast(p) == out(p) * out(p));
  }

  // sub_image test
  {
    typedef image2d<int> I;
    typedef sub_image< image2d<int>, box2d > II;
    typedef sub_image< image2d<unsigned short>, box2d > III;

    I ima(size, size);
    II sub_ima(ima, b);

    data::fill(ima, 169);
    III out = data::transform(sub_ima, my::sqrt());

    II::piter p(sub_ima.domain());
    for_all(p)
      mln_assertion(sub_ima(p) == out(p) * out(p));
  }

  // extended image test
  {
    typedef image2d<int> I;
    typedef extension_val< image2d<int> > II;
    typedef extension_val< image2d<unsigned short> > III;

    I ima(size, size);
    II extend_ima(ima, 169);

    data::fill(ima, 169);
    III out = data::transform(extend_ima, my::sqrt());

    II::piter p(extend_ima.domain());
    for_all(p)
      mln_assertion(extend_ima(p) == out(p) * out(p));
  }
}
