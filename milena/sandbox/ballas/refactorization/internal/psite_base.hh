#ifndef INTERNAL_PSITE_BASE_HH_
# define INTERNAL_PSITE_BASE_HH_

# include <concept.hh>
# include <exact.hh>

namespace mln
{
  namespace internal
  {

    template <typename E, typename P>
    struct psite_base_ : public Psite<E>,
			 public impl<P>
    {
      operator E() const;

    protected:
      psite_base_();
    };

# ifndef MLN_INCLUDE_ONLY

    template <typename E, typename P>
    psite_base_<E, P>::operator E() const
    {
      return exact(this)->to_psite();
    }

    template <typename E, typename P>
    psite_base_<E, P>::psite_base_()
    {
    }

# endif // !MLN_INCLUDE_ONLY

  }
}

#endif // ! INTERNAL_PSITE_BASE_HH_
