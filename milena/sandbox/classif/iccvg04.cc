
#include <iostream>

#include <mln/core/image/image2d.hh>
#include <mln/core/image/image3d.hh>
#include <mln/histo/data.hh>
#include <mln/value/all.hh>

#include <mln/level/fill.hh>

#include <mln/io/ppm/load.hh>

#include <mln/io/pgm/save.hh>

using namespace mln;

template <typename I>
mln::image3d<unsigned>
fill_histo(const I& ima)
{
  image3d<unsigned> histo(256,256,256);
  level::fill(histo, 0);

  mln_piter(I) p(ima.domain());
  for_all(p)
  {
    point3d p3(ima(p).red(),ima(p).green(), ima(p).blue());
    histo(p3)++;
  }
  return histo;
}

int main(int argc, char **argv)
{
  image2d<value::rgb8> ima;
  ima = io::ppm::load<value::rgb8>(argv[1]);

  image3d<unsigned> histo = fill_histo(ima);

  image2d< value::int_u16 > out(256,256);
  level::fill(out, 0);
  mln_piter_(image2d< value::int_u16 >) p(out.domain());
  for_all(p)
  {
    for (unsigned i = 0; i < 256; i++)
      if ((out(p) + histo(point3d(p[0],p[1],i))) < 65536)
        out(p) += histo(point3d(p[0],p[1],i));
  }
  io::pgm::save(out,"./chiche.pgm");
}
