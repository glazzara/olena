// g++ -Wall -Wextra -DNDEBUG -O3 -I../.. -o irm irm.cc
// g++ -Wall -Wextra -ggdb -I../.. -o irm irm.cc

#include <iterator>
#include <iostream>
#include <algorithm>

#include <set>
#include <map>

#include <mln/core/image2d.hh>

#include <mln/core/neighb2d.hh>
#include <mln/core/window2d.hh>
#include <mln/core/image_if.hh>
#include <mln/win/rectangle2d.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/ppm/save.hh>

#include <mln/value/int_u8.hh>
#include <mln/value/rgb8.hh>
#include <mln/literal/colors.hh>

#include <mln/level/transform.hh>

#include <mln/convert/to_window.hh>
#include <mln/convert/to_image.hh>
#include <mln/convert/to_fun.hh>

#include <mln/morpho/gradient.hh>
#include <mln/morpho/closing_area.hh>
#include <mln/morpho/meyer_wst.hh>

#include <mln/accu/mean.hh>
#include <mln/level/take.hh>

#include <mln/util/graph.hh>

#include <mln/debug/println.hh>

// #include <segm_to_pregraph.hh>
// #include <fusion_graph2.hh>

mln::value::int_u8 foo(unsigned u)
{
  return u == 0 ?
    0 : // wshed line
    1 + (u - 1) % 255; // basin
}


namespace my_mln
{
  using namespace mln;

  
  template <typename I>
  void region_10(const I& lbl)
  {
    mln_piter(I) p(lbl.domain());

    I carte;
    initialize(carte, lbl);

    for_all(p)
    {
      if (lbl(p) == 10)
      {
        carte(p) = 0;
      }
      else
      {
        carte(p) = 255;
      }
    }

    io::pgm::save(level::transform(carte, convert::to_fun(foo)),
                  "tmp_region_10.pgm");

  }
  
  template <typename I>
  void puzzle(const I& irm,
	    const image2d<unsigned>& lbl,
	    unsigned nlabels)
  {
//  {
//     accu::mean_<unsigned> m;

//     mln_piter(I) p(irm.domain());
//     for_all(p)
//       if (lbl(p) == 82)
// 	m.take(irm(p));

//     level::take(irm | (pw::value(lbl) == pw::cst(82)), m);

//     std::cout << "reg 82 has mean = " << m << std::endl;
//  }

    std::vector< accu::mean_<unsigned> > m(nlabels + 1);
    mln_piter(I) p(irm.domain());
    for_all(p)
      m[lbl(p)].take(irm(p));

    I carte;
    initialize(carte, irm);

    for_all(p)
    {
      carte(p) = m[lbl(p)].to_result();
    }
   

    io::pgm::save(carte, "tmp_puzzle.pgm");

//    accu::mean_<unsigned> mm;
//    mm.take(m[70]);
//    mm.take(m[77]);
//    mm.take(m[80]);
//    mm.take(m[82]);
//    mm.take(m[99]);
//    std::cout << mm.to_result() << std::endl;

  }


//   template <typename I, typename J, typename N>
//   void mk_graph(const I& lbl,
//   		const J& irm,
// 		N nbh,
// 		unsigned nlabels)
//   {
//     std::vector< std::vector<bool> > adja(nlabels + 1);
//     for (unsigned l = 0; l <= nlabels; ++l)
//       adja[l].resize(nlabels + 1, false);
// 
//     mln_piter(I) p(lbl.domain());
//     mln_niter(N) n(nbh, p);
// 
//     // We compute the adjacency matrix of the RAG.
//     for_all(p)
//       if (lbl(p) == 0) // wshed
// 	{
// 	  std::set<unsigned> s;
// 	  for_all(n) if (lbl.has(n))
// 	    {
// 	      if (lbl(n) == 0)
// 		continue;
// 	      s.insert(lbl(n));
// 	    }
// 	  if (s.size() < 2)
// 	    {
// 	      std::cout << "#" << std::endl;
// 	      continue;
// 	    }
// 	  std::set<unsigned>::const_iterator l1, l2;
// 	  for (l1 = s.begin(); l1 != s.end(); ++l1)
// 	    {
// 	      l2 = l1;
// 	      for (++l2; l2 != s.end(); ++l2)
// 		adja[*l1][*l2] = true;
// 	    }
// 	}
// 
//     unsigned count = 0;
//     for (unsigned l1 = 1; l1 <= nlabels; ++l1)
//       for (unsigned l2 = 1; l2 <= nlabels; ++l2)
// 	if (adja[l1][l2])
// 	  ++count;
//     std::cout << "link count = " << count << std::endl;
// 
//     // Graph.
//     util::graph<float> g;
//     // Vertices.
//     for (unsigned i = 0; i <= nlabels; ++i)
//     {
//       accu::mean_<unsigned> m;
//       level::take(irm | (pw::value(lbl) == pw::cst(i)), m);
//       g.add_vertex(m.to_result());
//     }
//     // Edges.
//     for (unsigned l1 = 1; l1 <= nlabels; ++l1)
//       for (unsigned l2 = l1 + 1; l2 <= nlabels; ++l2)
// 	if (adja[l1][l2])
// 	  g.add_edge(l1, l2);
//     //g.print_debug (std::cout);
//     for( int i = 0 ; i < g.nvertices() ; ++i)
//       std::cout << "[" << i << " : " << g.vertex_data(i) << " ] ";
//     std::cout << std::endl;
//   }


  /*------------------------.
  | Region adjacent graph.  |
  `------------------------*/

  /// Build a region adjacency graph.
  template <typename I, typename J, typename N>
  util::graph< accu::mean_<mln_value(J)> >
  make_rag(const I& label, const J& input, N nbh, unsigned nlabels)
  {
    std::vector< std::vector<bool> > adja(nlabels + 1);
    for (unsigned l = 0; l <= nlabels; ++l)
      adja[l].resize(nlabels + 1, false);

    mln_piter(I) p(label.domain());
    mln_niter(N) n(nbh, p);

    // We compute the adjacency matrix of the RAG.
    for_all(p)
      if (label(p) == 0) // wshed
        {
          std::set<unsigned> s;
          for_all(n) if (label.has(n))
	    {
	      if (label(n) == 0)
		continue;
	      s.insert(label(n));
	    }
          if (s.size() < 2)
	    {
	      std::cout << "#" << std::endl;
	      continue;
	    }
          std::set<unsigned>::const_iterator l1, l2;
          for (l1 = s.begin(); l1 != s.end(); ++l1)
	    {
	      l2 = l1;
	      for (++l2; l2 != s.end(); ++l2)
		adja[*l1][*l2] = true;
	    }
        }

    unsigned count = 0;
    for (unsigned l1 = 1; l1 <= nlabels; ++l1)
      for (unsigned l2 = 1; l2 <= nlabels; ++l2)
	if (adja[l1][l2])
	  ++count;
    std::cout << "link count = " << count << std::endl;

    // Graph.
    util::graph< accu::mean_<mln_value(J)> > g;
    // Vertices.
    for (unsigned i = 0; i <= nlabels; ++i)
      {
	accu::mean_<mln_value(J)> m;
	level::take(input | (pw::value(label) == pw::cst(i)), m);
	g.add_vertex(m);
      }
    // Edges.
    for (unsigned l1 = 1; l1 <= nlabels; ++l1)
      for (unsigned l2 = l1 + 1; l2 <= nlabels; ++l2)
	if (adja[l1][l2])
	  g.add_edge(l1, l2);
    //g.print_debug (std::cout);
    for( unsigned i = 0 ; i < g.nvertices() ; ++i)
      std::cout << "[" << i << " : " << g.vertex_data(i) << " ] ";
    std::cout << std::endl;
    return g;
  }

//   template <typename I, typename J, typename N>
//   void mk2_graph(const I& label , const J& input , const N&
//                  nbh, int nbasins)
//     {
//       typedef mln::p_set<mln_point(I)> R;
//       std::map<int, R > regions;
      
//       mln_piter(I) p(label.domain());
//       for_all(p)
//       {
//         if (unsigned int i = label(p))
//           regions[i].insert(p);
//       }

//       std::cout << "Boite englobante de la région 10 : " << regions[10].bbox() << std::endl;

//       /*
//       for (unsigned i = 0 ; i<nbasins ; ++i)
//       {
        
//       }
//       for (unsigned i = 0 ; i<regions.size() ; ++i)
//         g.add_vertex(regions[]);
//       */
//     }


  /*---------------.
  | Fusion graph.  |
  `---------------*/

  /// The data associated to the vertex of a fusion graph.
  template <typename A>
  struct region_data
  {
    // The subregions composing the region.
    std::set<util::vertex_id> subregions;
    // An accumulator.
    A accumulator;
  };


  /// Build a fusion graph from a RAG.
  template <typename A>
  util::graph< region_data<A> >
  make_fg(const util::graph<A>& rag)
  {
    typedef util::graph<A> region_adjacency_graph;
    typedef util::graph< region_data<A> > fusion_graph;

    fusion_graph fg;

    // Vertices.
    for (util::vertex_id i = 0; i < rag.nvertices(); ++i.to_equiv())
      {
	// The singleton subregion: { I }.
	std::set<util::vertex_id> sr;
	sr.insert(i);
	// The accumulator.
	A a = rag.vertex_data(i);
	// The region data
	region_data<A> data = { sr, a };

	// Add the vertex at index I.
	fg.add_vertex(data);
      }

    // Edges.
    for (typename region_adjacency_graph::edges_t::const_iterator e =
	   rag.edges().begin(); e != rag.edges().end(); ++e)
      {
	util::vertex_id v1 = (*e)->v1();
	util::vertex_id v2 = (*e)->v2();
	fg.add_edge(v1, v2);
      }

    return fg;
  }

  /// Merge regions of the fusion graph.
  template <typename A, typename C>
  util::graph< region_data<A> >
  merge_fg(const util::graph< region_data<A> >& input, const C& criterion)
  {
    typedef util::graph< region_data<A> > fusion_graph;

    // A disjoint set of vertices represented as a forest of rooted
    // trees (disjoint set forest).
    std::vector<util::vertex_id> parent(input.nvertices());
    for (util::vertex_id i = 0; i < parent.size(); ++i.to_equiv())
      // « make_set ».
      parent[i] = i;
    
    // Parent.
    for (typename fusion_graph::edges_t::const_iterator e =
	   input.edges().begin(); e != input.edges().end(); ++e)
      {
	util::vertex_id v1 = (*e)->v1();
	util::vertex_id v2 = (*e)->v2();
	A accu1 = input.vertex_data(v1).accumulator;
	A accu2 = input.vertex_data(v2).accumulator;
	if (criterion(accu1.to_result(), accu2.to_result()))
	  // « union ».
	  parent[v1] = v2;
      }

    // Children.
    typedef std::multimap<util::vertex_id, util::vertex_id> children_t;
    children_t children;
    for (util::vertex_id i = 0; i < parent.size(); ++i.to_equiv())
      {
	// « find ».
	util::vertex_id r = i;
	while (r != parent[r])
	  r = parent[r];
	children.insert(std::make_pair(r, i));
      }

    /// Data associated to merged regions.
    typedef std::map< util::vertex_id, region_data<A> > merged_region_data_t;
    merged_region_data_t merged_region_data;
    for (typename children_t::const_iterator i = children.begin();
	 i != children.end(); ++i)
      {
	util::vertex_id root = i->first;
	util::vertex_id child = i->second;

	region_data<A>& root_data = merged_region_data[root];
	const region_data<A>& child_data = input.vertex_data(child);

	root_data.subregions.insert(child_data.subregions.begin(),
				    child_data.subregions.end());
	root_data.accumulator.take(child_data.accumulator);
      }

    // Fusion graph.
    fusion_graph output;
    std::map<util::vertex_id, util::vertex_id> old_to_new_root;
    // Vertices.
    for (typename merged_region_data_t::const_iterator i =
	   merged_region_data.begin(); i != merged_region_data.end(); ++i)
      {
	util::vertex_id v = output.add_vertex(i->second);
	old_to_new_root[i->first] = v;
      }
    // Edges.
    for (typename fusion_graph::edges_t::const_iterator e =
	   input.edges().begin(); e != input.edges().end(); ++e)
      {
	// The root of the first vertex connected to E.
	util::vertex_id r1 = (*e)->v1();
	while (r1 != parent[r1])
	  r1 = parent[r1];
	// The root of the second vertex connected to E.
	util::vertex_id r2 = (*e)->v2();
	while (r2 != parent[r2])
	  r2 = parent[r2];
	
	if (r1 != r2)
	  {
	    // The regions represented by R1 and R2 are connected.
	    output.add_edge(old_to_new_root[r1], old_to_new_root[r2]);
	  }
      }
    return output;
  }

} // end of namespace my_mln


struct abs_diff_threshold
{
  abs_diff_threshold(int threshold)
    : threshold_(threshold)
  {
  }

  template <typename T>
  bool operator() (const T& a, const T& b) const
  {
    return std::abs(a - b) < threshold_;
  }

private:
  int threshold_;
};



int main()
{
  using namespace mln;
  using namespace my_mln;
  using value::int_u8;
  using value::rgb8;

  image2d<int_u8> irm;
  io::pgm::load(irm, "./irm6.pgm");

//   io::pgm::save( morpho::gradient(irm, win::rectangle2d(3,3)),
//                  "tmp_grad_3x3.pgm" );

  window2d c4p = convert::to_window(c4());
  c4p.insert(0,0);
//   debug::println(convert::to_image(c4p));
  image2d<int_u8> grad = morpho::gradient(irm, c4p);
  io::pgm::save( grad, "tmp_grad_c4p.pgm" );

  image2d<int_u8> clo;
  initialize(clo, irm);

  morpho::closing_area(grad, c4(), 100, clo);
  io::pgm::save( clo, "tmp_clo_a100.pgm" );

  unsigned nbasins;
  image2d<unsigned> wshed = morpho::meyer_wst(clo, c4(), nbasins);
  std::cout << "nbasins = " << nbasins << std::endl;
  io::pgm::save( level::transform(wshed, convert::to_fun(foo)),
                  "tmp_wshed.pgm" );

  //typedef fusion_graph<unsigned int, void> _fg;
  //_fg fg(wshed , irm , c8() , nbasins);
  
//   fusion_graph fg(label&, irm&, c4(), nbasins);
//   std::cout << "nvertices = " << fg.nvertices() << std::endl
//             << "nregions = " << fg.nn() << std::endl;

  // Region adjacency graph (RAG).
  typedef util::graph< accu::mean_<int_u8> > region_adjacency_graph;
  region_adjacency_graph rag = make_rag(wshed, irm, c4(), nbasins);
  
  // Fusion graph (FG).
  typedef util::graph< region_data< accu::mean_<int_u8> > > fusion_graph;
  fusion_graph fg = make_fg(rag);
  fusion_graph work;
  do
    {
      work = fg;
      fg = merge_fg (work, abs_diff_threshold(20));
      std::cerr << "work.nvertices() = " << work.nvertices() << std::endl;
      std::cerr << "fg.nvertices() = " << fg.nvertices() << std::endl;
    }
  while (work.nvertices() != fg.nvertices());

  // Compute the values (means) for the output image.
  std::map<util::vertex_id, int_u8> region_value;
  for (util::vertex_id v = 0; v < fg.nvertices(); ++v.to_equiv())
    {
      // FIXME: Implicit cast.  We should rather change the type of
      // the accumulator instead.
      int_u8 mean = fg.vertex_data(v).accumulator.to_result();
      for (std::set<util::vertex_id>::const_iterator sr =
	     fg.vertex_data(v).subregions.begin();
	   sr != fg.vertex_data(v).subregions.end();
	   ++sr)
	region_value[*sr] = mean;
    }

  // Output.
  image2d<rgb8> output(wshed.domain());
  mln_piter_(image2d<rgb8>) p(output.domain());
  for_all (p)
  {
    if (wshed(p) == 0)
      output(p) = literal::red;
    else
      {
	int_u8 mean = region_value[wshed(p)];
	output(p) = rgb8(mean, mean, mean);
      }
  }
  io::ppm::save(output, "out.ppm");
}
