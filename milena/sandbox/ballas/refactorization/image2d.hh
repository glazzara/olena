#ifndef IMAGE2D_HH_
# define IMAGE2D_HH_

# include <internal/image_base.hh>

# include <box2d.hh>

namespace mln
{

  template <typename T>
  class image2d : public internal::image_base_< box2d<int>, image2d<T> >
  {
  public:

    typedef box2d<int> pset;
    typedef pset::fwd_piter fwd_piter;
    typedef pset::bkd_piter bkd_piter;



    typedef T         value;
    typedef const T& rvalue;
    typedef T&       lvalue;


    image2d(int nrows, int ncols);
    image2d(const box2d<int>& b);

    ///typedef mln::value::set<T> vset;
    //  const vset& values() const;
    bool is_ready() const;

    bool owns_(const point2d<int>& p) const;
    const box2d<int>& domain() const;

    const T& operator()(const point2d<int>& p) const;
    T& operator()(const point2d<int>& p);


  private:
    T  value_;
    box2d<int> b_;
    bool is_ready_;
  };

# ifndef MLN_INCLUDE_ONLY

  template <typename T>
  image2d<T>::image2d(int nrows, int ncols) :
    b_(nrows, ncols),
    is_ready_(false)
  {
  }

  template <typename T>
  image2d<T>::image2d(const box2d<int>& b) :
    b_(b),
    is_ready_(false)
  {
  }

  template <typename T>
  bool
  image2d<T>::is_ready() const
  {
    return is_ready_;
  }

  template <typename T>
  const T&
  image2d<T>::operator()(const point2d<int>&) const
  {
    return value_;
  }

  template <typename T>
  T&
  image2d<T>::operator()(const point2d<int>&)
  {
    is_ready_ = true;
    return value_;
  }

  template <typename T>
  const box2d<int>&
  image2d<T>::domain() const
  {
    return this->b_;
  }

# endif // ! MLN_INCLUDE_ONLY

}


#endif // ! IMAGE2D_HH_
