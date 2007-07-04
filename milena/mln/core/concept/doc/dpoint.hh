/*! \file mln/core/concept/doc/dpoint.hh
 * \brief This file documents the concept of mln::Dpoint.
 */

namespace mln
{

  namespace doc
  {

    /*! \brief Documentation class for mln::Dpoint.
     * \see mln::Dpoint
     */
    template <typename E>
    struct Dpoint : public Object<E>
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

      /*! \brief Read-only access to the \p i-th coordinate value.
       *
       * \param[in] i The coordinate index.
       *
       * \pre \p i < \c dim
       *
       * \return The value of the \p i-th coordinate.
       */
      coord operator[](unsigned i) const;
    };

  } // end of namespace mln::doc

} // end of namespace mln
