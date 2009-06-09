// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory (LRDE)
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

#include <mln/core/alias/dpoint2d.hh>
#include <mln/win/diff.hh>


int main()
{
  using namespace mln;
  
  window<dpoint2d> win1;
  win1
    .insert( 2, 7)
    .insert( 2, 1)
    .insert(-4, 0)
    .insert( 0, 0)
    .insert( 1, 1)
    .insert( 6, 5);

  window<dpoint2d> win2;
  win2
    .insert( 2, 7)
    .insert(-2, 1)
    .insert(-4, 0)
    .insert( 1,-1)
    .insert( 6, 5);

  window<dpoint2d> win3 = win1 - win2;

  mln_assertion(  win3.has(dpoint2d( 2, 1)));
  mln_assertion(  win3.has(dpoint2d( 0, 0)));
  mln_assertion(  win3.has(dpoint2d( 1, 1)));

  mln_assertion(! win3.has(dpoint2d( 2, 7)));
  mln_assertion(! win3.has(dpoint2d(-2, 1)));
  mln_assertion(! win3.has(dpoint2d(-4, 0)));
  mln_assertion(! win3.has(dpoint2d( 1,-1)));
  mln_assertion(! win3.has(dpoint2d( 6, 5)));
}
