#ifndef MLN_CORE_INTERNAL_IMAGE_BASE_HH
# define MLN_CORE_INTERNAL_IMAGE_BASE_HH

# include <mln/core/concept/image.hh>


namespace mln
{

  namespace internal
  {

    template <typename S, typename E>
    struct image_base_ : public Image<E>
    {
      typedef S pset;
      typedef mln_psite(S) psite;
      typedef mln_point(S) point;

      typedef mln_fwd_piter(S) fwd_piter;
      typedef mln_bkd_piter(S) bkd_piter;
      typedef fwd_piter piter;

      bool has(const psite& p) const;
      bool owns_(const psite& p) const; // default
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
