/*! \file mln/core/concept/doc/dpoint.hh
 * This file documents the concept of mln::Dpoint.
 */

namespace mln
{

  namespace doc
  {

    /*! Documentation class for mln::Dpoint.
     * \see mln::Dpoint
     */
    template <typename E>
    struct Dpoint : public Object<E>
    {
      typedef point;
      typedef coord;
      enum { dim };
      coord operator[](unsigned i) const;
    };

  } // end of namespace mln::doc

} // end of namespace mln
