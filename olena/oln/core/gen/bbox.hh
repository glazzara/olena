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

#ifndef OLENA_CORE_GEN_BBOX_HH
# define OLENA_CORE_GEN_BBOX_HH

# include <oln/core/abstract/bbox.hh>
# include <oln/core/abstract/point.hh>


namespace oln
{

  // Forward declarations.
  template <typename point> class bbox_;
  template <typename point> class fwd_piter_;
  template <typename point> class bkd_piter_;


  // Super type declaration.
  template <typename point>
  struct set_super_type< bbox_<point> >
  {
    typedef bbox_<point> self_t;
    typedef abstract::bbox<self_t> ret;
  };


  /// Virtual types associated to oln::bbox_<point>.
  template <typename point>
  struct vtypes_< bbox_<point> >
  {
    typedef point point_type;
    typedef fwd_piter_<point> fwd_piter_type;
    typedef bkd_piter_<point> bkd_piter_type;
    typedef typename point::coord_t coord_type;
    // BUG! typedef oln_type_of(point, grid) grid_type;
    // BUG! typedef typename point::grid_t grid_type;
  };


  /// Bounding box class based on a point class.
  template <typename point>
  class bbox_ : public abstract::bbox< bbox_<point> >,
		private mlc::assert_< mlc_is_a(point, abstract::point) >
  {
    typedef point point_t;
    typedef bbox_<point> self_t;
    typedef abstract::bbox<self_t> super_t;

  public:
      
    bbox_()
    {
    }

    bbox_(const point_t& pmin, const point_t& pmax)
      : super_t(pmin, pmax)
    {
    }

  }; // end of class oln::bbox_<point>


} // end of namespace oln


#endif // ! OLENA_CORE_GEN_BBOX_HH
