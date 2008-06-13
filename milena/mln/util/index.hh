// Copyright (C) 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_UTIL_INDEX_HH
# define MLN_UTIL_INDEX_HH

/*! \file mln/util/index.hh
 *
 * \brief Definition of an "index" type.
 */

# include <mln/core/concept/object.hh>


namespace mln
{

  namespace util
  {

    // Fwd decl.
    template <typename Tag> struct dindex_;


    /*! \brief Index structure.
     *
     */
    template <typename Tag = void>
    struct index_ // : public Object< index_<Tag> >
    {
      typedef dindex_<Tag> dpsite;
      typedef int coord;

      int i_;

      index_() {}
      index_(int i) : i_(i) {}

      operator int() const { return i_; }
    };

    typedef index_<void> index;


    template <typename Tag = void>
    struct dindex_ // : public Object< dindex_<Tag> >
    {
      typedef index_<Tag> psite;
      typedef index_<Tag> site;
      typedef int coord;

      int i_;

      dindex_() {}
      dindex_(int i) : i_(i) {}

      operator int() const { return i_; }
    };

    typedef dindex_<void> dindex;



# ifndef MLN_INCLUDE_ONLY

    // FIXME

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::util

} // end of namespace mln


#endif // ! MLN_UTIL_INDEX_HH
