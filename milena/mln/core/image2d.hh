#ifndef MLN_CORE_IMAGE2D_HH
# define MLN_CORE_IMAGE2D_HH

/*! \file mln/core/image2d.hh
 *
 * \brief Definition of the basic mln::image2d class.
 */

# include <mln/core/internal/image_base.hh>
# include <mln/core/box2d.hh>


namespace mln
{

  /*! \brief Basic 2D image class.
   *
   * The parameter \c T is the type of pixel values.  This image class
   * stores data in memory and has no virtual border.
   */
  template <typename T>
  struct image2d : public internal::image_base_< box2d, image2d<T> >
  {
    /// Value associated type.
    typedef T         value;

    /// Return type of read-only access.
    typedef const T& rvalue;

    /// Return type of read-write access.
    typedef T&       lvalue;

    /// Change value type.
    template <typename U>
    struct change_value
    {
      typedef image2d<U> ret;
    };

    /// Constructor without argument.
    image2d();

    /// Constructor with the numbers of rows and columns.
    image2d(int nrows, int ncols);

    /// Constructor with a box.
    image2d(const box2d& b);

    /// Copy constructor.
    image2d(const image2d<T>& rhs);

    /// Assignment operator.
    image2d& operator=(const image2d<T>& rhs);

    /// Test if this image has been initialized.
    bool has_data() const;

    /// Give the definition domain.
    const box2d& domain() const;

    /// Read-only access to the image value located at \p p.
    const T& operator()(const point2d& p) const;

    /// Read-write access to the image value located at \p p.
    T& operator()(const point2d& p);

    /// Destructor.
    ~image2d();

  private:

    box2d b_;
    T* buffer_;
    T** array_;

    void allocate_();
    void deallocate_();

    typedef internal::image_base_< box2d, image2d<T> > super;
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

  template <typename T>
  image2d<T>::image2d(const image2d<T>& rhs)
    : super(rhs),
      b_(rhs.domain())
  {
    allocate_();
    std::memcpy(this->buffer_,
		rhs.buffer_,
		b_.npoints() * sizeof(value));
  }

  // assignment

  template <typename T>
  image2d<T>&
  image2d<T>::operator=(const image2d<T>& rhs)
  {
    assert(rhs.has_data());
    if (& rhs == this)
      return *this;
    if (this->has_data())
      this->deallocate_();
    this->b_ = rhs.domain();
    std::memcpy(this->buffer_,
		rhs.buffer_,
		b_.npoints() * sizeof(value));
    return *this;
  }

  // methods

  template <typename T>
  bool
  image2d<T>::has_data() const
  {
    return buffer_ != 0 && array_ != 0;
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
    assert(this->has_data() && this->owns_(p));
    return array_[p.row()][p.col()];
  }

  template <typename T>
  T&
  image2d<T>::operator()(const point2d& p)
  {
    assert(this->has_data() && this->owns_(p));
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
