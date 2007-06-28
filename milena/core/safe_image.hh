#ifndef MLN_CORE_SAFE_IMAGE_HH
# define MLN_CORE_SAFE_IMAGE_HH

# include <core/internal/image_adaptor.hh>


namespace mln
{


  template <typename I>
  struct safe_image : public internal::image_adaptor_< I, safe_image<I> >
  {
    typedef internal::image_adaptor_< I, safe_image<I> > super;

    safe_image(Image<I>& ima);

    mln_rvalue(I) operator()(const mln_psite(I)& p) const;
    mln_lvalue(I) operator()(const mln_psite(I)& p);
  };



  template <typename I>
  safe_image<I> safe(Image<I>& ima);



# ifndef MLN_INCLUDE_ONLY


  template <typename I>
  safe_image<I>::safe_image(Image<I>& ima)
    : super(ima)
  {
  }

  template <typename I>
  mln_rvalue(I)
  safe_image<I>::operator()(const mln_psite(I)& p) const
  {
    static mln_value(I) tmp;
    if (not this->owns_(p))
      return tmp;
    return this->adaptee_(p);
  }

  template <typename I>
  mln_lvalue(I)
  safe_image<I>::operator()(const mln_psite(I)& p)
  {
    static mln_value(I) tmp;
    if (not this->owns_(p))
      return tmp;
    return this->adaptee_(p);
  }

  template <typename I>
  safe_image<I> safe(Image<I>& ima)
  {
    safe_image<I> tmp(ima);
    return tmp;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_SAFE_IMAGE_HH
