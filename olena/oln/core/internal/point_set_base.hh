// Copyright (C) 2006, 2007 EPITA Research and Development Laboratory
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

#ifndef OLN_CORE_INTERNAL_POINT_SET_BASE_HH
# define OLN_CORE_INTERNAL_POINT_SET_BASE_HH

# include <oln/core/concept/point_set.hh>


namespace oln
{


  namespace internal
  {

    template <typename Box_ptr, typename Exact>
    struct point_set_selector_ : public Point_Set<Exact>
    {};

    template <typename Exact>
    struct point_set_selector_< Exact*, Exact > : public Box<Exact>
    {};

  } // end of namespace oln::internal



  // point_set_base_ class


  // Fwd decls.
  namespace internal { template <typename Exact> struct point_set_base_; }
  template <typename P> class gen_box;


  // Super type.
  template <typename Exact>
  struct super_trait_< internal::point_set_base_<Exact> >
  {
    typedef internal::point_set_selector_< stc_deferred(box)*, Exact > ret;
  };


  // Virtual types.
  template <typename Exact>
  struct vtypes< internal::point_set_base_<Exact> >
  {
    typedef stc::abstract point;
    typedef stc::abstract fwd_piter;
    typedef stc::abstract bkd_piter;
    typedef stc::abstract box;

    typedef stc_deferred(point)     point__;
    typedef stc_deferred(fwd_piter) fwd_piter__;

    typedef stc::final< stc::is<Point_Set> > category;
    typedef stc::final< oln_grid(point__) >  grid;
    typedef stc::final< fwd_piter__ >        piter;
  };


  namespace internal
  {

    /// Base class for point sets.

    template <typename Exact>
    struct point_set_base_ : public super_trait_< point_set_base_<Exact> >::ret
    {
    protected:
      point_set_base_();
    };


# ifndef OLN_INCLUDE_ONLY

    template <typename Exact>
    point_set_base_<Exact>::point_set_base_()
    {
    }

# endif

  } // end of namespace oln::internal

} // end of namespace oln



/// \{
/// FIXME: Bad!

# include <oln/core/gen/fun.hh>
# include <oln/core/internal/op_pset_such_as_fp2b.hh>

namespace oln
{

  // Point_Set | Function.

  oln_decl_op_such_as(Point_Set, Function_p2b);


  // Specialization.

  template <typename S, typename B, typename P>
  op_<const S, such_as, const fun_p2b_<B (*)(P)> >
  operator | (const Point_Set<S>& lhs, B (*f)(P))
  {
    typedef oln_strip_(P) P_;
    mlc::assert_< mlc_is_a(P_, Point) >::check(); // FIXME: Add err msg.
    mlc::assert_equal_< P_, oln_point(S) >::check();
    op_<const S, such_as, const fun_p2b_<B (*)(P)> > tmp(exact(lhs), f);
    return tmp;
  }


} // end of namespace oln

/// \}


#endif // ! OLN_CORE_INTERNAL_POINT_SET_BASE_HH
