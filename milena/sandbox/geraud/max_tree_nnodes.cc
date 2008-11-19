
# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/level/fill.hh>
# include <mln/util/pix.hh>
# include <mln/morpho/includes.hh>
# include <mln/level/sort_psites.hh>

# include <mln/core/image/image2d.hh>
# include <mln/core/alias/neighb2d.hh>
# include <mln/value/int_u8.hh>
# include <mln/io/pgm/load.hh>



namespace mln
{

  template <typename I, typename N>
  struct max_tree_
  {
    typedef mln_site(I) point;
    typedef p_array<point> S;

    // in:
    const I& f;
    const N& nbh;

    // aux:
    S s;
    mln_ch_value(I, bool)  deja_vu;
    mln_ch_value(I, point) parent;
    mln_ch_value(I, point) zpar;

    max_tree_(const I& f, const N& nbh)
      : f(f),
	nbh(nbh)
    {
      run();
    }

    void run()
    {
      // init
      {
	initialize(deja_vu, f);
	mln::level::fill(deja_vu, false);
	initialize(parent, f);
	initialize(zpar, f);
	s = level::sort_psites_decreasing(f);
      }

      // first pass
      {
	mln_fwd_piter(S) p(s);
	mln_niter(N) n(nbh, p);
	for_all(p)
	{
	  make_set(p);
	  for_all(n)
	    if (f.has(n) && deja_vu(n))
	      do_union(n, p);
	  deja_vu(p) = true;
	}
      }

      // second pass: canonization
      {
	mln_bkd_piter(S) p(s);
	for_all(p)
	{
	  point q = parent(p);
	  if (f(parent(q)) == f(q))
	    parent(p) = parent(q);
	}
      }

    } // end of run()

    void make_set(const point& p)
    {
      parent(p) = p;
      zpar(p) = p;
    }

    bool is_root(const point& p) const
    {
      return parent(p) == p;
    }

    bool is_node(const point& p) const
    {
      return is_root(p) || f(parent(p)) != f(p);
    }

    point find_root(const point& x)
    {
      if (zpar(x) == x)
	return x;
      else
	return zpar(x) = find_root(zpar(x));
    }

    void do_union(const point& n, const point& p)
    {
      point r = find_root(n);
      if (r != p)
	{
	  parent(r) = p;
	  zpar(r) = p;
	}
    }

  };




  template <typename I, typename N>
  unsigned max_tree(const I& f, const N& nbh)
  {
    max_tree_<I,N> run(f, nbh);

    mln_piter(I) p(f.domain());
    unsigned nnodes = 0;
    for_all(p)
      if (run.is_node(p))
	++nnodes;
    return nnodes;
  }

}  // end of mln



int main(int argc, char* argv[])
{
  if (argc != 2)
    {
      std::cerr << "usage: " << argv[0] << " filename" << std::endl;
      return 1;
    }

  using namespace mln;
  using value::int_u8;

  image2d<int_u8> f;
  io::pgm::load(f, argv[1]);

  std::cout << "n nodes = " << mln::max_tree(f, c8()) << std::endl;
}
