# include <mln/core/image2d.hh>
# include <mln/debug/println.hh>
# include <mln/convert/to_p_run_set.hh>

# include <mln/core/sparse_image.hh>


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

  typedef p_run_set<point2d> Runs;

  bool vals[6][5] = {
    {1, 1, 1, 1, 1},
    {0, 0, 0, 0, 0},
    {1, 0, 1, 0, 1},
    {0, 1, 0, 1, 0},
    {0, 0, 1, 1, 1},
    {1, 1, 1, 0, 0}
  };
  image2d<bool> ima = make::image2d(vals);
  debug::println(ima);

  Runs rs = convert::to_p_run_set(ima);
  std::cout << rs << std::endl;

  mln_assertion(rs.nruns() == 8);
  mln_assertion(rs.nsites() == 16);
  mln_assertion(rs.zratio() < 1);

  sparse_image<point2d,int> ima_(rs);
  display(ima_);
}
