// Copyright (C) 2007 EPITA Research and Development Laboratory
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef MLN_CORE_IMAGE3D_B_HH
# define MLN_CORE_IMAGE3D_B_HH

/*! \file mln/core/image3d_b.hh
 *
 * \brief Definition of the basic mln::image3d_b class.
 */

# include <mln/core/internal/image_base.hh>
# include <mln/core/box3d.hh>

# include <mln/border/thickness.hh>
# include <mln/value/set.hh>
# include <mln/fun/i2v/all.hh>

# include <mln/core/line_piter.hh>

# include <mln/core/internal/tracked_ptr.hh>
# include <mln/core/image3d_b_data.hh>

// FIXME:

// # include <mln/core/pixter3d_b.hh>
// # include <mln/core/dpoints_pixter.hh>


namespace mln
{

  // Fwd decl.
  template <typename T> struct image3d_b;


  namespace trait
  {

    template <typename T>
    struct is_fast< image3d_b<T> >
    {
	typedef metal::true_ ret;
    };

  } // end of mln::trait



  /*! \brief Basic 3D image class.
   *
   * The parameter \c T is the type of pixel values.  This image class
   * stores data in memory and has a virtual border with constant
   * thickness around data.
   */
  template <typename T>
  struct image3d_b : public internal::image_base_< box3d, image3d_b<T> >
  {
    // Warning: just to make effective types appear in Doxygen:
    typedef box3d   pset;
    typedef point3d psite;
    typedef point3d point;
    typedef dpoint3d dpoint;
    typedef mln_fwd_piter(box3d) fwd_piter;
    typedef mln_bkd_piter(box3d) bkd_piter;
    typedef line_piter_<point> line_piter;
    // End of warning.


    /// Value associated type.
    typedef T         value;

    /// Return type of read-only access.
    typedef const T& rvalue;

    /// Return type of read-write access.
    typedef T&       lvalue;


    /// Skeleton.
    typedef image3d_b< tag::value<T> > skeleton;


    /// Value_Set associated type.
    typedef mln::value::set<T> vset;


    /// Constructor without argument.
    image3d_b();

    /// Constructor with the numbers of indexes and the
    /// border thickness.
    image3d_b(int nslis, int nrows, int ncols, unsigned bdr = border::thickness);

    /// Constructor with a box and the border thickness (default is
    /// 3).
    image3d_b(const box3d& b, unsigned bdr = border::thickness);

    /// Copy constructor.
    image3d_b(const image3d_b<T>& rhs);

    /// Assignment operator.
    image3d_b& operator=(const image3d_b<T>& rhs);

    /// Destructor.
    ~image3d_b();


    /// Initialize an empty image.
    void init_with(int nslis, int nrows, int ncols, unsigned bdr = border::thickness);

    /// Initialize an empty image.
    void init_with(const box3d& b, unsigned bdr = border::thickness);


    /// Test if \p p is valid.
    bool owns_(const point3d& p) const;

    /// Test if this image has been initialized.
    bool has_data() const;

    /// Give the set of values of the image.
    const vset& values() const;

    /// Give the definition domain.
    const box3d& domain() const;

    /// Give the border thickness.
    unsigned border() const;

    /// Give the number of cells (points including border ones).
    std::size_t ncells() const;

    /// Read-only access to the image value located at point \p p.
    const T& operator()(const point3d& p) const;

    /// Read-write access to the image value located at point \p p.
    T& operator()(const point3d& p);

    /// Read-only access to the image value located at offset \p o.
    const T& operator[](unsigned o) const;

    /// Read-write access to the image value located at offset \p o.
    T& operator[](unsigned o);

    /// Read-only access to the image value located at (\p ind).
    const T& at(int sli, int row, int col) const;

    /// Read-write access to the image value located at (\p ind).
    T& at(int sli, int row, int col);


    /// Fast Image method

    /// Give the offset corresponding to the delta-point \p dp.
    int offset(const dpoint3d& dp) const;

    /// Give the point corresponding to the offset \p o.
    point3d point_at_offset(unsigned o) const;

    /// Give a hook to the value buffer.
    const T* buffer() const;

    /// Give a hook to the value buffer.
    T* buffer();


  private:

    tracked_ptr< image3d_b_data<T> > data_;

    typedef internal::image_base_< box3d, image3d_b<T> > super;
  };



# ifndef MLN_INCLUDE_ONLY

  // ctors

  template <typename T>
  image3d_b<T>::image3d_b()
    : data_(0)
  {
    data_->bdr_ = border::thickness; // default value in ctors.
  }

  template <typename T>
  image3d_b<T>::image3d_b(int nslis, int nrows, int ncols, unsigned bdr)
    : data_(0)
  {
    init_with(nslis, nrows, ncols, bdr);
  }

  template <typename T>
  void
  image3d_b<T>::init_with(int nslis, int nrows, int ncols, unsigned bdr)
  {
    mln_precondition(! this->has_data());
    data_ = new image3d_b_data<T>(make::box3d(nslis, nrows, ncols), bdr);
  }

  template <typename T>
  image3d_b<T>::image3d_b(const box3d& b, unsigned bdr)
    : data_(0)
  {
    init_with(b, bdr);
  }

  template <typename T>
  void
  image3d_b<T>::init_with(const box3d& b, unsigned bdr)
  {
    mln_precondition(! this->has_data());
    data_ = new image3d_b_data<T>(b, bdr);
  }

  template <typename T>
  image3d_b<T>::image3d_b(const image3d_b<T>& rhs)
    : super(rhs),
      data_(rhs.data_)
  {
  }

  // assignment

  template <typename T>
  image3d_b<T>&
  image3d_b<T>::operator=(const image3d_b<T>& rhs)
  {
    mln_precondition(rhs.has_data());
    if (& rhs == this)
      return *this;

    this->data_ = rhs.data_;
    return *this;
  }

  // methods

  template <typename T>
  bool
  image3d_b<T>::has_data() const
  {
    return data_ != 0;
  }

  template <typename T>
  const typename image3d_b<T>::vset&
  image3d_b<T>::values() const
  {
    return vset::the();
  }

  template <typename T>
  const box3d&
  image3d_b<T>::domain() const
  {
    mln_precondition(this->has_data());
    return data_->b_;
  }

  template <typename T>
  unsigned
  image3d_b<T>::border() const
  {
    mln_precondition(this->has_data());
    return data_->bdr_;
  }

  template <typename T>
  std::size_t
  image3d_b<T>::ncells() const
  {
    mln_precondition(this->has_data());
    return data_->vb_.npoints();
  }

  template <typename T>
  bool
  image3d_b<T>::owns_(const point3d& p) const
  {
    mln_precondition(this->has_data());
    return data_->vb_.has(p);
  }

  template <typename T>
  const T&
  image3d_b<T>::operator()(const point3d& p) const
  {
    mln_precondition(this->owns_(p));
    return data_->array_[p.sli()][p.row()][p.col()];
  }

  template <typename T>
  T&
  image3d_b<T>::operator()(const point3d& p)
  {
    mln_precondition(this->owns_(p));
    return data_->array_[p.sli()][p.row()][p.col()];
  }

  template <typename T>
  const T&
  image3d_b<T>::operator[](unsigned o) const
  {
    mln_precondition(o < ncells());
    return *(data_->buffer_ + o);
  }

  template <typename T>
  T&
  image3d_b<T>::operator[](unsigned o)
  {
    mln_precondition(o < ncells());
    return *(data_->buffer_ + o);
  }

  template <typename T>
  const T&
  image3d_b<T>::at(int sli, int row, int col) const
  {
    mln_precondition(this->owns_(make::point3d(sli, row, col)));
    return data_->array_[sli][row][col];
  }

  template <typename T>
  T&
  image3d_b<T>::at(int sli, int row, int col)
  {
    mln_precondition(this->owns_(make::point3d(sli, row, col)));
    return data_->array_[sli][row][col];
  }

  template <typename T>
  image3d_b<T>::~image3d_b()
  {
  }

  template <typename T>
  const T*
  image3d_b<T>::buffer() const
  {
    mln_precondition(this->has_data());
    return data_->buffer_;
  }

  template <typename T>
  T*
  image3d_b<T>::buffer()
  {
    mln_precondition(this->has_data());
    return data_->buffer_;
  }

  template <typename T>
  int
  image3d_b<T>::offset(const dpoint3d& dp) const
  {
    mln_precondition(this->has_data());
    int o = dp[0];
    return o;
  }

  template <typename T>
  point3d
  image3d_b<T>::point_at_offset(unsigned o) const
  {
    mln_precondition(o < ncells());
    point3d p = make::point3d(o / (data_->vb_.len(1) * data_->vb_.len(2)) + data_->vb_.min_sli(),
			      (o % (data_->vb_.len(1) * data_->vb_.len(2))) / data_->vb_.len(2) + data_->vb_.min_row(),
			      o % data_->vb_.len(2) + data_->vb_.min_col());
    mln_postcondition(& this->operator()(p) == this->data_->buffer_ + o);
    return p;
  }



# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln



# include <mln/core/trait/pixter.hh>
# include <mln/core/dpoints_pixter.hh>
# include <mln/core/pixter3d_b.hh>
# include <mln/core/w_window.hh>


namespace mln
{

  namespace trait
  {

    // pixter

    template <typename T>
    struct fwd_pixter< image3d_b<T> >
    {
      typedef fwd_pixter3d_b< image3d_b<T> > ret;
    };

    template <typename T>
    struct fwd_pixter< const image3d_b<T> >
    {
      typedef fwd_pixter3d_b< const image3d_b<T> > ret;
    };

    template <typename T>
    struct bkd_pixter< image3d_b<T> >
    {
      typedef internal::fixme ret;
    };

    // qixter

    template <typename T, typename W>
    struct fwd_qixter< image3d_b<T>, W >
    {
      typedef dpoints_fwd_pixter< image3d_b<T> > ret;
    };

    template <typename T, typename W>
    struct fwd_qixter< const image3d_b<T>, W >
    {
      typedef dpoints_fwd_pixter< const image3d_b<T> > ret;
    };

    template <typename T, typename W>
    struct bkd_qixter< image3d_b<T>, W >
    {
      typedef internal::fixme ret;
    };

  } // end of namespace mln::trait

} // end of namespace mln


#endif // ! MLN_CORE_IMAGE3D_B_HH
