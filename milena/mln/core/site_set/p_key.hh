// Copyright (C) 2008, 2009, 2013 EPITA Research and Development
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

#ifndef MLN_CORE_SITE_SET_P_KEY_HH
# define MLN_CORE_SITE_SET_P_KEY_HH

/// \file
///
/// Definition of a priority queue class.
///
/// \todo Be more verbose in run_().

# include <map>
# include <mln/core/concept/function.hh>
# include <mln/core/site_set/p_set.hh>
# include <mln/core/site_set/p_double.hh>
# include <mln/core/internal/site_set_base.hh>
# include <mln/util/ord.hh>


namespace mln
{

  // Forward declaration.
  template <typename K, typename P> class p_key;



  namespace trait
  {

    template <typename K, typename P>
    struct site_set_< p_key<K,P> >
    {
      typedef trait::site_set::nsites::known     nsites;
      typedef trait::site_set::bbox::unknown     bbox;
      typedef trait::site_set::contents::growing contents;
      typedef trait::site_set::arity::unique     arity;
    };

  } // end of namespace trait




  /// \brief Priority queue class.
  ///
  /// \ingroup modsitesetqueue
  template <typename K, typename P>
  class p_key : public internal::site_set_base_< P,
						 p_key<K,P> >
  {
    typedef p_key<K,P> self_;
  public:

    /// Element associated type.
    typedef P element;


    /// Psite associated type.
    typedef p_double_psite< self_, p_set<P> > psite;

    /// Forward Site_Iterator associated type.
    typedef p_double_piter<self_,
			   mln_fwd_eiter(util::set<K>),
			   mln_fwd_piter(p_set<P>)> fwd_piter;

    /// Backward Site_Iterator associated type.
    typedef p_double_piter<self_,
			   mln_bkd_eiter(util::set<K>),
			   mln_bkd_piter(p_set<P>)> bkd_piter;

    /// Site_Iterator associated type.
    typedef fwd_piter piter;


    /// Constructor.
    p_key();


    /// Test is the psite \p p belongs to this site set.
    bool has(const psite&) const;

    /// Test is the psite \p p belongs to this site set.
    bool has(const P& p) const;


    /// Test this set validity so returns always true.
    bool is_valid() const;

    /// Give the number of sites.
    unsigned nsites() const;


    /// Insertion element associated type.
    typedef std::pair<K,P> i_element;

    /// Insert a pair \p k_p (key k, site p).
    void insert(const i_element& k_p);

    /// Insert a pair (key \p k, site \p p).
    void insert(const K& k, const P& p);


    /// \cond INTERNAL_API
    void unsafe_insert_(const K& k, const P& p);
    /// \endcond



    /// Removal element associated type.
    typedef P r_element;

    /// Remove a site \p p.
    void remove(const P& p);

    /// Remove all sites with key \p k.
    void remove_key(const K& k);

    /// Change the key \p k into a new value \p new_k.
    void change_key(const K& k, const K& new_k);

    /// Change the keys by applying the function \p f.
    template <typename F>
    void change_keys(const Function_v2v<F>& f);



    /// Clear this site set.
    void clear();


    /// Give the queue with the priority \p priority.  This method
    /// always works: if the priority is not in this set, an empty
    /// queue is returned.
    const p_set<P>& operator()(const K& key) const;

    /// Give the key associated with site \p p.
    const K& key(const P& p) const;

    /// Give the set of keys.
    const util::set<K>& keys() const;


    /// Test if the \p priority exists.
    bool exists_key(const K& key) const;

    /// \cond INTERNAL_API
    // Required by p_double-related classes.
    const util::set<K>& set_1_() const;
    const p_set<P>& set_2_(const K& key) const;
    /// \endcond


    /// Return the size of this site set in memory.
    std::size_t memory_size() const;

  protected:

    // Bunch of keys.
    util::set<K> b_;

    // Function: key k  -> set of sites {p}.
    typedef std::map<K, p_set<P>, util::ord<K> > s_t;
    s_t s_;

    // Function: site p -> key k.
    typedef std::map<P, K, util::ord<P> > k_t;
    k_t k_;

    // Number of sites.
    unsigned n_;

    // Run invariance tests and return the result.
    bool run_() const;
  };


  /// \relates p_key
  template <typename K, typename P>
  std::ostream& operator<<(std::ostream& ostr, const p_key<K,P>& pk);



# ifndef MLN_INCLUDE_ONLY

  template <typename K, typename P>
  inline
  p_key<K,P>::p_key()
  {
    n_ = 0;
    mln_invariant(run_());
  }

  template <typename K, typename P>
  inline
  bool
  p_key<K,P>::has(const psite&) const
  {
    mln_invariant(run_());
    // FIXME
    return true;
  }

  template <typename K, typename P>
  inline
  bool
  p_key<K,P>::has(const P& p) const
  {
    mln_invariant(run_());
    return k_.find(p) != k_.end();
  }

  template <typename K, typename P>
  inline
  bool
  p_key<K,P>::is_valid() const
  {
    mln_invariant(run_());
    return true;
  }

  template <typename K, typename P>
  inline
  unsigned
  p_key<K,P>::nsites() const
  {
    mln_invariant(run_());
    return n_;
  }


  template <typename K, typename P>
  inline
  void
  p_key<K,P>::unsafe_insert_(const K& k, const P& p)
  {
    s_[k].insert(p);
    k_[p] = k;
    ++n_;
    b_.insert(k);
    mln_invariant(run_());
  }


  template <typename K, typename P>
  inline
  void
  p_key<K,P>::insert(const K& k, const P& p)
  {
    mln_invariant(run_());
    typename k_t::iterator p_k = k_.find(p);
    if (p_k != k_.end())
      // p is already in this set (so n_ is unchanged).
      {
	K p_key = p_k->second;
	mln_assertion(b_.has(p_key));
	mln_assertion(s_[p_key].has(p));
	if (p_key == k)
	  // No-op.
	  return;
	// p moves from s_[p_key] to s_[k].
	s_[p_key].remove(p);
	s_[k].insert(p);
	// Update key of p.
	p_k->second = k;
      }
    else
      // Actual insertion.
      {
	s_[k].insert(p);
	k_[p] = k;
	++n_;
      }
    b_.insert(k);
    mln_invariant(run_());
  }

  template <typename K, typename P>
  inline
  void
  p_key<K,P>::insert(const i_element& k_p)
  {
    this->insert(k_p.first, k_p.second); // Also test invariants.
  }

  template <typename K, typename P>
  inline
  void
  p_key<K,P>::remove(const P& p)
  {
    mln_invariant(run_());
    typename k_t::iterator p_k = k_.find(p);

    if (p_k == k_.end())
      // No-op because p does not belong to this site set.
      return;

    K p_key = p_k->second;
    mln_assertion(b_.has(p_key));

    // Update k_.
    k_.erase(p_k);

    // Update s_.
    typename s_t::iterator k_s = s_.find(p_key);
    mln_assertion(k_s != s_.end()); // p_key found in s_.
    p_set<P>& s = k_s->second;
    mln_assertion(s.has(p));        // p is in s_[p_key].

    if (s.nsites() == 1)
      {
	// Clean up for p is the only site with p_key.
	s_.erase(k_s);
	b_.remove(p_key);
      }
    else
      {
	// Simple removal.
	s.remove(p);
      }

    // Update n_.
    --n_;

    mln_invariant(run_());
  }

  template <typename K, typename P>
  inline
  void
  p_key<K,P>::remove_key(const K& k)
  {
    mln_invariant(run_());
    typename s_t::iterator k_s = s_.find(k);

    if (k_s == s_.end())
      // No-op because key k does not exist.
      return;

    // Update b_.
    b_.remove(k);

    // Update k_.
    p_set<P>& s = k_s->second;
    mln_piter(p_set<P>) p(s);
    for_all(p)
      k_.erase(p);

    // Update n_.
    n_ -= s.nsites();

    // Update s_.
    s_.erase(k_s);

    mln_invariant(run_());
  }


  template <typename K, typename P>
  inline
  void
  p_key<K,P>::change_key(const K& k, const K& new_k)
  {
    mln_invariant(run_());

    if (new_k == k)
      // No-op.
      return;

    typename s_t::iterator k_s = s_.find(k);
    if (k_s == s_.end())
      // No-op because key k does not exist.
      return;

    // Update b_.
    b_.remove(k);
    b_.insert(new_k);

    // Update k_.
    p_set<P>& s = k_s->second;
    if (s.nsites() < n_ / 10) // Few elements to be moved.
      {
	// s.nsites() iterations.
	mln_piter(p_set<P>) p(s);
	for_all(p)
	  k_[p] = new_k;
      }
    else
      {
	// n_ iterations.
	typename k_t::iterator p_k;
	for (p_k = k_.begin(); p_k != k_.end(); ++p_k)
	  if (p_k->second == k)
	    p_k->second = new_k;
      }

    // Update s_.
    s_[new_k] += s;
    s_.erase(k_s);

    mln_invariant(run_());
  }

  template <typename K, typename P>
  template <typename F>
  inline
  void
  p_key<K,P>::change_keys(const Function_v2v<F>& f_)
  {
    mln_invariant(run_());

    const F& f = exact(f_);
    std::map<K,K> lut;

    // Update b_.
    {
      util::set<K> new_b;
      mln_eiter(util::set<K>) k(b_);
      for_all(k)
	new_b.insert(lut[k] = f(k));
      b_ = new_b;
    }

    // Update k_ and s_.
    {
      s_t new_s;
      typename k_t::iterator p_k;
      for (p_k = k_.begin(); p_k != k_.end(); ++p_k)
	{
	  p_k->second = lut[p_k->second];
	  new_s[p_k->second].insert(p_k->first);
	}
      s_ = new_s;
    }

    mln_invariant(run_());
  }

  template <typename K, typename P>
  inline
  void
  p_key<K,P>::clear()
  {
    mln_invariant(run_());
    b_.clear();
    s_.clear();
    k_.clear();
    n_ = 0;
    mln_invariant(run_());
  }

  template <typename K, typename P>
  inline
  std::size_t
  p_key<K,P>::memory_size() const
  {
    mln_invariant(run_());
    return 0; // FIXME
//     std::size_t mem_q = 0;
//     typename std::map<P, Q>::const_iterator i;
//     for (i = q_.begin(); i != q_.end(); ++i)
//       mem_q += i->second.memory_size();
//     return p_.memory_size() + sizeof(q_) + sizeof(n_);
  }

  template <typename K, typename P>
  inline
  const p_set<P>&
  p_key<K,P>::operator()(const K& key) const
  {
    static const p_set<P> nil_ = p_set<P>();
    if (exists_key(key)) // Also test invariants.
      return s_.find(key)->second;
    else
      return nil_;
  }

  template <typename K, typename P>
  inline
  const K&
  p_key<K,P>::key(const P& p) const
  {
    mln_invariant(run_());
    mln_precondition(k_.find(p) != k_.end());
    return k_.find(p)->second;
  }

  template <typename K, typename P>
  inline
  const util::set<K>&
  p_key<K,P>::keys() const
  {
    mln_invariant(run_());
    return b_;
  }

  template <typename K, typename P>
  inline
  bool
  p_key<K,P>::exists_key(const K& key) const
  {
    mln_invariant(run_());
    return b_.has(key);
  }

  template <typename K, typename P>
  inline
  const util::set<K>&
  p_key<K,P>::set_1_() const
  {
    return b_;
  }

  template <typename K, typename P>
  inline
  const p_set<P>&
  p_key<K,P>::set_2_(const K& key) const
  {
    mln_precondition(b_.has(key));
    return s_.find(key)->second;
  }

  template <typename K, typename P>
  inline
  bool
  p_key<K,P>::run_() const
  {
    if (! implies(n_ == 0, b_.is_empty()))
      {
	std::cerr << "#1" << std::endl;
	return false;
      }

    if (b_.nelements() != s_.size())
      {
	// Containers b_ and s_ are not consistent in size!
	std::cerr << "#2" << std::endl;
	return false;
      }

    if (k_.size() != n_)
      {
	// The number of entries in k_ is not n_!
	std::cerr << "#3: k_.size=" << k_.size() << " n_=" << n_ << std::endl;
	return false;
      }

    unsigned n = 0;
    mln_eiter(util::set<K>) key(b_);
    for_all(key)
      {
	typename s_t::const_iterator k_s = s_.find(key);

	if (k_s == s_.end())
	  {
	    // This key is not found in s_!
	    std::cerr << "#4" << std::endl;
	    return false;
	  }

	const p_set<P>& s = k_s->second;

	if (s.nsites() == 0)
	  {
	    // The site set associated with this key is empty!
	    std::cerr << "#5" << std::endl;
	    return false;
	  }

	n += s.nsites();

	mln_piter(p_set<P>) p(s);
	for_all(p) // For every site p with the current key w.r.t. s_.
	  {
	    typename k_t::const_iterator p_k = k_.find(p);

	    if (p_k == k_.end())
	      {
		// There is no key entry for p in k_!
		std::cerr << "#6" << std::endl;
		return false;
	      }

	    if (p_k->second != key)
	      {
		// We have an inconsistency for p between s_ and k_!
		std::cerr << "#7" << std::endl;
		return false;
	      }
	  }
      }

    if (n != n_)
      {
	// The number of elements in s_ is not n_!
	std::cerr << "#8" << std::endl;
	return false;
      }

    return true;
  }





  // Operator<<.

  template <typename K, typename P>
  std::ostream& operator<<(std::ostream& ostr, const p_key<K,P>& pk)
  {
    ostr << '{';
    mln_fwd_eiter(util::set<K>) k(pk.keys());
    for_all(k)
      {
	ostr << ' ' << k << ':';
	ostr << pk.set_2_(k);
      }
    ostr << '}';
    return ostr;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_SITE_SET_P_KEY_HH
