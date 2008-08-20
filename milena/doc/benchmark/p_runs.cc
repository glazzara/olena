# include <mln/io/pbm/load.hh>
# include <mln/core/image2d.hh>
# include <mln/convert/to_p_runs.hh>
# include <mln/util/timer.hh>

const unsigned n_times = 32;


template <typename I>
float browse_ima(const I& ima, unsigned& c)
{
  mln::util::timer t;
  t.start();
  for (unsigned i = 0; i < n_times; ++i)
    {
      c = 0;
      mln_piter(I) p(ima.domain());
      for_all(p)
	if (ima(p))
	  ++c;
    }
  return t.read();
}

template <typename R>
float browse_runs(const R& runs, unsigned& c)
{
  mln::util::timer t;
  t.start();
  for (unsigned i = 0; i < n_times; ++i)
    {
      c = 0;
      mln_fwd_piter(R) p(runs);
      for_all(p)
	++c;
    }
  return t.read();
}


int main()
{
  using namespace mln;
  image2d<bool> ima;
  io::pbm::load(ima, "../../img/lena.pbm");

  p_runs<point2d> rs = convert::to_p_runs(ima);
  mln_assertion(rs.zratio() < 1);

  unsigned c1, c2;
  std::cout << browse_ima(ima, c1) << std::endl;
  std::cout << browse_runs(rs, c2) << std::endl;
  mln_assertion(c2 == c1);
}
