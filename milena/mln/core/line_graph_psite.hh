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
// License.
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef MLN_CORE_LINE_GRAPH_PSITE_HH
# define MLN_CORE_LINE_GRAPH_PSITE_HH

/// \file mln/core/line_graph_psite.hh
/// \brief Definition of a line graph-based point site.

# include <mln/core/p_line_graph.hh>

/* FIXME: This class shares a lot with graph_psite.  Factor as much as
   possible.  */


namespace mln
{

  // Fwd decl.
  template<typename P> class p_line_graph;


  /// \brief Point site associated to a mln::graph_image.
  template<typename P>
  class line_graph_psite : public Point_Site< line_graph_psite<P> >
  {
    typedef line_graph_psite<P> self_;
    typedef Point_Site<self_> super_;

  public:
    typedef P point;
    typedef mln_mesh(point) mesh;
    enum { dim = point::dim };
    typedef mln_dpoint(point) dpoint;
    typedef mln_coord(point) coord;

    /// Construction and assignment.
    /// \{
    line_graph_psite();
    line_graph_psite(const p_line_graph<P>& plg, util::edge_id id);
    line_graph_psite(const self_& rhs);
    self_& operator= (const self_& rhs);
    /// \}

    /// Access to psite.
    const self_& to_psite() const;

    /// Access to point.
    /// \{
    const point& to_point() const;
    coord operator[](unsigned id) const;
    /// \}

    /// Return the mln::p_line_graph this point site belongs to.
    const p_line_graph<P>& plg() const;
    /// Return the edge id of this point site.
    util::edge_id id() const;

    /// Return the first associated vertex.
    P first() const;
    /// Return the second associated vertex.
    P second() const;

    /// Return the id of the first associated vertex.
    util::vertex_id first_id() const;
    /// Return the id of the second associated vertex.
    util::vertex_id second_id() const;

    /// Is this psite valid?
    bool is_valid() const;

  private:
    /// The p_line_graph this point site belongs to.
    const p_line_graph<P>* plg_;
    /// The id of the edge this psite is pointing towards.
    util::edge_id id_;
    /** \brief The point associated to this psite.
       
        Contrary to mln::graph_psite, this information is actually
        stored in the mln::line_graph_psite.  In mln::graph_psite, the
        point is retrieved from the data associated with the
        corresponding vertex in the graph.  We cannot do this here,
        since points associated to edges are computed on the fly
        (storing them in the graph could be possible, but too costly
        in space).  */
    // FIXME: Actually, this is a dummy value!
    point p_;
  };


  /// Comparison of two mln::line_graph_psite<P> instances.
  /// \{
  /* FIXME: Shouldn't those comparisons be part of a much general
     mechanism?  */

  /// \brief Is \a lhs equal to \a rhs?
  ///
  /// \pre Arguments \a lhs and \a rhs must belong to the same
  /// mln::p_line_graph.
  template <typename P>
  bool
  operator==(const line_graph_psite<P>& lhs, const line_graph_psite<P>& rhs);

  /// \brief Is \a lhs ``less'' than \a rhs?
  ///
  /// This comparison is required by algorithms sorting psites.
  ///
  /// \pre Arguments \a lhs and \a rhs must belong to the same
  /// mln::p_line_graph.
  template <typename P>
  bool
  operator< (const line_graph_psite<P>& lhs, const line_graph_psite<P>& rhs);
  /// \}


  /* FIXME: This hand-made delegation is painful.  We should rely on
     the general mechanism provided by Point_Site.  But then again, we
     need to refine/adjust the interface of Point_Site w.r.t. the
     mandatory conversions to points.  */
  template <typename P>
  inline
  std::ostream&
  operator<<(std::ostream& ostr, const line_graph_psite<P>& p);


# ifndef MLN_INCLUDE_ONLY

  /* Careful, the current interface of line_graph_psite allows the
     construction of ill-formed psites (i.e., whose edge is is beyond
     bounds).  Actually, p_line_graph_piters *do* create ill-formed
     psites at their initialization.  */

  template<typename P>
  inline
  line_graph_psite<P>::line_graph_psite()
    // Dummy initializations.
    : super_(),
      plg_(0),
      id_(-1),
      p_()
  {
  }

  template<typename P>
  inline
  line_graph_psite<P>::line_graph_psite(const p_line_graph<P>& plg,
					util::edge_id id)
    : super_(),
      plg_(&plg),
      id_(id),
      p_()
  {
  }

  template<typename P>
  inline
  line_graph_psite<P>::line_graph_psite(const line_graph_psite<P>& rhs)
    : super_(rhs),
      plg_(rhs.plg_),
      id_(rhs.id_),
      p_()
  {
  }

  template<typename P>
  inline
  line_graph_psite<P>&
  line_graph_psite<P>::operator=(const line_graph_psite<P>& rhs)
  {
    if (&rhs == this)
      return *this;
    plg_ = rhs.plg_;
    id_ = rhs.id_;
    return *this;
  }

  template<typename P>
  inline
  bool
  line_graph_psite<P>::is_valid() const
  {
    return plg_ && id_ < plg_->gr_->nedges();
  }

  template<typename P>
  inline
  const line_graph_psite<P>&
  line_graph_psite<P>::to_psite() const
  {
    return *this;
  }

  template<typename P>
  inline
  const P&
  line_graph_psite<P>::to_point() const
  {
    // Dummy value.
    return p_;
  }

  template<typename P>
  inline
  mln_coord(P)
  line_graph_psite<P>::operator[](unsigned i) const
  {
    mln_assertion(is_valid());
    return to_point()[i];
  }

  template<typename P>
  inline
  const p_line_graph<P>&
  line_graph_psite<P>::plg() const
  {
    mln_assertion(plg_);
    return *plg_;
  }

  template<typename P>
  inline
  util::edge_id
  line_graph_psite<P>::id() const
  {
    return id_;
  }

  template<typename P>
  inline
  P
  line_graph_psite<P>::first() const
  {
    mln_assertion(is_valid());
    return plg().gr_->vertex_data(first_id());
  }

  template<typename P>
  inline
  P
  line_graph_psite<P>::second() const
  {
    mln_assertion(is_valid());
    return plg().gr_->vertex_data(second_id());
  }


  template<typename P>
  inline
  util::vertex_id
  line_graph_psite<P>::first_id() const
  {
    mln_assertion(is_valid());
    return plg().gr_->edge(id_).v1();
  }

  template<typename P>
  inline
  util::vertex_id
  line_graph_psite<P>::second_id() const
  {
    mln_assertion(is_valid());
    return plg().gr_->edge(id_).v2();
  }


  /*--------------.
  | Comparisons.  |
  `--------------*/

  template <typename P>
  bool
  operator==(const line_graph_psite<P>& lhs, const line_graph_psite<P>& rhs)
  {
    mln_assertion(&lhs.plg() == &rhs.plg());
    return lhs.id() == rhs.id();
  }

  template <typename P>
  bool
  operator< (const line_graph_psite<P>& lhs, const line_graph_psite<P>& rhs)
  {
    mln_assertion(&lhs.plg() == &rhs.plg());
    return lhs.id() < rhs.id();
  }


  /*------------------.
  | Pretty-printing.  |
  `------------------*/

  template <typename P>
  inline
  std::ostream&
  operator<<(std::ostream& ostr, const line_graph_psite<P>& p)
  {
    return ostr << '(' << p.first() << " -- " << p.second() << ')';
  }

# endif // ! MLN_INCLUDE_ONLY


} // end of mln

#endif // MLN_CORE_LINE_GRAPH_PSITE_HH
