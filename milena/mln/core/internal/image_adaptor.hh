#ifndef MLN_CORE_INTERNAL_IMAGE_ADAPTOR_HH
# define MLN_CORE_INTERNAL_IMAGE_ADAPTOR_HH

/*! \file mln/core/internal/image_adaptor.hh
 *
 * \brief Definition of a base class for image adaptors.
 */

# include <mln/core/internal/image_base.hh>


namespace mln
{

  namespace internal
  {

    /*! \brief A base class for image adaptors.
     *
     * \internal
     */
    template <typename I, typename E>
    struct image_adaptor_ : public internal::image_base_< mln_pset(I), E >
    {
      /// Psite associated type.
      typedef mln_psite(I) psite;

      /// Point_Set associated type.
      typedef mln_pset(I)  pset;

      /// Value associated type.
      typedef mln_value(I)   value;

      /// Return type of read-only access.
      typedef mln_rvalue(I) rvalue;

      /// Return type of read-write access.
      typedef mln_lvalue(I) lvalue;

      /// Test if this image has been initialized.
      bool has_data() const;

      /// Test if a pixel value is accessible at \p p.
      bool owns_(const psite& p) const;

      /// Give the definition domain.
      const pset& domain() const;

      /// Read-only access of pixel value at point site \p p.
      rvalue operator()(const psite& p) const;

      /// Read-write access of pixel value at point site \p p.
      lvalue operator()(const psite& p);

    protected:
      I& adaptee_;

      /// Constructor from an \p adaptee image.
      image_adaptor_(Image<I>& adaptee);
    };

    // FIXME: image_const_adaptor_



# ifndef MLN_INCLUDE_ONLY

    template <typename I, typename E>
    bool image_adaptor_<I,E>::has_data() const
    {
      return adaptee_.has_data();
    }

    template <typename I, typename E>
    bool image_adaptor_<I,E>::owns_(const psite& p) const
    {
      return adaptee_.owns_(p);
    }

    template <typename I, typename E>
    const image_adaptor_<I,E>::pset&
    image_adaptor_<I,E>::domain() const
    {
      return adaptee_.domain();
    }

    template <typename I, typename E>
    typename image_adaptor_<I,E>::rvalue
    image_adaptor_<I,E>::operator()(const psite& p) const
    {
      return adaptee_(p);
    }

    template <typename I, typename E>
    typename image_adaptor_<I,E>::lvalue
    image_adaptor_<I,E>::operator()(const psite& p)
    {
      return adaptee_(p);
    }

    template <typename I, typename E>
    image_adaptor_<I,E>::image_adaptor_(Image<I>& adaptee)
      : adaptee_(exact(adaptee))
    {
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::internal

} // end of namespace mln


#endif // ! MLN_CORE_INTERNAL_IMAGE_ADAPTOR_HH
