// Copyright (C) 2007, 2008, 2009, 2013 EPITA Research and Development
// Laboratory (LRDE)
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

#include <mln/metal/converts_to.hh>

#include <mln/accu/math/sum.hh>


struct from_1
{
};

struct dest
{
  dest(const from_1&) {}
};

struct from_2
{
  operator dest() const { dest* tmp = 0; return *tmp; }
};


struct base
{
};

struct base_
{
protected:
  base_(int) {}
  base_(const base_&) {}
};

struct derived : base, base_
{
  derived() : base(), base_(0) {}
};


int main()
{
  using namespace mln;

  metal::converts_to<int, float>::check();

  metal::converts_to<from_1, dest>::check();
  metal::converts_to<from_2, dest>::check();

  metal::converts_to<derived*, base*>::check();
  metal::converts_to<derived, base>::check();
  metal::converts_to<derived, const base>::check();

  metal::converts_to<accu::math::sum<int, int>, int>::check();


  metal::converts_to<from_1, float>::check_not();

  metal::converts_to<base, derived>::check_not();
  metal::converts_to<base_, derived>::check_not();

  /* Unfortunately, we cannot perform the following checks, as they
     trigger an error (*before* SFINAE comes into play) about base_'s
     ctor(s) being protected:

      metal::converts_to<derived, base_>::check_not();
      metal::converts_to<derived, const base_>::check_not();
  */
}
