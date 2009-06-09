// Copyright (C) 2009 EPITA Research and Development Laboratory
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
/// Test on mln::labeling::wrap.

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/value/int_u8.hh>
#include <mln/value/int_u16.hh>
#include <mln/make/image2d.hh>
#include <mln/labeling/wrap.hh>
#include <mln/data/compare.hh>

#include <mln/debug/println.hh>

#include "tests/data.hh"


int main()
{
  using namespace mln;
  using value::int_u8;
  using value::int_u16;

  int_u16 values[] = { 1, 1000,
		       0, 65534 };

  int_u8 ref_values[] = { 1, 235,
			  0, 254 };

  image2d<int_u16> ima = make::image2d(values);
  image2d<int_u8> ima_ref = make::image2d(ref_values);

  image2d<int_u8> ima_wrapped = labeling::wrap(int_u8(), ima);

  mln_assertion(ima_ref == ima_wrapped);
}
