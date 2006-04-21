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
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
// 02110-1301 USA
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

#ifndef OLENA_CORE_ABSTRACT_IMAGE_ENTRY_HH
# define OLENA_CORE_ABSTRACT_IMAGE_ENTRY_HH

# include <oln/core/abstract/image.hh>
# include <stc/entry.hh>

namespace oln
{
  namespace abstract
  {
    // Forward declaration.
    template <typename E> struct image_entry;
  }

  /// \brief Uplink.
  ///
  /// oln::abstract::image<E> is not the direct super type of
  /// oln::abstract::image_entry<E>, however, it is declared as such
  /// using set_super_type, to make the virtual type retrieval system
  /// work.
  template <typename E>
  struct set_super_type< abstract::image_entry<E> >
  {
    typedef abstract::image<E> ret;
  };

  namespace abstract
  {

    template <typename E>
    struct image_entry : public stc::entry<E>
    {
    protected:
      image_entry() {}
    };

    /// \brief Tags for abstractions.
    /// Use them instead of hard-coded numbers.
    enum abstraction_tags
      {
	dimension_tag = 1,
	constness_tag
	// ...
      };

  }  // end of namespace oln::abstract

} // end of namespace oln


#endif // ! OLENA_CORE_ABSTRACT_IMAGE_ENTRY_HH
