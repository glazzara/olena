#ifndef MLN_MESH_PSITE_HH
# define MLN_MESH_PSITE_HH


namespace mln
{

  template<typename P> class mesh_p;

  template<typename P>
  struct mesh_psite : public Point_Site< mesh_psite<P> >
  {
    typedef mln_mesh(P) mesh;
    enum { dim = P::dim };
    typedef P point;
    typedef mln_dpoint(P) dpoint;
    typedef mln_coord(P) coord;

    mesh_psite ()
    {
    }

    mesh_psite(unsigned i, mesh_p<P>* m_ptr)
      : i_(i) ,
      m_ptr_(m_ptr)
    {}

    ~mesh_psite()
    {
    }

    P to_point() const
    {
      return m_ptr_->loc_[i_];
    }

    operator P() const
    {
      return m_ptr_->loc_[i_];
    }

    const point* pointer_() const { return 0; }

    coord operator[](unsigned i) const
    {
      return to_point()[i];
    }

    unsigned   i_;
    mesh_p<P>* m_ptr_;
  };

} // end of mln

#endif // MLN_MESH_PSITE_HH
