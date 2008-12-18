# include <mln/core/image/image2d.hh>
# include <mln/core/image/plain.hh>
# include <mln/debug/println.hh>
# include <mln/debug/iota.hh>
# include <mln/level/fill_with_value.hh>
# include <mln/opt/at.hh>


template <typename I>
void picture(const I& ima)
{
  using namespace mln;
  const unsigned
    nr = opt::at(ima, 0,0).domain().nrows(),
    nc = opt::at(ima, 0,0).domain().ncols();
  for (unsigned row = 0; row < ima.nrows(); ++row)
    for (unsigned r = 0; r < nr; ++r)
      {
	for (unsigned col = 0; col < ima.ncols(); ++col)
	  for (unsigned c = 0; c < nc; ++c)
	    std::cout << opt::at(ima, row, col)(point2d(r,c)) << ' ';
	std::cout << std::endl;
      }
}


int main()
{
  using namespace mln;

  typedef image2d<int> I;
  typedef plain<I> I_;

  I ima(3, 3);
  debug::iota(ima);
  debug::println(ima);

  image2d<I_> mos(2, 2);
  level::fill_with_value(mos, ima);
  level::fill_with_value(opt::at(mos, 1,1), 0);
  picture(mos);
}
