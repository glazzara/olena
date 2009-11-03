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

#ifndef DYN_WRAPPERS_MILENA_HH
# define DYN_WRAPPERS_MILENA_HH

/// \file
/// \brief Declaration of Milena wrappers.
///
/// \todo The next step is to generate (semi-)automatically this file
/// from Milena's (possibly annotated) source files.

# include <dyn-all.hh>

namespace dyn
{

  namespace mln
  {

    /*-----------------.
    | Initialization.  |
    `-----------------*/

    /* FIXME: This is not really elegant, but this is a lot safer than
       the previous approach relying on the (implementation-defined)
       order of initialization of global objects' ctors.  We can
       probably improve this by reworking dyn::function_loader.  See
       also hints and advice from
       http://en.allexperts.com/q/C-1040/Constructors-Global-Object.htm.  */

    // This function *must* be called prior to any call to the wrapped
    // routines below in order to find the headers of the functions
    // they wrap.
    void initialize();


    /*-------------------------.
    | A few wrapped routines.  |
    `-------------------------*/

    extern dyn::language::ctor mk_image2d_int;

    extern dyn::language::fun fill;
    extern dyn::language::fun iota;
    extern dyn::language::fun println;

    // FIXME: Wrap more routines.

    // ...

  } // end of namespace dyn::mln

} // end of namespace dyn

#endif // ! DYN_WRAPPERS_MILENA_HH
