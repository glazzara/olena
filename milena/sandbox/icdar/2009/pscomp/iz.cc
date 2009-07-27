
// From mln/canvas/distance_geodesic.hh

# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/core/routine/duplicate.hh>
# include <mln/core/site_set/p_queue_fast.hh>
# include <queue>
# include <mln/data/fill.hh>
# include <mln/extension/adjust_fill.hh>


// Local.

#include <map>
#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/value/int_u8.hh>
#include <mln/io/pbm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/debug/println.hh>
#include <mln/data/saturate.hh>

#include <mln/labeling/blobs.hh>
#include <mln/labeling/value.hh>




namespace mln
{


  // From  mln/canvas/distance_geodesic.hh
  //  and  mln/transform/internal/influence_zone_functor.hh
  //  and  mln/transform/influence_zone_geodesic.hh


  template <typename I, typename N>
  std::map< std::pair<mln_value(I),mln_value(I)>, unsigned >
  distances(// in:
	    const I& input, const N& nbh,
	    // out:
	    mln_ch_value(I, unsigned)& dmap,
	    mln_concrete(I)& iz)
  {
    trace::entering("canvas::impl::generic::distance_geodesic");

    typedef mln_value(I) L;
    std::map< std::pair<L,L>, unsigned > dist; // NEW

    const unsigned max = mln_max(unsigned);
    typedef mln_site(I) P;
    p_queue_fast<P> q;

    // Initialization.
    {
      iz = duplicate(input); // <-- init

      data::fill(dmap, max);

      mln_piter(I) p(input.domain());
      mln_niter(N) n(nbh, p);
      for_all(p)
	if (input(p) != 0) // <-- inqueue_p_wrt_input_p
	  {
	    ; // <-- init_p
	    dmap(p) = 0;
	    for_all(n)
	      if (input.domain().has(n) &&
		  input(n) == 0) // <-- inqueue_p_wrt_input_n
		{
		  q.push(p);
		  break;
		}
	  }
    }

    // Propagation.
    {
      P p;
      mln_niter(N) n(nbh, p);
      while (! q.is_empty())
	{
	  p = q.pop_front();
	  if (dmap(p) == max)
	    {
	      // Saturation so stop.
	      q.clear();
	      break;
	    }
	  for_all(n)
	  {
	    if (! input.domain().has(n))
	      continue;
	    if (dmap(n) == max)
	      {
		dmap(n) = dmap(p) + 1;
		iz(n) = iz(p); // <- process
		q.push(n);
	      }
	    else
	      {
		if (iz(n) != iz(p))
		  {
		    L l1 = iz(n), l2 = iz(p);
		    if (l1 > l2)
		      std::swap(l1, l2);
		    unsigned& d_ = dist[std::make_pair(l1,l2)];
		    unsigned d12 = dmap(p) + dmap(n) + 1;
		    if (d_ == 0 || d12 < d_)
		      d_ = d12;
		  }
	      }
	  }
	}
    }

    return dist;
  }


} // mln



int main(int argc, char* argv[])
{
  using namespace mln;

  typedef unsigned L;

  image2d<bool> input;
  io::pbm::load(input, argv[1]);

  L n_labels;
  image2d<L> lab = labeling::value(input, true, c8(), n_labels);
  std::cout << "n labels = " << n_labels << std::endl;


  image2d<unsigned> dmap(input.domain());
  image2d<L> iz(input.domain());

  typedef std::map< std::pair<L,L>, unsigned > map_t; 
  map_t d = distances(lab, c8(), dmap, iz);

  io::pgm::save(data::saturate(value::int_u8(), dmap), "dmap.pgm");

  // debug::println("lab = ", lab);
  std::cout << "map size = " << d.size() << std::endl;


//   {
//     // Compute statistics.
//     std::map<unsigned, unsigned> h;
//     for (map_t::const_iterator i = d.begin();
// 	 i != d.end(); ++i)
//       ++h[i->second];

//     for (std::map<unsigned, unsigned>::const_iterator i = h.begin();
// 	 i != h.end(); ++i)
//       std::cout << i->first << " " << i->second << std::endl;
//   }


//   {
//     // Print: d(label1, label2) = shortest distance
//     for (map_t::const_iterator i = d.begin();
// 	 i != d.end(); ++i)
//       std::cout << "d(" << i->first.first << ", " << i->first.second << ") = "
// 		<< i->second << std::endl;
//   }

}
