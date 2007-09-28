#ifndef MLN_MESH_P_HH
# define MLN_MESH_P_HH

# include <mln/core/concept/point.hh>
# include "graph.hh"
# include "mesh_psite.hh"

namespace mln
{

  template<typename P>
  class mesh_p// : public Point_Set<mesh_p<P> >
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

    /// Mesh associated type.
    typedef mln_mesh(P) mesh;

    /// Point_Site associated type.
    typedef mesh_psite<P> psite;

    /// Point associated type.
    typedef P point;

    /// Dpoint associated type.
    typedef mln_dpoint(P) dpoint;
    
    //FIXME
    /// Forward Point_Iterator associated type.
    typedef P fwd_piter;

    /// Backward Point_Iterator associated type.
    typedef P bkd_piter;
    //END FIXME

    std::size_t npoints() const
    {
      return this->gr_.nb_node_;
    }

//     bool has(const P& p) const
//     {
//       typename std::vector<P>::const_iterator it = this->loc_.begin ();

//       for (; it != this->loc_.begin () ; ++it)
// 	if (*it == p)
// 	  return true;
//       return false;
//     }
                                                
    util::graph<void>	gr_;
    std::vector<P>	loc_;
  };
} // end of mln

#endif // MLN_MESH_P_HH
