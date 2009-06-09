// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef MLN_TAG_SKELETON_HH
# define MLN_TAG_SKELETON_HH

/// \file
///
/// Definition of tags used in the skeleton types associated to
/// image types.


namespace mln
{

  namespace tag
  {

    // With param.
    template <typename D> struct data_     { typedef D param; };
    template <typename E> struct ext_      { typedef E param; };
    template <typename F> struct function_ { typedef F param; };
    template <typename G> struct graph_	   { typedef G param; };
    template <typename I> struct image_    { typedef I param; };
    template <typename N> struct neighb_   { typedef N param; };
    template <typename P> struct psite_    { typedef P param; };
    template <typename S> struct domain_   { typedef S param; };
    template <typename V> struct value_    { typedef V param; };

    // With value.
    template <unsigned u> struct unsigned_ { enum { value = u }; };

  } // end of namespace mln::tag

} // end of namespace mln


#endif // ! MLN_TAG_SKELETON_HH
