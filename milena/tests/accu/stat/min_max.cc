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

#include <mln/value/builtin/integers.hh>

#include <mln/accu/stat/min_max.hh>

int main()
{
  using namespace mln;

  {
    mln_accu_with_(accu::meta::stat::min_max, int) accu;

    accu.take(7);

    mln_assertion(accu.to_result().first == 7);
    mln_assertion(accu.to_result().second == 7);
  }

  {
    mln_accu_with_(accu::meta::stat::min_max, int) accu;

    accu.take(2);
    accu.take(1);
    accu.take(0);

    accu.take(6);
    accu.take(5);
    accu.take(4);
    accu.take(3);

    accu.take(10);
    accu.take(9);
    accu.take(8);
    accu.take(7);

    mln_assertion(accu.first()  ==  0);
    mln_assertion(accu.second() == 10);
  }
}
