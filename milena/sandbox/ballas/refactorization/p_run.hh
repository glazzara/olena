#ifndef P_RUN_HH_
# define P_RUN_HH_

# include <cstddef>
# include <iostream>
# include <internal/pset_base.hh>
# include <internal/piter_base.hh>

namespace mln
{
  // Fwd decls.
  template <typename P> struct p_run_fwd_piter_;

  template <typename P>
  class p_run :
    public internal::pset_base_< P, p_run<P> >
  {
  public:
    typedef P site;
    typedef P psite;

    typedef p_run_fwd_piter_<P> fwd_piter;
    typedef p_run_fwd_piter_<P> bkd_piter;
    typedef fwd_piter piter;

    p_run();
    p_run(const P& start, std::size_t len);
    void set_run(const P& start, std::size_t len);

    bool owns_(const P& p) const;
    std::size_t nsites() const;
    std::size_t length() const;

    P operator[](unsigned i) const;

    const P& first() const;

  protected:
    P p_;
    std::size_t len_;
    bool is_valid_;
  };

  template <typename P>
  std::ostream& operator<<(std::ostream& out, const p_run<P>& pr);

# ifndef MLN_INCLUDE_ONLY

  template <typename P>
  inline
  p_run<P>::p_run()
  {
    is_valid_ = false;
  }

  template <typename P>
  inline
  p_run<P>::p_run(const P& start, std::size_t len)
    : p_(start),
      len_(len)
  {
    assert(len != 0);
    P p = start;
//     bb_.init();
//     bb_.take(p);
//     p[P::dim - 1] += len - 1;
//     bb_.take(p);
    is_valid_ = true;
  }

  template <typename P>
  inline
  void
  p_run<P>::set_run(const P& start, std::size_t len)
  {
    assert(len != 0);
    p_ = start;
    len_ = len;
    P p = start;
//     bb_.init();
//     bb_.take(p);
//     p[P::dim - 1] += len - 1;
//     bb_.take(p);
    is_valid_ = true;
  }

  template <typename P>
  inline
  bool
  p_run<P>::owns_(const P& p) const
  {
    assert(is_valid_);
    bool res = true;
    for (int i = P::dim - 2; i >= 0; --i)
      if (!(res = (res && p[i] == p_[i])))
	return false;
    return (p[P::dim - 1] >= p_[P::dim - 1]
	    && p[P::dim - 1] < p_[P::dim - 1] + (signed)len_);
  }

  template <typename P>
  inline
  std::size_t
  p_run<P>::nsites() const
  {
    assert(is_valid_);
    return len_;
  }

  template <typename P>
  inline
  std::size_t
  p_run<P>::length() const
  {
    assert(is_valid_);
    return len_;
  }

  template <typename P>
  inline
  P
  p_run<P>::operator[](unsigned i) const
  {
    assert(is_valid_);
    assert(i < nsites());
    P p = p_;
    p[P::dim - 1] += i;
    return p;
  }

  template <typename P>
  inline
  const P&
  p_run<P>::first() const
  {
    return p_;
  }


  template <typename P>
  std::ostream&
  operator<<(std::ostream& out, const p_run<P>& pr)
  {
    out << "Run: (" << pr.first() << ", " << pr.length() << ")";
    return out;
  }

# endif // ! MLN_INCLUDE_ONLY


  /// Iterators

  template <typename P>
  struct p_run_fwd_piter_ :
    public internal::piter_base_< p_run_fwd_piter_<P>, P, p_run<P> >
  {
    typedef p_run_fwd_piter_<P> self_;
    typedef internal::piter_base_< p_run_fwd_piter_<P>, P, p_run<P> > super_;
  public:

    typedef P site;
    typedef p_run<P> psite;

    enum { dim = P::dim };

    p_run_fwd_piter_();
    p_run_fwd_piter_(const p_run<P>& pr);
    void assign_run(const p_run<P>& pr);

    const site& to_site() const;
    const psite& to_psite() const;

    /// operator[] ?

    bool is_valid() const;
    void invalidate();
    void start();
    void next_();

    /// Get the index of the point in the run.
    unsigned ind() const;


  protected:
    const p_run<P>* run_;
    bool is_valid_;
    unsigned i_;
    P p_;
  };


# ifndef MLN_INCLUDE_ONLY

  template <typename P>
  inline
  p_run_fwd_piter_<P>::p_run_fwd_piter_()
    : run_ (0),
      is_valid_(false)
  {
  }

  template <typename P>
  inline
  p_run_fwd_piter_<P>::p_run_fwd_piter_(const p_run<P>& pr)
    : run_(&pr),
      is_valid_(false)
  {
  }

  template <typename P>
  inline
  void
  p_run_fwd_piter_<P>::assign_run(const p_run<P>& pr)
  {
    run_ = &pr;
    is_valid_ = false;
  }

  template <typename P>
  inline
  const typename p_run_fwd_piter_<P>::site&
  p_run_fwd_piter_<P>::to_site() const
  {
    assert(is_valid());
    return p_;
  }

  template <typename P>
  inline
  const typename p_run_fwd_piter_<P>::psite&
  p_run_fwd_piter_<P>::to_psite() const
  {
    assert(is_valid());
    return *run_;
  }

  template <typename P>
  inline
  bool
  p_run_fwd_piter_<P>::is_valid() const
  {
    return is_valid_;
  }

  template <typename P>
  inline
  void
  p_run_fwd_piter_<P>::invalidate()
  {
    is_valid_ = false;
  }

  template <typename P>
  inline
  void
  p_run_fwd_piter_<P>::start()
  {
    p_ = run_->first();
    i_ = 0;
    is_valid_ = true;
  }

  template <typename P>
  inline
  void
  p_run_fwd_piter_<P>::next_()
  {
    p_[dim - 1]++;
    ++i_;
    is_valid_ = p_[dim - 1] - run_->first()[dim - 1] < (signed)run_->length();
  }

  template <typename P>
  inline
  unsigned
  p_run_fwd_piter_<P>::ind() const
  {
    return i_;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // !P_RUN_HH_
