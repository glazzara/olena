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

/*! \file tests/value/mixin.hh
 *
 *  \brief Mixin types.
 */


#include <mln/value/mixin.hh>
#include <mln/value/rgb.hh>
#include <mln/value/rgb8.hh>


namespace mln
{

  namespace value
  {
    template <unsigned int n>
    struct op_less< mln::value::rgb<n> >
    {
      bool operator()(const mln::value::rgb<n> &a, const mln::value::rgb<n> &b)
      {
	return a.red() < b.red();
      }
    };
  }
}

int main ()
{
  mln::value::rgb8 r(12, 13, 14);
  mln::value::rgb8 s(13, 14, 15);

  std::cout
    << ( *(mln::value::mixin<mln::value::rgb8, mln::value::op_less>*)(void*) &r <
	 *(mln::value::mixin<mln::value::rgb8, mln::value::op_less>*)(void*) &s )
    << std::endl;
}
