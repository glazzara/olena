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

#ifndef MLN_CORE_SITE_SET_P_SET_OF_HH
# define MLN_CORE_SITE_SET_P_SET_OF_HH

/*! \file
 *
 * \brief Definition of a set of site sets.
 *
 * \todo Zed: Add element browsing.
 */

# include <mln/core/site_set/p_double.hh>
# include <mln/core/internal/site_set_base.hh>
# include <mln/core/internal/site_set_impl.hh>
# include <mln/util/set.hh>



namespace mln
{

  // Forward declaration.
  template <typename S> class  p_set_of;


  namespace trait
  {

    template <typename S>
    struct site_set_< p_set_of<S> >
    {
      typedef mln_trait_site_set_nsites(S)       nsites;
      typedef mln_trait_site_set_bbox(S)         bbox;
      typedef trait::site_set::contents::growing contents;
      typedef trait::site_set::arity::multiple   arity;
    };

  } // end of namespace trait



  /*! \brief p_set_of is a set of site sets.
   *
   * Parameter \c S is the type of the contained site sets.
   */
  template <typename S>
  class p_set_of : public internal::site_set_base_< mln_site(S),
						    p_set_of<S> >,
		   public internal::site_set_impl<S>,
		   private mlc_is_a(S, Site_Set)::check_t
  {
    typedef p_set_of<S>  self_;
    typedef util::set<S> set_;
  public:

    /// Element associated type.
    typedef S element;


    /// Psite associated type.
    typedef p_double_psite<self_, element> psite;

    /// Forward Site_Iterator associated type.
    typedef p_double_piter<self_,
			   mln_fwd_eiter(set_),
			   mln_fwd_piter(S)> fwd_piter;

    /// Backward Site_Iterator associated type.
    typedef p_double_piter<self_,
			   mln_bkd_eiter(set_),
			   mln_bkd_piter(S)> bkd_piter;

    /// Site_Iterator associated type.
    typedef fwd_piter piter;


    /// Constructor without arguments.
    p_set_of();


    /// Test if \p p belongs to this point set.
    bool has(const psite& p) const;

    /// Test if this set of runs is valid.
    bool is_valid() const;


    /// Insertion element associated type.
    typedef S i_element;

    /// Insert a site set \p s.
    void insert(const S& s);


    /// Return the \p i-th site set.
    const S& operator[](unsigned i) const;

    /// Give the number of elements (site sets) of this composite.
    unsigned nelements() const;


    /// Clear this set.
    void clear();


    /// Return the size of this site set in memory.
    std::size_t memory_size() const;

    /// Hook to the set of runs.
    const util::set<S>& set_hook_() const;


    /// \cond INTERNAL_API
    // Required by p_double-related classes.
    const util::set<S>& set_1_() const;
    template <typename I>
    const S& set_2_(const I& it) const;
    /// \endcond

  protected:

    /// Set of site sets.
    util::set<S> s_;
  };


  /// \relates mln::p_set_of
  template <typename S>
  std::ostream& operator<<(std::ostream& ostr, const p_set_of<S>& s);




# ifndef MLN_INCLUDE_ONLY

  template <typename S>
  inline
  p_set_of<S>::p_set_of()
  {
  }

  template <typename S>
  inline
  bool
  p_set_of<S>::has(const psite& p) const
  {
    return p.index() < s_.nelements() && s_[p.index()].has(p.p());
  }

  template <typename S>
  inline
  bool
  p_set_of<S>::is_valid() const
  {
    return true;
  }

  template <typename S>
  inline
  void
  p_set_of<S>::insert(const S& s)
  {
    mln_precondition(s.is_valid());
    s_.insert(s);
    this->update_nsites_(s);
    this->update_bbox_(s);
  }

  template <typename S>
  inline
  const S&
  p_set_of<S>::operator[](unsigned i) const
  {
    mln_precondition(i < s_.nelements());
    return s_[i];
  }

  template <typename S>
  inline
  unsigned
  p_set_of<S>::nelements() const
  {
    return s_.nelements();
  }

  template <typename S>
  inline
  void
  p_set_of<S>::clear()
  {
    s_.clear();
    mln_postcondition(this->is_empty());
  }

  template <typename S>
  inline
  std::size_t
  p_set_of<S>::memory_size() const
  {
    return s_.memory_size();
  }

  template <typename S>
  inline
  const util::set<S>&
  p_set_of<S>::set_hook_() const
  {
    return s_;
  }

  template <typename S>
  inline
  const util::set<S>&
  p_set_of<S>::set_1_() const
  {
    return s_;
  }

  template <typename S>
  template <typename I>
  inline
  const S&
  p_set_of<S>::set_2_(const I& it) const
  {
    return it.element();
  }


  template <typename S>
  std::ostream& operator<<(std::ostream& ostr, const p_set_of<S>& s)
  {
    return ostr << s.set_hook_();
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_SITE_SET_P_SET_OF_HH
