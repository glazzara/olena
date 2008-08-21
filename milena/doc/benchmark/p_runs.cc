# include <mln/io/pbm/load.hh>
# include <mln/core/image2d.hh>
# include <mln/core/alias/p_run2d.hh>
# include <mln/core/p_set_of.hh>
# include <mln/convert/from_to.hh>
# include <mln/util/timer.hh>

# include <sandbox/geraud/p_runs__with_dedicated_piter.hh>


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

  unsigned c;
  std::cout << "ref: " << browse_ima(ima, c) << std::endl;

  {
    util::timer t;
    t.start();

    // Conversion.
    p_set_of<p_run2d> rs;
    convert::from_to(ima, rs);
    std::cout << "enc: " << t.read() << std::endl;
    // FIXME: mln_assertion(rs.zratio() < 1);

    // Browsing.
    unsigned cr;
    std::cout << "brs: " << browse_runs(rs, cr) << std::endl;
    mln_assertion(cr == c);
  }

  {
    util::timer t;
    t.start();

    // Conversion.
    p_runs<point2d> rs;
    convert::from_to(ima, rs);
    std::cout << "enc: " << t.read() << std::endl;
    // FIXME: mln_assertion(rs.zratio() < 1);

    // Browsing.
    unsigned cr;
    std::cout << "brs: " << browse_runs(rs, cr) << std::endl;
    mln_assertion(cr == c);
  }

}
