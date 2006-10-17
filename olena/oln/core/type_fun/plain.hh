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

#ifndef OLN_CORE_TYPE_FUN_PLAIN_HH
# define OLN_CORE_TYPE_FUN_PLAIN_HH

# include <oln/core/typedefs.hh>

# include <oln/core/fwd_decls.hh>
# include <oln/morpher/fwd_decls.hh>

# include <oln/core/type_fun/ch_value.hh>



namespace oln
{

  namespace type_fun
  {

    template <typename T>
    struct plain
    {
      typedef mlc::undefined ret;
    };


    // image?d

    template <typename T>
    struct plain< image1d<T> >
    {
      typedef image1d<T> ret;
    };

    template <typename T>
    struct plain< image2d<T> >
    {
      typedef image2d<T> ret;
    };

    template <typename T>
    struct plain< image3d<T> >
    {
      typedef image3d<T> ret;
    };


    // morpher::add_neighborhood

    template <typename Image, typename Neighb>
    struct plain< morpher::add_neighborhood<Image, Neighb> >
    {
      typedef typename plain<Image>::ret plain_Image;
      typedef morpher::add_neighborhood<plain_Image, Neighb> ret;
    };


    // add_isubset

    template <typename Image, typename Isubset>
    struct plain< morpher::add_isubset<Image, Isubset> >
    {
      typedef typename plain<Image>::ret plain_Image;
      typedef morpher::add_isubset<plain_Image, Isubset> ret;
    };


    // identity

    template <typename Image>
    struct plain< morpher::identity<Image> >
    {
      typedef typename plain<Image>::ret plain_Image;
      typedef plain_Image ret;
    };


    // thru_fun
  
    template <typename Image, typename Fun>
    struct plain< morpher::thru_fun<Image, Fun> >
    {
      typedef morpher::thru_fun<Image, Fun> self_t;
      typedef typename plain<Image>::ret plain_Image;
      typedef typename ch_value<plain_Image, oln_value(self_t)>::ret ret;
    };


    // value_cast
  
    template <typename Image, typename Value>
    struct plain< morpher::value_cast<Image, Value> >
    {
      typedef typename plain<Image>::ret plain_Image;
      typedef typename ch_value<plain_Image, Value>::ret ret;
    };


  } // end of namespace oln::type_fun

} // end of namespace oln


#endif // ! OLN_CORE_TYPE_FUN_PLAIN_HH
