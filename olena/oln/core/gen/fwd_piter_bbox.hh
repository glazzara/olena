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

#ifndef OLN_CORE_GEN_FWD_PITER_BBOX_HH
# define OLN_CORE_GEN_FWD_PITER_BBOX_HH

# include <oln/core/abstract/topology.hh>
# include <oln/core/abstract/point.hh>
# include <oln/core/internal/bbox_fwd_piter.hh>


namespace oln
{


  // Forward declaration.
  template <typename point_t> class fwd_piter_bbox_;


  // Super type declaration.
  template <typename point_t>
  struct set_super_type< fwd_piter_bbox_<point_t> >
  {
    typedef fwd_piter_bbox_<point_t> self_t;
    typedef internal::bbox_fwd_piter<self_t> ret;
  };


  /// Virtual types associated to oln::fwd_piter_bbox_<point_t>.
  template <typename point_t>
  struct vtypes< fwd_piter_bbox_<point_t> >
  {
    typedef point_t point_type;
    typedef oln_type_of(point_t, grid) grid_type;
  };



  /// Abstract forward point iterator class.
  template <typename point_t>
  class fwd_piter_bbox_ : public internal::bbox_fwd_piter< fwd_piter_bbox_<point_t> >,
			  private mlc::assert_< mlc_is_a(point_t, abstract::point) >
  {
    typedef fwd_piter_bbox_<point_t> self_t;
    typedef internal::bbox_fwd_piter<self_t> super_t;

  public:
    
    template <typename T>
    fwd_piter_bbox_(const abstract::topology<T>& topo)
      : super_t(topo.exact().bbox())
    {
    }

    void print(std::ostream& ostr) const
    {
      ostr << "{ p=" << this->p_
	   << " }";
    }

    friend
    std::ostream& operator<<(std::ostream& ostr, const fwd_piter_bbox_<point_t>& t)
    {
      t.print(ostr);
      return ostr;
    }

  }; // end of class oln::fwd_piter_bbox_<point>
  

} // end of namespace oln


#endif // ! OLN_CORE_GEN_FWD_PITER_BBOX_HH
