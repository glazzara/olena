#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/debug/println.hh>
#include <mln/io/pbm/load.hh>
#include <mln/io/pbm/save.hh>
#include <mln/data/compare.hh>
#include <mln/util/timer.hh>

#include "reconstruction_on_set.hh"


// by_dilation

template <typename F, typename G>
struct rd_functor
{
  const F& f;
  const G& g;

  rd_functor(const F& f, const G& g)
    : f(f), g(g)
  {
  }

  mln_concrete(F) output;
  typedef mln_psite(F) P;

  void set_default()
  {
    mln::data::fill(output, false);
  }

  bool domain(const P& p) const
  {
    return g(p) == true;
  }

  void init(const P& p)
  {
    output(p) = f(p);
  }

  void merge(const P& p, const P& r)
  {
    output(p) = output(p) || output(r);
  }

  void border(const P&, const P&)
  {
  }

  // Fastest versions.
  void set_extension_()
  {
    mln::border::fill(g, false);
  }
  bool domain_(unsigned p) const
  {
    return g.element(p) == true;
  }
  void init_(unsigned p)
  {
    output.element(p) = f.element(p);
  }
  void merge_(unsigned p, unsigned r)
  {
    output.element(p) = output.element(p) || output.element(r);
  }
  bool inspect_border_() const
  {
    return false;
  }
  void border_(unsigned, unsigned)
  {
  }
};





template <typename F, typename G>
struct rd_alt_functor    //  /!\  alternative version
{
  const F& f;
  const G& g;

  rd_alt_functor(const F& f, const G& g)
    : f(f), g(g)
  {
  }

  mln_concrete(F) output;
  typedef mln_psite(F) P;

  void set_default()
  {
    mln::data::fill(output, f);
  }

  bool domain(const P& p) const
  {
    return g(p) && ! f(p);
  }

  void init(const P& p)
  {
    output(p) = false;
  }

  void merge(const P& p, const P& r)
  {
    output(p) = output(p) || output(r);
  }

  void border(const P& p, const P& n)
  {
    if (f(n))
      output(p) = true;
  }

  // Fastest versions.
  void set_extension_()
  {
    mln::border::fill(f, false);
    mln::border::fill(g, false);
  }
  bool domain_(unsigned p) const
  {
    return g.element(p) && ! f.element(p);
  }
  void init_(unsigned p)
  {
    output.element(p) = false;
  }
  void merge_(unsigned p, unsigned r)
  {
    output.element(p) = output.element(p) || output.element(r);
  }
  bool inspect_border_() const
  {
    return true;
  }
  void border_(unsigned p, unsigned n)
  {
    if (f.element(n))
      {
	mln_invariant(f.domain().has(f.point_at_index(n)));
	output.element(p) = true;
      }
  }
};



// by_erosion

template <typename F, typename G>
struct re_functor
{
  const F& f;
  const G& g;

  re_functor(const F& f, const G& g)
    : f(f), g(g)
  {
  }

  mln_concrete(F) output;
  typedef mln_psite(F) P;

  void set_default()
  {
    mln::data::fill(output, true);
  }

  bool domain(const P& p) const
  {
    return g(p) == false;
  }

  void init(const P& p)
  {
    output(p) = f(p);
  }

  void merge(const P& p, const P& r)
  {
    output(p) = output(p) && output(r);
  }

  void border(const P&, const P&)
  {
  }

  // Fastest versions.
  void set_extension_()
  {
    mln::border::fill(g, true);
  }
  bool domain_(unsigned p) const
  {
    return g.element(p) == false;
  }
  void init_(unsigned p)
  {
    output.element(p) = f.element(p);
  }
  void merge_(unsigned p, unsigned r)
  {
    output.element(p) = output.element(p) && output.element(r);
  }
  bool inspect_border_() const
  {
    return false;
  }
  void border_(unsigned, unsigned)
  {
  }
};




template <typename F, typename G>
struct re_alt_functor
{
  const F& f;
  const G& g;

  re_alt_functor(const F& f, const G& g)
    : f(f), g(g)
  {
  }

  mln_concrete(F) output;
  typedef mln_psite(F) P;

  void set_default()
  {
    mln::data::fill(output, g);
  }

  bool domain(const P& p) const
  {
    return f(p) && ! g(p);
  }

  void init(const P& p)
  {
    output(p) = true;
  }

  void merge(const P& p, const P& r)
  {
    output(p) = output(p) && output(r);
  }

  void border(const P& p, const P& n)
  {
    if (! f(n))
      output(p) = false;
  }

  // Fastest versions.
  void set_extension_()
  {
    mln::border::fill(f, true);
    mln::border::fill(g, true);
  }
  bool domain_(unsigned p) const
  {
    return f.element(p) && ! g.element(p);
  }
  void init_(unsigned p)
  {
    output.element(p) = true;
  }
  void merge_(unsigned p, unsigned r)
  {
    output.element(p) = output.element(p) && output.element(r);
  }
  bool inspect_border_() const
  {
    return true;
  }
  void border_(unsigned p, unsigned n)
  {
    if (! f.element(n))
      output.element(p) = false;
  }
};




int main()
{
  using namespace mln;

  typedef image2d<bool> I;

  I f, g, ref, o;
  neighb2d nbh = c4();

  io::pbm::load(f, "f_and_g.pbm");
  io::pbm::load(g, "g.pbm");

  util::timer t;


  // by dilation
  {
    rd_functor<I,I> fun(f, g);
    {
      t.start();
      ref = morpho::canvas::impl::generic::reconstruction_on_set_union_find(f, g, nbh, fun);
      std::cout << t.stop() << std::endl;
    }
    {
      t.start();
      o = morpho::canvas::impl::reconstruction_on_set_union_find_fastest(f, g, nbh, fun);
      std::cout << t.stop() << std::endl;
      std::cout << (o == ref ? "OK" : "KO") << std::endl;
    }

    rd_alt_functor<I,I> fun_alt(f, g);
    {
      t.start();
      o = morpho::canvas::impl::generic::reconstruction_on_set_union_find(f, g, nbh, fun_alt);
      std::cout << t.stop() << std::endl;
      std::cout << (o == ref ? "OK" : "KO") << std::endl;
    }
    {
      t.start();
      o = morpho::canvas::impl::reconstruction_on_set_union_find_fastest(f, g, nbh, fun_alt);
      std::cout << t.stop() << std::endl;
      std::cout << (o == ref ? "OK" : "KO") << std::endl;
    }

    io::pbm::save(ref, "rd.pbm");
  }

  // by erosion
  {
    re_functor<I,I> fun(g, f);
    {
      t.start();
      ref = morpho::canvas::impl::generic::reconstruction_on_set_union_find(g, f, nbh, fun);
      std::cout << t.stop() << std::endl;
    }
    {
      t.start();
      o = morpho::canvas::impl::reconstruction_on_set_union_find_fastest(g, f, nbh, fun);
      std::cout << t.stop() << std::endl;
      std::cout << (o == ref ? "OK" : "KO") << std::endl;
    }

    re_alt_functor<I,I> fun_alt(g, f);
    {
      t.start();
      o = morpho::canvas::impl::generic::reconstruction_on_set_union_find(g, f, nbh, fun_alt);
      std::cout << t.stop() << std::endl;
      std::cout << (o == ref ? "OK" : "KO") << std::endl;
    }
    {
      t.start();
      o = morpho::canvas::impl::reconstruction_on_set_union_find_fastest(g, f, nbh, fun_alt);
      std::cout << t.stop() << std::endl;
      std::cout << (o == ref ? "OK" : "KO") << std::endl;
    }

    io::pbm::save(ref, "re.pbm");
  }

}
