#include <mln/core/image/image2d.hh>

#include <mln/core/image/sub_image.hh>

#include <mln/core/image_if_value.hh>

#include <mln/accu/bbox.hh>
#include <mln/data/fill.hh>
#include <mln/debug/println.hh>

#include "types.hh"

using namespace mln;
using value::int_u8;

// class ran_domains
// {
//  public:

//   ran_domains(const box2d& b)
//     : ima_(b)
//   {
//     bb_.init();
//     data::fill(ima_, false);
//     bb_.take(point2d(2,2));
//     bb_.take(point2d(2,3));
//   }

//   sub_image<image2d<value::int_u8>, box2d> image()
//   { return ima_ | bb_.to_result(); }

//   image2d<value::int_u8>& full_image() { return ima_; }

//   const box2d& bbox() const { return bb_.to_result(); }

//  private:

//   image2d<int_u8> ima_;
//   accu::bbox<point2d> bb_;
// };

# define image_if_value_sub image_if_value<const sub_image<image2d<value::int_u8>, box2d> >
# define sub_if_value_image sub_image<const image_if_value<image2d<int_u8 > >, box2d>
# define SET_R 3
# define SET_A 2
# define SET_N 1

int main()
{

  {
    int_u8 vs[5][5] = { {1,  2, 3, 4,  9},
			{5,  6, 7, 8,  9},
			{9, 10, 6,12,  9},
			{13,14,15, 6,  9},
			{13,14,15,16,  9} };

    image2d<int_u8> ima = make::image2d(vs);

    box2d b(make::box2d(1,1,3,3));

    // Browse a image_if_value.
    {
      image_if_value<image2d<value::int_u8> > ima2 = ima | 6;
      image_if_value<image2d<value::int_u8> >::fwd_piter qa(ima2.domain());
      for_all(qa)
      {}
    }

    // Browse a sub_image.
    {
      sub_image<image2d<value::int_u8>, box2d> ima2 = ima | b;
      sub_image<image2d<value::int_u8>, box2d>::fwd_piter qa(ima2.domain());
      for_all(qa)
      {}
    }

    // Sub_image, image_if_value conversion.
    {
      image_if_value<const sub_image<image2d<value::int_u8>, box2d> > s = ima | b | 6;
      sub_image<image2d<value::int_u8>, box2d> ima2 = s;
    }

    // Problem 1 : Browse a sub_image of a image_if_value.
    {
      sub_if_value_image ima2 = ima | 6 | b;
      debug::println(ima2);
    }

    // Problem 2 : Browse a image_if_value of a sub_image.
    {
      image_if_value_sub s = ima | b | 6;

      image_if_value_sub::fwd_piter qa(s.domain());
      for_all(qa)
      {}
    }

  }
}


// mln::image_if_value<const mln::sub_image<mln::image2d<mln::value::int_u<8u> >, mln::box_<mln::point_<mln::grid::square, int> > > >
// mln::image_if_value<mln::sub_image<mln::image2d<mln::value::int_u<8u> >, mln::box_<mln::point_<mln::grid::square, int> > > >
