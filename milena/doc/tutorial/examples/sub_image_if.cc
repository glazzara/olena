# include <mln/core/image/image2d.hh>
# include <mln/core/image/sub_image_if.hh>

# include <mln/core/image/sub_image.hh>
# include <mln/core/image/image_if.hh>

# include <mln/pw/all.hh>
# include <mln/core/var.hh>

# include <mln/core/alias/neighb2d.hh>
# include <mln/labeling/blobs.hh>
# include <mln/geom/bbox.hh>

# include <mln/data/fill.hh>
# include <mln/debug/println.hh>


template <typename I>
void foo(const I& input)
{
  using namespace mln;

  std::cout << std::endl << "foo!" << std::endl << std::endl;

  typedef mln_ch_value(I, float) O;

// p_if< box2d,
//       fun::p2b::has< image_if< image2d<float>, F> >
//      > 

// p_if< box2d,
//       fun::p2b::has< image_if< image2d_int_u8, F > >
//     >



  // type of   " mln_ch_value(I, float) "

// sub_image_if< I = image_if< image2d<float>, F >
//               S = box2d >
//
//  where:
//
//  F = fun::neq_p2b_expr_< pw::value_< image2d<value::int_u<8u> > >,
//                          pw::cst_<unsigned int> >
//                        >,

  O output;
  initialize(output, input);
  data::fill(output, 9);

  std::cout << geom::bbox(output) << std::endl;
  debug::println(output);
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
  image2d<bool> ima = make::image(vals);
  debug::println(ima);

  int_u8 nlabels;
  image2d<int_u8> lab = labeling::blobs(ima, c4(), nlabels);
  debug::println(lab);
  
  mln_VAR(lab_0,  lab | (pw::value(lab) != pw::cst(0u)));
  debug::println(lab_0);

//   box2d b = geom::bbox(lab | (pw::value(lab) == pw::cst(3u)));
//   std::cout << b << std::endl;


//   {
//     mln_VAR(temp, lab | (pw::value(lab) == pw::cst(3u)));
//     std::cout << temp.un_if().bbox() << std::endl;
//     debug::println(temp);
//     std::cout << geom::bbox(lab | (pw::value(lab) == pw::cst(3u))) << std::endl;

//     foo(temp);
//   }

  box2d bb = make::box2d(2,1, 5,4);
  debug::println(lab / bb);

  mln_VAR(lab_3, lab_0 / bb);
  debug::println(lab_3);
  
  foo(lab_3);
}
