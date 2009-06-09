// Copyright (C) 2008, 2009 EPITA Research and Development Laboratory
// (LRDE)
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

/// \file
///
/// Tests on mln::data::transform_inplace.


#include <mln/core/image/image1d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/core/image/flat_image.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/core/image/dmorph/sub_image.hh>
#include <mln/core/image/dmorph/extension_val.hh>
#include <mln/core/routine/duplicate.hh>

#include <mln/fun/v2v/inc.hh>
#include <mln/fun/v2v/dec.hh>
#include <mln/fun/p2b/chess.hh>

#include <mln/debug/iota.hh>

#include <mln/data/transform_inplace.hh>
#include <mln/data/compare.hh>



int main()
{
  using namespace mln;
  const unsigned size = 50;


  // image 2d tests
  {
    typedef int T;
    image2d<T> ref(size, size);
    debug::iota(ref);

    image2d<T> ima = duplicate(ref);
    data::transform_inplace(ima, fun::v2v::inc<T>());
    data::transform_inplace(ima, fun::v2v::dec<T>());

    mln_assertion(ima == ref);
  }

  /// image 1d test
  {
    typedef unsigned short T;
    image1d<T> ref(size);
    debug::iota(ref);

    image1d<T> ima = duplicate(ref);
    data::transform_inplace(ima, fun::v2v::inc<T>());
    data::transform_inplace(ima, fun::v2v::dec<T>());

    mln_assertion(ima == ref);
  }


  /// image 3d test
  {
    typedef unsigned short T;
    image3d<T> ref(size, size, size);
    debug::iota(ref);

    image3d<T> ima = duplicate(ref);
    data::transform_inplace(ima, fun::v2v::inc<T>());
    data::transform_inplace(ima, fun::v2v::dec<T>());

    mln_assertion(ima == ref);
  }

  // flat image test
  {
    typedef short T;
    flat_image<T, box2d>
      ref(5, make::box2d(size, size)),
      ima(5, make::box2d(size, size));

    data::transform_inplace(ima, fun::v2v::inc<T>());
    data::transform_inplace(ima, fun::v2v::dec<T>());

    mln_assertion(ima == ref);
  }

  // image if test
  {
    typedef unsigned short T;
    typedef image2d<T> I;
    typedef image_if<I, fun::p2b::chess> II;

    I ref(size, size);
    debug::iota(ref);
    II ref_if = ref | fun::p2b::chess();

    I ima = duplicate(ref);
    II ima_if = ima | fun::p2b::chess();

    data::transform_inplace(ima_if, fun::v2v::inc<T>());
    data::transform_inplace(ima_if, fun::v2v::dec<T>());

    mln_assertion(ima_if == ref_if);
  }

  // sub_image test
  {
    typedef image2d<int> I;
    typedef sub_image< image2d<int>, box2d > II;
    typedef image2d<unsigned short> III;

    I ref(size, size);
    debug::iota(ref);
    II sub_ref(ref, make::box2d(4,4, 10,10));

    I ima = duplicate(ref);
    II sub_ima(ima, make::box2d(4,4, 10,10));

    data::transform_inplace(sub_ima, fun::v2v::inc<int>());
    data::transform_inplace(sub_ima, fun::v2v::dec<int>());

    mln_assertion(sub_ima == sub_ref);
  }

  // extended image test
  {
    typedef int T;
    typedef image2d<T> I;
    typedef extension_val< image2d<T> > II;

    I ref(size, size);

    I ima = duplicate(ref);
    II extend_ima(ima, 5);

    data::transform_inplace(extend_ima, fun::v2v::inc<T>());
    data::transform_inplace(extend_ima, fun::v2v::dec<T>());

    mln_assertion(extend_ima == ref);

  }
}
