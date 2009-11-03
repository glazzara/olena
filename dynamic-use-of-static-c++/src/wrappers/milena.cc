/* Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)

   This file is part of Olena.

   Olena is free software: you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free
   Software Foundation, version 2 of the License.

   Olena is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Olena.  If not, see <http://www.gnu.org/licenses/>.

   As a special exception, you may use this file as part of a free
   software project without restriction.  Specifically, if other files
   instantiate templates or use macros or inline functions from this
   file, or you compile this file and link it with other files to produce
   an executable, this file does not by itself cause the resulting
   executable to be covered by the GNU General Public License.  This
   exception does not however invalidate any other reasons why the
   executable file might be covered by the GNU General Public License.  */

#include <wrappers/milena.hh>

/// \file
/// \brief Definition of Milena wrappers.
///
/// \todo The next step is to generate (semi-)automatically this file
/// from Milena's (possibly annotated) source files.

namespace dyn
{

  namespace mln
  {

    /*-----------------.
    | Initialization.  |
    `-----------------*/

    void initialize()
    {
      dyn::include_dir(MILENA_DIR);
      dyn::include("mln/core/image/image2d.hh");
      dyn::include("mln/data/fill.hh");
      dyn::include("mln/debug/iota.hh");
      dyn::include("mln/debug/println.hh");
    }


    /*-------------------------.
    | A few wrapped routines.  |
    `-------------------------*/

    ctor mk_image2d_int("mln::image2d<int>");

    fun fill("mln::data::fill");
    fun iota("mln::debug::iota");
    fun println("mln::debug::println");

    // FIXME: Wrap more routines.

    // ...

  } // end of namespace dyn::mln

} // end of namespace dyn
