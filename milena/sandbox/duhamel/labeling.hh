#ifndef LABELING_HH
# define LABELING_HH

/*! \file mln/canvas/labeling.hh
 *
 * \brief Connected component labeling of the object part in a binary
 * image.
 */

# include <mln/level/fill.hh>
# include "fill.hh"
# include <mln/level/sort_points.hh>


namespace mln
{

  namespace canvas
  {

    // General version.

    // In mln/canvas/labeling.hh


    // FIXME: Fast version.
    template <typename F>
    struct labeling_fast
    {
      F& f;

      typedef typename F::I I;
      typedef typename F::N N;
      typedef typename F::O O;
      typedef typename F::S S;
      typedef mln_psite(I) point;

//       // aux:
//       mln_ch_value(O, bool)  deja_vu;
//       mln_ch_value(O, point) parent;

      labeling_fast(F& f)
	: f(f)// ,
// 	  deja_vu(f.output.domain()),
// 	  parent(f.output.domain())
      {
	run();
      }

      void run()
      {
	// init
	{
	  f.nlabels = 0;
// 	  fill_opt2 (deja_vu, false);
// 	  mln::level::fill(deja_vu, false);
	  f.init();
	}
	// first pass
	{
	  mln_fwd_pixter(S) p(f.s);
	  mln_qiter(N) q(f.nbh, p);
	  for_all(p) if (f.handles(p))
	    {
	      make_set(p);
	      for_all(q)
// 		if (f.input.has(n) && deja_vu(n))
		  if (f.equiv(q, p))
		    do_union(q, p);
		  else
		    f.do_no_union(q, p);
// 	      deja_vu(p) = true;
	    }
	}

	// second pass
	{
	  mln_bkd_piter(S) p(f.s);
	  for_all(p) if (f.handles(p))
	    {
	      if (is_root(p))
		{
		  if (f.labels(p))
		    {
		      if (f.nlabels == mln_max(mln_value(O)))
			{
			  f.status = false;
			  return;
			}
		      f.output(p) = ++f.nlabels;
		    }
		}
	      else
		f.output(p) = f.output(parent(p));
	    }
	  f.status = true;
	}

      } // end of run()

      void make_set(const point& p)
      {
	parent(p) = p;
	f.init_attr(p);
      }

      bool is_root(const point& p) const
      {
	return parent(p) == p;
      }

      point find_root(const point& x)
      {
	if (parent(x) == x)
	  return x;
	else
	  return parent(x) = find_root(parent(x));
      }

      void do_union(const point& n, const point& p)
      {
	point r = find_root(n);
	if (r != p)
	  {
	    parent(r) = p;
	    f.merge_attr(r, p);
	  }
      }

    };
    // END FIX

  } // end of namespace mln::canvas

} // end of namespace mln


#endif // ! LABELING_HH
