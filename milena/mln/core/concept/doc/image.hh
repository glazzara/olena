/*! \file mln/core/concept/doc/image.hh
 * \brief This file documents the concept of mln::Image.
 */

namespace mln
{

  namespace doc
  {

    /*! \brief Documentation class for mln::Image.
     * \see mln::Image
     */
    template <typename E>
    struct Image : public Object<E>
    {
      /*! \brief Value associated type.
       *
       *  \invariant This type is neither qualified by const, nor by
       * reference.
       */
      typedef void value;

      /*! \brief Type returned by the read pixel value operator.
       */
      typedef void rvalue;

      /*! \brief Type returned by the read-write pixel value operator.
       */
      typedef void lvalue;

      /*! \brief Test if the image have been initialized.
       */
      bool has_data() const;

      /*! \brief Test if the image owns the poinst site \p p.
       *
       * \return True if accessing the image value at \p p is
       * possible, that is, does not abort the execution.
       */
      bool owns_(const psite& p) const;

      /*! \brief Give the definition domain of the image.
       *
       * \return A reference to the domain point set.
       */
      const pset& domain() const;

      /*! \brief Read access to the image value located at \p p.
       *
       * \param[in] p A point site.
       *
       * \pre The image has to own the site \p p.
       *
       * \return The value at \p p.
       */
      rvalue operator()(const psite& p) const;

      /*! \brief Read-write access to the image value located at \p p.
       *
       * \param[in] p A point site.
       *
       * \pre The image has to own the site \p p.
       *
       * \return The value at \p p.
       */
      lvalue operator()(const psite& p);



      /*! \brief Trait that gives the corresponding image type with
	the value type changes to T.
       */
      template <typename T>
      struct change_value
      {
        typedef ret;
      };



      /*! \brief Point set associated type.
       * \invariant This type has to derive from mln::Point_Set.
       */
      typedef void pset;

      /*! \brief Point associated type.
       * \invariant This type has to derive from mln::Point.
       */
      typedef void point;

      /*! \brief Psite associated type.
       * \invariant This type has to derive from mln::Psite.
       */
      typedef void psite;

      /*! \brief Piter associated type.
       * \invariant This type has to derive from mln::Piter.
       */
      typedef void piter;

      /*! \brief Forward point iterator associated type.
       * \invariant This type has to derive from mln::Piter.
       */
      typedef void fwd_piter;

      /*! \brief Backward point iterator associated type.
       * \invariant This type has to derive from mln::Piter.
       */
      typedef void bkd_piter;

      /*! \brief Test if \p p belongs to the image domain. 
       *
       * \param[in] p A point site.
       *
       * \return True if \p p belongs to the image domain.
       *
       * \invariant has(p) is true => owns_(p) is also true.
       */
      bool has(const psite& p) const;

      /*! \brief Give a bounding box of the image domain. 
       *
       * This bounding box may be larger than the smallest bounding
       * box (the optimal one).  Practically an image type is not
       * obliged to update its bounding box so that it is always
       * optimal.
       *
       * \return A bounding box of the image domain.
       */
      const box_<point>& bbox() const;
    };

  } // end of namespace mln::doc

} // end of namespace mln
