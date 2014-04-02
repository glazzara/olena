// Copyright (C) 2007, 2008, 2009, 2014 EPITA Research and Development
// Laboratory (LRDE).
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

/// \brief Exercise mln::test::positive.

#include <mln/core/image/image2d.hh>
#include <mln/test/positive.hh>

#include <mln/data/fill.hh>
#include <mln/debug/iota.hh>

int main()
{
  using namespace mln;
  typedef image2d<int> I;

  /* Null values.

     Note that mln::test::positive actually checks whether all the
     values of an image are non-negative, thus considering 0 as a
     valid value (which is usually not the case of the term
     ``positive'' in English). */
  I zeros(3,3);
  data::fill(zeros, 0);
  mln_assertion(test::positive(zeros));

  /* Positive (non-null) natural numbers.  */
  I naturals(3,3);
  debug::iota(naturals);
  mln_assertion(test::positive(naturals));

  /* Negative (non-null) numbers.  */
  I negatives(3,3);
  data::fill(negatives, -42);
  mln_assertion(!test::positive(negatives));
}
