# include <mln/core/image/image2d.hh>
# include <mln/core/alias/p_run2d.hh>
# include <mln/core/site_set/p_set_of.hh>

# include <mln/convert/from_to.hh>
# include <mln/debug/println.hh>
// # include <mln/core/image/sparse_image.hh>


template <typename I>
void display(I& ima)
{
  mln_fwd_piter(I) p(ima.domain());
  unsigned i = 0;
  for_all(p)
    ima(p) = i++;
  mln::debug::println(ima);
}


int main()
{
  using namespace mln;

  typedef p_set_of<p_run2d> Runs;

  bool vals[6][5] = {
    {1, 1, 1, 1, 1},
    {0, 0, 0, 0, 0},
    {1, 0, 1, 0, 1},
    {0, 1, 0, 1, 0},
    {0, 0, 1, 1, 1},
    {1, 1, 1, 0, 0}
  };
  image2d<bool> ima = make::image(vals);
  debug::println(ima);

  Runs rs;
  convert::from_to(ima, rs);
  std::cout << rs << std::endl;

  mln_assertion(rs.nelements() == 8);
  mln_assertion(rs.nsites() == 16);
//   mln_assertion(rs.zratio() < 1);

//   sparse_image<point2d,int> ima_(rs);
//   display(ima_);
}
