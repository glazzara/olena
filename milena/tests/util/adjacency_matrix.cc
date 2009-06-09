// Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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

#include <mln/util/adjacency_matrix.hh>
#include <mln/value/int_u8.hh>
#include <mln/value/int_u16.hh>


int main()
{
  using namespace mln;

  // The underlying data structure is a image2d.
  {
    util::adjacency_matrix<value::int_u8> adj(5);
    adj.add(3, 4);
    adj.add(2, 3);
    adj.add(1, 2);

    mln_assertion(adj.are_adjacent(2,3));
    mln_assertion(adj.are_adjacent(4,3));
    mln_assertion(adj.are_adjacent(2,1));
    mln_assertion(adj.are_adjacent(1,2));
    mln_assertion(! adj.are_adjacent(1,4));

    adj.remove(2,3);
    mln_assertion(! adj.are_adjacent(2,3));
    mln_assertion(! adj.are_adjacent(2,2));
  }

  // The underlying data structure is a util::set
  {
    util::adjacency_matrix<value::int_u16> adj(5);
    adj.add(3, 4);
    adj.add(2, 3);
    adj.add(1, 2);

    mln_assertion(adj.are_adjacent(2,3));
    mln_assertion(adj.are_adjacent(4,3));
    mln_assertion(adj.are_adjacent(2,1));
    mln_assertion(adj.are_adjacent(1,2));
    mln_assertion(! adj.are_adjacent(1,4));

    adj.remove(2,3);
    mln_assertion(! adj.are_adjacent(2,3));
    mln_assertion(! adj.are_adjacent(2,2));
  }

}
