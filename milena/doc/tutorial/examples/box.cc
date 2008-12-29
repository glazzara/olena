# include <mln/core/image/image2d.hh>
# include <mln/debug/println.hh>
# include <mln/data/fill.hh>


template <typename B>
void picture(const B& b)
{
  using namespace mln;

  image2d<char> ima(5, 5);
  data::fill(ima, '-');
  
  unsigned i = 0;
  mln_piter(B) p(b);
  for_all(p)
    ima(p) = '0' + i++;

  debug::println(ima);
}



int main()
{
  using namespace mln;

  {
    box2d b = make::box2d(2, 3);
    picture(b);
  }
  {
    box2d b = make::box2d(1,1, 3,2);
    picture(b);
  }
}
