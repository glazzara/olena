#include <mln/core/var.hh>

#include <mln/core/image/image1d.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/image/unproject_image.hh>
#include <mln/fun/v2v/projection.hh>

#include <mln/core/image/image_if.hh>
#include <mln/pw/value.hh>

#include <mln/accu/count.hh>
#include <mln/accu/mean.hh>
#include <mln/accu/image/init.hh>
#include <mln/accu/image/take.hh>

#include <mln/io/pbm/load.hh>
#include <mln/io/pgm/load.hh>
#include <mln/debug/println.hh>



int main()
{
  using namespace mln;

  {
    using value::int_u8;
    
    image2d<int_u8> lena;
    io::pgm::load(lena, "../../../img/fly.pgm");
    debug::println(lena);
    
    typedef accu::mean<int_u8, unsigned, int_u8> A;
    
    image1d<A> vmean(lena.ncols());
    accu::image::init(vmean);
    {
      fun::v2v::projection<point2d, 0> vproj;
      mln_VAR( vmean_, unproject(vmean, lena.domain(), vproj) );
      accu::image::take(vmean_, lena);
    }
    debug::println(vmean);
  }

  std::cout << std::endl;

  {
    
    image2d<bool> pic;
    io::pbm::load(pic, "../../../img/fly.pbm");
    debug::println(pic);
    
    typedef accu::count<bool> A;
    
    image1d<A> h(pic.ncols());
    accu::image::init(h);
    {
      fun::v2v::projection<point2d, 0> vproj;
      mln_VAR( h_, unproject(h, pic.domain(), vproj) );
      accu::image::take(h_, (pic | pw::value(pic)));
    }
    debug::println(h);
  }


}
