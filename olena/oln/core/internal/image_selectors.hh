// Copyright (C) 2007 EPITA Research and Development Laboratory
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

#ifndef OLN_CORE_INTERNAL_IMAGE_SELECTORS_HH
# define OLN_CORE_INTERNAL_IMAGE_SELECTORS_HH

# include <oln/core/concept/image.hh>
# include <oln/core/2d/grid2d.hh>
# include <oln/value/tags.hh>


namespace oln
{

  namespace internal
  {


    // 1. mutability

    typedef  selector<Image, 1>  Image_mutability;
  
    template <typename Exact>
    struct case_< Image_mutability, Exact,  1 >
      :
      where_< stc_type_is_found(lvalue) >
    {
      typedef Mutable_Image<Exact> ret;
    };

    template <typename Exact>
    struct case_< Image_mutability, Exact,  2 >
      :
      where_< mlc::and_< stc_type_is_found(lvalue),
			 stc_type_is_found(index) > > // FIXME: Wrong!
    {
      typedef Fast_Image<Exact> ret;
    };


    // 2. dimension

    typedef  selector<Image, 2>  Image_dimension;
  
    template <typename Exact>
    struct case_< Image_dimension, Exact,  1 >
      :
      where_< stc_is_a(grid, Grid_2D) >
    {
      typedef Image_2D<Exact> ret;
    };

    // FIXME: ...


    // 3. point-wise accessibility

    typedef  selector<Image, 3>  Image_pw_accessibility;
  
    template <typename Exact>
    struct case_< Image_pw_accessibility, Exact,  1 >
      :
      where_< mlc::and_list_< mlc::eq_< stc_get_type(psite), stc_get_type(point) >,
			      stc_type_is_found(lvalue),
			      stc_is_a(grid, Grid_2D) > >
    {
      typedef Point_Wise_Mutable_Image_2D<Exact> ret;
    };

    template <typename Exact>
    struct case_< Image_pw_accessibility, Exact,  2 >
      :
      where_< mlc::and_< mlc::eq_< stc_get_type(psite), stc_get_type(point) >,
			 stc_is_a(grid, Grid_2D) > >
    {
      typedef Point_Wise_Accessible_Image_2D<Exact> ret;
    };

    // FIXME: ...

    template <typename Exact>
    struct case_< Image_pw_accessibility, Exact,  3 >
      :
      where_< mlc::eq_< stc_get_type(psite), stc_get_type(point) > >
    {
      typedef Point_Wise_Accessible_Image<Exact> ret;
    };


    // 4. nbh

    typedef  selector<Image, 4>  Image_nbh;

    template <typename Exact>
    struct case_< Image_nbh, Exact,  1 >
      :
      where_< stc_type_is_found(nbh) >
    {
      typedef Image_with_Nbh<Exact> ret;
    };


    // 5. border

    typedef  selector<Image, 5>  Image_border;

    template <typename Exact>
    struct case_< Image_border, Exact,  1 >
      :
      where_< stc_type_is_found(vborder) >
    {
      typedef Image_with_Border<Exact> ret;
    };


    // 6. value kind

    typedef  selector<Image, 6>  Image_value_kind;

    template <typename Exact>
    struct case_< Image_value_kind, Exact,  1 >
      :
      where_< value::is_gray_level<stc_get_type(value)> >
    {
      typedef Gray_Level_Image<Exact> ret;
    };

    template <typename Exact>
    struct case_< Image_value_kind, Exact,  2 >
      :
      where_< value::is_binary<stc_get_type(value)> >
    {
      typedef Binary_Image<Exact> ret;
    };

    template <typename Exact>
    struct case_< Image_value_kind, Exact,  3 >
      :
      where_< value::is_string<stc_get_type(value)> >
    {
      typedef String_Image<Exact> ret;
    };

    template <typename Exact>
    struct case_< Image_value_kind, Exact,  4 >
      :
      where_< value::is_label<stc_get_type(value)> >
    {
      typedef Label_Image<Exact> ret;
    };

    template <typename Exact>
    struct case_< Image_value_kind, Exact,  5 >
      :
      where_< value::is_color<stc_get_type(value)> >
    {
      typedef Color_Image<Exact> ret;
    };

    // FIXME: Deformation_Field_Image

    template <typename Exact>
    struct default_case_< Image_value_kind, Exact >
    {
      typedef Data_Image<Exact> ret;
    };


  } // end of namespace oln::internal
  
} // end of namespace oln


#endif // ! OLN_CORE_INTERNAL_IMAGE_SELECTORS_HH
