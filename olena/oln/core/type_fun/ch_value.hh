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

#ifndef OLN_CORE_TYPE_FUN_CH_VALUE_HH
# define OLN_CORE_TYPE_FUN_CH_VALUE_HH

# include <mlc/flags.hh>

# include <oln/core/fwd_decls.hh>
# include <oln/morpher/fwd_decls.hh>



namespace oln
{

  namespace type_fun
  {

    template <typename Image, typename Value>
    struct ch_value
    {
      typedef mlc::undefined ret;
    };


    // image?d

    template <typename T, typename Value>
    struct ch_value< image1d<T>, Value >
    {
      typedef image1d<Value> ret;
    };

    template <typename T, typename Value>
    struct ch_value< image2d<T>, Value >
    {
      typedef image2d<Value> ret;
    };

    template <typename T, typename Value>
    struct ch_value< image3d<T>, Value >
    {
      typedef image3d<Value> ret;
    };


    // add_neighborhood.

    template <typename Image, typename Neighb, typename Value>
    struct ch_value< morpher::add_neighborhood<Image, Neighb>, Value >
    {
      typedef typename ch_value<Image, Value>::ret ImageV;
      typedef morpher::add_neighborhood<ImageV, Neighb> ret;
    };


    // add_isubset.

    template <typename Image, typename Isubset, typename Value>
    struct ch_value< morpher::add_isubset<Image, Isubset>, Value >
    {
      typedef typename ch_value<Image, Value>::ret ImageV;
      typedef morpher::add_isubset<ImageV, Isubset> ret;
    };


    // identity.

    template <typename Image, typename Value>
    struct ch_value< morpher::identity<Image>, Value >
    {
      typedef typename ch_value<Image, Value>::ret ret;
    };


    // thru_fun.

    template <typename Image, typename Fun, typename Value>
    struct ch_value< morpher::thru_fun<Image, Fun>, Value >
    {
      typedef typename ch_value<Image, Value>::ret ret;
    };


    // value_cast.

    template <typename Image, typename V, typename Value>
    struct ch_value< morpher::value_cast<Image, V>, Value >
    {
      typedef typename ch_value<Image, Value>::ret ret;
    };


  } // end of namespace type_fun

} // end of namespace oln


#endif // ! OLN_CORE_TYPE_FUN_CH_VALUE_HH
