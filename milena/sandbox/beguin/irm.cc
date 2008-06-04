#include <iterator>
#include <iostream>
#include <algorithm>
#include <map>

#include <mln/core/image2d.hh>

#include <mln/core/neighb2d.hh>
#include <mln/core/window2d.hh>
#include <mln/core/image_if.hh>
#include <mln/win/rectangle2d.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>

#include <mln/value/int_u8.hh>
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

//#include <segm_to_pregraph.hh>
#include <fusion_graph2.hh>

mln::value::int_u8 foo(unsigned u)
{
  return u == 0 ?
    0 : // wshed line
    1 + (u - 1) % 255; // basin
}


namespace mln

{
  
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
//     // Nodes.
//     for (unsigned i = 0; i <= nlabels; ++i)
//     {
//       accu::mean_<unsigned> m;
//       level::take(irm | (pw::value(lbl) == pw::cst(i)), m);
//       g.add_node(m.to_result());
//     }
//     // Edges.
//     for (unsigned l1 = 1; l1 <= nlabels; ++l1)
//       for (unsigned l2 = l1 + 1; l2 <= nlabels; ++l2)
// 	if (adja[l1][l2])
// 	  g.add_edge(l1, l2);
//     //g.print_debug (std::cout);
//     for( int i = 0 ; i < g.nnodes() ; ++i)
//       std::cout << "[" << i << " : " << g.node_data(i) << " ] ";
//     std::cout << std::endl;
//   }

  /// Build a region adjacency graph.
  template <typename I, typename J, typename N>
  util::graph< accu::mean_<mln_value(J)> >
  make_rag(const I& label, const J& input, N nbh, unsigned nlabels)
      {
        std::vector< std::vector<bool> > adja(nlabels + 1);
        for (unsigned l = 0; l <= nlabels; ++l)
          adja[l].resize(nlabels + 1, false);

        mln_piter(I) p(lbl.domain());
        mln_niter(N) n(nbh, p);

    // We compute the adjacency matrix of the RAG.
        for_all(p)
          if (lbl(p) == 0) // wshed
        {
          std::set<unsigned> s;
          for_all(n) if (lbl.has(n))
          {
            if (lbl(n) == 0)
              continue;
            s.insert(lbl(n));
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
    // Nodes.
        for (unsigned i = 0; i <= nlabels; ++i)
        {
          accu::mean_<mln_value(J)> m;
          level::take(irm | (pw::value(lbl) == pw::cst(i)), m);
          g.add_node(m);
        }
    // Edges.
        for (unsigned l1 = 1; l1 <= nlabels; ++l1)
          for (unsigned l2 = l1 + 1; l2 <= nlabels; ++l2)
            if (adja[l1][l2])
              g.add_edge(l1, l2);
    //g.print_debug (std::cout);
        for( int i = 0 ; i < g.nnodes() ; ++i)
          std::cout << "[" << i << " : " << g.node_data(i) << " ] ";
        std::cout << std::endl;
        return g;
      }

  template <typename I, typename J, typename N>
  void mk2_graph(const I& lbl , const J& irm , const N&
                 nbh, int nbasins)
    {
      typedef mln::p_set<mln_point(I)> R;
      std::map<int, R > regions;
      
      mln_piter(I) p(lbl.domain());
      for_all(p)
      {
        if (unsigned int i = lbl(p))
          regions[i].insert(p);
      }

      std::cout << "Boite englobante de la région 10 : " << regions[10].bbox() << std::endl;

      /*
      for (unsigned i = 0 ; i<nbasins ; ++i)
      {
        
      }
      for (unsigned i = 0 ; i<regions.size() ; ++i)
        g.add_node(regions[]);
      */
    }
   
  
} // end of namespace mln

template <typename A>
struct region_data
{
  // The subregions composing the region.
  std::set<util::vertex_id> subregions;
  // An accumulator.
  A a;
}

int main()
{
  using namespace mln;
  using value::int_u8;

  image2d<int_u8> irm;
  io::pgm::load(irm, "./+irm6.pgm");

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
  
//   fusion_graph fg(lbl&, irm&, c4(), nbasins);
//   std::cout << "nnodes = " << fg.nnodes() << std::endl
//             << "nregions = " << fg.nn() << std::endl;

  // Region adjacency graph.
  util::graph< accu::mean_<int_u8> > rag = make_rag(wshed, irm, c4(), nbasins);
  
  // Fusion graph.
  
  typedef util::graph< region_data< accu::mean<int_u8> > fusion_graph;
  
  fusion_graph output;
  fusion_graph work;
  do
    work = output;
    output = merge(work);
  while (work.nvertices() != output.nvertices());

}
