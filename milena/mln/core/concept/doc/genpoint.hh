/*! \file mln/core/concept/genpoint.doc.hh
 * This file documents the concept of mln::GenPoint.
 */

namespace mln
{

  namespace doc
  {

    template <typename E>
    struct GenPoint
    {
      typedef void point; ///< Associated type of point.

      typedef void dpoint;
      typedef void coord;
      // typedef void topo;

      operator point() const;

      const point* pointer() const;
      coord operator[](unsigned i) const;
    };

  } // end of namespace mln::doc

} // end of namespace mln
