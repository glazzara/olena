/*! \file mln/core/concept/doc/iterator.hh
 * \brief This file documents the concept of mln::Iterator.
 */


namespace mln
{

  namespace doc
  {

    /*! \brief Documentation class for mln::Iterator.
     * \see mln::Iterator
     */
    template <typename E>
    struct Iterator : public Object<E>
    {
      /*! \brief Returns true if the iterator is valid, that is, designates
       * an element.
       */
      bool is_valid() const;

      /*! \brief Invalidate the iterator.
       */ 
      void invalidate();

      /*! \brief Start an iteration.
       *
       * Make the iterator designate the first element if it exists.
       * If this first element does not exist, the iterator is not
       * valid.
       */ 
      void start();

      /*! \brief Go to the next element.
       *
       * Make the iterator jump from the current element to the next
       * one.  The current element is thus updated.
       */ 
      void next_();

    protected:
      Iterator();
    };

  } // end of namespace mln::doc

} // end of namespace mln
