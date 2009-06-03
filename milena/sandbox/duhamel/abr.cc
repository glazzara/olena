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
// License.  
// reasons why the executable file might be covered by the GNU General
// Public License.

/*!
 *  \file   tests/abr.cc
 *  
 *  \brief  test of mln::util::abr
 * 
 */

#include "abr.hh"

int main (void)
{
  using namespace mln;
  unsigned elt1 = 1;
  unsigned elt2 = 2;
  unsigned elt3 = 3;
  unsigned elt4 = 4;
  unsigned elt5 = 5;
  util::s_abr<unsigned> abr (elt1);

  abr.add_son (elt2);
  abr.add_son (elt3);
  abr.print ();
  util::s_abr<unsigned>* abr2 = abr.search (elt2);
  abr2->add_son (elt4);
  abr2->add_son (elt5);
  abr.print ();
}
