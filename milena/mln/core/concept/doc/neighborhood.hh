/*! \file mln/core/concept/doc/neighborhood.hh
 * \brief This file documents the concept of mln::Neighborhood.
 */

namespace mln
{

  namespace doc
  {

    /*! \brief Documentation class for mln::Neighborhood.
     *
     * \see mln::Neighborhood
     */
    template <typename E>
    struct Neighborhood : public Object<E>
    {
      /*! \brief Piter type associated to this neighborhood to browse
       * neighbors.
       */
      typedef void niter;

      /*! \brief Piter type associated to this neighborhood to browse
       * neighbors in a forward way.
       */
      typedef void fwd_niter;

      /*! \brief Piter type associated to this neighborhood to browse
       * neighbors in a backward way.
       */
      typedef void bkd_niter;
    };

  } // end of namespace mln::doc

} // end of namespace mln
