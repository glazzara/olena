// Copyright (C) 2011 EPITA Research and Development Laboratory (LRDE)
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

/// \file

#include <mln/value/rgb8.hh>
#include <mln/literal/colors.hh>


int main()
{
  using namespace mln;

  value::rgb8 v = literal::red;
  mln_assertion(v == value::rgb8(255, 0, 0));

  v = literal::green;
  mln_assertion(v == value::rgb8(0, 255, 0));

  v = literal::blue;
  mln_assertion(v == value::rgb8(0, 0, 255));

  v = literal::brown;
  mln_assertion(v == value::rgb8(191, 127, 64));

  v = literal::lime;
  mln_assertion(v == value::rgb8(191, 255, 0));

  v = literal::orange;
  mln_assertion(v == value::rgb8(255, 127, 0));

  v = literal::pink;
  mln_assertion(v == value::rgb8(255, 191, 191));

  v = literal::purple;
  mln_assertion(v == value::rgb8(191, 0, 64));

  v = literal::teal;
  mln_assertion(v == value::rgb8(0, 127, 127));

  v = literal::violet;
  mln_assertion(v == value::rgb8(127, 0, 127));

  v = literal::cyan;
  mln_assertion(v == value::rgb8(0, 255, 255));

  v = literal::magenta;
  mln_assertion(v == value::rgb8(255, 0, 255));

  v = literal::yellow;
  mln_assertion(v == value::rgb8(255, 255, 0));

  v = literal::olive;
  mln_assertion(v == value::rgb8(127, 127, 0));
}
