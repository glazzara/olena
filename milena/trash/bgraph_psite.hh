// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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

#ifndef MLN_CORE_IMAGE_BGRAPH_PSITE_HH
# define MLN_CORE_IMAGE_BGRAPH_PSITE_HH

/// \file mln/core/image/bgraph_psite.hh
///
/// Definition of a boost-graph-based point site.

# include <mln/core/site_set/p_bgraph.hh>


namespace mln
{

  // Fwd decl.
  template<typename P> class p_bgraph;


  /// Point site associated to a mln::graph_image.
  template<typename P>
  class bgraph_psite : public Point_Site< bgraph_psite<P> >
  {
    typedef bgraph_psite<P> self_;
    typedef Point_Site< bgraph_psite<P> > super_;

  public:
    typedef mln_mesh(P) mesh;
    enum { dim = P::dim };
    typedef P point;
    typedef mln_dpoint(P) dpoint;
    typedef mln_coord(P) coord;

    /// Construction and assignment.
    /// \{
    bgraph_psite(const p_bgraph<P>& pg_, typename p_bgraph<P>::node_id id);
    bgraph_psite(const self_& rhs);
    /// \pre This psite must have the same graph point set as \a rhs.
    self_& operator= (const self_& rhs);
    /// \}

    /// Access to psite.
    const self_& to_psite() const;

    /// Access to point.
    /// \{
    operator P() const;
    const point& to_point() const;
    coord operator[](unsigned id) const;
    /// \}

    /// Return the p_graph this point site belongs to.
    const p_bgraph<P>& pg() const;
    /// Return the node id of this point site.
    typename p_bgraph<P>::node_id id() const;

  private:
    /// The p_graph this point site belongs to.
    const p_bgraph<P>& pg_;
    /// The id of the node this psite is pointing towards.
    typename p_bgraph<P>::node_id id_;
  };

# ifndef MLN_INCLUDE_ONLY

  template<typename P>
  inline
  bgraph_psite<P>::bgraph_psite(const p_bgraph<P>& g,
				typename p_bgraph<P>::node_id id)
    : pg_(g),
      id_(id)
  {
  }


  /// FIXME: Is it normal to have to call super_() ?
  template<typename P>
  inline
  bgraph_psite<P>::bgraph_psite(const self_& rhs)
    : super_(),
      pg_(rhs.pg_),
      id_(rhs.id_)
  {
  }

  template<typename P>
  inline
  bgraph_psite<P>&
  bgraph_psite<P>::operator= (const self_& rhs)
  {
    if (&rhs == this)
      return *this;
    // Assigning a psite from a graph point set to a psite from
    // another graph point set is meaningless.
    mln_assertion(&pg_.to_graph() == &rhs.pg_.to_graph());
    id_ = rhs.id_;
    return *this;
  }

  template<typename P>
  inline
  const bgraph_psite<P>&
  bgraph_psite<P>::to_psite() const
  {
    return *this;
  }

  template<typename P>
  inline
  bgraph_psite<P>::operator P() const
  {
    return pg_.point_from_id(id_);
  }

  template<typename P>
  inline
  const P&
  bgraph_psite<P>::to_point() const
  {
    return pg_.point_from_id(id_);
  }

  template<typename P>
  inline
  mln_coord(P)
  bgraph_psite<P>::operator[](unsigned i) const
  {
    return to_point()[i];
  }

  template<typename P>
  inline
  const p_bgraph<P>&
  bgraph_psite<P>::pg() const
  {
    return pg_;
  }

  template<typename P>
  inline
  typename p_bgraph<P>::node_id
  bgraph_psite<P>::id() const
  {
    return id_;
  }

# endif // ! MLN_INCLUDE_ONLY


} // end of mln

#endif // MLN_CORE_IMAGE_BGRAPH_PSITE_HH
