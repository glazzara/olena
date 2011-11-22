// Copyright (C) 2009, 2011 EPITA Research and Development Laboratory
// (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef MLN_CORE_IMAGE_GRAPH_ELT_WINDOW_IF_HH
# define MLN_CORE_IMAGE_GRAPH_ELT_WINDOW_IF_HH

/// \file
///
/// Definition of a custom ``window'' on a graph.
///
/// \todo Allow to pass a function instead of an image.

# include <mln/core/concept/window.hh>
# include <mln/core/internal/neighborhood_base.hh>
# include <mln/core/internal/graph_window_base.hh>
# include <mln/core/internal/is_masked_impl_selector.hh>
# include <mln/core/image/graph_window_if_piter.hh>


namespace mln
{

  /// Forward declaration
  template <typename G, typename S, typename I> class graph_elt_window_if;
  template <typename G, typename F> class p_edges;
  template <typename G, typename F> class p_vertices;


  namespace internal
  {

    template <typename G, typename S, typename I, typename E>
    struct neighborhood_impl<graph_elt_window_if<G,S,I>,E>
      : public neighborhood_extra_impl<graph_elt_window_if<G,S,I>,E>
    {
    };


    /// Default
    /// The given site set parameter is not supported yet!
    template <typename G, typename S>
    struct graph_window_if_iter_dispatch;

    template <typename G, typename F>
    struct graph_window_if_iter_dispatch<G, p_edges<G,F> >
    {
      typedef mln_edge_nbh_edge_fwd_iter(G) nbh_fwd_iter_;
      typedef mln_edge_nbh_edge_bkd_iter(G) nbh_bkd_iter_;
    };

    template <typename G, typename F>
    struct graph_window_if_iter_dispatch<G, p_vertices<G,F> >
    {
      typedef mln_vertex_nbh_vertex_fwd_iter(G) nbh_fwd_iter_;
      typedef mln_vertex_nbh_vertex_bkd_iter(G) nbh_bkd_iter_;
    };


  } // end of namespace mln::internal


  namespace trait
  {

    template <typename G, typename S, typename I>
    struct window_< mln::graph_elt_window_if<G,S,I> >
    {
      typedef trait::window::size::unknown       size;
      typedef trait::window::support::irregular  support;
      typedef trait::window::definition::varying definition;
    };

  } // end of namespace mln::trait


  /// Custom window on graph class. It is defined thanks to a mask.
  ///
  /// \p G is the graph type.
  /// \p S is the image site set.
  /// \p I is the graph image the type used as mask.
  ///
  template <typename G, typename S, typename I>
  class graph_elt_window_if
    : public graph_window_base<mln_result(S::fun_t),
			       graph_elt_window_if<G,S,I> >,
      public internal::graph_window_if_iter_dispatch<G,S>,
      private mlc_is(mln_value(I), bool)::check_t
  {
    typedef graph_elt_window_if<G,S,I> self_;
    typedef internal::graph_window_if_iter_dispatch<G,S> super_;

    typedef typename super_::nbh_fwd_iter_ nbh_fwd_iter_;
    typedef typename super_::nbh_bkd_iter_ nbh_bkd_iter_;

  public:
    /// The type of the image used as mask.
    typedef I mask_t;

    /// Constructor.
    /// \@{
    /// Default. Construct an invalid window.
    graph_elt_window_if();

    /// \param[in] mask A graph image of bool.
    ///
    /// \sa vertex_image, edge_image.
    graph_elt_window_if(const Image<I>& mask);
    /// \@}

    /// Associated types.
    /// \{

    /// The image domain on which this window iterates on.
    typedef S target;

    /// The type of psite corresponding to the window.
    typedef mln_psite(target) psite;

    /// Site_Iterator type to browse the psites of the window
    /// w.r.t. the ordering of vertices.
    typedef graph_window_if_piter<target,self_,nbh_fwd_iter_> fwd_qiter;

    /// Site_Iterator type to browse the psites of the window
    /// w.r.t. the reverse ordering of vertices.
    typedef graph_window_if_piter<target,self_,nbh_bkd_iter_> bkd_qiter;

    /// The default qiter type.
    typedef fwd_qiter qiter;
    /// \}

    /// Return the graph image used as mask.
    const I& mask() const;

    /// Change mask image.
    void change_mask(const Image<I>& mask);

    /// Return true by default.
    bool is_valid() const;

  private:
    // FIXME: Should be const!
    I mask_;
  };



# ifndef MLN_INCLUDE_ONLY


  template <typename G, typename S, typename I>
  inline
  graph_elt_window_if<G,S,I>::graph_elt_window_if()
  {
  }


  template <typename G, typename S, typename I>
  inline
  graph_elt_window_if<G,S,I>::graph_elt_window_if(const Image<I>& mask)
    : mask_(exact(mask))
  {
  }


  template <typename G, typename S, typename I>
  inline
  const I&
  graph_elt_window_if<G,S,I>::mask() const
  {
    return mask_;
  }


  template <typename G, typename S, typename I>
  inline
  void
  graph_elt_window_if<G,S,I>::change_mask(const Image<I>& mask)
  {
    mln_precondition(exact(mask).is_valid());
    mask_ = exact(mask);
  }

  template <typename G, typename S, typename I>
  inline
  bool
  graph_elt_window_if<G,S,I>::is_valid() const
  {
    return mask_.is_valid();
  }


# endif // ! MLN_INCLUDE_ONLY


} // end of namespace mln


#endif // ! MLN_CORE_IMAGE_GRAPH_ELT_WINDOW_IF_HH
