// Copyright (C) 2008 EPITA Research and Development Laboratory
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

/*! \file sandbox/pellegrin/cond_inheritance/test_cond_inherit.cc
 *
 * \brief Test my work on conditional inheritance.
 */

#include <iostream>
#include <string>
#include <mln/core/image/image2d.hh>
#include <mln/io/pgm/load.hh>

#include "p_set.hh"
#include "p_array.hh"


int main ()
{
  //const std::string file ("../../../img/lena.pgm");
  //mln::image2d<mln::value::int_u8> I = mln::io::pgm::load (file);
  //mln::image2d<mln::value::int_u8>::fwd_piter p (I.domain ());

  //for_all (p)
  //{
  //}
  std::cout << __FILE__ << ": construction d'un objet p_array<int>"
	    << std::endl;
  mln::p_array<int> int_array;

  std::cout << '\n' << __FILE__ << ": construction d'un objet p_set<int>"
	    << std::endl;
  mln::p_set<int> int_set;

  std::cout << "\nEnd of test on conditional inheritance" << std::endl;
}

