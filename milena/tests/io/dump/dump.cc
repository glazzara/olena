// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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
/// Test on mln::io::dump::*.

#include <mln/core/image/image2d.hh>
#include <mln/io/dump/load.hh>
#include <mln/io/dump/save.hh>

#include <mln/data/compare.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/rgb8.hh>

#include "tests/data.hh"

#include <mln/debug/println.hh>

int main()
{
  using namespace mln;

  /// Value: bool
  {
    bool data[4] = { 0, 1,
		     1, 0 };
    image2d<bool> pic = make::image2d(data);

    io::dump::save(pic, "pic.dump");

    image2d<bool> pic2;
    io::dump::load(pic2, "pic.dump");

    mln_assertion(pic.domain() == pic2.domain());
    mln_assertion(pic == pic2);
  }

  /// Value: int_u8
  {
    value::int_u8 data[4] = { 5, 1,
			      1, 9 };
    image2d<value::int_u8> pic = make::image2d(data);
    io::dump::save(pic, "pic.dump");
    image2d<value::int_u8> pic2;
    io::dump::load(pic2, "pic.dump");

    mln_assertion(pic.domain() == pic2.domain());
    mln_assertion(pic == pic2);
  }

  /// Value: unsigned
  {
    unsigned data[4] = { 5, 1,
			 1, 9 };
    image2d<unsigned> pic = make::image2d(data);
    io::dump::save(pic, "pic.dump");
    image2d<unsigned> pic2;
    io::dump::load(pic2, "pic.dump");

    mln_assertion(pic.domain() == pic2.domain());
    mln_assertion(pic == pic2);
  }

  /// Value: float
  {
    float data[4] = { 5, 1,
			 1, 9 };
    image2d<float> pic = make::image2d(data);
    io::dump::save(pic, "pic.dump");
    image2d<float> pic2;
    io::dump::load(pic2, "pic.dump");

    mln_assertion(pic.domain() == pic2.domain());
    mln_assertion(pic == pic2);
  }


  /// Value: rgb8
  {
    using value::rgb8;
    value::rgb8 data[4] = { rgb8(2,4,5),     rgb8(1,23,255),
			    rgb8(64,41,150), rgb8(23,53,49) };
    image2d<value::rgb8> pic = make::image2d(data);
    io::dump::save(pic, "pic.dump");

    image2d<value::rgb8> pic2;
    io::dump::load(pic2, "pic.dump");

    mln_assertion(pic.domain() == pic2.domain());
    mln_assertion(pic == pic2);
  }

}

