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

#ifndef OLN_CORE_GEN_TOPO_BBOX_HH
# define OLN_CORE_GEN_TOPO_BBOX_HH

# include <oln/core/gen/bbox.hh>
# include <oln/core/topology_entry.hh>


namespace oln
{

  // Forward declaration.
  template <typename point> class topo_bbox_;


  // Super type declaration.
  template <typename point>
  struct set_super_type< topo_bbox_<point> >
  {
    typedef topo_bbox_<point> self_t;
    typedef topology_entry<self_t> ret;
  };


  /// Virtual types associated to oln::topo_bbox_<point>.
  template <typename point>
  struct vtypes< topo_bbox_<point> >
  {
    /* Trick to force the instantiation of bbox_<point>.
       
       As bbox_<point> is a template type used to define the `bbox'
       virtual type of topo_bbox_<point>, it requires some attention
       regarding its instantiation.


       Metalic's mlc_is_a is used by the static hierarchy mechanism of
       Static.  mlc_is_a performs a static typedef introspection,
       taking advantage of the SFINAE rule.  However, there two
       phenomena seem to happen here (probably due to the very nature
       of SFINAE):

       1. the statement `mlc_is_a(B, A)' requires the knowledge of the
          entire declaration of the inspected class to answer
          positively if a `B' actually *is a* `A'.  If B is a template
          class, this implies that B must have been instantiated for
          mlc_is_a to be able to inspect it;

       2. mlc_is_a does *not* trigger the instantiation of the
          inspected type (probably to prevent any error, in accordance
          to the SFINAE rule).

       When a template type is used to define a virtual type, it
       *must* have been instantiated, because it might be used during
       the construction of the base classes of the class it belongs
       to.  That is the case of bbox_<point> here.

       So, how one can trigger such an instantiation?  We used to
       address this problem by using explicit template instantiations
       placed before the definition of the class holding the template
       type used a virtual type :

         template class bbox_<point1d>;
         template class bbox_<point2d>;
         template class bbox_<point3d>;

       And so on.  But this is not good w.r.t. software engineering:
       one must take care of the correct explicit instantiations at
       the right place, which is error-prone --and the C++ compiler
       won't produces easily understandable messages!-- and requires a
       manual addition for each instantiation of the template type
       potentially used as a definition of a virtual type.

       A better solution is to force the compiler to instantiate the
       type at the definition site of the virtual type (here, as for
       bbox_<point>).  Such an instantiation can be triggered by
       requesting the use of a typedef contained within this type.  As
       many template types define a typedef `self_t' to refer to
       themselves, we use this typename to both trigger the
       instantiation of bbox_<point>, and to define
       `vtypes< topo_bbox_<point> >::bbox_type'.  */
    typedef typename bbox_<point>::self_t bbox_type;
    typedef point                         point_type;
    typedef mlc::true_                    is_random_accessible_type;
  };


  /// Bounding box topology based on a point class.
  template <typename point>
  class topo_bbox_ :
    public topology_entry< topo_bbox_<point> >,
    private mlc::assert_< mlc_is_a(point, abstract::point) >
  {
    typedef topo_bbox_<point> self_t;
    typedef topology_entry<self_t> super_t;
    typedef oln_vtype(self_t, bbox) bbox_t;

  public:

    topo_bbox_();
    topo_bbox_(const bbox_t& bb);
    topo_bbox_(const point& pmin, const point& pmax);

    const bbox_t& impl_bbox() const;
    bbox_t& impl_bbox();

    bbox_t& impl_lbbox();
    
    bool impl_has(const point& p) const;
    bool impl_has_large(const point& p) const;

  protected:

    bbox_<point> bb_;

  };



# ifndef OLN_INCLUDE_ONLY

  template <typename point>
  topo_bbox_<point>::topo_bbox_()
  {
  }

  template <typename point>
  topo_bbox_<point>::topo_bbox_(const typename topo_bbox_<point>::bbox_t& bb)
    : bb_(bb)
  {
  }

  template <typename point>
  topo_bbox_<point>::topo_bbox_(const point& pmin, const point& pmax)
    : bb_(pmin, pmax)
  {
  }

  template <typename point>
  const typename topo_bbox_<point>::bbox_t&
  topo_bbox_<point>::impl_bbox() const
  {
    return bb_;
  }

  template <typename point>
  typename topo_bbox_<point>::bbox_t&
  topo_bbox_<point>::impl_bbox()
  {
    return bb_;
  }

  template <typename point>
  typename topo_bbox_<point>::bbox_t&
  topo_bbox_<point>::impl_lbbox()
  {
    return bb_;
  }

  template <typename point>
  bool
  topo_bbox_<point>::impl_has(const point& p) const
  {
    return bb_.has(p);
  }

  template <typename point>
  bool
  topo_bbox_<point>::impl_has_large(const point& p) const
  {
    return bb_.has(p);
  }

# endif

} // end of namespace oln


#endif // ! OLN_CORE_GEN_TOPO_BBOX_HH
