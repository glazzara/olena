#ifndef RLE_IMAGE_CC_
# define RLE_IMAGE_CC_


# include <rle_pset.hh>
# include <internal/image_base.hh>

namespace mln
{

  // note P must be a point
  template <typename P, typename T>
  class rle_image :
    public internal::image_base<rle_pset<P>, rle_image<P, T> >
  {
  public:

    typedef T value;
    typedef T& lvalue;
    typedef const T rvalue;

    typedef P site;
    typedef rle_psite<P> psite;

    typedef rle_pset<P> pset;
    typedef typename pset::fwd_piter fwd_piter;
    typedef typename pset::bkd_piter bkd_piter;
    typedef typename pset::fwd_piter piter;


    rle_image();

    void insert(const p_run<P>& pr, T value);
    rvalue operator() (const rle_psite<P>& psite) const;
    lvalue operator() (const rle_psite<P>& psite);
    bool is_valid() const;

    bool has(const rle_psite<P>& ps) const;
    const pset& domain() const;

  protected:
    std::vector<T> values_;
    rle_pset<P> domain_;
  };


# ifndef MLN_INCLUDE_ONLY

  template <typename P, typename T>
  inline
  rle_image<P, T>::rle_image()
  {
  }

  template <typename P, typename T>
  inline
  bool
  rle_image<P, T>::is_valid() const
  {
    return this->values_.size() != 0;
  }

  template <typename P, typename T>
  inline
  void
  rle_image<P, T>::insert(const p_run<P>& pr, T value)
  {
    this->domain_.insert(pr);
    this->values_.push_back(value);
  }

  template <typename P, typename T>
  inline
  typename rle_image<P, T>::rvalue
  rle_image<P, T>::operator() (const rle_psite<P>& psite)
    const
  {
    assert(this->has(psite));
    return this->values_[psite.p_of_run()];
  }

  template <typename P, typename T>
  inline
  typename rle_image<P, T>::lvalue
  rle_image<P, T>::operator() (const rle_psite<P>& psite)
  {
    assert(this->has(psite));
    return this->values_[psite.p_of_run()];
  }

  template <typename P, typename T>
  inline
  const typename rle_image<P, T>::pset&
  rle_image<P, T>::domain() const
  {
    return this->domain_;
  }

  template <typename P, typename T>
  inline
  bool
  rle_image<P, T>::has(const rle_psite<P>& ps) const
  {
    if (!this->is_valid())
      return false;
    else
      return (ps.p_of_run() < this->domain_.nruns() &&
	      ps.p_in_run() < this->domain_[ps.p_of_run()].length() &&
	      this->domain_[ps.p_of_run()][ps.p_in_run()] == ps.to_site());
    /// FIXME -> to_site....
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // !RLE_IMAGE_CC_
