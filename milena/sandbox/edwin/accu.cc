template <typename E>
struct Accumulator : public object<E>
{
  typedef mln_vtypes(E, take_with) take_with;

  

};

namespace accu
{
  template <typename T>
  struct card : public Accumulator< card<T> >
  {
    void take(const T& elt) { ++c_; }
    unsigned c_;
  };
}


namespace impl
{

  template <typename I, template<typename E> class A>
  void
  algebraic(const Image<I>& input,
	    const Accumulator< A<point2d> >& acc)
  {
    const I& ima = exact(input);

    mln_piter(I) p(ima.domain());
    for_all(p)
      acc.take(p);
  }

  // fast implementation
  template <typename I, template<typename E> class A>
  void
  leveling (const Image<I>& input,
	    const Accumulator< A< pix<I> > >& acc)
  {
    const I& ima = exact(input);

    mln_pixter(const I) px(ima);
    for_all(px)
      acc.take(px);
  }

  // generic implementation
  template <typename I, template<typename E> class A>
  void
  leveling (const Image<I>& input,
	    const Accumulator< A<point2d> >& acc)
  {
    const I& ima = exact(input);

    mln_piter(I) p(ima.domain());
    for_all(p)
      acc.take(p);
  }


} // impl

namespace internal
{
  template <typename I, typename A>
  void
  leveling_dispatch(const Image<I>& input,
		    const Accumulator<A>& acc)
  {

    //Si when_pix = use_only
    //  

    algebraic_dispatch(mlc_equal(mln_trait_image_speed(I),
				 trait::image::speed::fastest)::value,
		       input,
		       acc);
  }

}

// Facade.

template <typename M, typename I>
void
algebraic(const Image<I>& input, const Meta_Accumulator<M>& acc) // FIXME: on préfère Meta_Accumulator !
{
  internal::algebraic_dispatch(input, acc);
}
