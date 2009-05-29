#ifndef MAX_TREE_HH_
# define MAX_TREE_HH_

# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/data/fill.hh>
# include <mln/util/pix.hh>
# include <mln/morpho/includes.hh>
# include <mln/data/sort_psites.hh>

# include <mln/core/image/image2d.hh>
# include <mln/core/image/image3d.hh>
# include <mln/core/alias/neighb2d.hh>
# include <mln/value/int_u8.hh>
# include <mln/value/rgb8.hh>
# include <mln/io/pgm/load.hh>
# include <mln/core/site_set/p_array.hh>
# include <mln/debug/println.hh>
# include <mln/io/ppm/save.hh>

using namespace mln;

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
  mln_ch_value(I, point) new_parent;
  mln_ch_value(I, point) zpar;

  //tags
  image3d<bool> is_active;

  max_tree_(const I& f, const N& nbh)
    : f(f), nbh(nbh), is_active(f.domain()), new_parent(f.domain())
  {
    run();
    data::fill(is_active, true);

    new_parent = parent;
  }

  void run()
  {
    // init
    {
      initialize(deja_vu, f);
      mln::data::fill(deja_vu, false);
      initialize(parent, f);
      initialize(zpar, f);
      s = data::sort_psites_decreasing(f);
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

  point active_parent(const point& p)
  {
    point node = new_parent(p);

    while (not is_active(node) && not is_root(node))
      node = new_parent(node);

    return node;
  }

  void make_set(const point& p)
  {
    parent(p) = p;
    zpar(p) = p;
  }

  image3d<unsigned> compute_nb_represent()
  {
    image3d<unsigned> nb_represent(f.domain());

    data::fill(nb_represent, 0);
    mln_fwd_piter(S) p(s);

    for_all(p)
    {
      nb_represent(p) += f(p);
      if (not is_root(p))
        nb_represent(parent(p)) += nb_represent(p);
    }

    return nb_represent;
  }

  image3d<unsigned> compute_volume()
  {
    image3d<unsigned> volume(f.domain());

    data::fill(volume, 0);
    mln_fwd_piter(S) p(s);

    for_all(p)
    {
      volume(p) += 1;
      if (not is_root(p))
        volume(parent(p)) += volume(p);
    }

    return volume;
  }

  image3d< algebra::vec<3, double> > compute_mean_color()
  {
    image3d< algebra::vec<3, double> > mean_color(f.domain());

    update_parents();

    mln_fwd_piter(S) p(s);
    for_all(p)
      mean_color(p) = make::vec(p[0], p[1], p[2]);

    for_all(p)
      mean_color(new_parent(p)) = (mean_color(new_parent(p)) + mean_color(p)) / 2.;

    return mean_color;
  }

  // Filter method
  template<typename X>
  void lumberjack(const X& ima, const mln_value(X)& lambda)
  {
    unsigned progress = 0;
    unsigned step = s.nsites() / 100;

    mln_fwd_piter(S) p(s);

    for_all(p)
    {
      if (ima(p) < lambda)
	is_active(p) = false;

      ++progress;
      if (progress % step == 0)
      {
	std::cout << "." << std::flush;
	progress = 0;
      }
    }

    std::cout << std::endl;
  }

  // Fusion method
  template<typename X>
  void nuclear_fusion(const X& ima, double lambda)
  {
    unsigned progress = 0;
    unsigned step = s.nsites() / 100;

    update_parents();

    mln_fwd_piter(S) p(s);
    for_all(p)
    {
      if (ima(p) != 0)
        if (fabs(1. - ima(new_parent(p)) / (double) ima(p)) < lambda)
          is_active(p) = false;

      ++progress;
      if (progress % step == 0)
      {
	std::cout << "." << std::flush;
	progress = 0;
      }
    }

    std::cout << std::endl;
  }

  void color_fusion(int lambda)
  {
    unsigned progress = 0;
    unsigned step = s.nsites() / 100;

    update_parents();

    mln_fwd_piter(S) p(s);
    for_all(p)
    {
      if (new_parent(p)[0] - p[0] < lambda &&
	  new_parent(p)[1] - p[1] < lambda &&
	  new_parent(p)[2] - p[2] < lambda)
	is_active(p) = false;

      ++progress;
      if (progress % step == 0)
      {
	std::cout << "." << std::flush;
	progress = 0;
      }
    }
    std::cout << std::endl;
  }

  void update_parents()
  {
    unsigned progress = 0;
    unsigned step = s.nsites() / 100;

    std::cout << "Update parents";

    mln_fwd_piter(S) p(s);
    for_all(p)
    {
      new_parent(p) = active_parent(p);

      ++progress;
      if (progress % step == 0)
      {
	std::cout << "." << std::flush;
	progress = 0;
      }
    }
    std::cout << std::endl;
  }

  template < typename J >
  void to_ppm(const J& ima, const std::string& file, unsigned f,
              const image3d< algebra::vec<3, double> >& mean_color)
  {
    update_parents();

    J out(ima.domain());
    data::fill(out, value::rgb8(0, 0, 0));

    mln_piter(J) p(ima.domain());
    for_all(p)
    {
      point3d node = new_parent(point3d(ima(p).red() / f, ima(p).green() / f, ima(p).blue() / f));

      out(p) = value::rgb8(static_cast<unsigned char>(mean_color(node)[0] * f),
	                   static_cast<unsigned char>(mean_color(node)[1] * f),
			   static_cast<unsigned char>(mean_color(node)[2] * f));
    }

    io::ppm::save(out, file);
  }

  unsigned number_of_nodes()
  {
    return s.nsites();
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


#endif /* !MAX_TREE_HH_ */
