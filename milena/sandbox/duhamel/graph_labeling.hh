#include <mln/core/image2d_b.hh>
#include <mln/core/sub_image.hh>
#include <mln/core/neighb2d.hh>
#include <mln/value/int_u8.hh>
#include <mln/io/pgm/load.hh>
#include <mln/debug/println.hh>
#include <mln/debug/println.hh>
#include <mln/util/graph.hh>
#include <mln/core/mesh_p.hh>
#include <mln/core/mesh_psite.hh>
#include <mln/draw/mesh.hh>
#include <mln/core/mesh_image.hh>
#include <algorithm> 

namespace mln
{
  template <typename I, typename N>
  mesh_p<point2d>*
  make_graph (Image<I>& ima_,
	      const Neighborhood<N>& nbh)
  {
    util::graph<void> gr;
    std::vector<point2d> v (300);
    std::map<value::int_u8, int> m;
    std::map<value::int_u8, int> x;
    std::map<value::int_u8, int> y;
    std::map<value::int_u8, int> c;
    std::vector< std::vector<value::int_u8> > vec;
    unsigned cpt = 0;
    I& ima = exact(ima_);

    mln_fwd_piter(I) p(ima.domain());
    mln_niter(N) n(nbh, p);

    for_all (p)
      {
	if (ima(p) == 0)
	  {
	    std::vector<value::int_u8> v;
	    for_all (n)
	      {
		if (ima(n) != 0)
		  v.push_back (ima(n));
	      }
	    vec.push_back (v);
	  }
	else
	  {
	    if (m.find (ima(p)) == m.end ())
	      {
		m[ima(p)] = cpt++;
		x[ima(p)] = 0;
		y[ima(p)] = 0;
		c[ima(p)] = 1;
	      }
	    else
	      {
		x[ima(p)] += p[1];
		y[ima(p)] += p[0];
		++c[ima(p)];
	      }
	  }
      }

    typename std::map<value::int_u8, int>::iterator ite = m.begin ();
    for (; ite != m.end (); ++ite)
      {
	gr.add_node ();
	int tmp_x = y[(*ite).first] / c[(*ite).first];
	int tmp_y = x[(*ite).first] / c[(*ite).first];
	v[m[(*ite).first]] = (make::point2d(tmp_x, tmp_y));
      }
    typename std::vector< std::vector<value::int_u8> >::iterator it = vec.begin ();
    for (; it != vec.end (); ++it)
      {
	unique((*it).begin (), (*it).end());
	if ((*it).size () == 2)
	  gr.add_edge (m[(*it)[0]], m[(*it)[1]]);
	if ((*it).size () == 3)
	  {
	    gr.add_edge (m[(*it)[0]], m[(*it)[1]]);
	    gr.add_edge (m[(*it)[1]], m[(*it)[2]]);
	    gr.add_edge (m[(*it)[0]], m[(*it)[2]]);
	  }
	if ((*it).size () == 4)
	  {
	    gr.add_edge (m[(*it)[0]], m[(*it)[1]]);
	    gr.add_edge (m[(*it)[0]], m[(*it)[2]]);
	    gr.add_edge (m[(*it)[0]], m[(*it)[3]]);
	    gr.add_edge (m[(*it)[1]], m[(*it)[2]]);
	    gr.add_edge (m[(*it)[1]], m[(*it)[3]]);
	    gr.add_edge (m[(*it)[2]], m[(*it)[3]]);
	  }
      }

    return (new mesh_p<point2d>(gr, v));
  }
}
