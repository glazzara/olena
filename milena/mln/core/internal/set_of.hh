// Copyright (C) 2007 EPITA Research and Development Laboratory
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

#ifndef MLN_CORE_INTERNAL_SET_OF_HH
# define MLN_CORE_INTERNAL_SET_OF_HH

/*! \file mln/core/internal/set_of.hh
 *
 * \brief Definition of mln::set_of_ for internal use only.
 */

# include <vector>
# include <set>
# include <iterator>

# include <mln/core/internal/force_exact.hh>


namespace mln
{

  namespace internal
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
     * \invariant \a v_.size() == s_.size()
     *
     * The parameter \c E is the element type, which shall not be
     * const-qualified.
     *
     * \todo Add a remove method.
     */
    template <typename E>
    class set_of_
    {
    public:


      /*! \brief Insert an element \p elt into the set.
       *
       * \param[in] elt The element to be inserted.
       *
       * If \p elt is already in the set, this method is a no-op.
       *
       * \return The set itself after insertion.
       */
      set_of_<E>& insert(const E& elt);


      /*! \brief Return the i-th element of the set.
       *
       * \param[in] i Index of the element to retrieve.
       *
       * \pre i < nelements()
       *
       * The element is returned by reference and is constant.
       */
      const E& element(unsigned i) const;


      /*! \brief Return the number of elements of the set.
       */
      unsigned nelements() const;


      /*! \brief Test if the object \p elt belongs to the set.
       *
       * \param[in] elt A possible element of the set.
       *
       * \return True is \p elt is in the set.
       */
      bool has(const E& elt) const;


      /*! \brief Test if the set is empty.
       */
      bool is_empty() const;


      /*! \brief Make the set empty.
       *
       * All elements contained in the set are destroyed so the set is
       * emptied.
       *
       * \post is_empty() == true
       */
      void clear();


      /*! \brief Give access to the set elements.
       *
       * The complexity of this method is O(1).
       *
       * \return An array (std::vector) of elements.
       */
      const std::vector<E>& vect() const;

    protected:

      /// Constructor without arguments.
      set_of_();

    private:

      /*! \brief Array of elements.
       *
       * This structure is only updated (if required) when elements
       * are accessed.
       */
      mutable std::vector<E> v_;

      /*! \brief Set of elements.
       *
       * This structure is always up-to-date w.r.t. the set contents.
       */
      std::set<E> s_;


      /*! \brief Update \a v_ from \a s_.
       *
       * FIXME: explain.
       */
      void update_() const;

      /// Tell if \a v_ needs to be updated.
      mutable bool needs_update_;
    };


    /*! \brief Print a set \p s into the output stream \p
     *  ostr.
     *
     * \param[in,out] ostr An output stream.
     * \param[in] s A set.
     *
     * \return The modified output stream \p ostr.
     *
     * \relates mln::internal::set_of_
     */
    template <typename E>
    std::ostream& operator<<(std::ostream& ostr, const set_of_<E>& s);



# ifndef MLN_INCLUDE_ONLY

    template <typename E>
    set_of_<E>::set_of_()
    {
      needs_update_ = false;
    }

    template <typename E>
    set_of_<E>&
    set_of_<E>::insert(const E& elt)
    {
      s_.insert(elt);
      if (needs_update_ == false)
	needs_update_ = true;
      return internal::force_exact< set_of_<E> >(*this);
    }

    template <typename E>
    const E& 
    set_of_<E>::element(unsigned i) const
    {
      assert(i < v_.size());
      if (needs_update_)
	update_();
      return v_[i];
    }

    template <typename E>
    unsigned
    set_of_<E>::nelements() const
    {
      if (needs_update_)
	update_();
      return v_.size();
    }

    template <typename E>
    bool
    set_of_<E>::has(const E& elt) const
    {
      return s_.find(elt) != s_.end();
    }

    template <typename E>
    bool
    set_of_<E>::is_empty() const
    {
      return nelements() == 0;
    }

    template <typename E>
    void
    set_of_<E>::clear()
    {
      v_.clear();
      s_.clear();
      needs_update_ = false;
      mln_postcondition(is_empty());
    }

    template <typename E>
    const std::vector<E>&
    set_of_<E>::vect() const
    {
      if (needs_update_)
	update_();
      return v_;
    }

    template <typename E>
    void
    set_of_<E>::update_() const
    {
      mln_precondition(needs_update_);
      v_.clear();
      std::copy(s_.begin(), s_.end(), std::back_inserter(v_));
      // no s_.clear() here:
      // - we want to keep information up-to-date in s_
      // - we want to save some execution time
      needs_update_ = false;
    }

    template <typename E>
    std::ostream& operator<<(std::ostream& ostr,
			     const set_of_<E>& s)
    {
      ostr << '[';
      const unsigned n = s.nelements();
      for (unsigned i = 0; i < n; ++i)
	ostr << s.element(i)
	     << (i == s.nelements() - 1 ? ']' : ',');
      return ostr;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::internal

} // end of namespace mln


#endif // ! MLN_CORE_INTERNAL_SET_OF_HH
