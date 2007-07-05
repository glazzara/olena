#ifndef MLN_CORE_INTERNAL_SET_OF_HH
# define MLN_CORE_INTERNAL_SET_OF_HH

/*! \file mln/core/internal/set_of.hh
 *
 * \brief Definition of mln::set_of_ for internal use only.
 */

# include <vector>
# include <set>
# include <iterator>


namespace mln
{

  namespace internal
  {


    /*! \brief An "efficient" mathematical set class.
     *
     * \internal
     *
     * This set class is designed to store a mathematical set and to
     * present it to the user as a linear array.
     *
     * Elements are stored by copy.
     *
     * \invariant \a v_.size() == s_.size()
     *
     * The parameter \c E is the element type, which shall not be
     * const-qualified.
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
       */
      void insert(const E& elt);


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
      const std::vector<E>& vec() const;


    protected:
      /*! \brief Array of elements.
       *
       * This structure is always up-to-date so that the access method
       * vec is as fastest as possible.
       */
      std::vector<E> v_;


    private:
      /*! \brief Set of elements.
       *
       * This is an auxiliary structure.
       */
      std::set<E> s_;


      /*! \brief Update both attributes.
       *
       * FIXME: explain.
       */
      void update_();
      
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
    void
    set_of_<E>::insert(const E& elt)
    {
      s_.insert(elt);
      update_();
    }

    template <typename E>
    const E& 
    set_of_<E>::element(unsigned i) const
    {
      assert(i < v_.size());
      return v_[i];
    }

    template <typename E>
    unsigned
    set_of_<E>::nelements() const
    {
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
      return v_.size() == 0;
    }

    template <typename E>
    void
    set_of_<E>::clear()
    {
      v_.clear();
      s_.clear();
      mln_postcondition(is_empty());
    }

    template <typename E>
    const std::vector<E>&
    set_of_<E>::vec() const
    {
      return v_;
    }

    template <typename E>
    void
    set_of_<E>::update_()
    {
      v_.clear();
      std::copy(s_.begin(), s_.end(), std::back_inserter(v_));
      // no s_.clear() here to save some execution time
    }

    template <typename E>
    std::ostream& operator<<(std::ostream& ostr,
			     const set_of_<E>& s)
    {
      ostr << '[';
      for (unsigned i = 0; i < s.nelements(); ++i)
	ostr << s.element(i)
	     << (i == s.nelements() - 1 ? ']' : ',');
      return ostr;
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::internal

} // end of namespace mln


#endif // ! MLN_CORE_INTERNAL_SET_OF_HH
