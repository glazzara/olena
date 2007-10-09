#include <mln/morpho/includes.hh>

#include <mln/win/dyn_line.hh>
#include <mln/win/hline2d.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>

#include <mln/value/int_u8.hh>
#include <mln/core/image2d_b.hh>

#include <mln/level/paste.hh>

#include <iostream>

using namespace mln;


template <typename W>
void erosion_on_function(const image2d_b<value::int_u8>& input, const Window<W>& win_, image2d_b<value::int_u8>& output)
{
  const W& win   = exact(win_);
  
  accu::min_<value::int_u8> min;
  
  mln_piter(image2d_b<value::int_u8>) p(input.domain());
  mln_qiter(W) q(win, p);
  for_all(p)
    {
      min.init();
      for_all(q) if (input.has(q))
	min.take(input(q));
      output(p) = min.to_result();
    }
}


int main()
{
//  win::dyn_line<grid::square, int> h(17, 1);
  win::hline2d h(17);

  image2d_b<value::int_u8> lena = io::pgm::load("../../../img/lena.pgm");
  image2d_b<value::int_u8> out(lena.domain());

  erosion_on_function(lena, h, out);
  //level::paste(lena, out);
  io::pgm::save(out, "out.pgm");
}
