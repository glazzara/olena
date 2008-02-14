// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory (LRDE)
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

#ifndef MLN_CORE_LINE_GRAPH_ELT_NEIGHBORHOOD_HH
# define MLN_CORE_LINE_GRAPH_ELT_NEIGHBORHOOD_HH

/*! \file mln/core/line_graph_elt_neighborhood.hh
 *
 *  \brief Definition of the elementary ``window'' on a line graph.
 *
 *  \todo Make naming coherent: we have window (without '_') but
 *  point_, neighb_, etc.
 */

# include <mln/core/concept/neighborhood.hh>
# include <mln/core/line_graph_psite.hh>
# include <mln/core/line_graph_neighborhood_piter.hh>

/* FIXME: Factor those classes:
   - mln::graph_elt_window
   - mln::graph_elt_neighborhood
   - mln::line_graph_elt_window
   - mln::line_graph_elt_neighborhood.  */

namespace mln
{
  // Fwd decls.
  template <typename P> class line_graph_neighborhood_fwd_piter;
  template <typename P> class line_graph_neighborhood_bkd_piter;


  /*! \brief Elementary neighborhood on line graph class.
   *
   *  FIXME: Doc.
   */
  template <typename P>
  class line_graph_elt_neighborhood
    : public Neighborhood< line_graph_elt_neighborhood<P> >
  {
    typedef line_graph_elt_neighborhood<P> self_;

  public:
    /// Associated types.
    /// \{
    /// The type of point stored into the neighborhood.
    // FIXME: Is this right, if we consider that this neighborhood stores
    // psites, not points?
    typedef P point;

    // FIXME: This is a dummy value.
    typedef void dpoint;

    /*! \brief Point_Iterator type to browse the points of a generic
     * neighborhood w.r.t. the ordering of delta-points.
     */
    typedef line_graph_neighborhood_fwd_piter<P> fwd_niter;

    /*! \brief Point_Iterator type to browse the points of a generic
     * neighborhood w.r.t. the reverse ordering of delta-points.
     */
    typedef line_graph_neighborhood_bkd_piter<P> bkd_niter;

    /// The default qiter type.
    typedef fwd_niter niter;
    /// \}
  };

} // end of namespace mln


#endif // ! MLN_CORE_LINE_GRAPH_ELT_NEIGHBORHOOD_HH
