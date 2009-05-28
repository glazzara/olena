
#ifndef FUSION_GRAPH_2D_
#define FUSION_GRAPH_2D_

#include <mk_graph.hh>
#include <mln/util/graph.hh>

#include <mln/value/int_u8.hh>

#include <mln/core/image/image2d.hh>

#include <mln/core/alias/neighb2d.hh>
#include <mln/core/alias/window2d.hh>
#include <mln/core/image/dmorph/image_if.hh>
#include <mln/win/rectangle2d.hh>

//#include "map.hh"


//template <typename I, typename N>

using namespace mln;

class fusion_graph_2d
{
  typedef mln::p_set<point2d> R;
  util::graph<unsigned int, void> g;
  std::map<int, R> region_map;
  image2d<value::int_u8> ima;
  image2d<unsigned int> lbl;
  
  public:
    // Constructeur
    fusion_graph_2d(const image2d<unsigned>& lbl, const image2d<value::int_u8>& ima, neighb2d nbh, int nlabels)
    {
      // "Recopie" des données en interne
      this->ima = ima;
      this->lbl = lbl;
      
      // Création du graphe de régions
      g = mk_graph(lbl, ima, nbh, nlabels);
      
      // Création table de correspondance noeud/région
      mln_niter(N) n(nbh, p);
      for_all(p)
        this->region_map[lbl(p)].insert(p);
    }
    
    unsigned int nnodes()
    {
      return(g.nnodes);
    }
    unsigned int nn()
    {
      return(region_map.size();
    }
}


#endif
