#ifndef ALGEBRAIC_HH_
# define ALGEBRAIC_HH_

# include "accu_trait.hh"
# include <mln/metal/equal.hh>
# include <mln/accu/all.hh>
# include <mln/core/concept/image.hh>

namespace mln
{
  namespace impl
  {    

    // fast implementation
    template <typename I, typename A>
    void
    algebraic_fast (const Image<I>& input,
		   Accumulator<A>& acc)
    {
      const I& ima = exact(input);
      A& accu = exact (acc);

      mln_pixter(const I) px(ima);
      for_all(px)
		accu.take ();	
    }

    template <typename I, typename A>
    void
    algebraic(const Image<I>& input,
	      Accumulator<A>& acc)
    {
      const I& ima = exact(input);
      A& accu = exact (acc);

      mln_piter(I) p(ima.domain());
      for_all(p)
	  accu.take(p);
    }
  } // impl

  namespace internal
  {
    template <typename I, typename A>
    inline
    void
    algebraic_dispatch(metal::true_,
		       const Image<I>& input,
		       Accumulator<A>& acc)
    {
      impl::algebraic_fast(input, acc);
    }

    template <typename I, typename A>
    inline
    void
    algebraic_dispatch(metal::false_,
		       const Image<I>& input,
		       Accumulator<A>& acc)
    {
      impl::algebraic (input, acc);
    }


    template <typename I, typename A>
    inline
    void
    algebraic_dispatch(const Image<I>& input,
		       Accumulator<A>& acc)
    {
      enum {
	test = (mlc_equal(mln_trait_image_speed(I),
				  trait::image::speed::fastest)::value &&
			mlc_equal(mln_trait_accu_when_pix(A),
			  trait::accu::when_pix::use_whatever)::value)
      };
      algebraic_dispatch(metal::bool_<test>(), input, acc);
    }

  } // mln::internal

} //mln


template <typename I, typename A>
void
algebraic(const mln::Image<I>& input,
	  mln::Accumulator<A>& acc)
{
  mln::internal::algebraic_dispatch(input, acc);
}

#endif /* !ALGEBRAIC_HH_ */
