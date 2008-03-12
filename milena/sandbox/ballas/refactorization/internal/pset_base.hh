#ifndef INTERNAL_PSET_BASE_HH_
# define INTERNAL_PSET_BASE_HH_


# include <concept.hh>

namespace mln
{
  namespace internal
  {
    /// pset base class
    /// P is the psite type
    /// FIXME: psite = site => Do we declare a site
    ///        typedef in site_base class?
    template <typename P, typename E>
    struct pset_base_ : public Pset<E>
    {
      typedef typename P::site site;
      typedef P psite;
    };
  }
}

#endif // ! INTENAL_PSET_BASE_HH_
