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

#ifndef MLN_CORE_SITE_SET_P_MUTABLE_ARRAY_OF_HH
# define MLN_CORE_SITE_SET_P_MUTABLE_ARRAY_OF_HH

/*! \file
 *
 * \brief Definition of a mutable array of site sets.
 *
 * \todo Zed: Add another version (p_array_of) of this class that can
 * inherit, when possible, nsites and bbox (just like in p_vaccess).
 * It is a different class since such a feature is incompatible with
 * the "op[] mutable".
 *
 * \todo Zed: Add element browsing.
 */

# include <mln/core/site_set/p_double.hh>
# include <mln/core/internal/site_set_base.hh>
# include <mln/util/array.hh>



namespace mln
{

  // Forward declaration.
  template <typename S> class  p_mutable_array_of;


  namespace trait
  {

    template <typename S>
    struct site_set_< p_mutable_array_of<S> >
    {
      typedef trait::site_set::nsites::unknown   nsites;
      typedef trait::site_set::bbox::unknown     bbox;
      typedef trait::site_set::contents::growing contents;
      typedef trait::site_set::arity::multiple   arity;
    };

  } // end of namespace trait



  /// \brief p_mutable_array_of is a mutable array of site sets.
  ///
  /// Parameter \c S is the type of the contained site sets.
  ///
  /// \ingroup modsitesetbasic
  template <typename S>
  class p_mutable_array_of : public internal::site_set_base_< mln_site(S),
							      p_mutable_array_of<S> >,
			     private mlc_is_a(S, Site_Set)::check_t
  {
    typedef p_mutable_array_of<S>  self_;
    typedef util::array<S> array_;
  public:

    /// Element associated type.
    typedef S element;


    /// Psite associated type.
    typedef p_double_psite<self_, element> psite;

    /// Forward Site_Iterator associated type.
    typedef p_double_piter<self_,
			   mln_fwd_eiter(array_),
			   mln_fwd_piter(S)> fwd_piter;

    /// Backward Site_Iterator associated type.
    typedef p_double_piter<self_,
			   mln_bkd_eiter(array_),
			   mln_bkd_piter(S)> bkd_piter;

    /// Site_Iterator associated type.
    typedef fwd_piter piter;


    /// Constructor without arguments.
    p_mutable_array_of();


    /// Reserve memory for \p n elements.
    void reserve(unsigned n);


    /// Test if \p p belongs to this point set.
    bool has(const psite& p) const;

    /// Test this set validity so returns always true.
    bool is_valid() const;


    /// Insertion element associated type.
    typedef S i_element;

    /// Insert a site set \p s.
    /// \pre \p s is valid.
    void insert(const S& s);


    /// Return the \p i-th site set (const version).
    const S& operator[](unsigned i) const;

    /// Return the \p i-th site set (mutable version).
    S& operator[](unsigned i);

    /// Give the number of elements (site sets) of this composite.
    unsigned nelements() const;


    /// Clear this set.
    void clear();

    /// \cond INTERNAL_API
    /// Test if the site set is empty.
    bool is_empty_() const; // Override the default impl since we have not .nsites().
    /// \endcond

    /// Return the size of this site set in memory.
    std::size_t memory_size() const;

      /// \cond INTERNAL_API
    /// Hook to the set of runs.
    const util::array<S>& array_hook_() const;


    // Required by p_double-related classes.
    const util::array<S>& set_1_() const;
    template <typename I>
    const S& set_2_(const I& it) const;
    /// \endcond

  protected:

    /// Array of site sets.
    util::array<S> arr_;
  };


  /// \relates p_mutable_array_of
  template <typename S>
  std::ostream& operator<<(std::ostream& ostr, const p_mutable_array_of<S>& r);




# ifndef MLN_INCLUDE_ONLY

  template <typename S>
  inline
  p_mutable_array_of<S>::p_mutable_array_of()
  {
  }

  template <typename S>
  inline
  void
  p_mutable_array_of<S>::reserve(unsigned n)
  {
    arr_.reserve(n);
  }

  template <typename S>
  inline
  bool
  p_mutable_array_of<S>::has(const psite& p) const
  {
    return p.index() < arr_.nelements() && arr_[p.index()].has(p.p());
  }

  template <typename S>
  inline
  bool
  p_mutable_array_of<S>::is_valid() const
  {
    return true;
  }

  template <typename S>
  inline
  void
  p_mutable_array_of<S>::insert(const S& s)
  {
    mln_precondition(s.is_valid());
    arr_.append(s);
  }

  template <typename S>
  inline
  const S&
  p_mutable_array_of<S>::operator[](unsigned i) const
  {
    mln_precondition(i < arr_.nelements());
    return arr_[i];
  }

  template <typename S>
  inline
  S&
  p_mutable_array_of<S>::operator[](unsigned i)
  {
    mln_precondition(i < arr_.nelements());
    return arr_[i];
  }

  template <typename S>
  inline
  unsigned
  p_mutable_array_of<S>::nelements() const
  {
    return arr_.nelements();
  }

  template <typename S>
  inline
  void
  p_mutable_array_of<S>::clear()
  {
    arr_.clear();
    mln_postcondition(this->is_empty());
  }

  template <typename S>
  inline
  bool
  p_mutable_array_of<S>::is_empty_() const
  {
    return arr_.is_empty();
  }

  template <typename S>
  inline
  std::size_t
  p_mutable_array_of<S>::memory_size() const
  {
    return arr_.memory_size();
  }

  template <typename S>
  inline
  const util::array<S>&
  p_mutable_array_of<S>::array_hook_() const
  {
    return arr_;
  }

  template <typename S>
  inline
  const util::array<S>&
  p_mutable_array_of<S>::set_1_() const
  {
    return arr_;
  }

  template <typename S>
  template <typename I>
  inline
  const S&
  p_mutable_array_of<S>::set_2_(const I& it) const
  {
    return it.element();
  }


  template <typename S>
  std::ostream& operator<<(std::ostream& ostr, const p_mutable_array_of<S>& a)
  {
    return ostr << a.array_hook_();
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_SITE_SET_P_MUTABLE_ARRAY_OF_HH
