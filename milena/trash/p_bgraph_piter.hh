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

#ifndef MLN_CORE_SITE_SET_P_BGRAPH_PITER_HH
# define MLN_CORE_SITE_SET_P_BGRAPH_PITER_HH

# include <utility>

# include <mln/core/internal/site_iterator_base.hh>
# include <mln/core/site_set/p_bgraph.hh>
# include <mln/core/image/bgraph_psite.hh>

/*! \file mln/core/site_set/p_bgraph_piter.hh
 *
 * \brief Definition of point iterator on boost-graph-based point set.
 */

namespace mln
{
  // Fwd decls.
  template<typename P> class p_bgraph;
  template<typename P> class bgraph_psite;


  // FIXME: check the constraint due to the boost iterators
  template<typename P>
  class p_bgraph_piter_
    : public internal::site_iterator_base< P, p_bgraph_piter_<P> >
  {
    typedef p_bgraph_piter_<P> self_;
    typedef internal::site_iterator_base< P, self_ > super_;
    typedef std::pair<typename p_bgraph<P>::node_iterator,
		      typename p_bgraph<P>::node_iterator> iterators_type_;

  public:

    // Make definitions from super class available.
    enum { dim = super_::dim };
    typedef bgraph_psite<P> psite;
    typedef P point;

    p_bgraph_piter_(const p_bgraph<P>& pg);

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
    /// The p_graph this point site belongs to.
    const p_bgraph<P>& pg_;

    /// The psite corresponding to this iterator.
    psite psite_;
    /// The point corresponding to this iterator.
    point p_;

  private:
    iterators_type_ iterators_;
    typename p_bgraph<P>::node_iterator cur_;
  };



# ifndef MLN_INCLUDE_ONLY

  template<typename P>
  inline
  p_bgraph_piter_<P>::p_bgraph_piter_(const p_bgraph<P>& pg)
    : pg_(pg),
      // Initialize psite_ to a dummy value.
      psite_(pg, pg_.nsites()),
      p_()
  {
    iterators_ = boost::vertices(pg_.to_graph());
    // Invalidate id_.
    invalidate();
  }

  template<typename P>
  inline
  mln_coord(P)
  p_bgraph_piter_<P>::operator[](unsigned i) const
  {
    return p_[i];
  }

  template<typename P>
  inline
  bool
  p_bgraph_piter_<P>::is_valid() const
  {
    return cur_ != iterators_.second;
  }

  template<typename P>
  inline
  void
  p_bgraph_piter_<P>::invalidate()
  {
    cur_ = iterators_.second; // Past the end iterator.
  }

  template<typename P>
  inline
  void
  p_bgraph_piter_<P>::start()
  {
    cur_ = iterators_.first;
    /// FIXME: Hide implementation details?
    if (is_valid())
      update_();
  }

  template<typename P>
  inline
  void
  p_bgraph_piter_<P>::next_()
  {
    ++cur_;
    if (is_valid())
      update_();
  }

  template<typename P>
  inline
  void
  p_bgraph_piter_<P>::update_()
  {
    // Update psite_.
    psite_ = bgraph_psite<P>(pg_, *cur_);
    // Update p_.
    p_ = pg_.point_from_id(*cur_);
  }

  template<typename P>
  inline
  const P&
  p_bgraph_piter_<P>::to_point() const
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
  const bgraph_psite<P>&
  p_bgraph_piter_<P>::to_psite() const
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
  p_bgraph_piter_<P>::operator point() const
  {
    mln_precondition(is_valid());
    return p_;
  }

  template<typename P>
  inline
  p_bgraph_piter_<P>::operator psite() const
  {
    mln_precondition(is_valid());
    bgraph_psite<P> tmp(pg_, *cur_);
    return tmp; // psite_;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of mln


#endif // MLN_P_BGRAPH_PITER_HH
