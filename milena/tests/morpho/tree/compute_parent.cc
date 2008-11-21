// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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

/// \file tests/morpho/tree/compute_parent.cc
///
/// Tests on mln::morpho::tree::compute_parent.

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>

#include <mln/debug/println.hh>
#include <mln/debug/iota.hh>

#include <mln/core/var.hh>
#include <mln/core/image/image_if.hh>
#include <mln/pw/value.hh>

#include <mln/morpho/tree/compute_parent.hh>


int main()
{
  using namespace mln;

  {
    bool vals[] = { 1, 1, 1,
		    0, 1, 0,
		    0, 0, 1  };
    image2d<bool> ima = make::image2d(vals);
    mln_VAR(sub, ima | pw::value(ima));
    debug::println(sub);

    mln_VAR(par, morpho::tree::compute_parent(sub, c4(), sub.domain()));
    debug::println(par);
  }

/*
  {
    image2d<unsigned> ima(3, 3);
    debug::iota(ima);
    debug::println(ima);
    
    debug::println( morpho::tree::compute_parent(ima, c4(), ima.domain()) );
  }


  {
    image2d<unsigned> ima(3, 3);
    level::fill(ima, 0);
    debug::println(ima);
    
    debug::println( morpho::tree::compute_parent(ima, c4(), ima.domain()) );
  }
*/

}
