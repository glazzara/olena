// Copyright (C) 2005, 2006, 2007 EPITA Research and Development
// Laboratory
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

#ifndef OLN_CORE_GEN_PW_VALUE_HH
# define OLN_CORE_GEN_PW_VALUE_HH

# include <oln/core/concept/image.hh>
# include <oln/core/gen/fun_ops.hh>



namespace oln
{

  namespace ERROR
  {
    template <typename I>
    struct pw_value_works_on_images_not_on_;

  } // end of namespace oln::ERROR


  // Fwd decl.
  template <typename I> class pw_value_;


  namespace internal
  {

    template <bool b, typename I>
    struct pw_value_base_;

    template <typename I>
    struct pw_value_base_< true, I > : public Function_p2b< pw_value_<I> >
    {
    };

    template <typename I>
    struct pw_value_base_< false, I > : public Function_p2v< pw_value_<I> >
    {
    };

  } // end of namespace oln::internal


  template <typename I>
  class pw_value_
    :
    public internal::pw_value_base_< mlc_is_a(I, Binary_Image)::eval::value,
				     I >
  {
  public:
    typedef oln_point(I) argument; // FIXME: psite?
    typedef oln_value(I) result;

    pw_value_(const Image<I>& ima);
    oln_value(I) operator()(const oln_point(I)& p) const;

    const I& image() const;

  protected:
    const I& ima_;
  };



  template <typename I>
  pw_value_<I> pw_value(const Image<I>& ima);



# ifndef OLN_INCLUDE_ONLY

  template <typename I>
  pw_value_<I>::pw_value_(const Image<I>& ima)
    : ima_(exact(ima))
  {
    mlc::assert_< mlc_is_a(I, Point_Wise_Accessible_Image) >::check();
    // FIXME: Add err msg above.
  }

  template <typename I>
  oln_value(I)
  pw_value_<I>::operator()(const oln_point(I)& p) const
  {
    precondition(this->ima_.owns_(p));
    return this->ima_(p);
  }

  template <typename I>
  const I&
  pw_value_<I>::image() const
  {
    return this->ima_;
  }

  // pw_value

  template <typename I>
  pw_value_<I>
  pw_value(const Image<I>& ima)
  {
    mlc::assert_< mlc_is_a(I, Point_Wise_Accessible_Image) >::check();
    // FIXME: Add err msg above.
    pw_value_<I> tmp(ima);
    return tmp;
  }

# endif // ! OLN_INCLUDE_ONLY

} // end of namespace oln


#endif // ! OLN_CORE_GEN_PW_VALUE_HH
