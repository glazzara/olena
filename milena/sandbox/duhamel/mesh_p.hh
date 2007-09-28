#ifndef MLN_MESH_P_HH
# define MLN_MESH_P_HH

# include <mln/core/concept/point.hh>
# include <mln/core/internal/point_set_base.hh>
# include <mln/core/internal/point_iterator_base.hh>
# include <mln/accu/bbox.hh>
# include "graph.hh"
# include "mesh_psite.hh"

namespace mln
{

  template<typename P> class mesh_p_piter_;


  template<typename P>
  class mesh_p : public internal::point_set_base_< P, mesh_p<P> >
  {
  public:
    mesh_p () {}
    
    mesh_p (util::graph<void>&	gr,
	    std::vector<P>&	loc)
      : gr_ (gr),
	loc_ (loc)
    {
      accu::bbox<P> a;
      for (unsigned i = 0; i < loc.size(); ++i)
	a.take(loc[i]);
      bb_ = a.to_result();
    }

    /// Point_Site associated type.
    typedef mesh_psite<P> psite;
    
    //FIXME
    /// Forward Point_Iterator associated type.
    typedef mesh_p_piter_<P> fwd_piter;

    /// Backward Point_Iterator associated type.
    typedef mesh_p_piter_<P> bkd_piter;
    //END FIXME

    std::size_t npoints() const
    {
      return this->gr_.nb_node_;
    }

    /// Give the exact bounding box.
    const box_<P>& bbox() const
    {
      return bb_;
    }

    bool has(const psite& p) const
    {
      for (unsigned i = 0; i < loc_.size(); ++i)
	if (loc_[i] == p)
	  return true;
      return false;
    }
                                                
    util::graph<void>	gr_;
    std::vector<P>	loc_;
    box_<P> bb_;
  };



  template<typename P>
  class mesh_p_piter_ : public internal::point_iterator_base_< P, mesh_p_piter_<P> >
  {
    typedef mesh_p_piter_<P> self_;
    typedef internal::point_iterator_base_< P, self_ > super_;
  public:
    
    // Make definitions from super class available.
    enum { dim = super_::dim };

    mesh_p_piter_(const mesh_p<P>& s)
      : loc_(s.loc_)
    {
      invalidate();
    }

    /// Give a hook to the point address.
    const P* pointer_() const
    {
      return & p_;
    }

    /// Read-only access to the \p i-th coordinate.
    mln_coord(P) operator[](unsigned i) const
    {
      return p_[i];
    }

    /// Test if the iterator is valid.
    bool is_valid() const
    {
      return i_ != loc_.size();
    }

    /// Invalidate the iterator.
    void invalidate()
    {
      i_ = loc_.size();
    }

    /// Start an iteration.
    void start()
    {
      i_ = 0;
      if (is_valid())
	p_ = loc_[i_];
    }

    /// Go to the next point.
    void next_()
    {
      ++i_;
      if (is_valid())
	p_ = loc_[i_];
    }

    /// Convert the iterator into a point.
    operator P() const
    {
      mln_precondition(is_valid());
      return p_;
    }

  protected:
    const std::vector<P>& loc_;
    unsigned i_;
    P p_;
  };



} // end of mln


#endif // MLN_MESH_P_HH
