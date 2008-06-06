# include <mln/core/concept/neighborhood.hh>


namespace mln
{


  // Fwd decl.
  template <typename D, typename F>  class dbl_niter_;


  template <typename D, typename F>
  struct dbl_neighb_ : public Neighborhood< dbl_neighb_<D,F> >
  {
    typedef D dpoint;
    typedef mln_point(D) point;

    typedef dbl_niter_<D,F> fwd_niter;
    typedef dbl_niter_<D,F> bkd_niter;
    typedef dbl_niter_<D,F> niter;

    dbl_neighb_(F f)
      : f(f)
    {
    }

    dbl_neighb_()
      : f()
    {
    }

    bool is_empty() const
    {
      return trues.size() == 0 && falses.size() == 0;
    }

    dbl_neighb_& insert_true(const D& dp)
    {
      trues.push_back(dp);
      trues.push_back(-dp);
      return *this;
    }

    dbl_neighb_& insert_false(const D& dp)
    {
      falses.push_back(dp);
      falses.push_back(-dp);
      return *this;
    }

    template <typename N>
    dbl_neighb_& when_true(const Neighborhood<N>& nbh_)
    {
      const N& nbh = exact(nbh_);
      for (unsigned i = 0; i < nbh.ndpoints(); ++i)
	trues.push_back(nbh.dp(i));
      return *this;
    }

    template <typename N>
    dbl_neighb_& when_false(const Neighborhood<N>& nbh_)
    {
      const N& nbh = exact(nbh_);
      for (unsigned i = 0; i < nbh.ndpoints(); ++i)
	falses.push_back(nbh.dp(i));
      return *this;
    }

    std::vector<D> trues, falses;
    F f;
  };



  template <typename D, typename F>
  class dbl_niter_ 
    : public internal::point_iterator_base_< mln_point(D), dbl_niter_<D,F> >
  {
  public:

    typedef mln_point(D) point;

    template <typename Pref>
    dbl_niter_(const Neighborhood< dbl_neighb_<D,F> >& nbh,
	       const Point_Site<Pref>& p_ref) :
      dps_true(exact(nbh).trues),
      dps_false(exact(nbh).falses),
      p_ref_(exact(p_ref).to_point()),
      f(exact(nbh).f)
    {
      invalidate();
    }
      
    /// Convertion to point.
    operator point () const
    {
      mln_precondition(is_valid());
      return p_;
    }

    /// Reference to the corresponding point.
    const point& to_point() const
    {
      return p_;
    }

    /// Test the iterator validity.
    bool is_valid() const
    {
      return i_ != dps_true.size();
    }

    /// Invalidate the iterator.
    void invalidate()
    {
      i_ = dps_true.size();
    }

    /// Start an iteration.
    void start()
    {
      i_ = 0;
      update();
    }

    /// Go to the next point.
    void next_()
    {
      ++i_;
      update();
    }

    /// Give the i-th coordinate.
    int operator[](unsigned i) const
    {
      mln_precondition(is_valid());
      return p_[i];
    }

    /// The point around which this iterator moves.
    const point& center_point() const
    {
      return p_ref_;
    }

    /// Force this iterator to update its location to take into
    /// account that its center point may have moved.
    void update()
    {
      if (is_valid())
	p_ = p_ref_ + (f(p_ref_) ? dps_true[i_] : dps_false[i_]);
    }

  protected:

    const std::vector<D>& dps_true, dps_false;
    const point& p_ref_; // reference point (or "center point")
    F f;

    unsigned i_;
    point p_; // location of this iterator; p_ makes this iterator be
    // itself a potential center point.
  };



} // mln
