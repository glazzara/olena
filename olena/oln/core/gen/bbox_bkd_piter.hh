// Copyright (C) 2001, 2003, 2004, 2005, 2006 EPITA Research and
// Development Laboratory
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

#ifndef OLN_CORE_GEN_BBOX_BKD_PITER_HH
# define OLN_CORE_GEN_BBOX_BKD_PITER_HH

# include <oln/core/internal/bbox_bkd_piter.hh>


namespace oln
{


  // Forward declaration.
  template <typename point> class bbox_bkd_piter_;


  // Super type declaration.
  template <typename point>
  struct set_super_type< bbox_bkd_piter_<point> >
  {
    typedef bbox_bkd_piter_<point> self_t;
    typedef internal::bbox_bkd_piter<self_t> ret;
  };


  /// Virtual types associated to oln::bbox_bkd_piter_<point>.
  template <typename point>
  struct vtypes< bbox_bkd_piter_<point> >
  {
    typedef point point_type;
    typedef oln_type_of(point, grid) grid_type;
  };



  /// Abstract forward point iterator class.
  template <typename point>
  class bbox_bkd_piter_ : public internal::bbox_bkd_piter< bbox_bkd_piter_<point> >,
			  private mlc::assert_< mlc_is_a(point, abstract::point) >
  {
    typedef bbox_bkd_piter_<point> self_t;
    typedef internal::bbox_bkd_piter<self_t> super_t;

  public:
    
    bbox_bkd_piter_(const bbox_<point>& bb);
    
  }; // end of class oln::bbox_bkd_piter_<point>
  

# ifndef OLN_INCLUDE_ONLY

  template <typename point>
  bbox_bkd_piter_<point>::bbox_bkd_piter_(const bbox_<point>& bb)
    : super_t(bb)
  {
  }

# endif

} // end of namespace oln


#endif // ! OLN_CORE_GEN_BBOX_BKD_PITER_HH
