#include <mln/core/graph_image.hh>
#include <mln/core/graph_elt_neighborhood.hh>
#include <mln/core/p_set.hh>
#include <mln/core/point3d.hh>
#include <mln/debug/println.hh>

using namespace mln;

template <typename P>
    int nb_composant_connexe(graph_image<P, bool> gi, bool object, const P& p_ref, bool local)
{
  int T;
  p_set< P > done;
  p_set< P > neighbors;
  p_set< P > composant;
  
  done.insert(p_ref);
  graph_elt_neighborhood< P > nbh;
  mln_niter(graph_elt_neighborhood< P >) fq(nbh, p_ref);
  for_all(fq)
  {
    if (gi(fq)&&!done.has(fq))
    {
      neighbors.insert(fq);
    }
  }
//   std::cout << "nb_composant_connexe: neighbors " << neighbors.npoints() <<std::endl; 
  if(neighbors.npoints()<=1)
  {
    return neighbors.npoints();
  }
  else
    T=0;
    
  while(neighbors.npoints()!=0)
  {
    T++;
    done.insert(neighbors[0]);
    mln_niter(graph_elt_neighborhood< P >) t(nbh, neighbors[0]);
    neighbors.remove(neighbors[0]);
    for_all(t)
    {
      if (gi(t)&&!done.has(t))
      {
        composant.insert(t);
      }
    }
     
    while(composant.npoints()!=0)
    {
      done.insert(composant[0]);
      if(neighbors.has(composant[0]))
      {
        neighbors.remove(composant[0]);
        if(neighbors.npoints()==0)
          return T;
      }
      
      mln_niter(graph_elt_neighborhood< P >) r(nbh, composant[0]);
      composant.remove(composant[0]);
      for_all(r)
      {
        if (gi(r) && !done.has(r))
        {
          composant.insert(r);
        }
      }
    }
  }
  return T;
}

int main()
{
  util::graph<point3d> g;
  
  for(int i=0; i<3; i++)
  {
    for(int j=0; j<3; j++)
    {
      for(int k=0; k<3; k++)
      {
        g.add_vertex(make::point3d(i,j,k));
      }
    }
  }
  
  g.add_edge(0,1);
  g.add_edge(0,3);
  g.add_edge(0,9);
  g.add_edge(1,2);
  g.add_edge(1,4);
  g.add_edge(1,10);
  g.add_edge(2,5);
  g.add_edge(2,11);
  g.add_edge(3,4);
  g.add_edge(3,6);
  g.add_edge(3,12);
  g.add_edge(4,5);
  g.add_edge(4,7);
  g.add_edge(4,13);
  g.add_edge(5,8);
  g.add_edge(5,14);
  g.add_edge(6,7);
  g.add_edge(6,15);
  g.add_edge(7,8);
  g.add_edge(7,16);
  g.add_edge(8,17);
  g.add_edge(9,10);
  g.add_edge(9,12);
  g.add_edge(9,18);
  g.add_edge(10,11);
  g.add_edge(10,13);
  g.add_edge(10,19);
  g.add_edge(11,14);
  g.add_edge(11,20);
  g.add_edge(12,13);
  g.add_edge(12,15);
  g.add_edge(12,21);
  g.add_edge(13,14);
  g.add_edge(13,22);
  g.add_edge(14,17);
  g.add_edge(14,23);
  g.add_edge(15,16);
  g.add_edge(15,24);
  g.add_edge(16,17);
  g.add_edge(16,25);
  g.add_edge(17,26);
  g.add_edge(18,19);
  g.add_edge(18,21);
  g.add_edge(19,20);
  g.add_edge(19,22);
  g.add_edge(20,23);
  g.add_edge(21,22);
  g.add_edge(21,24);
  g.add_edge(22,23);
  g.add_edge(22,25);
  g.add_edge(23,26);
  g.add_edge(24,25);
  g.add_edge(25,26);
  
  g.print_debug(std::cout);
  
  p_graph<point3d> pg(g);
  
  std::vector<bool> val(27,true);
  
  typedef graph_image<point3d, bool> gi_t;
  gi_t gi(pg, val);
  
  mln_piter_(gi_t) p_ref(gi.domain());
  for_all(p_ref)
  {
    int T;
    p_set< point3d > done;
    p_set< point3d > neighbors;
    p_set< point3d > composant;
  
    done.insert(p_ref.to_point());
    graph_elt_neighborhood< point3d > nbh;
    mln_niter_(graph_elt_neighborhood< point3d >) fq(nbh, p_ref);
    for_all(fq)
    {
      if (gi(fq)&&!done.has(fq.to_point()))
      {
        neighbors.insert(fq.to_point());
      }
    }
//   std::cout << "nb_composant_connexe: neighbors " << neighbors.npoints() <<std::endl; 
    if(neighbors.npoints()<=1)
    {
      return neighbors.npoints();
    }
    else
      T=0;
    
    while(neighbors.npoints()!=0)
    {
      T++;
      done.insert(neighbors[0]);
      mln_niter_(graph_elt_neighborhood< point3d >) t(nbh, neighbors[0]);
      neighbors.remove(neighbors[0]);
      for_all(t)
      {
        if (gi(t)&&!done.has(t.to_point()))
        {
          composant.insert(t.to_point());
        }
      }
     
      while(composant.npoints()!=0)
      {
        done.insert(composant[0]);
        if(neighbors.has(composant[0]))
        {
          neighbors.remove(composant[0]);
          if(neighbors.npoints()==0)
            return T;
        }
      
        mln_niter_(graph_elt_neighborhood< point3d >) r(nbh, composant[0]);
        composant.remove(composant[0]);
        for_all(r)
        {
          if (gi(r) && !done.has(r.to_point()))
          {
            composant.insert(r.to_point());
          }
        }
      }
    }
    std::cout << "gi (" << p_ref << ") = " << T << std::endl;
  }
}
