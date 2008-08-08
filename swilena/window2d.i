//						       		-*- C++ -*-
// Copyright (C) 2008 EPITA Research and Development Laboratory (LRDE)
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
// License.
// reasons why the executable file might be covered by the GNU General
// Public License.

/// \file window2d.i
/// \brief A wrapper of mln::window2d.

%module window2d

%import "dpoint2d.i"

%{
#include "mln/core/window2d.hh"
%}

// FIXME: Move mln::window to its own file.

%include "mln/core/concept/window.hh"

// FIXME: Wrap mln::window by hand, for Milena macros disturb swig.
// Annotate the original source code instead?
namespace mln
{
  template <typename D>
  class window : public Window< window<D> >,
		 public internal::dpoints_base_<D, window<D> >
  {
    typedef internal::dpoints_base_<D, window<D> > super_;
  public:

    /*! \brief Point_Iterator type to browse the points of a generic window
     * w.r.t. the ordering of delta-points.
     */
    typedef dpoints_fwd_piter<D> fwd_qiter;

    /*! \brief Point_Iterator type to browse the points of a generic window
     * w.r.t. the reverse ordering of delta-points.
     */
    typedef dpoints_bkd_piter<D> bkd_qiter;


    /*! \brief Constructor without argument.
     *
     * The constructed window is empty. 
     */
    window();


    /*! \brief Test if the window is symmetric.
     */
    bool is_symmetric() const;

    /// Insert a delta-point \p dp.
    window<D>& insert(const D& dp);

// FIXME: Swig tries to wrap everything by default; prevent it from wrapping
// invalid methods (1D and 3D ctors for a point2d).
    /// \{ Insertion of a delta-point with different numbers of
    /// arguments (coordinates) w.r.t. the dimension.
//     window<D>& insert(const mln_coord(D)& dind); // For 1D.

// FIXME: The macro mln_coord disturbs swig.
//     window<D>& insert(const mln_coord(D)& drow,
// 		      const mln_coord(D)& dcol); // For 2D.
    window<D>& insert(const typename D::coord& drow,
 		      const typename D::coord& dcol); // For 2D.

//     window<D>& insert(const mln_coord(D)& dsli,
// 		      const mln_coord(D)& drow,
// 		      const mln_coord(D)& dcol); // For 3D.
//     /// \}

    /// Apply a central symmetry to the target window.
    window<D>& sym();

  protected:
    
// FIXME: The macro used here disturbs swig.
//     box_<mln_point(D)> b_;
    box_<typename D::point> b_;
  };

} // end of namespace mln

%include "mln/core/window2d.hh"

%template(window2d) mln::window< mln::dpoint_<mln::grid::square, int > >;
