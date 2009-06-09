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

#include <mln/core/image/image2d.hh>
#include <mln/core/image/imorph/decorated_image.hh>


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
  point2d p = point2d(0, 0);

  decorated_image< I, counter<I> > ima_ = decorate(ima, counter<I>());
  ima_(p) = ima_(p) = 51;
  mln_assertion(count_read == 1 && count_write == 2);

  const I& imac = ima;
  decorated_image< const I, counter<I> > cima_ = decorate(imac, counter<I>());
  /* Note that the statement
    
       cima_(p);
    
     alone would *not* trigger a read.  It seems the compiler
     optimizes in our back...  */
  int v = cima_(p);
  // Avoid a warning on unused value V.
  v = v;
  mln_assertion(count_read == 2 && count_write == 2);
}
