/*! \file mln/core/concept/doc/iterator.hh
 * This file documents the concept of mln::Iterator.
 */


namespace mln
{

  namespace doc
  {

    /*! Documentation class for mln::Iterator.
     * \see mln::Iterator
     */
    template <typename E>
    struct Iterator : public Object<E>
    {
      /*! Returns true if the iterator is valid, that is, designates
       * an element.
       */
      bool is_valid() const;

      /*! Invalidate the iterator.
       */ 
      void invalidate();

      /*! Start an iteration, that is, make the iterator designate the
       * first element if it exists.
       */ 
      void start();

      /*! Make the iterator designate the next element.
       */ 
      void next_();

    protected:
      Iterator();
    };

  } // end of namespace mln::doc

} // end of namespace mln
