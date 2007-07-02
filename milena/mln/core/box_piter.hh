#ifndef MLN_CORE_BOX_PITER_HH
# define MLN_CORE_BOX_PITER_HH

# include <mln/core/concept/piter.hh>
# include <mln/core/concept/box.hh>


namespace mln
{

  template <typename P>
  class box_fwd_piter_ : public Piter< box_fwd_piter_<P> >
  {
  public:

    enum { dim = P::dim };

    typedef P psite;
    typedef P point;
    typedef mln_coord(P) coord;
    typedef mln_dpoint(P) dpoint;

    box_fwd_piter_(const box_<P>& b)
      : b_(b)
    {
      nop_ = b_.pmax();
      ++nop_[0];
      invalidate();
    }

    operator P() const
    {
      return p_;
    }

    const P* pointer() const
    {
      return & p_;
    }

    coord operator[](unsigned i) const
    {
      assert(i < dim);
      return p_[i];
    }

    bool is_valid() const
    {
      return p_ != nop_;
    }

    void invalidate()
    {
      p_ = nop_;
    }

    void start()
    {
      p_ = b_.pmin();
    }

    void next_()
    {
      for (int i = dim - 1; i >= 0; --i)
	if (p_[i] == b_.pmax()[i])
	  p_[i] = b_.pmin()[i];
	else
	  {
	    ++p_[i];
	    break;
	  }
      if (p_ == b_.pmin())
	p_ = nop_;
    }
    
  private:
    const box_<P>& b_;
    P p_, nop_;
  };



  template <typename P>
  class box_bkd_piter_ : public Piter< box_bkd_piter_<P> >
  {
  public:

    enum { dim = P::dim };

    typedef P psite;
    typedef P point;
    typedef mln_coord(P) coord;

    box_bkd_piter_(const box_<P>& b)
      : b_(b)
    {
      nop_ = b_.pmin();
      --nop_[0];
      invalidate();
    }

    operator P() const
    {
      return p_;
    }

    const P* pointer() const
    {
      return & p_;
    }

    coord operator[](unsigned i) const
    {
      assert(i < dim);
      return p_[i];
    }

    bool is_valid() const
    {
      return p_ != nop_;
    }

    void invalidate()
    {
      p_ = nop_;
    }

    void start()
    {
      p_ = b_.pmax();
    }

    void next_()
    {
      for (int i = dim - 1; i >= 0; --i)
	if (p_[i] == b_.pmin()[i])
	  p_[i] = b_.pmax()[i];
	else
	  {
	    --p_[i];
	    break;
	  }
      if (p_ == b_.pmax())
	p_ = nop_;
    }
    
  private:
    const box_<P>& b_;
    P p_, nop_;
  };


} // end of namespace mln


#endif // ! MLN_CORE_BOX_PITER_HH
