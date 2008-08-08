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

/// \file neighb2d.i
/// \brief A wrapper of mln::neighb2d.

%module neighb2d

%import "dpoint2d.i"

%{
#include "mln/core/neighb2d.hh"
%}

// FIXME: Move mln::neighb_ to its own file.

%include "mln/core/concept/neighborhood.hh"

// FIXME: Wrap mln::neighb by hand, for Milena macros disturb swig.
// Annotate the original source code instead?
namespace mln
{

  // fwd decls
  template <typename D> class dpoints_fwd_piter;
  template <typename D> class dpoints_bkd_piter;
 

  /*! \brief Generic neighborhood class.
   *
   * This neighborhood of window is just like a set of delta-points.
   * The parameter is \c D, type of delta-point.
   */
  template <typename D>
  struct neighb_ : public Neighborhood< neighb_<D> >,
		   public internal::dpoints_base_<D, neighb_<D> >
  {
    /// Dpoint associated type.
    typedef D dpoint;

// FIXME: The macro mln_point disturbs swig.
//     /// Point associated type.
//     typedef mln_point(D) point;
    typedef typename D::point point;

    /*! \brief Point_Iterator type to browse the points of a generic
     * neighborhood w.r.t. the ordering of delta-points.
     */
    typedef dpoints_fwd_piter<D> fwd_niter;

    /*! \brief Point_Iterator type to browse the points of a generic
     * neighborhood w.r.t. the reverse ordering of delta-points.
     */
    typedef dpoints_bkd_piter<D> bkd_niter;

    /*! \brief Same as fwd_niter.
     */
    typedef fwd_niter niter;

    /*! \brief Constructor without argument.
     *
     * The constructed neighborhood is empty. You have to use insert()
     * to proceed to the neighborhood definition.
     */
    neighb_();

    /*! \brief Insert a delta-point \p dp in the neighborhood
     *  definition.
     *
     * \param[in] dp The delta-point to insert.
     *
     * This method also insert the symmetrical delta-point, - \p dp,
     * in the neighborhood definition; thus the client has not to
     * ensure the symmetry property; that is automatic.
     */
    neighb_<D>& insert(const D& dp);

// FIXME: Swig tries to wrap everything by default; prevent it from wrapping
// invalid methods (1D and 3D ctors for a point2d).
    /// \{ Insertion of a delta-point with different numbers of
    /// arguments (coordinates) w.r.t. the dimension.
//     neighb_<D>& insert(const mln_coord(D)& dind); // For 1D.

// FIXME: The macro mln_coord disturbs swig.
//     neighb_<D>& insert(const mln_coord(D)& drow,
// 		       const mln_coord(D)& dcol); // For 2D.
    neighb_<D>& insert(const typename D::coord& drow,
		       const typename D::coord& dcol); // For 2D.

//     neighb_<D>& insert(const mln_coord(D)& dsli,
// 		       const mln_coord(D)& drow,
// 		       const mln_coord(D)& dcol); // For 3D.
    /// \}
  };
 
} // end of namespace mln

%include "mln/core/neighb2d.hh"

%template(neighb2d) mln::neighb_< mln::dpoint_<mln::grid::square, int > >;
