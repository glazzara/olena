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

#ifndef OLN_CORE_3D_ALIASES_HH
# define OLN_CORE_3D_ALIASES_HH


namespace oln
{

  /// Forward declarations.
  /// \{
  template <unsigned D> struct grid_;
  template <typename C> class point3d_;
  template <typename C> class dpoint3d_;
  template <typename D> class neighb_;
  template <typename D> class window_;
  template <typename P> class bbox_;
  template <typename P> class topo_lbbox_;
  template <typename T> class fwd_piter_bbox_;
  template <typename T> class bkd_piter_bbox_;
  template <typename P> class fwd_qiter_win_;
// FIXME: Not yet available.
// template <typename P> class bkd_qiter_win_;
  /// \}


  /// Alliases.
  /// \{
  typedef grid_<3> grid3d;

  typedef  point3d_<int>  point3d;
  typedef dpoint3d_<int> dpoint3d;

  typedef neighb_<dpoint3d> neighb3d;
  typedef window_<dpoint3d> window3d;

  typedef bbox_<point3d> bbox3d;
  typedef topo_lbbox_<point3d> topo3d;

  typedef fwd_piter_bbox_<point3d> fwd_piter3d;
  typedef bkd_piter_bbox_<point3d> bkd_piter3d;

  typedef fwd_qiter_win_<point3d> fwd_qiter3d;
// FIXME: Not yet available.
// typedef bkd_qiter_win_<point3d> fwd_qiter3d;

  typedef  point3d_<float>  point3df;
  typedef dpoint3d_<float> dpoint3df;
  /// \}


} // end of namespace oln


#endif // ! OLN_CORE_3D_ALIASES_HH
