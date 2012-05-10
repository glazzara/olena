// Copyright (C) 2007, 2008, 2009, 2012 EPITA Research and Development
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

#ifndef MLN_CORE_SITE_SET_P_VACCESS_HH
# define MLN_CORE_SITE_SET_P_VACCESS_HH

/// \file
///
/// \todo Fix the FIXMEs.

# include <utility>
# include <vector>
# include <mln/core/concept/site_set.hh>
# include <mln/core/site_set/p_double.hh>
# include <mln/core/internal/site_set_base.hh>
# include <mln/core/internal/pseudo_site_base.hh>
# include <mln/core/internal/site_set_impl.hh>
# include <mln/value/set.hh>


namespace mln
{

  // Forward declaration.
  template <typename V, typename S> class p_vaccess;



  namespace trait
  {

    template <typename V, typename S>
    struct site_set_< p_vaccess<V,S> >
    {
      typedef mln_trait_site_set_nsites(S)      nsites;
      typedef mln_trait_site_set_bbox(S)        bbox;
      typedef trait::site_set::contents::growing contents;
      typedef trait::site_set::arity::multiple   arity;
    };

  } // end of namespace trait



  /// \brief Site set in which sites are grouped by their associated value.
  ///
  /// \ingroup modsitesetsparse
  template <typename V, typename S>
  class p_vaccess : public internal::site_set_base_< mln_site(S),
						     p_vaccess<V,S> >,
		    public internal::site_set_impl< S >
  {
    typedef p_vaccess<V,S> self_;
  public:

    /// Value associated type.
    typedef V value;

    /// Value_Set associated type.
    typedef mln::value::set<V> vset;
    typedef vset t_eligible_values_set;  // FIXME: Required by value set iterators...

    /// Inner site set associated type.
    typedef S pset;

    /// Psite associated type.
    typedef p_double_psite<self_, S> psite;

    /// Forward Site_Iterator associated type.
    typedef p_double_piter<self_, mln_fwd_viter(vset), mln_fwd_piter(S)> fwd_piter;

    /// Backward Site_Iterator associated type.
    typedef p_double_piter<self_, mln_bkd_viter(vset), mln_bkd_piter(S)> bkd_piter;

    /// Site_Iterator associated type.
    typedef fwd_piter piter;


    /// Constructor.
    p_vaccess();
    p_vaccess(const V& vset);


    /// Test if \p p belongs to this site set.
    bool has(const psite& p) const;

    /// Test if the couple (value \p v, psite \p p) belongs to this
    /// site set.
    bool has(const V& v, const mln_psite(S)& p) const;

    /// Test if this site set is valid.
    bool is_valid() const;


    /// Element associated type.
    typedef mln_element(S) element;

    /// Insertion element associated type.
    typedef std::pair<V, element> i_element;

    /// Insert a pair \p v_e (value v, element e).
    void insert(const i_element& v_e);

    /// Insert \p e at value \p v.
    void insert(const V& v, const element& e);


    // Clear this set.
    void clear();


    /// Give the set of values.
    const mln::value::set<V>& values() const;

    /// Return the site set at value \p v.
    const S& operator()(const V& v) const;


    /// Return the size of this site set in memory.
    std::size_t memory_size() const;

    // Required by p_double-related classes.
    const mln::value::set<V>& set_1_() const;
    const S& set_2_(const V& v) const;

  protected:

    mln::value::set<V> vs_;
    std::vector<S> ps_;
  };




# ifndef MLN_INCLUDE_ONLY


  // p_vaccess<V,S>

  template <typename V, typename S>
  inline
  p_vaccess<V,S>::p_vaccess()
    : vs_(),
      ps_(vs_.nvalues())
  {
  }

  template <typename V, typename S>
  inline
  p_vaccess<V,S>::p_vaccess(const V& vset)
    : vs_(vset),
      ps_(vs_.nvalues())
  {
  }

  template <typename V, typename S>
  inline
  bool
  p_vaccess<V,S>::has(const psite&) const
  {
    // FIXME
    return true;
  }

  template <typename V, typename S>
  inline
  bool
  p_vaccess<V,S>::has(const V& v, const mln_psite(S)& p) const
  {
    return ps_[vs_.index_of(v)].has(p);
  }

  template <typename V, typename S>
  inline
  bool
  p_vaccess<V,S>::is_valid() const
  {
    // FIXME
    return true;
  }

  template <typename V, typename S>
  inline
  void
  p_vaccess<V,S>::insert(const V& v, const element& e)
  {
    ps_[vs_.index_of(v)].insert(e);
    this->update_nsites_(e);
    this->update_bbox_(e);
  }

  template <typename V, typename S>
  inline
  void
  p_vaccess<V,S>::insert(const i_element& v_e)
  {
    insert(v_e.first, v_e.second);
  }

  template <typename V, typename S>
  inline
  void
  p_vaccess<V,S>::clear()
  {
    const unsigned n = ps_.size();
    for (unsigned i = 0; i < n; ++i)
      ps_[i].clear();
  }

  template <typename V, typename S>
  inline
  const S&
  p_vaccess<V,S>::operator()(const V& v) const
  {
    return ps_[vs_.index_of(v)];
  }

  template <typename V, typename S>
  inline
  std::size_t
  p_vaccess<V,S>::memory_size() const
  {
    std::size_t mem = 0;
    const unsigned n = ps_.size();
    for (unsigned i = 0; i < n; ++i)
      mem += ps_[i].memory_size();
    return sizeof(*this) + mem;
  }

  template <typename V, typename S>
  inline
  const mln::value::set<V>&
  p_vaccess<V,S>::values() const
  {
    return vs_;
  }

  template <typename V, typename S>
  inline
  const mln::value::set<V>&
  p_vaccess<V,S>::set_1_() const
  {
    return vs_;
  }

  template <typename V, typename S>
  inline
  const S&
  p_vaccess<V,S>::set_2_(const V& v) const
  {
    return ps_[vs_.index_of(v)];
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_SITE_SET_P_VACCESS_HH
