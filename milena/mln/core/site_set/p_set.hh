// Copyright (C) 2007, 2008, 2009, 2010 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_CORE_SITE_SET_P_SET_HH
# define MLN_CORE_SITE_SET_P_SET_HH

/// \file
///
/// Definition of a mathematical set of sites (based on
/// util::set).

# include <mln/core/site_set/p_array.hh>
# include <mln/util/set.hh>


namespace mln
{

  // Forward declaration.
  template <typename P> class p_set;


  namespace trait
  {

    template <typename P>
    struct site_set_< p_set<P> >
    {
      typedef trait::site_set::nsites::known  nsites;
      typedef trait::site_set::bbox::unknown  bbox;
      typedef trait::site_set::contents::free contents;
      typedef trait::site_set::arity::unique  arity;
    };

  } // end of namespace trait



  /// \brief Mathematical set of sites (based on util::set).
  ///
  /// \ingroup modsitesetsparse
  /*!
   * This is a mathematical set of sites (not a multi-set).
   *
   * The parameter \c P shall be a site or pseudo-site type.
   */
  template <typename P>
  class p_set : public internal::site_set_base_< P, p_set<P> >
  {
    typedef p_set<P> self_;
  public:

    /// Element associated type.
    typedef P element;

    /// Psite associated type.
    typedef p_indexed_psite<self_> psite;

    /// Forward Site_Iterator associated type.
    typedef p_indexed_fwd_piter<self_> fwd_piter;

    /// Backward Site_Iterator associated type.
    typedef p_indexed_bkd_piter<self_> bkd_piter;

    /// Site_Iterator associated type.
    typedef fwd_piter piter;


    /// Constructor.
    p_set();


    /// Test if psite \p p belongs to this point set.
    bool has(const psite& p) const;

    /// Test if \p p belongs to this point set.
    bool has(const P& p) const;

    /// Test if index \p i belongs to this point set.
    bool has(const util::index& i) const;

    /// Test this set validity so returns always true.
    bool is_valid() const;


    /// Give the number of sites.
    unsigned nsites() const;

    /// Is the set empty?
    bool is_empty() const;


    /// Insertion element associated type. 
    typedef P i_element;

    /// Insert a site \p p.
    void insert(const P& p);

    /// Removal element associated type. 
    typedef P r_element;

    /// Remove a site \p p.
    void remove(const P& p);

    /// Clear this set.
    void clear();


    /// Return the \p i-th site.
    const P& operator[](unsigned i) const;


    /// Return the size of this site set in memory.
    std::size_t memory_size() const;

    /// Return the corresponding std::vector of sites.
    const std::vector<P>& std_vector() const;

    /// Return the corresponding util::set of sites.
    const util::set<P>& util_set() const;

  protected:

    util::set<P> s_;
  };



# ifndef MLN_INCLUDE_ONLY

  template <typename P>
  inline
  p_set<P>::p_set()
  {
  }

  template <typename P>
  inline
  bool
  p_set<P>::has(const P& p) const
  {
    return s_.has(p);
  }

  template <typename P>
  inline
  bool
  p_set<P>::has(const psite& p) const
  {
    mln_precondition(p.target_() == this); // FIXME: Refine.
    if (! has(p.index()))
      return false;
    mln_invariant(p.to_site() == (*this)[p.index()]);
    return true;
  }

  template <typename P>
  inline
  bool
  p_set<P>::has(const util::index& i) const
  {
    return i >= 0 && unsigned(i) < nsites();
  }

  template <typename P>
  inline
  bool
  p_set<P>::is_valid() const
  {
    return true;
  }

  template <typename P>
  inline
  unsigned
  p_set<P>::nsites() const
  {
    return s_.nelements();
  }

  template <typename P>
  inline
  bool
  p_set<P>::is_empty() const
  {
    return s_.is_empty();
  }

  template <typename P>
  inline
  void
  p_set<P>::insert(const P& p)
  {
    s_.insert(p);
  }

  template <typename P>
  inline
  void
  p_set<P>::remove(const P& p)
  {
    s_.remove(p);
  }

  template <typename P>
  inline
  void
  p_set<P>::clear()
  {
    s_.clear();
  }

  template <typename P>
  inline
  const P&
  p_set<P>::operator[](unsigned i) const
  {
    mln_precondition(i < nsites());
    return s_[i];
  }

  template <typename P>
  inline
  std::size_t
  p_set<P>::memory_size() const
  {
    return s_.memory_size();
  }

  template <typename P>
  inline
  const std::vector<P>&
  p_set<P>::std_vector() const
  {
    return s_.std_vector();
  }

  template <typename P>
  inline
  const util::set<P>&
  p_set<P>::util_set() const
  {
    return s_;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_SITE_SET_P_SET_HH
