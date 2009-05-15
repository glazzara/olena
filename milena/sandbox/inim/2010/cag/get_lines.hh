# include <mln/core/concept/image.hh>
# include <mln/labeling/compute.hh>
# include <mln/data/fill.hh>

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

using namespace mln;

namespace graph
{
  // Get lines:
  //
  // 1) Find the position of each vertex.
  // 2) Mark each vertex and edge as drawn.
  // 3) For each edge for which abs((v1.x - v2.x) / (v1.y - v2.y)) <
  //    alpha, mark the edge and vertices as not drawn.
  // 4) Draw the output image.
  // 5) Label lines in the output image.
  template <typename I>
  inline
  mln_concrete(I)
  get_lines(const util::graph& g,
	    const Image<I>& edges_,
	    const Image<I>& vertices_,
	    float alpha)
  {
    const I& edges = exact(edges_);
    const I& vertices = exact(vertices_);
    typedef mln_value(I) L;
    typedef mln_site(I) P;
    typedef typename P::coord coord;

    mln::image2d<bool> lines(edges.domain());
    data::fill(lines, false);

    util::array<util::couple<coord, coord> > vid2coord;
    vid2coord.reserve(g.v_nmax());

    mln_piter(I) p(vertices.domain());
    for_all(p)
    {
      if (vertices(p) != mln_max(L))
	vid2coord[vertices(p)] = make::couple(p.row(), p.col());
    }

    std::vector<bool> vid2bool;
    vid2bool.reserve(g.v_nmax());

    std::vector<bool> eid2bool;
    eid2bool.reserve(g.e_nmax());

    for (unsigned i = 0; i < g.e_nmax(); ++i)
    {
      unsigned v1 = g.v1(i);
      unsigned v2 = g.v2(i);

      vid2bool[v1] = false;
      vid2bool[v2] = false;
      eid2bool[i] = false;

      if (v1 == v2)
      {
	continue;
      }
      coord dy = (vid2coord[v1].first() - vid2coord[v2].first());
      coord dx = (vid2coord[v1].second() - vid2coord[v2].second());
      if (dy == 0 || std::abs(dy) < alpha * std::abs(dx))
      {
	vid2bool[v1] = true;
	vid2bool[v2] = true;
	eid2bool[i] = true;
      }
    }

    for_all(p)
    {
      if (vertices(p) != mln_max(L))
      {
	if (vid2bool[vertices(p)])
	  lines(p) = true;
      }
      else if (edges(p) != mln_max(L))
      {
	if (eid2bool[edges(p)])
	  lines(p) = true;
      }
    }

    mln_concrete(I) output(edges.domain());
    data::fill(output, mln_max(L));

    mln_value(I) nlines;
    mln_concrete(I) ret = labeling::blobs(lines, c8(), nlines);

    for_all(p)
    {
      if (ret(p) == 0u)
	continue;
      output(p) = ret(p).prev();
    }

    return output;
  }
}
