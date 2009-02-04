# include <mln/metal/equal.hh>
# include <mln/metal/if.hh>
# include <mln/metal/is_const.hh>

# include <mln/core/concept/image.hh>
# include <mln/accu/all.hh>
# include <mln/util/pix.hh>
# include <mln/make/pix.hh>

# include "accu_trait.hh"

using namespace mln;

namespace mln
{

  namespace morpho
  {

    namespace accu
    {
      template <typename T>
      struct card : public mln::accu::internal::base< unsigned, card<T> >
      {
	typedef T argument;
	void init () { c_ = 0; };
	void take (const card<T>& elt) { ++c_; };
	void take (const T& elt) { ++c_; };
	void take (const mln_value(T)& v) { ++c_; };
	unsigned to_result() const { return c_; };
	operator unsigned () const { return c_; };
	bool is_valid () const { return true; };
	card () { init(); };

      private:
	unsigned c_;
      };
    } // accu
  } // morpho


  namespace impl
  {

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

    // fast implementation
    template <typename I, typename A>
    void
    leveling_fast (const Image<I>& input,
		   Accumulator<A>& acc)
    {
      const I& ima = exact(input);
      A& accu = exact (acc);

      mln_pixter(const I) px(ima);
      for_all(px)
	accu.take (px.val ());
    }

    // generic implementation
    template <typename I, typename A>
    void
    leveling (const Image<I>& input,
	      Accumulator<A>& acc)
    {
      const I& ima = exact(input);
      A& accu = exact (acc);

      mln_piter(I) p(ima.domain());
      for_all(p)
	accu.take (mln::make::pix(ima, p));
    }

  } // impl

  namespace internal
  {
    template <typename I, typename A>
    void
    leveling_dispatch(metal::false_,
		      const Image<I>& input,
		      Accumulator<A>& acc)
    {
      impl::leveling(input, acc);
    }

    template <typename I, typename A>
    inline
    void
    leveling_dispatch(metal::true_,
		      const Image<I>& input,
		      Accumulator<A>& acc)
    {
      impl::leveling_fast(input, acc);
    }

    template <typename I, typename A>
    inline
    void
    leveling_dispatch(const Image<I>& input,
		      Accumulator<A>& acc)
    {
      enum {
	test = (mlc_equal(mln_trait_image_speed(I),
			  trait::image::speed::fastest)::value &&
		mlc_equal(mln_trait_accu_when_pix(A),
			  trait::accu::when_pix::use_v)::value)
      };
      leveling_dispatch(metal::bool_<test>(), input, acc);
    }

  } // internal

} //mln

// Facade.

template <typename I, typename A>
void
leveling(const Image<I>& input,
	 Accumulator<A>& acc)
{
  mln::internal::leveling_dispatch(input, acc);
}


# include <mln/accu/all.hh>
# include <mln/core/image/image2d.hh>

# include <mln/debug/iota.hh>
# include <mln/debug/println.hh>
# include <mln/core/var.hh>
# include <mln/util/timer.hh>
int main()
{
  using namespace mln;
  typedef image2d<int> I;

  I ima(1000, 1000);
  mln::morpho::accu::card<util::pix<I> > acc;

  float elapsed;
  mln::util::timer chrono;

  debug::iota(ima);
  std::cout << "50 mean of a 1000x1000 image2d<int>" << std::endl;

  acc.init();
  chrono.start();
  for (int i = 0; i < 50; i++)
    leveling(ima, acc);
  elapsed = chrono.stop();

  std::cout << "(auto) " << elapsed << "s : " << acc.to_result() << std::endl;

  acc.init();
  chrono.start();
  for (int i = 0; i < 50; i++)
    mln::impl::leveling(ima, acc);
  elapsed = chrono.stop();

  std::cout << "(generic) " << elapsed << "s : " << acc.to_result() << std::endl;

  acc.init();
  chrono.start();
  for (int i = 0; i < 50; i++)
    mln::impl::leveling_fast(ima, acc);
  elapsed = chrono.stop();

  std::cout << "(fast) " << elapsed << "s : " << acc.to_result() << std::endl;
}

