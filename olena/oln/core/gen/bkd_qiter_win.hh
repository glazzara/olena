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

#ifndef OLN_CORE_GEN_BKD_QITER_WIN_HH
# define OLN_CORE_GEN_BKD_QITER_WIN_HH

# include <oln/core/abstract/iterator_on_points.hh>
# include <oln/core/abstract/window.hh>
# include <oln/core/gen/window.hh>


namespace oln
{


  // Backward declaration.
  template <typename point_t> class bkd_qiter_win_;


  // Super type declaration.
  template <typename point_t>
  struct set_super_type< bkd_qiter_win_<point_t> >
  {
    typedef bkd_qiter_win_<point_t> self_t;
    typedef abstract::iterator_on_points<self_t> ret;
  };


  /// Virtual types associated to oln::bkd_qiter_win_<point_t>.
  template <typename point_t>
  struct vtypes< bkd_qiter_win_<point_t> >
  {
    typedef point_t point_type;
    typedef oln_grid(point_t) grid_type;
  };



  /// Abstract backward point iterator class.
  template <typename point_t>
  class bkd_qiter_win_ : public abstract::iterator_on_points< bkd_qiter_win_<point_t> >
  {
    typedef bkd_qiter_win_<point_t> self_t;
    typedef abstract::iterator_on_points<self_t> super_t;

    typedef oln_dpoint(point_t) dpoint_t;

  public:
    
    template <typename P, typename W>
    bkd_qiter_win_(const abstract::iterator_on_points<P>& it,
		   const abstract::window<W>& win);

    template <typename P, typename W>
    bkd_qiter_win_(const abstract::point<P>& p,
		   const abstract::window<W>& win);

    void impl_start();

    void impl_next();

    void impl_invalidate();

    bool impl_is_valid() const;

    point_t impl_to_point() const;

    const point_t* impl_point_adr() const;

  protected:

    const point_t* p_ref_;
    window_<dpoint_t> win_;
    int i_;
    point_t p_;

  }; // end of class oln::bkd_qiter_win_<point_t>

  

# ifndef OLN_INCLUDE_ONLY

  template <typename point_t>
  template <typename P, typename W>
  bkd_qiter_win_<point_t>::bkd_qiter_win_(const abstract::iterator_on_points<P>& it,
					  const abstract::window<W>& win)
    : p_ref_(it.point_adr()),
      win_(win.exact())
  {
    precondition(win_.card() > 0);
    this->invalidate();
  }
    
  template <typename point_t>
  template <typename P, typename W>
  bkd_qiter_win_<point_t>::bkd_qiter_win_(const abstract::point<P>& p,
					  const abstract::window<W>& win)
    : p_ref_(&(p.exact())),
      win_(win.exact())
  {
    precondition(win_.card() > 0);
    this->invalidate();
  }

  template <typename point_t>
  void
  bkd_qiter_win_<point_t>::impl_start()
  {
    i_ = int(win_.card()) - 1;
    p_ = *p_ref_+ win_.dp(i_);
  }

  template <typename point_t>
  void
  bkd_qiter_win_<point_t>::impl_next()
  {
    --i_;
    if (i_ == -1)
      return;
    p_ = *p_ref_ + win_.dp(i_);
  }

  template <typename point_t>
  void
  bkd_qiter_win_<point_t>::impl_invalidate()
  {
    i_ = -1;
  }

  template <typename point_t>
  bool
  bkd_qiter_win_<point_t>::impl_is_valid() const
  {
    return i_ != -1;
  }

  template <typename point_t>
  point_t
  bkd_qiter_win_<point_t>::impl_to_point() const
  {
    return p_;
  }

  template <typename point_t>
  const point_t*
  bkd_qiter_win_<point_t>::impl_point_adr() const
  {
    return &p_;
  }

# endif

} // end of namespace oln


#endif // ! OLN_CORE_GEN_BKD_QITER_WIN_HH

