/*! \file mln/core/concept/doc/box.hh
 * \brief This file documents the concept of mln::Box.
 */

namespace mln
{

  namespace doc
  {

    /*! \brief Documentation class for mln::Box.
     * \see mln::Box
     */
    template <typename E>
    struct Box : public Point_Set<E>
    {
      /*! \brief Return the box "minimum" point.
       *
       * Return the "minimum" point w.r.t. the ordering between points.
       * For instance, with mln::box2d, this minimum is the top left
       * point of the box.
       */
      const point& pmin() const;

      /*! \brief Return the box "maximum" point.
       *
       * Return the "maximum" point w.r.t. the ordering between points.
       * For instance, with mln::box2d, this maximum is the bottom
       * right point of the box.
       */
      const point& pmax() const;

      /*! \brief Return the bounding box of this point set.
       *
       * Return the bounding box of this point set, so that is itself.
       * This method is declared by the mln::Point_Set concept.
       *
       * \warning This method is final for all box classes.
       */
      const E& bbox() const;

      /*! \brief Return the number of points of this box.
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
