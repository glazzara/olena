#ifndef MLN_MESH_P_HH
# define MLN_MESH_P_HH

#include "graph.hh"

namespace mln
{

  template<typename P>
  class Mesh_p
  {
  public:
    Mesh_p () {}
    
    Mesh_p (util::Graph<void>&	gr,
	    std::vector<P>&	loc)
      : gr_ (gr),
	loc_ (loc)
    {
    }
    
    ~Mesh_p () {}
    
    util::Graph<void>	gr_;
    std::vector<P>	loc_;
    //      Mesh_psite<P>	psite;
  };
} // end of mln

#endif // MLN_MESH_P_HH
