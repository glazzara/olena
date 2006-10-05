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

#ifndef OLN_CORE_GEN_FWD_QITER_WIN_HH
# define OLN_CORE_GEN_FWD_QITER_WIN_HH

# include <oln/core/abstract/topology.hh>
# include <oln/core/abstract/iterator_on_points.hh>
# include <oln/core/gen/win.hh>


namespace oln
{


  // Forward declaration.
  template <typename point_t> class fwd_qiter_win_;


  // Super type declaration.
  template <typename point_t>
  struct set_super_type< fwd_qiter_win_<point_t> >
  {
    typedef fwd_qiter_win_<point_t> self_t;
    typedef abstract::iterator_on_points<self_t> ret;
  };


  /// Virtual types associated to oln::fwd_qiter_win_<point_t>.
  template <typename point_t>
  struct vtypes< fwd_qiter_win_<point_t> >
  {
    typedef point_t point_type;
    typedef oln_type_of(point_t, grid) grid_type;
  };



  /// Abstract forward point iterator class.
  template <typename point_t>
  class fwd_qiter_win_ : public abstract::iterator_on_points< fwd_qiter_win_<point_t> >
  {
    typedef fwd_qiter_win_<point_t> self_t;
    typedef abstract::iterator_on_points<self_t> super_t;

    typedef oln_type_of(point_t, dpoint) dpoint_t;

  public:
    
    template <typename P, typename D>
    fwd_qiter_win_(const abstract::iterator_on_points<P>& it,
		   const window_<D>& win)
      : p_ref_(it.point_adr()),
	win_(win)
    {
      precondition(win_.card() > 0);
      i_ = -1;
    }
    
    template <typename P, typename D>
    fwd_qiter_win_(const abstract::point<P>& p,
		   const window_<D>& win)
      : p_ref_(&(p.exact())),
	win_(win)
    {
      precondition(win_.card() > 0);
      i_ = -1;
    }

    void impl_start()
    {
      i_ = 0;
      p_ = *p_ref_+ win_.dp(i_);
    }

    void impl_next()
    {
      ++i_;
      if (i_ == int(win_.card()))
	{
	  i_ = -1;
	  return;
	}
      p_ = *p_ref_ + win_.dp(i_);
    }

    void impl_invalidate()
    {
      i_ = -1;
    }

    bool impl_is_valid() const
    {
      return i_ != -1;
    }

    point_t impl_to_point() const
    {
      return p_;
    }

    const point_t* impl_point_adr() const
    {
      return &p_;
    }

  protected:

    const point_t* p_ref_;
    window_<dpoint_t> win_;
    int i_;
    point_t p_;

  }; // end of class oln::fwd_qiter_win_<point_t>
  

} // end of namespace oln


#endif // ! OLN_CORE_GEN_FWD_QITER_WIN_HH

