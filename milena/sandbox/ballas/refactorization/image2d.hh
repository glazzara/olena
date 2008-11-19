#ifndef IMAGE2D_HH_
# define IMAGE2D_HH_

# include <internal/image_base.hh>

# include <box2d.hh>

namespace mln
{

  template <typename T>
  class image2d : public internal::image_base< box2d<int>, image2d<T> >
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
    ~image2d();

    ///typedef mln::value::set<T> vset;
    //  const vset& values() const;
    bool is_ready() const;

    bool has(const point2d<int>& p) const;
    const box2d<int>& domain() const;

    const T& operator()(const point2d<int>& p) const;
    T& operator()(const point2d<int>& p);


  private:
    T*  values_;
    box2d<int> b_;
    bool is_ready_;
  };

# ifndef MLN_INCLUDE_ONLY

  template <typename T>
  image2d<T>::image2d(int nrows, int ncols) :
    b_(nrows, ncols),
    is_ready_(false)
  {
    values_ = new T[nrows * ncols];
    is_ready_ = true;
  }

  template <typename T>
  image2d<T>::image2d(const box2d<int>& b) :
    b_(b),
    is_ready_(false)
  {
    values_ = new T[(b.pmax()[0] - b.pmin()[0] + 1) *
		    (b.pmax()[1] - b.pmin()[1] + 1)];
    is_ready_ = true;
  }

  template <typename T>
  image2d<T>::~image2d()
  {
    delete[] values_;
  }

  template <typename T>
  bool
  image2d<T>::is_ready() const
  {
    return is_ready_;
  }

  template <typename T>
  const T&
  image2d<T>::operator()(const point2d<int>& p) const
  {
    return values_[p[0] * b_.pmax()[1] + p[1]];
  }

  template <typename T>
  T&
  image2d<T>::operator()(const point2d<int>& p)
  {
    return values_[p[0] * b_.pmax()[1] + p[1]];
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
