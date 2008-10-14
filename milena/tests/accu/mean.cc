// Copyright (C) 2007 EPITA Research and Development Laboratory
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

/*! \file tests/accu/mean.cc
 *
 * \brief Tests on mln::accu::mean.
 */

#include <mln/core/image/image2d.hh>
#include <mln/value/int_u8.hh>

#include <mln/accu/mean.hh>

int main()
{
  using namespace mln;

  {
    mln_accu_with_(accu::meta::mean, int) mean;

    mean.take(10);
    mean.take(9);
    mean.take(8);
    mean.take(7);
    mean.take(6);
    mean.take(5);
    mean.take(4);
    mean.take(3);
    mean.take(2);
    mean.take(1);
    mean.take(0);

    mln_assertion(mean.to_result() == 5);
  }

  {
    mln_accu_with_(accu::meta::mean, int) mean;

    mean.take(10);
    mean.take(8);
    mean.take(0);

    mln_assertion(mean.to_result() == 6);
  }
}
