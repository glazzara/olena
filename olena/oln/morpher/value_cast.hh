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

#ifndef OLN_MORPHER_VALUE_CAST
# define OLN_MORPHER_VALUE_CAST

# include <xtd/abstract/open_nary_fun.hh>
# include <xtd/res.hh>
# include <oln/morpher/internal/image_value_morpher.hh>



namespace oln
{

  namespace morpher
  {
    // Forward declaration.
    template <typename Image, typename Value> struct value_cast;

  } // end of namespace oln::morpher


  /// Super type.
  template <typename Image, typename Value>
  struct set_super_type< morpher::value_cast<Image, Value> >
  {
    typedef morpher::value_cast<Image, Value> self_t;
    typedef morpher::internal::image_value_morpher<Image, self_t> ret;
  };


  template <typename Image, typename Value>
  struct vtypes< morpher::value_cast<Image, Value> >
  {
    typedef mlc::true_ is_computed_type;
    typedef Value       value_type;
  };

  template <typename Image, typename Value>
  struct single_vtype< morpher::value_cast<Image, Value>, typedef_::rvalue_type >
  {
    typedef Value ret;
  };
  


  namespace morpher
  {

    /// 'Image thru Valuection' morpher.
    template <typename Image, typename Value>
    class value_cast : public internal::image_value_morpher< Image,
							     value_cast<Image, Value> >
    {
    private:

      typedef value_cast<Image, Value> self_t;
      typedef internal::image_value_morpher<Image, self_t> super_t;

      typedef oln_type_of(self_t, psite)  psite_t;
      typedef oln_type_of(self_t, rvalue) rvalue_t;

    public:

      value_cast(const Image& image);
    };


# ifndef OLN_INCLUDE_ONLY

    template <typename Image, typename Value>
    value_cast<Image, Value>::value_cast(const Image& image) :
      super_t(image)
    {
    }
    
# endif

  } // end of namespace oln::morpher


  template <typename Value, typename I>
  morpher::value_cast<I, Value>
  value_cast(const abstract::image<I>& ima);


# ifndef OLN_INCLUDE_ONLY

  template <typename Value, typename I>
  morpher::value_cast<I, Value>
  value_cast(const abstract::image<I>& ima)
  {
    morpher::value_cast<I, Value> tmp(ima.exact());
    return tmp;
  }

# endif


} // end of namespace oln



#endif // ! OLN_MORPHER_VALUE_CAST
