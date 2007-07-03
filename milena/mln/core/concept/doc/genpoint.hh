/*! \file mln/core/concept/doc/genpoint.hh
 * \brief This file documents the concept of mln::GenPoint.
 */

namespace mln
{

  namespace doc
  {

    /*! \brief Documentation class for mln::GenPoint.
     * \see mln::GenPoint
     */
    template <typename E>
    struct GenPoint
    {
      /*! \var dim
       * \brief Dimension of the space.
       * \invariant dim > 0 
       */
      enum { dim };

      /*! \brief Point associated type.
       * \invariant This type has to derive from mln::Point.
       */
      typedef void point;

      /*! \brief Dpoint associated type.
       * \invariant This type has to derive from mln::Dpoint.
       */
      typedef void dpoint;

      /*! Coordinate associated type.
       */
      typedef void coord;


      // FIXME:
      // typedef void topo;
      // operator point() const;


      /*! \brief Give a hook to the point address.
       *
       * This method allows for iterators to refer to a point.
       *
       * \return A point address.
       */
      const point* pointer() const;

      /*! \brief Read-only access to the \p i-th coordinate value.
       *
       * @param[in] i The coordinate index.
       *
       * \pre \p i < \c dim
       *
       * \return The value of the \p i-th coordinate.
       */
      coord operator[](unsigned i) const;
    };

  } // end of namespace mln::doc

} // end of namespace mln
