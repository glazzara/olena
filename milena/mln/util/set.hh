// Copyright (C) 2007, 2008 EPITA Research and Development Laboratory
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
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef MLN_UTIL_SET_HH
# define MLN_UTIL_SET_HH

/*! \file mln/util/set.hh
 *
 * \brief Definition of mln::util::set.
 * 
 * \todo Clean code and test!
 */

# include <vector>
# include <set>
# include <iterator>
# include <algorithm>
# include <iostream>

# include <mln/core/contract.hh>


namespace mln
{

  namespace util
  {

    /*! \brief An "efficient" mathematical set class.
     *
     * \internal
     *
     * This set class is designed to store a mathematical set and to
     * present it to the user as a linear array (std::vector).
     *
     * Elements are stored by copy.  Implementation is lazy.
     *
     * The set has two states: frozen or not.  There is an automatic
     * switch of state when the user modifies its contents (insert,
     * remove, or clear) or access to its contents (op[i]).
     *
     * The parameter \c T is the element type, which shall not be
     * const-qualified.
     *
     * \todo Add a remove method.
     */
    template <typename T>
    class set
    {
    public:

      /// Type of elements.
      typedef T element;


      /*! \brief Insert an element \p elt into the set.
       *
       * \param[in] elt The element to be inserted.
       *
       * \pre The set is not frozen.
       *
       * If \p elt is already in the set, this method is a no-op.
       *
       * \return The set itself after insertion.
       */
      set<T>& insert(const T& elt);


      /*! \brief Remove an element \p elt into the set.
       *
       * \param[in] elt The element to be inserted.
       *
       * \pre The set is not frozen.
       *
       * If \p elt is already in the set, this method is a no-op.
       *
       * \return The set itself after suppression.
       */
      set<T>& remove(const T& elt);


      /*! \brief Empty the set.
       *
       * All elements contained in the set are destroyed so the set is
       * emptied.
       *
       * \pre The set is not frozen.
       *
       * \post is_empty() == true
       */
      void clear();


      /*! \brief Return the number of elements of the set.
       */
      unsigned nelements() const;


      /*! \brief Test if the set is empty.
       */
      bool is_empty() const;



      /*! \brief Return the i-th element of the set.
       *
       * \param[in] i Index of the element to retrieve.
       *
       * \pre i < nelements()
       *
       * The element is returned by reference and is constant.
       */
      const T& operator[](unsigned i) const;


      /*! \brief Test if the object \p elt belongs to the set.
       *
       * \param[in] elt A possible element of the set.
       *
       * \return True is \p elt is in the set.
       */
      bool has(const T& elt) const;


      /*! \brief Give access to the set elements.
       *
       * The complexity of this method is O(1).
       *
       * \post The set is frozen.
       *
       * \return An array (std::vector) of elements.
       */
      const std::vector<T>& vect() const;


      /// Constructor without arguments.
      set();


    private:

      /*! \brief Array of elements.
       *
       * This structure is only updated (if required) when elements
       * are accessed.
       */
      mutable std::vector<T> v_;

      /*! \brief Set of elements.
       *
       * This structure is always up-to-date w.r.t. the set contents.
       */
      mutable std::set<T> s_;


      /*! \brief Freeze the contents of the set (update \a v_ from \a
       * s_, then clear s_).
       */
      void freeze_() const;

      /*! \brief Unfreeze the contents of the set.
       */
      void unfreeze_() const;

      /// Tell if the set is frozen.
      mutable bool frozen_;
    };


# ifndef MLN_INCLUDE_ONLY

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
    bool
    set<T>::has(const T& elt) const
    {
      return frozen_
	? std::find(v_.begin(), v_.end(), elt) != v_.end()
	: s_.find(elt) != s_.end();
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
    set<T>::vect() const
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
    std::ostream& operator<<(std::ostream& ostr,
    			     const mln::util::set<T>& s)
    {
      ostr << '[';
      const unsigned n = s.nelements();
      for (unsigned i = 0; i < n; ++i)
    	ostr << s[i]
    	     << (i == n - 1 ? ']' : ',');
      return ostr;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::util

} // end of namespace mln


#endif // ! MLN_UTIL_SET_HH
