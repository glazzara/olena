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

    oln_rvalue(I) operator()(const oln_psite(I)& p) const;
    oln_lvalue(I) operator()(const oln_psite(I)& p);
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
  oln_rvalue(I)
  safe_image<I>::operator()(const oln_psite(I)& p) const
  {
    static oln_value(I) tmp;
    if (not this->owns_(p))
      return tmp;
    return this->adaptee_(p);
  }

  template <typename I>
  oln_lvalue(I)
  safe_image<I>::operator()(const oln_psite(I)& p)
  {
    static oln_value(I) tmp;
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
