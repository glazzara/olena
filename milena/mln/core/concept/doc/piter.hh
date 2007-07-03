/*! \file mln/core/concept/doc/piter.hh
 * \brief This file documents the concept of mln::Piter.
 */

namespace mln
{

  namespace doc
  {

    /*! \brief Documentation class for mln::Piter.
     * \see mln::Piter
     */
    template <typename E>
    struct Piter : public Iterator<E>,
		   public GenPoint<E>
    {
      // FIXME: HERE
      typedef void psite;
      typedef void point;

      operator psite() const;
      operator point() const;
      const point* pointer() const;
    };

  } // end of namespace mln::doc

} // end of namespace mln
