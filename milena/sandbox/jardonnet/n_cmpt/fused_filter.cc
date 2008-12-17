#include <mln/core/image/image2d.hh>
#include <mln/value/int_u8.hh>
#include <mln/io/pgm/load.hh>
#include <mln/debug/println.hh>

#include <mln/core/site_set/p_array.hh>
#include <mln/level/sort_psites.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/morpho/tree/data.hh>

#include <mln/accu/count.hh>
#include <mln/util/set.hh>

#include <mln/labeling/regional_minima.hh>
#include <mln/morpho/tree/compute_attribute_image.hh>
#include <mln/morpho/closing_area.hh>
#include <mln/level/fill.hh>

#include <mln/debug/iota.hh>

#include <mln/pw/all.hh>
#include <mln/core/image/image_if.hh>

namespace mln
{

  template <typename I>
  void println_par(const I& par)
  {
    int nr = par.nrows(), nc = par.ncols();
    for (int r = 0; r < nr; ++r)
      {
	for (int c = 0; c < nc; ++c)
	  if (par.at(r,c) == point2d(r,c))
	    std::cout << "(    ) ";
	  else
	    std::cout << par.at(r,c) << ' ';
	std::cout << std::endl;
      }
  }

  template <typename P>
  inline
  mln_value(P) find_root__(P& par, const mln_value(P)& x)
  {
    if (par(x) == x)
      return x;
    else
      return par(x) = find_root__(par, par(x));
  }


  template <typename I, typename A, typename N>
  mln_ch_value(I, util::set<unsigned>)
    compute_labels(const I& f, const A& a, const N& nbh,
		   unsigned n_objects,
		   bool echo = false)
  {
    if (echo)
      debug::println("f =", f);

    typedef p_array<mln_psite(I)> S;
    S s = level::sort_psites_increasing(a);
    // s maps increasing attributes.

    mln_ch_value(I, mln_site(I)) par;
    mln_ch_value(I, bool) deja_vu;
    mln_ch_value(I, util::set<unsigned>) labels;
    unsigned nbassins, current_n;

    // init fused image
    mln_ch_value(I, bool) fused;
    initialize(fused, a);
    mln::level::fill(fused, false);

    // iota
    mln_ch_value(I,value::int_u<16>) iota(a.domain());
    debug::iota(iota);

    // labels
    mln_ch_value(I, unsigned) regmin = labeling::regional_minima(a, nbh,
                                                                 nbassins);

    {
      // Initialization.
      mln_piter(A) p(f.domain());

      // parent
      initialize(par, f);
      for_all(p)
        par(p) = p;

      // deja_vu
      initialize(deja_vu, f);
      level::fill(deja_vu, false);


      if (n_objects >= nbassins)
      {
        std::cerr << "The number of expected objects is higher than the number of regional minima!" << std::endl;
        std::abort();
      }

      if (echo)
        debug::println("regmin(f) =", regmin);

      initialize(labels, f);
      for_all(p)
        if (regmin(p) != 0) // p in a reg min of the attribute image
        {
          labels(p).insert(regmin(p));
          fused(p) = true;
        }
    }

    current_n = nbassins;

    // First pass.
      mln_site(I) r;
      mln_fwd_piter(S) p(s);
      mln_niter(N) n(nbh, p);
      for_all(p)
      {
        std::cout << p << std::endl;
        for_all(n)
        {
          if (a.domain().has(n) && deja_vu(n))
          {
            std::cout << " " << n << std::endl;
            r = find_root__(par, n);
            if (r != p)
            {
              par(r) = p; // Union.


              /////TMP
              if (labels(r).is_empty())
                // No-op.
                ;
              else
                if (labels(p).is_empty())
                  labels(p) = labels(r);
                else
                  if (labels(p) == labels(r))
                  {
                    // No-op.
                    // Should only happen if p and r are minima
                  }
                  else
                    labels(p).insert(labels(r));

              /////

              if (p == point2d(1,0) && r == point2d(2,0))
              {
                std::cerr << "fused(r) " << fused(r) << std::endl;
                std::cerr << "regmin(p) == 0 " << (regmin(p) == 0) << std::endl;
                std::cerr << "fused(p) " << fused(p) << std::endl;
                std::cerr << "current_n > n_objects " << (current_n > n_objects) << std::endl;
              }

              // min_v != 0   <=> volume(p) == 1 ?
              if (fused(r)       &&
                  regmin(p) == 0 &&// p is not a minima
                  fused(p)       &&// p already belong to a cmpt (fused for an another n)
                  current_n > n_objects) // union is still allowed
              {
                std::cerr << p << " <- " << r
                          << labels(p) << labels(r) << std::endl;

                current_n--;
                std::cout << "dec" << std::endl;
              }

              //mln_invariant(fused(r) || a(r) == a(p));
              // DOESNT WORK

              // Union made if
              if (current_n >= n_objects || // union is still allowed or
                   not fused(r) || // r not fused or
                   not fused(p) || // p not fused or
                   regmin(p) != 0) // p is a minima
               {
                 //par(r) = p;
                 fused(p) = fused(r);
                 //iota(p) = iota(r);

//              std::cout << "volume " << a(p) << " - " << current_n << std::endl;
//              debug::println(iota | pw::value(fused) == pw::cst(true));
//              std::cout << "---------------------" << std::endl;
               }
            }
          }
        }
        deja_vu(p) = true;
      }
    return labels;
  }


} // end of namespace mln


void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.pgm n echo" << std::endl;
  std::cerr << "n: number of expected objects (n > 0)" << std::endl;
  std::cerr << "echo: 0 (silent) or 1 (verbose)" << std::endl;
  std::cerr << "merge using sets of labels from regional minima and save the highest label image" << std::endl;
  std::abort();
}


int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;

  if (argc != 4)
    usage(argv);

  typedef image2d<int_u8> I;
  I f;
  // input image
  io::pgm::load(f, argv[1]);

  // n
  int n = std::atoi(argv[2]);
  if (n <= 0)
    usage(argv);

  // echo
  int echo = std::atoi(argv[3]);
  if (echo != 0 && echo != 1)
    usage(argv);

  typedef p_array<point2d> S;
  S s = level::sort_psites_decreasing(f);

  // Children go towards lower levels so leafs are regional minima.
  // We get a min-tree so that we can perform morphological closings.

  morpho::tree::data<I,S> t(f, s, c4());
  accu::count< util::pix<I> > attr;

  image2d<unsigned> a = morpho::tree::compute_attribute_image(attr, t);


  image2d< util::set<unsigned> > labels = compute_labels(f, a, c4(), n, echo);
  if (echo)
    debug::println("labels =", labels);
}
