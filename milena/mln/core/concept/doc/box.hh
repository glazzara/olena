/*! \file mln/core/concept/doc/box.hh
 * This file documents the concept of mln::Box.
 */

namespace mln
{

  namespace doc
  {

    /*! Documentation class for mln::Box.
     * \see mln::Box
     */
    template <typename E>
    struct Box : public Point_Set<E>
    {
      /*! \fn const point& pmin() const
       *
       * Return the minimum point w.r.t. the ordering between points.
       *
       * In 2D this minimum is the top left point of the box.
       */
      const point& pmin() const;

      /*! \fn const point& pmax() const
       *
       * Return the maximum point w.r.t. the ordering between points.
       *
       * In 2D this maximum is the bottom right point of the box.
       */
      const point& pmax() const;

      /*! \fn const E& bbox() const
       *
       * Return the bounding box of this point set, so that is itself.
       * This method is declared by the mln::Point_Set concept.
       *
       * \warning This method is final for all box classes.
       */
      const E& bbox() const;

      /*! \fn std::size_t npoints() const
       *
       * Return the number of points of this box.  This method is
       * declared by the mln::Point_Set concept.
       *
       * \warning This method is final for all box classes.
       */
      std::size_t npoints() const;
    };

  } // end of namespace mln::doc

} // end of namespace mln
