#ifndef INTERNAL_PITER_BASE_HH_
# define INTERNAL_PITER_BASE_HH_

# include <concept.hh>
# include <point2d_impl.hh>

# include <exact.hh>
# include <metal/bool.hh>
# include <metal/equal.hh>

namespace mln
{

  namespace internal
  {

    /// fwd declaration
    template <typename E, typename Site, typename B>
    struct piter_base_site_cast;

    /// Psite = Site
    template <typename E, typename Site>
    struct piter_base_site_cast<E, Site, metal::true_> : public Piter<E>
    {
    };

    /// Psite != site
    template <typename E, typename Site>
    struct piter_base_site_cast<E, Site, metal::false_> : public Piter<E>
    {
      operator Site() const;
    };

    /// Piter base
    template <typename E, typename Site, typename Psite>
    struct piter_base_ :
      public impl<E, Site>,
      public piter_base_site_cast<E, Site,
				  typename mlc_equal(Site, Psite)::eval>
    {
      operator Psite () const;

    protected:
      piter_base_();
    };

# ifndef MLN_INCLUDE_ONLY

    template <typename E, typename Site>
    piter_base_site_cast<E, Site, metal::false_>::operator Site () const
    {
      return exact(this)->to_site();
    }


    template <typename E, typename Site, typename Psite>
    piter_base_<E, Site, Psite>::operator Psite() const
    {
      return exact(this)->to_psite();
    }


    template <typename E, typename Site, typename Psite>
    piter_base_<E, Site, Psite>::piter_base_()
    {
    }

# endif // ! MLN_INCLUDE_ONLY

  }

}


#endif // !INTERNAL_PITER_BASE_HH_
