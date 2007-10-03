# include <mln/core/queue_p.hh>
# include "queue_p_fast.hh"
# include <mln/core/clone.hh>
# include <mln/debug/println.hh>


#include <mln/core/image2d_b.hh>
#include <mln/core/sub_image.hh>
#include <mln/core/neighb2d.hh>
#include <mln/value/int_u8.hh>
#include <mln/io/pgm/load.hh>
#include <mln/debug/println.hh>
#include <mln/util/graph.hh>
#include <mln/core/mesh_p.hh>
#include <mln/accu/mean.hh>
#include <mln/estim/min_max.hh>
#include <algorithm> 
#include <utility> 
#include <map> 
#include <mln/metal/vec.hh>

namespace mln
{

//   namespace util
//   {
//     template <typename T>
//     struct pair
//     {
//       pair (T t1, T t2) :
// 	t1_ (t1),
// 	t2_ (t2)
//       {
//       }

//       T t1_;
//       T t2_;
//     };

//     template <typename T>
//     bool operator == (const pair<T> lhs, const pair<T> rhs)
//     {
//       if ((lhs.t1_ == rhs.t1_) && (lhs.t2_ == rhs.t2_))
// 	return true;
//       if ((lhs.t1_ == rhs.t2_) && (lhs.t2_ == rhs.t1_))
// 	return true;
//       return false;
//     }

//     template <typename T>
//     bool operator < (const pair<T> lhs, const pair<T> rhs)
//     {
//       if ((lhs.t1_ + lhs.t2_ * 10 < rhs.t1_ + rhs.t2_ * 10))// && (lhs.t2_ < rhs.t2_))
// 	return true;
//       return false;
//     }

//   } // end of mln::util

  namespace make
  {
    template <typename I, typename N>
    mesh_p<mln_psite(I)>
    graph_with_no_border (Image<I>& ima_,
			  const Neighborhood<N>& nbh)
    {
      typedef metal::vec<2,float> X;
      typedef mln_value(I) V;
      typedef mln_psite(I) P;

      I& ima = exact(ima_);
      util::graph<void> gr;
      V min, max;
      estim::min_max (ima, min, max);
      unsigned nb = max - min + 1;
      std::vector<P> v(nb);
      std::vector< accu::mean_< X > > tab_mean (nb);
      std::map<std::pair<V, V>, bool> m;
      
      mln_piter(I) p(ima.domain());
      mln_niter(N) n(nbh, p);

      for_all(p)
	{
	  X x = mln_point(I)(p);
	  tab_mean[ima(p) - min].take(x);
	  for_all (n) if (ima.has(n))
	    if (ima(p) != ima(n))
	      m[std::pair<V, V>(ima(p) - min, ima(n) - min)] = true;
	}
      
      for (unsigned i = min; i <= max; ++i)
	{
	  gr.add_node ();
 	  v[i - min] = make::point2d ((unsigned)tab_mean[i].to_result ()[0],
				      (unsigned)tab_mean[i].to_result ()[1]);
	}

      typename std::map<std::pair<V, V>, bool>::const_iterator it = m.begin ();
      for (; it != m.end (); ++it)
	gr.add_edge((*it).first.first, (*it).first.second);

      mesh_p<P> res(gr, v);
      return res;
    }
  } // end of mln::make


  template <typename I, typename N>
  I
  make_algo (Image<I>& ima_,
	     const Neighborhood<N>& nbh)
  {
    I& ima = exact(ima_);
    I out = clone(ima_);
    queue_p<mln_psite(I)> q;

    // Init.
    {
      mln_piter(I) p(ima.domain());
      mln_niter(N) n(nbh, p);

      for_all(p) if (ima(p) == 0)
	for_all(n) if (ima(n) != 0)
	  {
	    q.push(p);
	    break;
	  }
    }

//     // Body.
//     {
//       while (! q.empty())
// 	{
// 	  mln_psite(I) p = q.front();
// 	  q.pop();
// 	  mln_invariant(ima(p) == 0);

// 	  mln_niter(N) n(nbh, p);
// 	  for_all(n) if (ima.has(n))
// 	    if (out(n) != 0)
// 	      out(p) = out(n);
// 	    else
// 	      if (! q.has(n))
// 		q.push(n);
// 	}
//     }

    // Body: alternative version.
    {
      while (! q.empty())
	{
	  mln_psite(I) p = q.front();
	  q.pop();
	  if (out(p) != 0) // p has already been processed so ignore
	    continue;

	  mln_niter(N) n(nbh, p);
	  for_all(n) if (ima.has(n))
	    if (out(n) != 0)
	      out(p) = out(n);
	    else
	      q.push_force(n); // n may already be in the queue,
	                       // yet we then queue again this psite
	}
    }

    return out;
  }

  template <typename I, typename N>
  I
  make_algo_debug (Image<I>& ima_,
	     const Neighborhood<N>& nbh)
  {
    I& ima = exact(ima_);
    I out = clone(ima_);
    queue_p<mln_psite(I)> q;

    // Init.
    {
      mln_piter(I) p(ima.domain());
      mln_niter(N) n(nbh, p);

      for_all(p) if (ima(p) == 0)
	for_all(n) if (ima(n) != 0)
	  {
	    std::cout << "push : " << p << std::endl;
	    q.push(p);
	    break;
	  }
      std::cout << "init => q has " << q.npoints() << " points"
		<< std::endl;
    }

    // Body.
    {
      while (! q.empty())
	{
 	  debug::println(out);
	  mln_psite(I) p = q.front();
	  std::cout << "pop : " << p << std::endl;
	  q.pop();
	  mln_invariant(ima(p) == 0);

	  mln_niter(N) n(nbh, p);
	  for_all(n) if (ima.has(n))
	    if (out(n) != 0)
	      out(p) = out(n);
	    else
	      if (! q.has(n))
		{
		  std::cout << "push : " << n << std::endl;
		  q.push(n);
		}
	}
    }

//     // Body: alternative version.
//     {
//       while (! q.empty())
// 	{
// 	  mln_psite(I) p = q.front();
// 	  q.pop();
// 	  if (out(p) != 0) // p has already been processed so ignore
// 	    continue;

// 	  mln_niter(N) n(nbh, p);
// 	  for_all(n) if (ima.has(n))
// 	    if (out(n) != 0)
// 	      out(p) = out(n);
// 	    else
// 	      q.push_force(n); // n may already be in the queue,
// 	                       // yet we then queue again this psite
// 	}

    return out;
  }

} // end of mln
