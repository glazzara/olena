// Copyright (C) 2010 EPITA Research and Development Laboratory (LRDE)
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

#include <mln/core/image/image2d.hh>

#include <mln/data/paste_without_localization.hh>
#include <mln/data/compare.hh>

#include <mln/debug/iota.hh>
#include <mln/debug/println.hh>


int main()
{
  using namespace mln;

  box2d b(point2d(1,2), point2d(6,8));
  image2d<unsigned> ima(b, 3);
  debug::iota(ima);

  image2d<unsigned> tmp(6, 7, 3);
  image2d<unsigned> ref(6, 7, 3);
  debug::iota(ref);


  // Lines
  {
    data::impl::paste_without_localization_lines(ima, tmp);
    mln_assertion(tmp == ref);
  }


  // Fastest
  {
    data::impl::paste_without_localization_fastest(ima, tmp);
    mln_assertion(tmp == ref);
  }

  // Fast
  {
    data::impl::paste_without_localization_fast(ima, tmp);
    mln_assertion(tmp == ref);
  }

  // Generic
  {
    data::impl::generic::paste_without_localization(ima, tmp);
    mln_assertion(tmp == ref);
  }


  // Dispatch
  {
    data::paste_without_localization(ima, tmp);
    mln_assertion(tmp == ref);
  }


}
