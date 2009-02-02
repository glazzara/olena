

template <typename E>
struct Accumulator
{
};


template <typename E>
struct Meta_Accumulator
{
};

// Cf. mln/core/concept/*


namespace accu
{

  template <typename T>
  struct card : Accumulator<card>
  {
    void take(const T& elt) { ++c_; }
    unsigned c_;
  };

  namespace meta
  {
    struct card : Meta_Accumulator<card>
    {
      template <typename T>
      struct with
      {
	typedef accu::card<T> ret;
      };
    };

    // FIXME: passez de accu::meta::card a accu::card<T> ?

    // Cf. mln/accu/count.hh
    //  et mln/accu/compute.hh

  } // meta

} // accu



namespace impl
{

  // Leveling
  // --------

  template <typename I, typename A>
  void
  leveling(const Image<I>& input, const Accumulator<A>& acc)
  {
    const I& ima = exact(input);
    // algo en resume :
    mln_piter(I) p(ima.domain());
    for_all(p)
      acc.take(make::pix(ima, p)); // util::pix<I>
  }

  template <typename I, typename A>
  void
  leveling_fastest(const Image<I>& input_, const Accumulator<A>& acc)
  {
    const I& input = exact(input_);

    // algo en resume :
    mln_pixter(const I) pxl(input);
    for_all(pxl)
      acc.take(pxl);
  }


  // FIXME: pb: on a 2 fois le même code, plusieurs fois car 1
  // spécialisation pour les images fastest


  // Algebraic
  // ---------


  template <typename I, typename A>
  void
  generic_algebraic(const Image<I>& input, const Accumulator<A>& acc)
  {
    // algo en resume :
    mln_piter(I) p(ima.domain());
    for_all(p)
      acc.take(p); // psite
  }

  template <typename I, typename A>
  void
  algebraic_fastest(const Image<I>& input, const Accumulator<A>& acc)
  { 
    // algo en resume :
    mln_pixter(const I) pxl(input);
    for_all(pxl)
      acc.take(pxl);
  }


  // FIXME: pb: soit on passe du "site", soit du "pixel"...


  // pour mémoire mais on s'en fout (!!!) :
  /*
  template <typename I, typename A>
  void
  algebraic_fastest_alt(const Image<I>& input, const Accumulator<A>& acc)
  { 
    for (unsigned i = 0; i < n; ++i)
      acc.take(input, i); // (image, offset) -> site
    // Cf. methode image2d<T>::point_at_index(i)
  }
  */


  // FIXME: On veut qqch qui ressemble à :

  template <typename I, typename A>
  void
  generic_algebraic_or_leveling(const Image<I>& input, const Accumulator<A>& acc)
  {
    // algo en resume :
    mln_piter(I) p(ima.domain());
    for_all(p)
      acc.take( FIXME );
  }

  template <typename I, typename A>
  void
  algebraic_or_leveling_fastest(const Image<I>& input, const Accumulator<A>& acc)
  {
    // algo en resume :
    mln_pixter(const I) pxl(input);
    for_all(pxl)
      acc.take( FIXME );
  }


  // FIXME: Mais ce n'est peut-être pas possible avec des
  // Accumulator...


} // impl



namespace internal
{

  // dispatch for geneneric implementation.
  template <typename M, typename I>
  void
  algebraic_dispatch(metal::false_,
		     const Image<I>& input,
		     const Meta_Accumulator<M>& acc)
  {
    typedef mln_accu_with(M, mln_piter(I)) A;

    mln_piter(I)* piter;
    A accu = accu::unmeta(exact(acc), piter);

    generic_algebraic(input, acc);
  }


  // dispatch for fastest implementation.
  template <typename M, typename I>
  void
  algebraic_dispatch(metal::true_,
		     const Image<I>& input,
		     const Meta_Accumulator<M>& acc)
  {
    typedef mln_accu_with(M, util::pix<I>) A;

    util::pix<I>* pix_;
    A accu = accu::unmeta(exact(acc), pix_);

    algebraic_fastest(input, acc);
  }

  template <typename M, typename I>
  void
  algebraic_dispatch(const Image<I>& input,
		     const Meta_Accumulator<M>& acc)
  {


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



int main()
{
  algebraic(ima, accu::meta::card());
}
