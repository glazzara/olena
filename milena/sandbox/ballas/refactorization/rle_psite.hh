#ifndef RLE_PSITE_HH_
# define RLE_PSITE_HH_


# include <internal/psite_base.hh>

namespace mln
{

  template <typename P> class rle_pset;

  template <typename P>
  class rle_psite : public internal::psite_base_< rle_psite<P>, P>
  {
  public:

    enum { dim = 2 };

    typedef P site;
    typedef rle_psite<P> psite;
    // FIXME dpsite??

    rle_psite(const rle_pset<P>& pr, unsigned in_run, unsigned of_run);

    P& range_start_();
    const P& range_start_() const;
    unsigned p_of_run() const;
    unsigned& p_of_run();
    unsigned p_in_run() const;
    unsigned& p_in_run();
    const P& to_site() const;
    void update_();

  protected:
    const rle_pset<P>& pr_;

    unsigned p_in_run_;
    unsigned p_of_run_;

    P p_;
  };

  template <typename P>
  inline
  bool operator==(const rle_psite<P>& lhs_, const rle_psite<P>& rhs_);

# ifndef MLN_INCLUDE_ONLY

  template <typename P>
  void
  rle_psite<P>::update_()
  {
    this->p_.operator= (pr_[p_of_run_][p_in_run_]);
  }

  template <typename P>
  inline
  rle_psite<P>::rle_psite(const rle_pset<P>& pr,
			  unsigned in_run,
			  unsigned of_run)
    : pr_(pr),
      p_in_run_(in_run),
      p_of_run_(of_run),
      p_(pr[p_of_run_][p_in_run_])
  {
  }

  template <typename P>
  inline
  const P&
  rle_psite<P>::range_start_() const
  {
    return pr_[p_of_run_].first();
  }

  template <typename P>
  inline
  P&
  rle_psite<P>::range_start_()
  {
    return pr_[p_of_run_].first();
  }

  template <typename P>
  inline
  unsigned
  rle_psite<P>::p_of_run() const
  {
    return p_of_run_;
  }

  template <typename P>
  inline
  unsigned&
  rle_psite<P>::p_of_run()
  {
    return p_of_run_;
  }

  template <typename P>
  inline
  unsigned
  rle_psite<P>::p_in_run() const
  {
    return p_in_run_;
  }

  template <typename P>
  inline
  unsigned&
  rle_psite<P>::p_in_run()
  {
    return p_in_run_;
  }

  template <typename P>
  inline
  const P&
  rle_psite<P>::to_site() const
  {
    return p_;
  }

  template <typename P>
  inline
  bool operator==(const rle_psite<P>& lhs_, const rle_psite<P>& rhs_)
  {
    return lhs_.p_of_run_ == lhs_.p_of_run && lhs_.p_in_run_ == rhs_.p_in_run_;
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln

# include <rle_psite.hh>

#endif // !RLE_PSITE_HH_
