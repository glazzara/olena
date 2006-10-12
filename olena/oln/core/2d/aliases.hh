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

#ifndef OLN_CORE_2D_ALIASES_HH
# define OLN_CORE_2D_ALIASES_HH


namespace oln
{

  /// \{
  /// Forward declarations.
  template <typename C> class point2d_;
  template <typename C> class dpoint2d_;
  template <typename D> class neighb_;
  template <typename D> class window_;
  template <typename P> class bbox_;
  template <typename P> class topo_lbbox_;
  template <typename P> class fwd_piter_bbox_;
  template <typename P> class bkd_piter_bbox_;
  template <typename P> class fwd_qiter_win_;
// FIXME: Not yet available.
// template <typename P> class bkd_qiter_win_;
  class grid2d;
  /// \}


  /// \{
  /// Alliases.
  typedef  point2d_<int>  point2d;
  typedef dpoint2d_<int> dpoint2d;

  typedef neighb_<dpoint2d> neighb2d;
  typedef window_<dpoint2d> window2d;

  typedef bbox_<point2d> bbox2d;
  typedef topo_lbbox_<point2d> topo2d;

  typedef fwd_piter_bbox_<point2d> fwd_piter2d;
  typedef bkd_piter_bbox_<point2d> bkd_piter2d;

  typedef fwd_qiter_win_<point2d> fwd_qiter2d;
// FIXME: Not yet available.
// typedef bkd_qiter_win_<point2d> fwd_qiter2d;

  typedef  point2d_<float>  point2df;
  typedef dpoint2d_<float> dpoint2df;
  /// \}


} // end of namespace oln


#endif // ! OLN_CORE_2D_ALIASES_HH
