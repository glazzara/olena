#ifndef INTERNAL_IMAGE_BASE_HH_
# define INTERNAL_IMAGE_BASE_HH_

#include <concept.hh>

namespace mln
{
  namespace internal
  {
    /// Image base
    /// S is the point set type
    template <typename S, typename E>
    struct image_base_ : public Image<E>
    {
      typedef S pset;
      typedef typename S::psite psite;
      typedef typename S::site site;

      typedef typename S::fwd_piter fwd_piter;
      typedef typename S::bkd_piter bkd_piter;

    protected:
      image_base_();
    };

    ///FIXME: is_ready

# ifndef MLN_INCLUDE_ONLY

    template <typename S, typename E>
    image_base_<S, E>::image_base_()
    {
    }

# endif // ! MLN_INCLUDE_ONLY
  }
}

#endif // ! INTERNAL_IMAGE_BASE_HH_
