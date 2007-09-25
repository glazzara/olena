#ifndef MLN_MESH_PSITE_HH
# define MLN_MESH_PSITE_HH

# include "mesh_p.hh"

namespace mln
{
  template<typename P>
  class Mesh_psite
  {
  public:
    Mesh_psite()	{}
    ~Mesh_psite()	{}
    P to_point ()	{return m_ptr->loc_[i];}
  private:
    unsigned		i;
    Mesh_p<P>*	m_ptr;
  };
} // end of mln
#endif // MLN_MESH_PSITE_HH
