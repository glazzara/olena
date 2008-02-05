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

#ifndef MLN_CORE_P_GRAPH_PITER_HH
# define MLN_CORE_P_GRAPH_PITER_HH

# include <mln/core/internal/point_iterator_base.hh>
# include <mln/core/p_graph.hh>
# include <mln/core/graph_psite.hh>

/*! \file mln/core/p_graph_piter.hh
 *
 * \brief Definition of point iterator on graph-based point set.
 */

namespace mln
{
  // Fwd decls.
  template<typename P> class p_graph;
  template<typename P> class graph_psite;


  // FIXME: Why `p_graph_piter_' and not `p_graph_piter' (without `_')?

  template<typename P>
  class p_graph_piter_
    : public internal::point_iterator_base_< P, p_graph_piter_<P> >
  {
    typedef p_graph_piter_<P> self_;
    typedef internal::point_iterator_base_< P, self_ > super_;

  public:
    
    // Make definitions from super class available.
    enum { dim = super_::dim };
    typedef graph_psite<P> psite;
    typedef P point;

    p_graph_piter_(const p_graph<P>& psite_set);

    /// Read-only access to the \p i-th coordinate.
    mln_coord(P) operator[](unsigned i) const;

    /// Test if the iterator is valid.
    bool is_valid() const;

    /// Invalidate the iterator.
    void invalidate();

    /// Start an iteration.
    void start();

    /// Go to the next point.
    void next_();

    /// Update the internal data of the iterator.
    void update_();

    /// Reference to the corresponding point.
    const point& to_point () const;

    /// Reference to the corresponding point site.
    const psite& to_psite () const;

    /// Convert the iterator into a point.
    operator point() const;

    /// Convert the iterator into a graph psite.
    operator psite() const;

  protected:
    const p_graph<P>& psite_set_;
    unsigned i_;
    P p_;
    psite psite_;
  };



# ifndef MLN_INCLUDE_ONLY

  template<typename P>
  inline
  p_graph_piter_<P>::p_graph_piter_(const p_graph<P>& psite_set)
    : psite_set_(psite_set),
      p_(),
      // Initialize psite_ to a dummy value.
      psite_(psite_set, psite_set_.loc_.size())
  {
    // Invalidate i_.
    invalidate();
  }

  template<typename P>
  inline
  mln_coord(P)
  p_graph_piter_<P>::operator[](unsigned i) const
  {
    return p_[i];
  }

  template<typename P>
  inline
  bool
  p_graph_piter_<P>::is_valid() const
  {
    return i_ != psite_set_.loc_.size();
  }

  template<typename P>
  inline
  void
  p_graph_piter_<P>::invalidate()
  {
    i_ = psite_set_.loc_.size();
  }

  template<typename P>
  inline
  void 
  p_graph_piter_<P>::start()
  {
    i_ = 0;
    if (is_valid())
      update_();
  }

  template<typename P>
  inline
  void 
  p_graph_piter_<P>::next_()
  {
    ++i_;
    if (is_valid())
      update_();
  }

  template<typename P>
  inline
  void
  p_graph_piter_<P>::update_()
  {
    // Update p_.
    p_ = psite_set_.loc_[i_];
    // Update psite_.
    psite_ = graph_psite<P>(psite_set_, i_);
  }

  template<typename P>
  inline
  const P&
  p_graph_piter_<P>::to_point() const
  {
    /* We don't check whether the iterator is valid before returning
       the value using

         mln_precondition(is_valid());

       since this method may be called *before* the iterator is
       actually initialized.  This is the case for instance when this
       point iterator (say, P) is used to initialize another iterator
       on window or neighborhood (say, Q); most of the time, for_all()
       is responsible for the initialization of P, but it takes place
       *after* the creation of Q.  */
    return p_;
  }

  template<typename P>
  inline
  const graph_psite<P>&
  p_graph_piter_<P>::to_psite() const
  {
    /* We don't check whether the iterator is valid before returning
       the value using

         mln_precondition(is_valid());

       since this method may be called *before* the iterator is
       actually initialized.  This is the case for instance when this
       point iterator (say, P) is used to initialize another iterator
       on window or neighborhood (say, Q); most of the time, for_all()
       is responsible for the initialization of P, but it takes place
       *after* the creation of Q.  */
    return psite_;
  }

  template<typename P>
  inline
  p_graph_piter_<P>::operator P() const
  {
    mln_precondition(is_valid());
    return p_;
  }

  template<typename P>
  inline
  p_graph_piter_<P>::operator graph_psite<P>() const
  {
    mln_precondition(is_valid());
    return psite_;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of mln


#endif // MLN_P_GRAPH_PITER_HH
