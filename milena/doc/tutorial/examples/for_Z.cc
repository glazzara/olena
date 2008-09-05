# include <mln/core/image/image2d.hh>

# include <mln/core/image/image_if.hh>
# include <mln/core/image/sub_image.hh>

# include <mln/core/site_set/p_vaccess.hh>
# include <mln/convert/from_to.hh>
# include <mln/core/alias/p_runs2d.hh>

# include <mln/core/alias/neighb2d.hh>
# include <mln/core/var.hh>
# include <mln/value/int_u8.hh>
# include <mln/value/rgb8.hh>
# include <mln/literal/colors.hh>

# include <mln/pw/all.hh>
# include <mln/convert/to_fun.hh>
# include <mln/debug/println.hh>
# include <mln/labeling/blobs.hh>
# include <mln/level/fill.hh>
# include <mln/geom/bbox.hh>


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



namespace my
{

  template <typename I>
  void fill(I& ima, mln_value(I) v)
  {
    mln_piter(I) p(ima.domain());
    for_all(p)
      ima(p) = v;
  }

  template <typename I, typename J>
  void paste(const I& data, J& dest)
  {
    mln_piter(I) p(data.domain());
    for_all(p)
      dest(p) = data(p);
  }

} // end of namespace my





int main()
{
  using namespace mln;
  using value::int_u8;
  using value::rgb8;


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
  

  mln_VAR(lab_0,  lab | (pw::value(lab) != 0u));
  debug::println(lab_0);


//   box2d b3 = geom::bbox(lab | (pw::value(lab) == 3u));
//   std::cout << b3 << std::endl
// 	    << std::endl;

// //   mln_VAR(lab3, lab | b3);
// //   debug::println(lab3);
// //   std::cout << lab3.domain() << std::endl
// // 	    << std::endl;


//   {
//     std::cout << "(ima | sub_D) | pred" << std::endl << std::endl;

//     mln_VAR(pred,  pw::value(lab) == 3u);

//     std::cout << (lab | b3).domain() << std::endl;
//     debug::println(lab | b3);

//     std::cout << ((lab | b3) | pred).domain() << std::endl;
//     debug::println((lab | b3) | pred);
//   }


//   // il existe une difference entre:
//   //
//   // -  ima | sub_D   ou le sub_D DOIT etre inclus dans ima.domain
//   // et
//   // -  ima / sub_D  qui reste a ecrire...
//   //    ou dans ce cas, on aurait  (ima / sub_D).domain() == sub_D | f:p->b = ima.domain().has(p)


//   {
//     std::cout << "(ima | pred) | sub_D" << std::endl << std::endl;

//     mln_VAR(pred,  pw::value(lab) == 3u);

//     // OK  :-)
//     std::cout << (lab | pred).domain() << std::endl;
//     debug::println(lab | pred);

//     // KO  :-)
//     // Cf. commentaire plus haut
//     // ici l'erreur est que b3 n'est pas un sous-domaine de celui de "lab | pred"...
//     /*
//       std::cout << ((lab | pred) | b3).domain() << std::endl;
//       debug::println((lab | pred) | b3);
//     */
//   }


//   debug::println(lab | row_oddity);

//   my::fill(lab_0, 9);
//   debug::println(lab_0);
//   debug::println(lab);


//   image2d<rgb8> cool(ima.domain());
//   level::fill(cool, literal::black);

//   level::fill( inplace(cool | (pw::value(lab) == 1u)),
// 	       literal::red );

//   debug::println(cool);


  {
    p_vaccess< int_u8, p_runs2d > s;
    convert::from_to(lab_0, s);
    std::cout << s << std::endl;
    std::cout << s(3) << std::endl;
    std::cout << s(3).bbox() << std::endl;
  }

}
