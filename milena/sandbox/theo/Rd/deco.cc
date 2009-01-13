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

/*! \file tests/decorated_image.cc
 *
 * \brief Tests on mln::decorated_image.
 * \todo Make this test work.
 */

#include <mln/core/image2d.hh>
#include <mln/core/decorated_image.hh>


unsigned count_read = 0, count_write = 0;

template <typename I>
struct counter
{
  void reading(const I&, const mln_psite(I)&) const
  {
    ++count_read;
  }
  void writing(I&, const mln_psite(I)&, const mln_value(I)&)
  {
    ++count_write;
  }
};


int main()
{
  using namespace mln;

  typedef image2d<int> I;
  I ima(1, 1);
  point2d p = make::point2d(0, 0);

  decorated_image< I, counter<I> > ima_ = decorate(ima, counter<I>());
  ima_(p) = ima_(p) = 51;

  std::cout << count_read << ' ' << count_write << std::endl;

  mln_assertion(count_read == 1 && count_write == 2);

  const I& imac = ima;
  decorated_image< const I, counter<I> > cima_ = decorate(imac, counter<I>());
}
