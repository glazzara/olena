#ifndef ACCU_TRAIT_HH_
# define ACCU_TRAIT_HH_

# include <mln/trait/undef.hh>

# define mln_trait_accu_when_pix(A) typename trait::accu::accu_traits<A>::when_pix


namespace mln
{
  namespace morpho {
    namespace accu
    {
      template <typename T>
      struct card;
    }
  }

  namespace trait
  {
    namespace accu
    {

      struct when_pix
      {
	struct use_v {};
	struct use_p {};
	struct use_pix {};
	struct use_whatever {};
	struct not_ok {};
      };

      template <typename A>
      struct accu_traits
      {
	typedef undef when_pix;
      };

      template <>
      template <typename T>
      struct accu_traits< mln::morpho::accu::card <T> >
      {
	typedef when_pix::use_p when_pix;
      };
    } //accu

  }//trait

} //mln
#endif /* !ACCU_TRAIT_HH_ */
