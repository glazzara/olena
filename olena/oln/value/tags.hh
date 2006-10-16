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

#ifndef OLN_VALUE_TAGS_HH
# define OLN_VALUE_TAGS_HH

# include <mlc/bexpr.hh>


namespace oln
{


  namespace color
  {
    template <typename T> class rgb_;
  } // end of namespace oln::color


  namespace value
  {

    // Color trait.

    template <typename T>
    struct is_color : public mlc::bexpr_<false>
    {
    };

    template <typename T>
    struct is_color < color::rgb_<T> > : public mlc::bexpr_<true> {};


    // Grey-level trait.

    template <typename T>
    struct is_grey_level : public mlc::bexpr_<false>
    {};

    template <unsigned nbits>
    class greylevel_;

    template <unsigned nbits>
    struct is_grey_level < greylevel_<nbits> > : public mlc::bexpr_<true> {};

    class greylevel;

    template <>
    struct is_grey_level < greylevel > : public mlc::bexpr_<true> {};


    // Binary trait.

    template <typename T>
    struct is_binary : public mlc::bexpr_<false>
    {
    };

    template <>
    struct is_binary < bool > : public mlc::bexpr_<true> {};

    typedef greylevel_<1> bin;

    template <>
    struct is_binary < bin >  : public mlc::bexpr_<true> {};


    // Label trait.

    template <typename T>
    struct is_label : public mlc::bexpr_<false>
    {
    };
    

  } // end of namespace oln::value

} // end of namespace oln


#endif // ! OLN_VALUE_TAGS_HH
