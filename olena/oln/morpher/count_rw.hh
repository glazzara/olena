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

#ifndef OLN_MORPHER_COUNT_RW_HH
# define OLN_MORPHER_COUNT_RW_HH

# include <oln/value/rw_counter.hh>
# include <oln/morpher/internal/image_value_morpher.hh>


namespace oln
{


  namespace morpher
  {
    // Forward declaration.
    template <typename Image> struct count_rw;

  } // end of namespace oln::morpher


  /// Super type.
  template <typename Image>
  struct set_super_type< morpher::count_rw<Image> >
  {
    typedef morpher::count_rw<Image> self_t;
    typedef morpher::internal::image_value_morpher<Image, self_t> ret;
  };


  template <typename Image>
  struct vtypes< morpher::count_rw<Image> >
  {
  public:
    typedef mlc::true_ is_computed_type;

    typedef oln_value(Image)          value_type;
    typedef value::rw_counter<Image> lvalue_type;
  };

  template <typename Image>
  struct single_vtype< morpher::count_rw<Image>, typedef_::rvalue_type >
  {
    typedef value::ro_counter<Image> ret;
  };
  


  namespace morpher
  {

    /// 'Image thru Function' morpher.
    template <typename Image>
    class count_rw : public internal::image_value_morpher< Image,
							   morpher::count_rw<Image> >
    {
    private:

      typedef count_rw<Image> self_t;
      typedef internal::image_value_morpher<Image, self_t> super_t;

      using super_t::image_;

    public:

      count_rw(Image& image);
      count_rw(oln::abstract::mutable_image<Image>& image);

      oln_rvalue(self_t) impl_op_read(const oln_psite(self_t)& p) const;
      oln_lvalue(self_t) impl_op_readwrite(const oln_psite(self_t)& p);
    };


    namespace ERROR
    {

      struct FIXME;

    } // end of namespace oln::morpher::ERROR


# ifndef OLN_INCLUDE_ONLY

    // public

    template <typename Image>
    count_rw<Image>::count_rw(Image& image) :
      super_t(image)
    {
    }

    template <typename Image>
    count_rw<Image>::count_rw(oln::abstract::mutable_image<Image>& image) :
      super_t(image.exact())
    {
    }
    
    template <typename Image>
    oln_rvalue(count_rw<Image>)
    count_rw<Image>::impl_op_read(const oln_psite(count_rw<Image>)& p) const
    {
      value::ro_counter<Image> tmp(image_, p);
      return tmp;
    }
    
    template <typename Image>
    oln_lvalue(count_rw<Image>)
    count_rw<Image>::impl_op_readwrite(const oln_psite(count_rw<Image>)& p)
    {
      value::rw_counter<Image> tmp(image_, p);
      return tmp;
    }
    
# endif

  } // end of namespace oln::morpher


  template <typename I>
  morpher::count_rw<I>
  count_rw(oln::abstract::mutable_image<I>& input)
  {
    morpher::count_rw<I> tmp(input.exact());
    return tmp;
  }


} // end of namespace oln


#endif // ! OLN_MORPHER_COUNT_RW_HH
