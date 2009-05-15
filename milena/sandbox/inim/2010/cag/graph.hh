#ifndef GRAPH_HH
# define GRAPH_HH

# include <map>

# include <iostream>

# include <mln/core/concept/image.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/core/image/image2d.hh>
# include <mln/core/alias/box2d.hh>
# include <mln/extension/adjust_fill.hh>
# include <mln/labeling/compute.hh>
# include <mln/accu/center.hh>
# include <mln/util/graph.hh>
# include <mln/util/set.hh>
# include <mln/util/couple.hh>

#define debug(STR) std::cout << STR << std::endl

using namespace mln;

namespace graph
{

  // Building the CAG:
  //
  // Similar to rag_and_labeled_wsl() in Olena.
  //
  // Return: (graph, (labeled_edges, labeled_vertices))
  // Note: returned images are labeled from 0.
  //       "empty" points have a value of mln_max(...).
  //
  // Steps:
  //
  // 1) Find vertex and edge sites on the watershed image:
  //    - wshd(p) != 0 && 2 adjacent basins => edge
  //    - wshd(p) != 0 && !(2 adjacent basins) => vertex
  //
  // 2) Label vertices
  // 3) Label edges according to the 2 adjacent basins
  // 4) For each edge, find the adjacent vertices
  // 5) Build the graph
  template <typename I, typename N>
  inline
  util::couple<util::graph, util::couple<mln_concrete(I), mln_concrete(I)> >
  make_cag(const Image<I>& wshd_,
	   const Neighborhood<N>& nbh_,
	   const mln_value(I)& nbasins)
  {
    trace::entering("make::impl::generic::make_cag");

    const I& wshd = exact(wshd_);
    const N& nbh = exact(nbh_);
    typedef mln_value(I) L;
    typedef mln_site(I) P;

    mln_concrete(I) edges(wshd.domain());
    data::fill(edges, mln_max(L));

    mln_concrete(I) vertices(wshd.domain());
    data::fill(vertices, mln_max(L));

    mln::image2d<bool> bvertices(wshd.domain());
    data::fill(bvertices, false);

    std::map<util::couple<L,L>, L> c2la;
    util::array<util::couple<L,L> > la2c;
    L nedges = literal::zero;

    L l1, l2;
    bool is_vertex;
    mln_piter(I) p(wshd.domain());
    mln_niter(N) n(nbh, p);
    mln::border::fill(wshd, mln_max(L));

    // 1) Find vertex and edge sites on the watershed image:
    //    - wshd(p) != 0 && 2 adjacent basins => edge
    //    - wshd(p) != 0 && !(2 adjacent basins) => vertex
    for_all(p)
    {
      if (wshd(p) != 0u)
	continue;
      // p is in the watershed line.
      l1 = l2 = 0;
      is_vertex = false;
      for_all(n)
      {
	if (!wshd.has(n) || wshd(n) == mln_max(L))
	{
	  is_vertex = true;
	  break;
	}
        if (wshd(n) != 0u)
	{
	  if (l1 == 0u) // First label to be stored.
	    l1 = wshd(n);
	  else
	    if (wshd(n) != l1 && l2 == 0)
	    { // Second label to be stored.
	      l2 = wshd(n);
	    }
	    else
	      if (wshd(n) != l1 && wshd(n) != l2)
	      {
		is_vertex = true; // three or more labels means a vertex
		break;
	      }
	}
      }
      if (l2 == 0u || l1 == 0u || is_vertex)
      {
	bvertices(p) = true;
	continue;
      }
      if (l2 < l1)
	std::swap(l1, l2);

      // 3) Label edges according to the 2 adjacent basins
      // adjacency l1 l2
      util::couple<L,L> c = make::couple(l2,l1);
      if (c2la.find(c) == c2la.end())
      {
	c2la[c] = nedges++;
	la2c.append(c);
      }
      edges(p) = c2la[c];
    }

    // 2) Label vertices
    L nvertices;
    mln_concrete(I) vertices_labeled =
      labeling::blobs(bvertices, c8(), nvertices); // try c4

    // 4) For each edge, find the adjacent vertices
    // Init edge to vertices array
    typedef util::set<L> vset;
    typedef typename util::set<L>::eiter vset_eiter;
    util::array<vset> e2vs;
    for (unsigned i = 0; i < nedges; ++i)
    {
      e2vs.append(vset());
    }

    // Link vertices to their edge(s)
    for_all(p)
    {
      if (vertices_labeled(p) == 0u)
	continue;
      vertices(p) = vertices_labeled(p).prev();
      for_all(n)
      {
	if (edges.has(n) && edges(n) != mln_max(L))
	{
	  vset& st = e2vs[edges(n)];
	  st.insert(vertices(p));
	}
      }
    }

    // 5) Build the graph
    // Construct graph.
    util::graph g;
    g.add_vertices(nvertices);
    for (unsigned i = 0; i < e2vs.nelements(); ++i)
    {
      vset& st = e2vs[i];
      if (st.nelements() > 1)
      {
	L e1 = st.first_element();
	L e2 = st.last_element();
	g.add_edge(e1, e2);
      }
      else
      {
	// Hack to keep transition indexes and values in the output
	// image consistent.
	g.add_edge(0, 0);
      }
    }

    trace::exiting("make::impl::generic::make_cag");

    return make::couple(g, make::couple(edges, vertices));
  }

} // ! graph

#endif // ! GRAPH_HH
