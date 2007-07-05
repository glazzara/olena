#ifndef MLN_CORE_INTERNAL_IMAGE_BASE_HH
# define MLN_CORE_INTERNAL_IMAGE_BASE_HH

/*! \file mln/core/internal/image_base.hh
 *
 * \brief Definition of a base class for some images.
 */

# include <mln/core/concept/image.hh>


namespace mln
{

  namespace internal
  {

    /*! \brief A base class for images.
     *
     * \internal
     */
    template <typename S, typename E>
    struct image_base_ : public Image<E>
    {
      /// Point_Set associated type.
      typedef S pset;

      /// Psite associated type.
      typedef mln_psite(S) psite;

      /// Point associated type.
      typedef mln_point(S) point;

      /// Forward Piter associated type.
      typedef mln_fwd_piter(S) fwd_piter;

      /// Backward Piter associated type.
      typedef mln_bkd_piter(S) bkd_piter;

      /// Piter associated type.
      typedef fwd_piter piter;

      /// Test if \p p belongs to the image domain.
      bool has(const psite& p) const;

      /// Test if a pixel value is accessible at \p p.
      bool owns_(const psite& p) const;

      /// Give a bounding box of the image domain.
      const box_<point>& bbox() const;

    protected:
      image_base_();
    };


# ifndef MLN_INCLUDE_ONLY

    template <typename S, typename E>
    bool
    image_base_<S,E>::has(const psite& p) const
    {
      return exact(this)->domain().has(p);
    }

    template <typename S, typename E>
    bool
    image_base_<S,E>::owns_(const psite& p) const // default
    {
      return this->has(p);
    }

    template <typename S, typename E>
    const box_<mln_point(S)>&
    image_base_<S,E>::bbox() const
    {
      return exact(this)->domain().bbox();
    }

    template <typename S, typename E>
    image_base_<S,E>::image_base_()
    {
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::internal

} // end of namespace mln


#endif // ! MLN_CORE_INTERNAL_IMAGE_BASE_HH
