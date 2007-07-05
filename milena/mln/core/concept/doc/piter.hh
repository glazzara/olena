/*! \file mln/core/concept/doc/piter.hh
 * \brief This file documents the concept of mln::Piter.
 */

namespace mln
{

  namespace doc
  {

    /*! \brief Documentation class for mln::Piter.
     *
     * \see mln::Piter
     */
    template <typename E>
    struct Piter : public Iterator<E>,
		   public GenPoint<E>
    {
      /*! \brief Psite associated type.
       * \invariant This type has to derive from mln::Psite.
       */
      typedef void psite;

      /*! \brief Convertion into a point-site.
       *
       * \return A point site.
       */
      operator psite() const;
    };

  } // end of namespace mln::doc

} // end of namespace mln
