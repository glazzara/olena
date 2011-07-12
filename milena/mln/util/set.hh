// Copyright (C) 2007, 2008, 2009 EPITA Research and Development
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

#ifndef MLN_UTIL_SET_HH
# define MLN_UTIL_SET_HH

/// \file
///
/// Definition of mln::util::set.
///
/// \todo Clean code and test!
///
/// \todo Zed: Group methods into 2 categories: when frozen, when not.

# include <vector>
# include <set>
# include <iterator>
# include <algorithm>
# include <iostream>

# include <mln/core/concept/proxy.hh>
# include <mln/util/ord.hh>


namespace mln
{

  namespace util
  {

    // Forward declarations.
    template <typename T> class set_fwd_iter;
    template <typename T> class set_bkd_iter;


    /// \brief An "efficient" mathematical set class.
    ///
    /// This set class is designed to store a mathematical set and to
    /// present it to the user as a linear array (std::vector).
    ///
    /// Elements are stored by copy.  Implementation is lazy.
    ///
    /// The set has two states: frozen or not.  There is an automatic
    /// switch of state when the user modifies its contents (insert,
    /// remove, or clear) or access to its contents (op[i]).
    ///
    /// The parameter \c T is the element type, which shall not be
    /// const-qualified.
    ///
    /// The unicity (and the order) of set elements is handled by the
    /// mln::util::ord mechanism.
    ///
    /// \see mln::util::ord
    ///
    /// \ingroup modutil
    //
    template <typename T>
    class set : public Object< mln::util::set<T> >
    {
    public:

      /// Element associated type.
      typedef T element;


      /// Forward iterator associated type.
      typedef set_fwd_iter<T> fwd_eiter;

      /// Backward iterator associated type.
      typedef set_bkd_iter<T> bkd_eiter;

      /// Iterator associated type.
      typedef fwd_eiter eiter;


      /// \brief Insert an element \p elt into the set.
      ///
      /// \param[in] elt The element to be inserted.
      ///
      /// If \p elt is already in the set, this method is a no-op.
      ///
      /// \return The set itself after insertion.
      ///
      set<T>& insert(const T& elt);


      /// \brief Insert the elements of \p other into the set.
      ///
      /// \param[in] other The set containing the elements to be inserted.
      ///
      /// \return The set itself after insertion.
      ///
      template <typename U>
      set<T>& insert(const set<U>& other);


      /// \brief Remove an element \p elt into the set.
      ///
      /// \param[in] elt The element to be inserted.
      ///
      /// If \p elt is already in the set, this method is a no-op.
      ///
      /// \return The set itself after suppression.
      ///
      set<T>& remove(const T& elt);


      /// \brief Empty the set.
      ///
      /// All elements contained in the set are destroyed so the set is
      /// emptied.
      ///
      /// \post is_empty() == true
      ///
      void clear();


      /// \brief Return the number of elements of the set.
      ///
      unsigned nelements() const;


      /// \brief Test if the set is empty.
      ///
      bool is_empty() const;



      /// \brief Return the i-th element of the set.
      ///
      /// \param[in] i Index of the element to retrieve.
      ///
      /// \pre i < nelements()
      ///
      /// The element is returned by reference and is constant.
      ///
      const T& operator[](unsigned i) const;

      /// Return the first element of the set.
      /// \pre not is_empty()
      const T first_element() const;

      /// Return the last element of the set.
      /// \pre not is_empty()
      const T last_element() const;


      /// \brief Test if the object \p elt belongs to the set.
      ///
      /// \param[in] elt A possible element of the set.
      ///
      /// \return True is \p elt is in the set.
      ///
      bool has(const T& elt) const;


      /// \brief Give access to the set elements.
      ///
      /// The complexity of this method is O(1).
      ///
      /// \post The set is frozen.
      ///
      /// \return An array (std::vector) of elements.
      ///
      const std::vector<T>& std_vector() const;


      /// Constructor without arguments.
      set();


      /// Return the size of this set in memory.
      std::size_t memory_size() const;

      /// Test if the set is frozen.
      bool is_frozen_() const;

    private:

      /// \brief Array of elements.
      ///
      /// This structure is only updated (if required) when elements
      /// are accessed.
      ///
      mutable std::vector<T> v_;

      /// \brief Set of elements.
      ///
      /// This structure is always up-to-date w.r.t. the set contents.
      ///
      mutable std::set< T, util::ord<T> > s_;


      /// \brief Freeze the contents of the set (update \a v_ from \a
      /// s_, then clear s_).
      ///
      void freeze_() const;

      /// \brief Unfreeze the contents of the set.
      ///
      void unfreeze_() const;

      /// Tell if the set is frozen.
      mutable bool frozen_;


      // Used in method has(elt) when v_ holds data.
      bool v_has_(const T& elt) const;
      unsigned dicho_(const T& elt, unsigned beg, unsigned end) const;
    };


    template <typename T>
    bool operator == (const set<T>& lhs, const set<T>& rhs);


    template <typename T>
    bool operator <  (const set<T>& lhs, const set<T>& rhs);


    template <typename T>
    bool operator <= (const set<T>& lhs, const set<T>& rhs);



    // set_fwd_iter<T>

    template <typename T>
    class set_fwd_iter : public Proxy< set_fwd_iter<T> >,
			 public mln::internal::proxy_impl< const T&, set_fwd_iter<T> >
    {
    public:

      /// Constructor without argument.
      set_fwd_iter();

      /// Constructor from a set \p s.
      set_fwd_iter(const set<T>& s);

      /// Change the set it iterates on to \p s.
      void change_target(const set<T>& s);

      /// Start an iteration.
      void start();

      /// Go to the next element.
      void next();

      /// Returns true if the iterator is valid.
      bool is_valid() const;

      /// Invalidate the iterator.
      void invalidate();

      /// Give the element the iterator designates.
      const T& element() const;

      // As a Proxy.
      const T& subj_();

      /// Give the current index.
      unsigned index_() const;

    protected:
      unsigned i_;
      const set<T>* s_;
    };




    // set_bkd_iter<T>

    template <typename T>
    class set_bkd_iter : public Proxy< set_bkd_iter<T> >,
			 public mln::internal::proxy_impl< const T&, set_bkd_iter<T> >
    {
    public:

      /// Constructor without argument.
      set_bkd_iter();

      /// Constructor from a set \p s.
      set_bkd_iter(const set<T>& s);

      /// Change the set it iterates on to \p s.
      void change_target(const set<T>& s);

      /// Start an iteration.
      void start();

      /// Go to the next element.
      void next();

      /// Returns true if the iterator is valid.
      bool is_valid() const;

      /// Invalidate the iterator.
      void invalidate();

      /// Give the element the iterator designates.
      const T& element() const;

      // As a Proxy.
      const T& subj_();

      /// Give the current index.
      unsigned index_() const;

    protected:
      unsigned i_;
      const set<T>* s_;
    };



# ifndef MLN_INCLUDE_ONLY


    // util::set<T>


    template <typename T>
    inline
    set<T>::set()
    {
      frozen_ = false;
    }

    template <typename T>
    inline
    set<T>&
    set<T>::insert(const T& elt)
    {
      if (frozen_) unfreeze_();
      s_.insert(elt);
      return *this;
    }

    template <typename T>
    template <typename U>
    inline
    set<T>&
    set<T>::insert(const set<U>& other)
    {
      if (other.is_empty())
	// No-op.
	return *this;
      if (frozen_) unfreeze_();
      s_.insert(other.std_vector().begin(), other.std_vector().end());
      return *this;
    }

    template <typename T>
    inline
    set<T>&
    set<T>::remove(const T& elt)
    {
      if (frozen_) unfreeze_();
      s_.erase(elt);
      return *this;
    }

    template <typename T>
    inline
    void
    set<T>::clear()
    {
      if (frozen_)
	{
	  mln_invariant(s_.empty());
	  v_.clear();
	  frozen_ = false;
	}
      else
	{
	  mln_invariant(v_.empty());
	  s_.clear();
	}
      mln_postcondition(is_empty());
    }

    template <typename T>
    inline
    unsigned
    set<T>::nelements() const
    {
      return frozen_ ? v_.size() : s_.size();
    }

    template <typename T>
    inline
    const T&
    set<T>::operator[](unsigned i) const
    {
      mln_precondition(i < nelements());
      if (! frozen_) freeze_();
      return v_[i];
    }

    template <typename T>
    inline
    const T
    set<T>::first_element() const
    {
      mln_precondition(! is_empty());
      return frozen_ ? *v_.begin() : *s_.begin();
    }

    template <typename T>
    inline
    const T
    set<T>::last_element() const
    {
      mln_precondition(! is_empty());
      return frozen_ ? *v_.rbegin() : *s_.rbegin();
    }

    template <typename T>
    inline
    bool
    set<T>::has(const T& elt) const
    {
      return frozen_ ? v_has_(elt) : s_.find(elt) != s_.end();
    }

    template <typename T>
    inline
    bool
    set<T>::is_empty() const
    {
      return nelements() == 0;
    }

    template <typename T>
    inline
    const std::vector<T>&
    set<T>::std_vector() const
    {
      if (! frozen_) freeze_();
      return v_;
    }

    template <typename T>
    inline
    void
    set<T>::freeze_() const
    {
      mln_precondition(frozen_ == false);
      mln_invariant(v_.empty());
      std::copy(s_.begin(), s_.end(), std::back_inserter(v_));
      s_.clear();
      frozen_ = true;
    }

    template <typename T>
    inline
    void
    set<T>::unfreeze_() const
    {
      mln_precondition(frozen_ == true);
      mln_invariant(s_.empty());
      s_.insert(v_.begin(), v_.end());
      v_.clear();
      frozen_ = false;
    }

    template <typename T>
    inline
    std::size_t
    set<T>::memory_size() const
    {
      return nelements() * sizeof(T);
    }

    template <typename T>
    inline
    bool
    set<T>::is_frozen_() const
    {
      return frozen_;
    }

    template <typename T>
    inline
    bool
    set<T>::v_has_(const T& elt) const
    {
      mln_precondition(frozen_);
      if (is_empty() ||
	  util::ord_strict(elt, v_[0]) ||
	  util::ord_strict(v_[nelements() - 1], elt))
	return false;
      return v_[dicho_(elt, 0, nelements())] == elt;
    }

    template <typename T>
    inline
    unsigned
    set<T>::dicho_(const T& elt, unsigned beg, unsigned end) const
    {
      mln_precondition(frozen_);
      mln_precondition(beg <= end);
      if (end - beg <= 1)
	return beg;
      unsigned med = (beg + end) / 2;
      return util::ord_strict(elt, v_[med])
	? dicho_(elt, beg, med)
	: dicho_(elt, med, end);
    }



    // util::set_fwd_iter<T>


    template <typename T>
    inline
    set_fwd_iter<T>::set_fwd_iter()
    {
      s_ = 0;
    }

    template <typename T>
    inline
    set_fwd_iter<T>::set_fwd_iter(const set<T>& s)
    {
      change_target(s);
    }

    template <typename T>
    inline
    void
    set_fwd_iter<T>::change_target(const set<T>& s)
    {
      s_ = &s;
      invalidate();
    }

    template <typename T>
    inline
    void
    set_fwd_iter<T>::start()
    {
      mln_precondition(s_ != 0);
      i_ = 0;
    }

    template <typename T>
    inline
    void
    set_fwd_iter<T>::next()
    {
      mln_precondition(is_valid());
      ++i_;
    }
      
    template <typename T>
    inline
    bool
    set_fwd_iter<T>::is_valid() const
    {
      return s_ != 0 && i_ != s_->nelements();
    }

    template <typename T>
    inline
    void
    set_fwd_iter<T>::invalidate()
    {
      if (s_ != 0)
	i_ = s_->nelements();
      mln_postcondition(! is_valid());
    }

    template <typename T>
    inline
    const T&
    set_fwd_iter<T>::element() const
    {
      mln_precondition(is_valid());
      return s_->operator[](i_);
    }

    template <typename T>
    inline
    const T&
    set_fwd_iter<T>::subj_()
    {
      mln_precondition(is_valid());
      return s_->operator[](i_);
    }

    template <typename T>
    inline
    unsigned
    set_fwd_iter<T>::index_() const
    {
      return i_;
    }



    // util::set_bkd_iter<T>


    template <typename T>
    inline
    set_bkd_iter<T>::set_bkd_iter()
    {
      s_ = 0;
    }

    template <typename T>
    inline
    set_bkd_iter<T>::set_bkd_iter(const set<T>& s)
    {
      change_target(s);
    }

    template <typename T>
    inline
    void
    set_bkd_iter<T>::change_target(const set<T>& s)
    {
      s_ = &s;
      invalidate();
    }

    template <typename T>
    inline
    void
    set_bkd_iter<T>::start()
    {
      mln_precondition(s_ != 0);
      if (! s_->is_empty())
	i_ = s_->nelements() - 1;
    }

    template <typename T>
    inline
    void
    set_bkd_iter<T>::next()
    {
      mln_precondition(is_valid());
      if (i_ == 0)
	invalidate();
      else
	--i_;
    }
      
    template <typename T>
    inline
    bool
    set_bkd_iter<T>::is_valid() const
    {
      return s_ != 0 && i_ != s_->nelements();
    }

    template <typename T>
    inline
    void
    set_bkd_iter<T>::invalidate()
    {
      if (s_ != 0)
	i_ = s_->nelements();
      mln_postcondition(! is_valid());
    }

    template <typename T>
    inline
    const T&
    set_bkd_iter<T>::element() const
    {
      mln_precondition(is_valid());
      return s_->operator[](i_);
    }

    template <typename T>
    inline
    const T&
    set_bkd_iter<T>::subj_()
    {
      mln_precondition(is_valid());
      return s_->operator[](i_);
    }

    template <typename T>
    inline
    unsigned
    set_bkd_iter<T>::index_() const
    {
      return i_;
    }



    // Operators.

    template <typename T>
    std::ostream& operator<<(std::ostream& ostr,
    			     const mln::util::set<T>& s)
    {
      ostr << '{';
      const unsigned n = s.nelements();
      for (unsigned i = 0; i < n; ++i)
	{
	  ostr << s[i];
	  if (i != n - 1)
	    ostr << ", ";
	}
      ostr << '}';
      return ostr;
    }

    template <typename T>
    bool operator == (const set<T>& lhs, const set<T>& rhs)
    {
      const unsigned n = lhs.nelements();
      if (rhs.nelements() != n)
	return false;
      for (unsigned i = 0; i < n; ++i)
	if (lhs[i] != rhs[i])
	  return false;
      return true;
    }

    template <typename T>
    bool operator <  (const set<T>& lhs, const set<T>& rhs)
    {
      return lhs.nelements() < rhs.nelements() && lhs <= rhs;
    }

    template <typename T>
    bool operator <= (const set<T>& lhs, const set<T>& rhs)
    {
      const unsigned
	nl = lhs.nelements(),
	nr = rhs.nelements();
      if (nl > nr)
	return false;
      // Every element of lhs can be found in rhs.
      for (unsigned l = 0, r = 0; l < nl; ++l, ++r)
	{
	  while (rhs[r] != lhs[l] && r < nr)
	    ++r;
	  if (r == nr)
	    return false; // lhs[l] has not been found in rhs.
	}
      return true;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::util

} // end of namespace mln


#endif // ! MLN_UTIL_SET_HH
