# include <mln/core/queue_p.hh>
# include <mln/core/clone.hh>

namespace mln
{
  template <typename I, typename N>
  I
  make_algo (Image<I>& ima_,
	     const Neighborhood<N>& nbh)
  {
    I& ima = exact(ima_);
    I out = clone(ima_);
    queue_p<mln_point (I)> q;
    int i;

    // init
    {
    mln_fwd_piter(I) p(ima.domain());
    mln_niter(N) n(nbh, p);

    for_all (p)
      {
	if (ima(p) == 0)
	  for_all(n)
	    if (ima(n) != 0)
	      {
		q.push (p);
		goto end;
	      }
      end:
	i = 0;
      }
    }

    std::cout << "q points = "
	      << q.npoints ()
	      << std::endl;
    //body
    {
      while (q.npoints ())
	{

	  mln_point (I) po = q.front ();
	  q.pop ();
	  mln_invariant (ima(po) == 0);

	  mln_niter(N) ne(nbh, po);
	  for_all (ne)
	    {
	      if (ima.has(ne))
		{
		  if (out(ne) != 0)
		    out(po) = out (ne);
		  else
		    if (!q.has (ne))
		      q.push (ne);
		}
	    }
	}
    }
    return out;
  }
}
