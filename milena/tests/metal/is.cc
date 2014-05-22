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

#include <mln/metal/is.hh>
#include <mln/metal/is_not.hh>



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
protected: base() {}
};

struct derived : base
{
};


struct only
{
};


int main()
{
  using namespace mln;

  metal::is<only, only>::check();

  metal::is<derived, base>::check();
  metal::is<derived, const base>::check();
  metal::is<derived*, const base*>::check();
  metal::is<derived&, base&>::check();
  metal::is<derived&, const base&>::check();

  metal::is<int, float>::check_not();
  metal::is<from_1, dest>::check_not();
  metal::is<from_2, dest>::check_not();

  metal::is_not<int, float>::check();
  metal::is_not<from_1, dest>::check();
  metal::is_not<from_2, dest>::check();
}
