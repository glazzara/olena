#ifndef INTERNAL_PSITE_BASE_HH_
# define INTERNAL_PSITE_BASE_HH_

# include <concept.hh>
# include <exact.hh>

namespace mln
{
  namespace internal
  {

    template <typename E>
    struct psite_base : public Psite<E>,
			public impl<typename E::site>
    {
      operator typename E::psite () const;

    protected:
      psite_base();
    };

# ifndef MLN_INCLUDE_ONLY

    template <typename E>
    psite_base<E>::operator typename E::psite () const
    {
      return exact(this)->to_site();
    }

    template <typename E>
    psite_base<E>::psite_base()
    {
    }

# endif // !MLN_INCLUDE_ONLY

  }
}

#endif // ! INTERNAL_PSITE_BASE_HH_
