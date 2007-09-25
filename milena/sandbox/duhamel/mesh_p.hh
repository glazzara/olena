#ifndef MLN_MESH_P_HH
# define MLN_MESH_P_HH

#include "graph.hh"

namespace mln
{

  namespace util
  {

    template<typename P>
    class Mesh_p
    {
    public:
      Mesh_p ()
      {
      }
      Mesh_p (Graph<void>&	gr)
	: gr_ (gr)
      {
      }

      ~Mesh_p () {}

      std::vector<P>& loc_get ()
      {
	return loc_;
      }

    private:
      Graph<void>	gr_;
      std::vector<P>	loc_;
      //      Mesh_psite<P>	psite;
    };
  } // end of util
} // end of mln

#endif // MLN_MESH_P_HH
