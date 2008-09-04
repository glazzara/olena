# include <mln/core/image/image2d.hh>
# include <mln/core/image/image_if.hh>
# include <mln/core/alias/neighb2d.hh>
# include <mln/value/int_u8.hh>

# include <mln/pw/all.hh>
# include <mln/convert/to_fun.hh>
# include <mln/debug/println.hh>
# include <mln/labeling/blobs.hh>


namespace mln
{

  namespace trait
  {

//     template <template <class, class> class Op,
// 	      typename F, typename S>
//     struct set_binary_< Op,  mln::Function_v2v, F,  mln::value::Scalar, S >
//     {
//       typedef Op< F, pw::cst_<mln_value_equiv(S)> > Op_;
//       typedef typename Op_::ret ret;
//     };

    template <template <class, class> class Op,
	      typename F,
	      typename R, typename A>
    struct set_precise_binary_< Op,  F,  R (*)(A) >
    {
      typedef Op< F, fun::C<R (*)(A)> > Op_;
      typedef typename Op_::ret ret;
    };

    template <typename F, typename S>
    struct set_binary_< op::eq,  mln::Function_v2v, F,  mln::value::Scalar, S >
    {
      typedef mln_trait_op_eq(F, pw::cst_<mln_value_equiv(S)>) ret;
    };


    // const Image | Function_v2v

//     template <typename I, typename F>
//     struct set_binary_< op::lor,  mln::Image, /* FIXME: const */ I,  mln::Function_v2v, F >
//     {
//       typedef image_if<const I, F> ret;
//     };

  } // end of namespace mln::trait


  template <typename F, typename S>
  mln_trait_op_eq(F,S)
    operator == (const Function_v2v<F>& fun, const value::Scalar<S>& s)
  {
    return exact(fun) == pw::cst( value::equiv(s) );
  }


  template <typename I, typename R, typename A>
  image_if< const I, fun::C<R(*)(A)> >
  operator | (const Image<I>& ima, R (*f)(A) )
  {
    return exact(ima) | convert::to_fun(f);
  }


//   template <typename O, typename R, typename A>
//   mln_trait_op_lor(const O, fun::C<R(*)(A)>)
//   operator | (const Object<O>& lhs, R (*rhs)(A) )
//   {
//     return exact(lhs) | convert::to_fun(rhs);
//   }

} // end of namespace mln


bool row_oddity(mln::point2d p)
{
  return p.row() % 2;
}


int main()
{
  using namespace mln;
  using value::int_u8;


  bool vals[6][5] = {
    {0, 1, 1, 0, 0},
    {0, 1, 1, 0, 0},
    {0, 0, 0, 0, 0},
    {1, 1, 0, 1, 0},
    {1, 0, 1, 1, 1},
    {1, 0, 0, 0, 0}
  };
  image2d<bool> ima = make::image2d(vals);
  debug::println(ima);

  int_u8 nlabels;
  image2d<int_u8> lab = labeling::blobs(ima, c4(), nlabels);
  debug::println(lab);
  
  debug::println(lab | (pw::value(lab) != 0u));
  debug::println(lab | row_oddity);
}
