/*! \file mln/core/concept/doc/neighborhood.hh
 * This file documents the concept of mln::Neighborhood.
 */

namespace mln
{

  namespace doc
  {

    /*! Documentation class for mln::Neighborhood.
     * \see mln::Neighborhood
     */
    template <typename E>
    struct Neighborhood : public Object<E>
    {
      typedef void niter;
      typedef void fwd_niter;
      typedef void bkd_niter;
    };

  } // end of namespace mln::doc

} // end of namespace mln
