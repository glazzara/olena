#ifndef MLN_MESH_P_HH
# define MLN_MESH_P_HH

#include "graph.hh"

namespace mln
{

  template<typename P>
  class mesh_p
  {
  public:
    mesh_p () {}
    
    mesh_p (util::graph<void>&	gr,
	    std::vector<P>&	loc)
      : gr_ (gr),
	loc_ (loc)
    {
    }
    
    ~mesh_p () {}
                                                
    util::graph<void>	gr_;
    std::vector<P>	loc_;
  };
} // end of mln

#endif // MLN_MESH_P_HH
