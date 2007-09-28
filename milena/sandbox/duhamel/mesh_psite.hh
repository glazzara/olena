#ifndef MLN_MESH_PSITE_HH
# define MLN_MESH_PSITE_HH

# include "mesh_p.hh"

namespace mln
{
  template<typename P>
  class mesh_psite
  {
  public:
    mesh_psite () {}
    mesh_psite (unsigned i// , mesh_p<P> * m_ptr
		) :
    i_ (i)// ,
//     m_ptr_ (m_ptr)
    {}
    ~mesh_psite()	{}
    //    P to_point ()	{return m_ptr_->loc_[i_];}
  private:
    unsigned		i_;
    //    mesh_p<P>*		m_ptr_;
  };
} // end of mln
#endif // MLN_MESH_PSITE_HH
