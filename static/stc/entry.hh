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

#ifndef STATIC_ENTRY_HH
# define STATIC_ENTRY_HH

/** \file stc/entry.hh
    \brief Extensible entry points in a class hierarchy.

    The following simplified diagram shows a typical use of this
    facility.

	      First hierarchy           Second hierarchy


		    /A/                       /B/
		     ^                         ^
		     |                         |
	       ,-----+-----.             ,-----+-----.
	       |           |             |           |
	      /A1/        /A2/          /B1/        /B2/
	       |           |             |           |
	       o           o             o           o
		     o                         o
		     |                         |
	 stc::set_entry_node<C,1>    stc::set_entry_node<C,2>
	     (first selector)           (second selector)
		     ^                         ^
		     |                         |

		     `------------+------------'
				  |
			      stc::entry
				  ^
				  |
				  C
			   (a client class)


   Actually, the inheritance tree is (almost) linear, and the real
   code rather looks like this:

				 /B/
				  ^
				  |
			    ,-----+-----.
			    |           |
			   /B1/        /B2/
			    |           |
			    o           o
				  o
				  |
		       stc::set_entry_node<C,2>
				  ^
				  |
				 /A/
				  ^
				  |
			    ,-----+-----.
			    |           |
			   /A1/        /A2/
			    |           |
			    o           o
				  o
				  |
		       stc::set_entry_node<C,1>
				  ^
				  |
			      stc::entry
				  ^
				  |
				  C
*/

#include <mlc/if.hh>
#include <mlc/is_a.hh>
#include <mlc/flags.hh>
#include <mlc/comma.hh>

namespace stc
{
  namespace internal
  {
    // End of the recurring inheritance.
    struct none {};
  }

  /** \brief Entry point of a hierarchy.
      To be possibly specialized by the user.

      This class is used as selector ``plugging'' itself (inheriting)
      to (from) a bottom class in one of the multiple hierarchies.  */
  template <typename exact, unsigned num>
  struct set_entry_node : public mlc::undefined
  {
    // Nothing.
  };  

  namespace internal
  {
    // Not to be defined by the user.
    // FIXME: Use a lock?
    template <typename exact, unsigned num>
    struct get_entry_node :
      public set_entry_node<exact, num>,
      public mlc::if_< mlc_is_a( mlc_comma_1( set_entry_node<exact, num + 1> ),
				 mlc::undefined ),
		       internal::none,
		       internal::get_entry_node<exact, num + 1> >::ret
    {
    };
  }

  /** \brief Bottom entry point.
     
      This class must not to be specialized by the user.  However,
      this class is meant to be super class of classes below the
      multiple hierarchies.  */
  template <typename exact>
  struct entry : public internal::get_entry_node<exact, 1>
  {
  };

} // end of namespace stc

#endif // ! STATIC_ENTRY_HH
