#ifndef MAX_TREE_HH_
# define MAX_TREE_HH_

# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/level/fill.hh>
# include <mln/util/pix.hh>
# include <mln/morpho/includes.hh>
# include <mln/level/sort_psites.hh>

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
  mln_ch_value(I, point) zpar;

  // image of volumes
  image3d<unsigned> vol;
  image3d<unsigned> nb_represent;
  image3d<double> density;

  //tags
  image3d<bool> is_active;
  image3d< algebra::vec<3, double> > mean_color;

  max_tree_(const I& f, const N& nbh)
    : f(f), nbh(nbh), vol(f.domain()), nb_represent(f.domain()), density(f.domain()),
      is_active(f.domain()), mean_color(f.domain())
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

  void area()
  {
    image2d<value::int_u16> area(f.domain());
    level::fill(area, 1);
    mln_fwd_piter(S) p(s);
    for_all(p)
    {
      if (parent(p) == p)
        continue;
      area(parent(p)) += area(p);
    }
    debug::println(area);
  }

  void volume()
  {
    level::fill(vol, 0);
    level::fill(nb_represent, 0);

    {
      mln_fwd_piter(S) p(s);

      for_all(p)
      {
	vol(p) += 1;
	nb_represent(p) += f(p);

	if (parent(p) != p)
	{
	  nb_represent(parent(p)) += nb_represent(p);
	  vol(parent(p)) += vol(p);
	}

	density(p) = nb_represent(p) / (double) vol(p);
      }
    }

    {
      mln_fwd_piter(S) p(s);
      for_all(p)
      {
	std::cerr << " Color " << p << std::endl
	          << "   vol          = " << vol(p) << " vertices" << std::endl
	          << "   nb_represent = " << nb_represent(p) << std::endl
	          << "   f            = " << f(p) << std::endl
		  << "   density      = " << density(p) << " representant / vertices " << std::endl << std::endl;
      }
    }
  }

  void compute_mean_color()
  {
    level::fill(mean_color, make::vec(0, 0, 0));

    mln_fwd_piter(S) p(s);
    for_all(p)
      mean_color(p) = make::vec(p[0], p[1], p[2]);

    for_all(p)
      mean_color(parent(p)) = (mean_color(parent(p)) + mean_color(p)) / 2.;

    for_all(p)
      if (mean_color(p)[0] < 10 && mean_color(p)[1] < 10 && mean_color(p)[2] < 10)
	std::cerr << "Warning " << p << " == " << mean_color(p) << std::endl;
  }

  // Cut components with less represents than lambda
  void volume_fusion(int lambda)
  {
    level::fill(is_active, true);

    mln_fwd_piter(S) p(s);
    for_all(p)
    {
      if (vol(p) < lambda)
	is_active(p) = false;
    }
  }

  void nb_represent_fusion(int lambda)
  {
    level::fill(is_active, true);

    mln_fwd_piter(S) p(s);
    for_all(p)
    {
      if (nb_represent(p) < lambda)
	is_active(p) = false;
    }
  }

  template < typename J >
  void to_ppm(const J& ima, const std::string& file, unsigned f)
  {
    J out(ima.domain());
    level::fill(out, value::rgb8(0, 0, 0));

    mln_piter(J) p(ima.domain());
    for_all(p)
    {
      point3d p3 = point3d(ima(p).red() / f, ima(p).green() / f, ima(p).blue() / f);

      point3d node = p3;
      if (not is_node(p3))
	node = parent(p3);

      while (not is_active(node))
	node = parent(node);

write:
      out(p) = value::rgb8(static_cast<unsigned char>(mean_color(node)[0] * f),
	                   static_cast<unsigned char>(mean_color(node)[1] * f),
			   static_cast<unsigned char>(mean_color(node)[2] * f));
      //out(p) = value::rgb8(mean_color(node)[0] * f, mean_color(node)[1] * f, mean_color(node)[2] * f);
    }

    io::ppm::save(out, file);
  }

  unsigned number_of_nodes()
  {
    p_array<point> node;

    mln_fwd_piter(S) p(s);
    for_all(p)
      if (is_node(p))
	node.insert(p);

    std::cout << s.nsites() << std::endl;
    std::cout << parent.domain().nsites() << std::endl;
    std::cout << node.nsites() << std::endl;

    return s.nsites();
  }

  void print_class_info()
  {
    int nb_class = 0;

    mln_fwd_piter(S) p(s);

    for_all(p)
    {
      if (is_active(p))
      {
	std::cout << "Class " << nb_class << " : " << p << std::endl;
	++nb_class;
      }
    }
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
