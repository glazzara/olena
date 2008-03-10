#ifndef INTERNAL_SITE_BASE_HH_
# define INTERNAL_SITE_BASE_HH_

# include <concept>

namespace mln
{
  namespace internal
  {

    /// Site base class
    template <typename S, typename E>
    struct site_base_ : public site_base_<E>
    {
      typedef S site;
    };
  }
}


#endif // ! INTERNAL_SITE_BASE_HH_
