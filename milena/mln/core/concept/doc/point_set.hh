/*! \file mln/core/concept/doc/point_set.hh
 * \brief This file documents the concept of mln::Point_Set.
 */

namespace mln
{

  namespace doc
  {

    /*! \brief Documentation class for mln::Point_Set.
     * \see mln::Point_Set
     */
    template <typename E>
    struct Point_Set : public Object<E>
    {
      /*! \brief Point associated type.
       */
      typedef void point;

      /*! \brief Psite associated type.
       */
      typedef void psite;

      /*! \brief Piter associated type.
       */
      typedef void piter;

      /*! \brief Forward Piter associated type.
       */
      typedef void fwd_piter;

      /*! \brief Backward Piter associated type.
       */
      typedef void bkd_piter;

      /*! \brief Test if the \p p belongs to this point set. 
       *
       * @param[in] p A point site.
       * \return True if \p p is an element of the point set.
       */
      bool has(const psite& p) const;

      /*! \brief Give the bounding box of this point set.
       */
      const box_<point>& bbox() const;

      /*! \brief Give the number of points of this point set.
       */
      std::size_t npoints() const;
    };

  } // end of namespace mln::doc

} // end of namespace mln
