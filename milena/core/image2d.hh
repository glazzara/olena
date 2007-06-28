#ifndef MLN_CORE_IMAGE2D_HH
# define MLN_CORE_IMAGE2D_HH

# include <core/internal/image_base.hh>
# include <core/box2d.hh>


namespace mln
{

  template <typename T>
  struct image2d : public internal::image_base_< box2d, image2d<T> >
  {
    typedef T         value;
    typedef const T& rvalue;
    typedef T&       lvalue;

    image2d();
    image2d(int nrows, int ncols);
    image2d(const box2d& b);

    bool has_data() const;
    const box2d& domain() const;

    const T& operator()(const point2d& p) const;
    T& operator()(const point2d& p);

    ~image2d();

  private:

    box2d b_;
    T* buffer_;
    T** array_;

    void allocate_();
    void deallocate_();
  };


# ifndef MLN_INCLUDE_ONLY

  // ctors

  template <typename T>
  image2d<T>::image2d()
  {
    buffer_ = 0;
    array_ = 0;
  }

  template <typename T>
  image2d<T>::image2d(int nrows, int ncols)
  {
    b_ = mk_box2d(nrows, ncols);
    allocate_();
  }

  template <typename T>
  image2d<T>::image2d(const box2d& b)
    : b_(b)
  {
    allocate_();
  }

  // methods

  template <typename T>
  bool
  image2d<T>::has_data() const
  {
    return buffer_ != 0 and array_ != 0;
  }

  template <typename T>
  const box2d&
  image2d<T>::domain() const
  {
    return b_;
  }

  template <typename T>
  const T&
  image2d<T>::operator()(const point2d& p) const
  {
    assert(this->has_data() and this->owns_(p));
    return array_[p.row()][p.col()];
  }

  template <typename T>
  T&
  image2d<T>::operator()(const point2d& p)
  {
    assert(this->has_data() and this->owns_(p));
    return array_[p.row()][p.col()];
  }

  template <typename T>
  image2d<T>::~image2d()
  {
    deallocate_();
  }

  // private

  template <typename T>
  void
  image2d<T>::allocate_()
  {
    unsigned
      nrows = 1 + b_.pmax().row() - b_.pmin().row(),
      ncols = 1 + b_.pmax().col() - b_.pmin().col(),
      len = nrows * ncols;
    buffer_ = new T[len];
    array_ = new T*[nrows];
    T* buf = buffer_ - b_.pmin().col();
    for (unsigned i = 0; i < nrows; ++i)
      {
	array_[i] = buf;
	buf += ncols;
      }
    array_ -= b_.pmin().row();
  }

  template <typename T>
  void
  image2d<T>::deallocate_()
  {
    if (buffer_)
      {
	delete[] buffer_;
	buffer_ = 0;
      }
    if (array_)
      {
	array_ += b_.pmin().row();
	delete[] array_;
	array_ = 0;
      }
  }

# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_IMAGE2D_HH
