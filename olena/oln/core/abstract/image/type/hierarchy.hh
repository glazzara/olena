// Copyright (C) 2005, 2006 EPITA Research and Development Laboratory
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

#ifndef OLN_CORE_ABSTRACT_IMAGE_TYPE_HIERARCHY_HH
# define OLN_CORE_ABSTRACT_IMAGE_TYPE_HIERARCHY_HH

# include <oln/core/abstract/image.hh>
# include <oln/core/abstract/image/hierarchies.hh>
# include <xtd/valtraits.hh>


/* Image ``type'' hierarchy (summary).


                                    /image<I>/
                                        ^
                                        |
           ,--------------+-------------+-------------+--------.
           |              |             |             |        |
 /grey_level_image/ /color_image/ /label_image/ /data_image/  ...
           ^              ^         ^       ^         ^        ^
           |              |         |       |         |        |
           |              |         |  /binary_image/ |        |
           |              |         |       ^         |        |
           |              |         |       |         |        |
       ,-------------------------------------------------------------.
       |  The selection of the super class(es) is made according to  |
       |              the value of type_of(I, value).                |
       `-------------------------------------------------------------'
           |              |         |       |         |        |
           o              o         o       o         o        o

                                        o 
                                        | 
                             (image `type' selector)
                                        ^
                                        |
                                 image_entry<I>
                                        ^
                                        |
                                        I
                                (a concrete image)


  Default case: If no known value type is returned by `oln_type_of(I, value)',
  the entry is plugged to abstract::data_image<I>.  */



/*--------------------.
| Type abstractions.  |
`--------------------*/

# include <oln/core/abstract/image/type/binary.hh>
# include <oln/core/abstract/image/type/color.hh>
# include <oln/core/abstract/image/type/data.hh>
# include <oln/core/abstract/image/type/grey_level.hh>
# include <oln/core/abstract/image/type/label.hh>

// FIXME: do we really want this file?
// # include <oln/core/abstract/image/type/integre.hh>



/*--------------.
| Type switch.  |
`--------------*/

namespace oln
{

  /// Switch on on the grid dimension.
  /// \{

  // ----------------------------------------------- //
  // Cases where the value type is an builtin type.  //
  // ----------------------------------------------- //

  /// Binary case.
  template <typename E>
  struct case_< image_hierarchy_wrt_type, E, 1 > :
    where_< xtd_is_binary(oln_type_of(E, value)) >
  {
    // Definition of the super class corresponding to this case.
    typedef abstract::binary_image<E> ret;
  };

  /// Grey-level case.
  template <typename E>
  struct case_< image_hierarchy_wrt_type, E, 2 > :
    where_< mlc::or_list_< mlc::eq_<oln_type_of(E, value),          char>,
			   mlc::eq_<oln_type_of(E, value),   signed char>,
			   mlc::eq_<oln_type_of(E, value), unsigned char> > >
  {
    // Definition of the super class corresponding to this case.
    typedef abstract::grey_level_image<E> ret;
  };


  // -------------- //
  // Default case.  //
  // -------------- //

  /// Default case: image of ``data''.
  template <typename E>
  struct default_case_< image_hierarchy_wrt_type, E >
  {
    // Definition of the super class corresponding to this case.
    typedef abstract::data_image<E> ret;
  };

  /// \}

} // end of namespace oln


#endif // ! OLN_CORE_ABSTRACT_IMAGE_TYPE_HIERARCHY_HH
