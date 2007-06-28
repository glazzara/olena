#ifndef MLN_CORE_INTERNAL_IMAGE_ADAPTOR_HH
# define MLN_CORE_INTERNAL_IMAGE_ADAPTOR_HH

# include <core/internal/image_base.hh>


namespace mln
{

  namespace internal
  {

    template <typename I, typename E>
    struct image_adaptor_ : public internal::image_base_< mln_pset(I), E >
    {
      typedef mln_psite(I) psite;
      typedef mln_pset(I)  pset;

      typedef mln_value(I)   value;
      typedef mln_rvalue(I) rvalue;
      typedef mln_lvalue(I) lvalue;

      bool has_data() const;

      bool owns_(const psite& p) const;
      const pset& domain() const;

      rvalue operator()(const psite& p) const;
      lvalue operator()(const psite& p);

    protected:
      I& adaptee_;
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
    const mln_pset(I)&
    image_adaptor_<I,E>::domain() const
    {
      return adaptee_.domain();
    }

    template <typename I, typename E>
    mln_rvalue(I)
    image_adaptor_<I,E>::operator()(const psite& p) const
    {
      return adaptee_(p);
    }

    template <typename I, typename E>
    mln_lvalue(I)
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
