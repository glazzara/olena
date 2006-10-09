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

#ifndef OLN_CORE_GEN_FWD_NITER_NEIGHB_HH
# define OLN_CORE_GEN_FWD_NITER_NEIGHB_HH

# include <oln/core/abstract/topology.hh>
# include <oln/core/abstract/iterator_on_points.hh>
# include <oln/core/gen/neighb.hh>


namespace oln
{


  // Forward declaration.
  template <typename point_t> class fwd_niter_neighb_;


  // Super type declaration.
  template <typename point_t>
  struct set_super_type< fwd_niter_neighb_<point_t> >
  {
    typedef fwd_niter_neighb_<point_t> self_t;
    typedef abstract::iterator_on_points<self_t> ret;
  };


  /// Virtual types associated to oln::fwd_niter_neighb_<point_t>.
  template <typename point_t>
  struct vtypes< fwd_niter_neighb_<point_t> >
  {
    typedef point_t point_type;
    typedef oln_type_of(point_t, grid)  grid_type;
  };



  /// Abstract forward point iterator class.
  template <typename point_t>
  class fwd_niter_neighb_ : public abstract::iterator_on_points< fwd_niter_neighb_<point_t> >
  {
    typedef fwd_niter_neighb_<point_t> self_t;
    typedef abstract::iterator_on_points<self_t> super_t;

    typedef oln_type_of(point_t, dpoint) dpoint_t;

  public:
    
    template <typename P, typename T>
    fwd_niter_neighb_(const abstract::iterator_on_points<P>& it,
		      const abstract::topology<T>& topo);

    //     template <typename P, typename N>
    //     fwd_niter_neighb_(const abstract::iterator_on_points<P>& it,
    // 		      const abstract::neighborhood<N>& nbh)
    //       : p_ref_(it.point_adr()),
    // 	nbh_(nbh.exact())
    //     {
    //       precondition(nbh_.card() > 0);
    //       i_ = -1;
    //     }

    //     template <typename P, typename T>
    //     fwd_niter_neighb_(const abstract::point<P>& p,
    // 		      const abstract::topology<T>& topo)
    //       : p_ref_(&(p.exact())),
    // 	nbh_(topo.exact().neighborhood())
    //     {
    //       precondition(nbh_.card() > 0);
    //       i_ = -1;
    //     }

    //     template <typename P, typename N>
    //     fwd_niter_neighb_(const abstract::point<P>& p,
    // 		      const abstract::neighborhood<N>& nbh)
    //       : p_ref_(&(p.exact())),
    // 	nbh_(nbh.exact())
    //     {
    //       precondition(nbh_.card() > 0);
    //       i_ = -1;
    //     }

    void impl_start();

    void impl_next();

    void impl_invalidate();

    bool impl_is_valid() const;

    point_t impl_to_point() const;

    const point_t* impl_point_adr() const;

  protected:

    const point_t* p_ref_;
    neighb_<dpoint_t> nbh_;
    int i_;
    point_t p_;

  }; // end of class oln::fwd_niter_neighb_<point_t>
  



# ifndef OLN_INCLUDE_ONLY

  template <typename point_t>
  template <typename P, typename T>
  fwd_niter_neighb_<point_t>::fwd_niter_neighb_(const abstract::iterator_on_points<P>& it,
						const abstract::topology<T>& topo)
    : p_ref_(it.point_adr()),
      nbh_(topo.exact().neighborhood())
  {
    precondition(nbh_.card() > 0);
    i_ = -1;
  }

  template <typename point_t>
  void
  fwd_niter_neighb_<point_t>::impl_start()
  {
    i_ = 0;
    p_ = *p_ref_+ nbh_.dp(i_);
  }

  template <typename point_t>
  void
  fwd_niter_neighb_<point_t>::impl_next()
  {
    ++i_;
    if (i_ == int(nbh_.card()))
      {
	i_ = -1;
	return;
      }
    p_ = *p_ref_ + nbh_.dp(i_);
  }

  template <typename point_t>
  void
  fwd_niter_neighb_<point_t>::impl_invalidate()
  {
    i_ = -1;
  }

  template <typename point_t>
  bool
  fwd_niter_neighb_<point_t>::impl_is_valid() const
  {
    return i_ != -1;
  }

  template <typename point_t>
  point_t
  fwd_niter_neighb_<point_t>::impl_to_point() const
  {
    return p_;
  }

  template <typename point_t>
  const point_t*
  fwd_niter_neighb_<point_t>::impl_point_adr() const
  {
    return &p_;
  }

# endif


} // end of namespace oln


#endif // ! OLN_CORE_GEN_FWD_NITER_NEIGHB_HH

