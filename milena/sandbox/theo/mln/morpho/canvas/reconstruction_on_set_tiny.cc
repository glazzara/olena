#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/debug/println.hh>

#include "reconstruction_on_set.hh"



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





int main()
{
  using namespace mln;

  typedef image2d<bool> I;

  I f, g, o;
  neighb2d nbh = c4();

  bool gvals[9][5] = { { 0, 1, 1, 1, 0 },
		       { 0, 0, 0, 1, 0 },
		       { 0, 1, 1, 1, 0 },
		       { 0, 1, 0, 0, 0 },
		       { 0, 1, 1, 1, 0 },
		       { 0, 0, 0, 1, 0 },
		       { 0, 1, 1, 1, 0 },
		       { 0, 1, 0, 0, 0 },
		       { 0, 1, 1, 1, 0 } };
  g = make::image(gvals);
  debug::println("g", g);

  bool fvals[9][5] = { { 0, 0, 0, 0, 0 },
		       { 0, 0, 0, 0, 0 },
		       { 0, 0, 0, 0, 0 },
		       { 0, 0, 0, 0, 0 },
		       { 0, 0, 1, 0, 0 },
		       { 0, 0, 0, 0, 0 },
		       { 0, 0, 0, 0, 0 },
		       { 0, 0, 0, 0, 0 },
		       { 0, 0, 0, 0, 0 } };
  f = make::image(fvals);
  debug::println("f", f);

  rd_alt_functor<I,I> fun(f, g);
  o = morpho::canvas::reconstruction_on_set_union_find(f, g, nbh, fun);

  debug::println("o", o);
}
