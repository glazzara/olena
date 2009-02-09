#ifndef LEVELING_HH_
# define LEVELING_HH_


# include "accu_trait.hh"
# include <mln/metal/equal.hh>
# include <mln/accu/all.hh>
# include <mln/core/concept/image.hh>
# include <mln/util/pix.hh>
# include <mln/make/pix.hh>

using namespace mln;

namespace mln
{
  namespace impl
  {
    // fast implementation
    template <typename I, typename A>
    void
    leveling_fast (const Image<I>& input,
                   Accumulator<A>& acc)
    {
      const I& ima = exact (input);
      A& accu = exact (acc);

      mln_pixter (const I) px (ima);
      for_all (px)
		accu.take (px.val ());
    }

    // generic implementation
    template < typename I, typename A>
    void
    leveling (const Image<I>& input,
              Accumulator<A>& acc)
    {
      const I& ima = exact (input);
      A& accu = exact (acc);

      mln_piter (I) p (ima.domain());
      for_all (p)
		accu.take (mln::make::pix (ima, p));
    }

  } // mln::impl

  namespace internal
  {
    template <typename I, typename A>
    void
    leveling_dispatch (metal::false_,
                       const Image<I>& input,
                       Accumulator<A>& acc)
    {
      impl::leveling (input, acc);
    }

    template <typename I, typename A>
    inline
    void
    leveling_dispatch (metal::true_,
                       const Image<I>& input,
                       Accumulator<A>& acc)
    {
      impl::leveling_fast (input, acc);
    }

    template <typename I, typename A>
    inline
    void
    leveling_dispatch (const Image<I>& input,
                       Accumulator<A>& acc)
    {
      enum
      {
        test = (mlc_equal (mln_trait_image_speed (I),
                           trait::image::speed::fastest) ::value &&
                (mlc_equal (mln_trait_accu_when_pix (A),
                            trait::accu::when_pix::use_v) ::value ||
                 mlc_equal (mln_trait_accu_when_pix (A),
                            trait::accu::when_pix::use_whatever) ::value))
      };
      leveling_dispatch (metal::bool_<test>(), input, acc);
    }

  } // mln::internal

} //mln


// Facade.
template <typename I, typename A>
void
leveling (const Image<I>& input,
          Accumulator<A>& acc)
{
  mln::internal::leveling_dispatch (input, acc);
}


#endif /* !LEVELING_HH_ */
