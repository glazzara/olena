/*#include <mln/util/graph.hh>
#include <mln/core/exact.hh>

#include <iterator>
#include <iostream>
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

#include <mln/debug/println.hh>*/

#ifndef FUSION_GRAPH_H
#define FUSION_GRAPH_H



namespace mln
{
  
  template <typename I, typename N>
  class fusion_graph
  {
    
    typedef mln::p_set<mln_point(I)> R;
    
    util::graph<int> g;
    
    std::map<int, R> regions;
    fusion_graph(I&);
    public :
      void fusion(unsigned int key1, unsigned int key2);
    //Constructors
      fusion_graph(const image2d<unsigned>&, const I&, const N&, unsigned);
  }
  
  
  
  // Constructor
  template<typename I, typename N>
  fusion_graph::fusion_graph(image2d<unsigned>& lbl, I& ima, N& neighb, unsigned nlabels)
  {
    mln_piter(I) p(lbl.domain());
    mln_niter(N) n(nbh, p);
    
    std::vector< std::vector<bool> > adja(nlabels + 1);
    for (unsigned l = 0; l <= nlabels; ++l)
      adja[l].resize(nlabels + 1, false);
    
    for_all(p)
      this->regions[lbl(p)].insert(p);
    
    //Adjacency Matrix Computing
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
    
    for(int i = 0 ; i < nlabels ; ++i)
      g.add_node(i);
    for (unsigned l1 = 1; l1 <= nlabels; ++l1)
      for (unsigned l2 = l1 + 1; l2 <= nlabels; ++l2)
        if (adja[l1][l2])
          g.add_edge(l1, l2);

  }
}
#endif
