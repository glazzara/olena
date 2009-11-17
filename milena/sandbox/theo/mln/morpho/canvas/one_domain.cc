
# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/morpho/canvas/internal/find_root.hh>

# include <mln/data/fill.hh>
# include <mln/data/sort_psites.hh>
# include <mln/border/equalize.hh>
# include <mln/border/fill.hh>


// cc

#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/debug/println.hh>
#include <mln/io/pbm/load.hh>
#include <mln/io/pgm/load.hh>
#include <mln/io/pbm/save.hh>
#include <mln/io/pgm/save.hh>
#include <mln/data/compare.hh>
#include <mln/util/timer.hh>

#include <mln/labeling/regional_maxima.hh>
#include <mln/labeling/regional_minima.hh>
#include <mln/pw/all.hh>



namespace mln
{

  namespace morpho
  {

    namespace canvas
    {


      template <typename I, typename N, typename F>
      inline
      mln_result(F)
      one_domain_union_find(const Image<I>& f_, const Neighborhood<N>& nbh_,
			    F& functor)
      {
	trace::entering("morpho::canvas::one_domain_union_find");

	const I& f = exact(f_);
	const N& nbh = exact(nbh_);
	    
	typedef typename F::S S;
	typedef mln_psite(I) P;
	typedef mln_value(I) V;

	// Auxiliary data.
	mln_ch_value(I, bool) deja_vu;
	mln_ch_value(I, P)    parent;
	mln_result(F)         output;

	// Initialization.
	{
	  initialize(output, f);
	  functor.output = output;

	  initialize(parent, f);
	  initialize(deja_vu, f);
	  data::fill(deja_vu, false);

	  functor.set_default();                               //  <--  set_default
	}

	// First pass.
	{
	  mln_bkd_piter(S) p(functor.s());
	  mln_niter(N) n(nbh, p);
	  for_all(p)
	    if (functor.domain(p))                             //  <--  domain
	      {
		// Make-Set.
		parent(p) = p;
		functor.init(p);                               //  <--  init

		for_all(n) if (f.domain().has(n))
		  {
		    // See below (*)
		    if (functor.domain(n))                     //  <--  domain
		      {
			if (deja_vu(n))
			  {
			    if (functor.equiv(p, n))           //  <--  equiv
			      {
				// Do-Union.
				P r = internal::find_root(parent, n);
				if (r != p)
				  {
// 				    if (do_merge(p, r))
				      {
					parent(r) = p;
					functor.merge(p, r);       //  <--  merge
				      }
// 				    else
// 				      no_merge(p, r);
				  }
			      }
			    else
			      functor.do_no_union(p, n);       //  <--  do_no_union
			  }
		      }
		    else
		      {
			mln_invariant(deja_vu(n) == false);
			functor.border(p, n);                  //  <--  border
		      }
		  }
		deja_vu(p) = true;
	      }
	}

	// Second pass.
	{
	  mln_fwd_piter(S) p(functor.s());
	  for_all(p)
	    if (functor.domain(p))                             //  <--  domain
	      if (parent(p) == p)
		functor.root(p);                               //  <--  root
	      else
		output(p) = output(parent(p));
	}

	trace::exiting("morpho::canvas::one_domain_union_find");
	return output;
      }

    } // end of namespace mln::morpho::canvas

  } // end of namespace mln::morpho



  // base functor

  struct functor_base
  {
    void set_default()
    {
    }
    template <typename P>
    void init(const P&)
    {
    }
    template <typename P>
    void root(const P&)
    {
    }
    template <typename P>
    bool domain(const P&)
    {
      return true;
    }
    template <typename P1, typename P2>
    void do_no_union(const P1&, const P2&)
    {
    }
    template <typename P1, typename P2>
    void border(const P1&, const P2&)
    {
    }
    template <typename P1, typename P2>
    bool equiv(const P1&, const P2&)
    {
      return true;
    }
    template <typename P1, typename P2>
    void merge(const P1&, const P2&)
    {
    }
    // no default for:
    //  typedef S
    //  .s()
  };


  // "binary reconstruction by dilation" functor

  template <typename F, typename G>
  struct rd_functor : functor_base
  {
    typedef mln_concrete(F) result;

    const F& f;
    const G& g;

    typedef mln_domain(F) S;

    rd_functor(const F& f, const G& g)
      : f(f), g(g)
    {
    }

    result output;
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

    S s() const
    {
      return f.domain();
    }

  };


  // "labeling" functor

  template <typename I, typename L>
  struct lab_functor : functor_base
  {
    typedef mln_ch_value(I, L) result;

    const I& input;
    mln_value(I) val;

    L nlabels;
    bool status;

    lab_functor(const I& input, const mln_value(I)& val)
      : input(input), val(val)
    {
    }

    result output;
    typedef mln_psite(I) P;

    typedef mln_domain(I) S;

    S s() const
    {
      return input.domain();
    }

    void set_default()
    {
      data::fill(output, L(literal::zero));
      nlabels = 0;
      status = true;
    }

    bool domain(const P& p) const
    {
      return input(p) == val;
    }

    bool equiv(const P& p, const P& n)
    {
      return input(n) == val;
    }

    void root(const P& p)
    {
      if (status == false)
	return;

      if (nlabels == mln_max(L))
	{
	  status = false;
	  trace::warning("labeling aborted!");
	}
      else
	output(p) = ++nlabels;
    }

  };


  // "regional maxima labeling" functor

  template <typename I, typename L>
  struct regmaxlab_functor : functor_base
  {
    typedef mln_ch_value(I, L) result;

    const I& input;

    L nlabels;
    bool status;
    mln_ch_value(I, bool) attr;

    typedef mln_psite(I) P;
    typedef p_array<P> S;
    S s_;

    result output;

    regmaxlab_functor(const I& input)
      : input(input)
    {
      s_ = data::sort_psites_increasing(input);
    }

    const S& s() const
    {
      return s_;
    }

    void set_default()
    {
      initialize(attr, input);
      data::fill(attr, true);
      data::fill(output, L(literal::zero));
      nlabels = 0;
      status = true;
    }

    bool equiv(const P& p, const P& n)
    {
      return input(n) == input(p);
    }

    void merge(const P& p, const P& r)
    {
      attr(p) = attr(p) && attr(r);
    }

    void do_no_union(const P& p, const P& n)
    {
      mln_invariant(input(n) > input(p));
      attr(p) = false;
      (void)n;
    }

    void root(const P& p)
    {
      if (attr(p) == false || status == false)
	return;

      if (nlabels == mln_max(L))
	{
	  status = false;
	  trace::warning("labeling aborted!");
	}
      else
	output(p) = ++nlabels;
    }

  };

  
  // "regional minima identification" functor

  template <typename I>
  struct regminid_functor : functor_base
  {
    typedef mln_ch_value(I, bool) result;

    const I& input;

    typedef mln_psite(I) P;
    typedef p_array<P> S;
    S s_;

    result output;

    regminid_functor(const I& input)
      : input(input)
    {
      s_ = data::sort_psites_decreasing(input);
    }

    const S& s() const
    {
      return s_;
    }

    void init(const P& p)
    {
      output(p) = true;
    }

    bool equiv(const P& p, const P& n)
    {
      return input(n) == input(p);
    }

    void merge(const P& p, const P& r)
    {
      output(p) = output(p) && output(r);
    }

    void do_no_union(const P& p, const P& n)
    {
      if (input(n) < input(p))
	output(p) = false;
    }

  };



  // "reconstruction by dilation on function" functor

  template <typename F, typename G>
  struct rdf_functor : functor_base
  {
    typedef mln_concrete(F) result;

    const F& f;
    const G& g;

    typedef mln_value(F) V;
    typedef mln_psite(F) P;
    typedef p_array<P> S;
    S s_;

    result output;

    rdf_functor(const F& f, const G& g)
      : f(f), g(g)
    {
      s_ = data::sort_psites_decreasing(g);
    }

    const S& s() const
    {
      return f.domain();
    }

    void set_default()
    {
      mln::data::fill(output, f);
    }

    bool equiv(const P& p, const P& n)
    {
      return g(r) == g(p) || g(p) >= output(r);
    }

    void merge(const P& p, const P& r)
    {
      if (output(r) > output(p))
	output(p) = output(r);
    }

    void do_no_union(const P& p, const P& n)
    {
      output(p) = mln_max(V);
    }

    void root(const P& p)
    {
      if (output(p) == mln_max(V))
	output(p) = g(p);
    }

  };

  

} // end of namespace mln



int main()
{
  using namespace mln;

  typedef value::int_u8 L;
  neighb2d nbh = c4();

  util::timer t;


  // On sets.

//   {
//     typedef image2d<bool> I;

//     I f, g, ref;

//     io::pbm::load(f, "f_and_g.pbm");
//     io::pbm::load(g, "g.pbm");

//     {
//       t.start();
//       rd_functor<I,I> fun(f, g);
//       I rd = morpho::canvas::one_domain_union_find(f, nbh, fun);
//       std::cout << "rd: " << t.stop() << std::endl;
//       io::pbm::save(rd, "rd.pbm");
//     }

//     {
//       t.start();
//       lab_functor<I,L> fun(f, true);
//       image2d<L> lab = morpho::canvas::one_domain_union_find(f, nbh, fun);
//       std::cout << "lab: " << t.stop() << std::endl;
//       io::pgm::save(lab, "lab.pgm");
//     }
//   }


  // On functions.

  {
    typedef image2d<value::int_u8> I;
    I f;
    io::pgm::load(f, "./lena_blurred.pgm");

//     {
//       // regional maxima labeling
//       t.start();
//       regmaxlab_functor<I,L> fun(f);
//       image2d<L> lab = morpho::canvas::one_domain_union_find(f, nbh, fun);
//       std::cout << "regmax lab: " << t.stop() << std::endl;
//       io::pgm::save(lab, "regmaxlab.pgm");

//       L nlabels_ref;
//       if (lab != labeling::regional_maxima(f, nbh, nlabels_ref)
// 	  || fun.nlabels != nlabels_ref)
// 	std::cerr << "oops" << std::endl;
//     }

//     {
//       // regional minima identification
//       t.start();
//       regminid_functor<I> fun(f);
//       image2d<bool> id = morpho::canvas::one_domain_union_find(f, nbh, fun);
//       std::cout << "regmin id: " << t.stop() << std::endl;
//       io::pbm::save(id, "regminid.pbm");

//       L forget_it;
//       if (id != ((pw::value(labeling::regional_minima(f, nbh, forget_it)) != pw::cst(0)) | id.domain()))
// 	std::cerr << "oops" << std::endl;
//     }

    {
      // reconstruction by dilation
      I f, g;
      io::pgm::load(f, "lena.pgm");
      io::pgm::load(g, "lena_min.pgm");
      if (! (f <= g))
	std::cerr << "OK" << std::endl;

      rdf_functor<I,I> fun(f, g);
      I rdf = morpho::canvas::one_domain_union_find(f, nbh, fun);
      std::cout << "rdf: " << t.stop() << std::endl;
      io::pgm::save(rdf, "rdf.pbm");
 
    }

  }

}
