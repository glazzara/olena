#include <vector>
#include <mln/core/image/image2d.hh>
#include <mln/core/routine/clone.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/core/site_set/p_queue_fast.hh>
#include <mln/labeling/blobs.hh>
#include <mln/io/pbm/load.hh>
#include <mln/debug/println.hh>


namespace mln
{

  template <typename I, typename N>
  mln_concrete(I)
  influence_zones(const I& input, const N& nbh)
  {
    mln_concrete(I) output = clone(input);

    p_queue_fast<mln_site(I)> q;

    // Init.
    {
      mln_piter(I) p(input.domain());
      mln_niter(N) n(nbh, p);
      for_all(p)
	if (input(p) != 0)
	  for_all(n) if (input.has(n))
	    if (input(n) == 0)
	      {
		q.push(p);
		break;
	      }
    }

    // Body.
    {
      mln_site(I) p;
      mln_niter(N) n(nbh, p);
      while (! q.is_empty())
	{
	  p = q.pop_front();
	  mln_invariant(output(p) != 0);
	  for_all(n) if (input.has(n))
	    if (output(n) == 0)
	      {
		output(n) = output(p);
		q.push(n);
	      }
	}
    }

    return output;

  } // IZ


  template <typename I>
  void
  mk_graph(const I& iz, unsigned nlabels)
  {
    std::vector< std::vector<bool> > adj(nlabels + 1);
    for (unsigned l = 1; l <= nlabels; ++l)
      adj[l].resize(l, false);

    mln_piter(I) p(iz.domain());
    for_all(p)
      {
	mln_site(I) r = p + right, b = p + down;
	if (iz.has(r) && iz(p) != iz(r))
	  {
	    if (iz(p) < iz(r))
	      adj[iz(p)][iz(r)] = true;
	    else
	      adj[iz(r)][iz(p)] = true;
	  }
	if (iz.has(b) && iz(p) != iz(b))
	  {
	    if (iz(p) < iz(b))
	      adj[iz(p)][iz(b)] = true;
	    else
	      adj[iz(b)][iz(p)] = true;
	  }
      }

    for (unsigned l1 = 1; l1 <= nlabels; ++l1)
      for (unsigned l2 = 1; l2 <= nlabels; ++l2)
	if (adj[l1][l2])
	  std::cout << l1 << ' ' << l2 << std::endl;
  }

}



int main()
{
  using namespace mln;

  border::thickness = 0;

  image2d<bool> seeds;
  io::pbm::load(seeds, "+seeds.pbm");

  debug::println(seeds);

  unsigned n;
  image2d<unsigned> label = labeling::blobs(seeds, c4(), n);
  debug::println(label);

  image2d<unsigned> iz = influence_zones(label, c4());
  debug::println(iz);

  mk_graph(iz, n);
}
