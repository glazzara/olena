#include <mln/core/image/image2d.hh>
#include <mln/value/int_u8.hh>
#include <mln/pw/value.hh>
#include <mln/pw/cst.hh>
#include <mln/fun/ops.hh>
#include <mln/value/scalar.hh>


namespace mln
{


  namespace trait
  {

//     template <typename L, typename R>
//     struct set_binary_< op::less,
// 			mln::Function_p2v,  L,
// 			mln::value::Scalar, R >
//     {
//       typedef fun::less_p2b_expr_< L, pw::cst_<mln_result(L)> > ret;
//     };


    template <typename L, typename R>
    struct set_binary_< op::less,
			mln::Function_p2v,  L,
			mln::Object,        R >
    {
      typedef fun::less_p2b_expr_< L, pw::cst_<mln_result(L)> > ret;
    };
    
  }
  

//   template <typename L, typename R>
//   fun::less_p2b_expr_< L, pw::cst_<mln_result(L)> >
//   operator < (const Function_p2v<L>& lhs, const value::Scalar<R>& rhs)
//   {
//     mlc_converts_to(mln_equiv(R), mln_result(L))::check();
//     mln_equiv(R) rhs_ = exact(rhs).to_equiv();
//     typedef pw::cst_<mln_result(L)> C;
//     fun::less_p2b_expr_<L, C> tmp(exact(lhs), C(rhs_));
//     return tmp;
//   }


  template <typename L, typename R>
  fun::less_p2b_expr_< L, pw::cst_<mln_result(L)> >
  operator < (const Function_p2v<L>& lhs, const Object<R>& rhs)
  {
    // mln_trait_op_less(L, mln_result(L)) b = lhs;

    typedef pw::cst_<mln_result(L)> C;
    fun::less_p2b_expr_<L, C> tmp(exact(lhs), C(exact(rhs)));
    return tmp;
  }


//   template <typename L>
//   fun::less_p2b_expr_< L, pw::cst_<mln_result(L)> >
//   operator < (const Function_p2v<L>& lhs, const mln_result(L)& rhs)
//   {
//     mln_trait_op_less(L, mln_result(L)) b = lhs;

//     typedef pw::cst_<mln_result(L)> C;
//     fun::less_p2b_expr_<L, C> tmp(exact(lhs), C(rhs));
//     return tmp;
//   }
  

}




int main()
{
  using namespace mln;

  image2d<value::int_u8> ima(1,1);
  point2d p(0,0);
  ima(p) = 2;

//   void* v =
  (pw::value(ima) < 3);

//   std::cout << (pw::value(ima) < 3u)(p) << std::endl;

  // pw::value(ima) < 3;
}
