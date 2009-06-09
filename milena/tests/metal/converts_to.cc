// Copyright (C) 2007 EPITA Research and Development Laboratory
//
// This file is part of the Milena Library.  This library is free
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

/*! \file
 *
 * \brief Test on mln::metal::converts_to.
 */

#include <mln/metal/converts_to.hh>



struct from_1
{
};

struct dest
{
  dest(const from_1&) {}
};

struct from_2
{
  operator dest() const { dest* tmp; return *tmp; }
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

//   metal::converts_to<derived, base_>::check_not();
//   metal::converts_to<derived, const base_>::check_not();
}
