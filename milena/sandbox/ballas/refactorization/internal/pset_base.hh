#ifndef INTERNAL_PSET_BASE_HH_
# define INTERNAL_PSET_BASE_HH_


# include <concept.hh>
# include <exact.hh>

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
      /// !!!
      typedef typename P::site site;
      typedef P psite;


      bool has(const P& ps) const;
    protected:
      pset_base_();
    };


# ifndef MLN_INCLUDE_ONLY

    template <typename P, typename E>
    pset_base_<P, E>::pset_base_()
    {
    }

    template <typename P, typename E>
    bool
    pset_base_<P, E>::has(const P& ps) const
    {
      return exact(this)->has(ps);
    }

# endif // ! MLN_INCLUDE_ONLY

  }
}

#endif // ! INTENAL_PSET_BASE_HH_
