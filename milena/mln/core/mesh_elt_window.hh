// Copyright (C) 2007 EPITA Research and Development Laboratory
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

#ifndef MLN_WIN_MESH_ELT_WINDOW_HH
# define MLN_WIN_MESH_ELT_WINDOW_HH

/*! \file mln/core/mesh_elt_window.hh
 *
 *  \brief Definition of the elementary neighborhood (wrongly
 *  -- but purposefully -- named "window" here, for consistency
 *  reasons) on a mesh (a graph, in fact).
 *
 *  \todo Make naming coherent: we have window (without '_') but
 *  point_, neighb_, etc.
 */

# include <mln/core/concept/window.hh>
# include <mln/core/mesh_psite.hh>
# include <mln/core/mesh_window_piter.hh>


namespace mln
{
  // Fwd decls.
  template <typename P> class mesh_window_fwd_piter;
  template <typename P> class mesh_window_bkd_piter;


  /*! \brief Elementary window on mesh class.
   *
   *  FIXME: Doc.
   */
  template <typename P>
  class mesh_elt_window : public Window< mesh_elt_window<P> >
  {
    typedef mesh_elt_window<P> self_;

  public:
    /// Associated types.
    /// \{
    /// The type of point stored into the window.
    // FIXME: Is this right, if we consider that this window stores
    // psites, not points?
    typedef P point;
      
    // FIXME: This is a dummy value.  This is yet another evidence
    // that mesh_elt_window shall not be a Window, and therefore be
    // renamed as mesh_elt_neighb, or better: elt_graph_neighb.
    typedef void dpoint;

    /*! \brief Point_Iterator type to browse the points of a generic window
     * w.r.t. the ordering of delta-points.
     */
    typedef mesh_window_fwd_piter<P> fwd_qiter;

    /*! \brief Point_Iterator type to browse the points of a generic window
     * w.r.t. the reverse ordering of delta-points.
     */
    typedef mesh_window_bkd_piter<P> bkd_qiter;

    /// The default qiter type.
    typedef fwd_qiter qiter;
    /// \}

    /// Construct an elementary mesh window centered on \a psite.
    mesh_elt_window(/*const mesh_psite<P>& psite*/);

    // FIXME: The following methods make no sense for a
    // general-purpose neighborhood.  Anyway, we provide
    // implementations for them for this first draft of graph
    // neighborhood.
    bool is_empty() const;
    bool is_centered() const;
    bool is_symmetric() const;
    unsigned delta() const;
    self_& sym();

    //     protected:
    //       const mesh_psite<P>& psite_;
  };


  // FIXME: Add an operator<< on ostreams ?


# ifndef MLN_INCLUDE_ONLY

  template <typename P>
  inline
  mesh_elt_window<P>::mesh_elt_window(/*const mesh_psite<P>& psite*/)
  //       : psite_(psite)
  {
  }

  template <typename P>
  inline
  bool
  mesh_elt_window<P>::is_empty() const
  {
    // FIXME: Dummy value.
    return false;
  }

  template <typename P>
  inline
  bool
  mesh_elt_window<P>::is_centered() const
  {
    // FIXME: Dummy value.
    return false;
  }

  template <typename P>
  inline
  bool
  mesh_elt_window<P>::is_symmetric() const
  {
    // FIXME: Dummy value.
    return false;
  }

  template <typename P>
  inline
  unsigned
  mesh_elt_window<P>::delta() const
  {
    // FIXME: Dummy value.
    return 0;
  }

  template <typename P>
  inline
  mesh_elt_window<P>&
  mesh_elt_window<P>::sym()
  {
    // FIXME: Dummy value.
    return *this;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_WIN_MESH_ELT_WINDOW_HH
