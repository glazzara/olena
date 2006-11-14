// Copyright (C) 2006 EPITA Research and Development Laboratory
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

#ifndef OLN_CORE_MACROS_HH
# define OLN_CORE_MACROS_HH


# include <oln/core/typedefs.hh>


# define oln_type_of_(OlnType, Alias)					\
   oln::direct_type_of_<OlnType, oln::typedef_:: Alias##_type>::ret


# define oln_type_of(OlnType, Alias)		\
   typename oln_type_of_(OlnType, Alias)



# define oln_deduce_type_of(OlnType, Alias1, Alias2)							\
typename oln::direct_type_of_<typename oln::direct_type_of_<OlnType, oln::typedef_::Alias1##_type >::ret,	\
			       oln::typedef_::Alias2##_type >::ret

# define oln_deduce(OlnType, Alias1, Alias2) \
  oln_deduce_type_of(OlnType, Alias1, Alias2)



#define oln_virtual_typedef(Typedef) \
  typedef oln_type_of(E, Typedef) Typedef


// FIXME: Rec? The macro below is to ensure that static checks are removed during "Rec" tests.
// #define oln_virtual_typedef(Typedef) typedef void Typedef


#endif // ! OLN_CORE_MACROS_HH
